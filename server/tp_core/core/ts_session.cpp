#include "ts_session.h"
#include "ts_env.h"

#include <mbedtls/sha1.h>
#include <teleport_const.h>

TsSessionManager g_session_mgr;

TsSessionManager::TsSessionManager() :
        ExThreadBase("sid-mgr-thread") {
}

TsSessionManager::~TsSessionManager() {
    ts_connections::iterator it_conn = m_connections.begin();
    for (; it_conn != m_connections.end(); ++it_conn) {
        delete it_conn->second;
    }
    m_connections.clear();
}

void TsSessionManager::_thread_loop(void) {
    for (;;) {
        ex_sleep_ms(1000);
        if (m_need_stop)
            return;
        _remove_expired_connect_info();
    }
}

void TsSessionManager::_remove_expired_connect_info(void) {
    // ����15��δ�������ӵ�connect-info�ᱻ�Ƴ�

    ExThreadSmartLock locker(m_lock);

    ex_u64 _now = ex_get_tick_count();
    ts_connections::iterator it = m_connections.begin();
    for (; it != m_connections.end();) {
        //EXLOGD("[core] check expired connect info: [%s] %d, %d %d %d\n", it->first.c_str(), it->second->ref_count, int(_now), int(it->second->ticket_start), int(_now - it->second->ticket_start));
        if (it->second->ref_count == 0 && _now - 15000 > it->second->ticket_start) {
            EXLOGD("[core] remove connection info, because timeout: %s\n", it->first.c_str());
            delete it->second;
            m_connections.erase(it++);
            EXLOGD("[core] there are %d connection info exists.\n", m_connections.size());
        } else {
            ++it;
        }
    }
}

bool TsSessionManager::get_connect_info(const ex_astr &sid, TS_CONNECT_INFO &info) {
    ExThreadSmartLock locker(m_lock);

    ts_connections::iterator it = m_connections.find(sid);
    if (it == m_connections.end())
        return false;

    info.sid = it->second->sid;
    info.user_id = it->second->user_id;
    info.host_id = it->second->host_id;
    info.acc_id = it->second->acc_id;
    info.user_username = it->second->user_username;
    info.host_ip = it->second->host_ip;
    info.conn_ip = it->second->conn_ip;
    info.conn_port = it->second->conn_port;
    info.client_ip = it->second->client_ip;
    info.acc_username = it->second->acc_username;
    info.acc_secret = it->second->acc_secret;
    info.username_prompt = it->second->username_prompt;
    info.password_prompt = it->second->password_prompt;
    info.protocol_type = it->second->protocol_type;
    info.protocol_sub_type = it->second->protocol_sub_type;
    info.protocol_flag = it->second->protocol_flag;
    info.record_flag = it->second->record_flag;
    info.auth_type = it->second->auth_type;

    it->second->ref_count++;

    return true;
}

bool TsSessionManager::free_connect_info(const ex_astr &sid) {
    ExThreadSmartLock locker(m_lock);

    ts_connections::iterator it = m_connections.find(sid);
    if (it == m_connections.end())
        return false;

    it->second->ref_count--;

    // ����RDP��˵����ʱ��Ҫ�Ƴ�������Ϣ��ϵͳ�Դ�RDP�ͻ����ڵ�һ������ʱ����Э��Э�̣�Ȼ�����ϻ�Ͽ���֮��������������һ�Σ��ڶ�������֮ǰ���ܻ���ʾ֤����Ϣ������û���ʱ�䲻���������ܻᵼ�³�ʱ����
    // ��ˣ����ǽ������ü������ͣ�������һ��������ʱ�䣬�ö�ʱ�����Ƴ�����
    if (it->second->protocol_type != TP_PROTOCOL_TYPE_RDP) {
        if (it->second->ref_count <= 0) {
            EXLOGD("[core] remove connection info, because all connections closed: %s\n", it->first.c_str());
            delete it->second;
            m_connections.erase(it);
            EXLOGD("[core] there are %d connection info exists.\n", m_connections.size());
        }
    } else {
        if (it->second->ref_count == 1)
            it->second->ref_count = 0;
        it->second->ticket_start = ex_get_tick_count() + 45000; // ���ǽ�ʱ������ƶ�45�룬�������û�з���RDP�ĵڶ������ӣ����������Ϣ�ͻ���һ���Ӻ������
    }


    return true;
}

bool TsSessionManager::request_session(ex_astr &sid, TS_CONNECT_INFO *info) {
    ExThreadSmartLock locker(m_lock);

    EXLOGD("[core] request session: account: [%s], protocol: [%d], auth-mode: [%d]\n", info->acc_username.c_str(),
           info->protocol_type, info->auth_type);

    ex_astr _sid;
    int retried = 0;
    ts_connections::iterator it;
    for (;;) {
        _gen_session_id(_sid, info, 6);
        it = m_connections.find(_sid);
        if (it == m_connections.end())
            break;

        retried++;
        if (retried > 50)
            return false;
    }

    info->sid = _sid;
    info->ref_count = 0;
    info->ticket_start = ex_get_tick_count();
    m_connections.insert(std::make_pair(_sid, info));

    sid = _sid;
    if (info->protocol_type == TP_PROTOCOL_TYPE_RDP) {
        info->ref_count = 1; // ��ΪRDP����֮ǰ���ܻ��кܳ�ʱ������ȷ���Ƿ����ӡ��Ƿ�����֤�飬���Ժ����׳�ʱ��������Ϊ�����ü���+1����ֹ��ʱ�������
        char szTmp[8] = {0};
        snprintf(szTmp, 8, "%02X", (unsigned char) (info->acc_username.length() + info->acc_secret.length()));
        sid += szTmp;
    }

    return true;
}

void TsSessionManager::_gen_session_id(ex_astr &sid, const TS_CONNECT_INFO *info, int len) {
    mbedtls_sha1_context sha;
    ex_u8 sha_digist[20] = {0};

    ex_u64 _tick = ex_get_tick_count();
    ex_u64 _tid = ex_get_thread_id();

    mbedtls_sha1_init(&sha);
    mbedtls_sha1_starts(&sha);
    mbedtls_sha1_update(&sha, (const unsigned char *) &_tick, sizeof(ex_u64));
    mbedtls_sha1_update(&sha, (const unsigned char *) &_tid, sizeof(ex_u64));
    mbedtls_sha1_update(&sha, (const unsigned char *) info->conn_ip.c_str(), info->conn_ip.length());
    mbedtls_sha1_update(&sha, (const unsigned char *) info->client_ip.c_str(), info->client_ip.length());
    mbedtls_sha1_update(&sha, (const unsigned char *) info->acc_username.c_str(), info->acc_username.length());
    mbedtls_sha1_finish(&sha, sha_digist);
    mbedtls_sha1_free(&sha);

    char szTmp[64] = {0};
    int _len = len / 2 + 1;
    int i = 0;
    int offset = 0;
    for (i = 0; i < _len; ++i) {
        snprintf(szTmp + offset, 64 - offset, "%02X", sha_digist[i]);
        offset += 2;
    }

    sid.assign(szTmp, len);
}
