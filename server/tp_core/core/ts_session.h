#ifndef __TS_SESSION_H__
#define __TS_SESSION_H__

#include "../common/ts_const.h"
#include "../common/protocol_interface.h"

#include <ex.h>

typedef struct TS_CONNECT_INFO
{
	// TODO:
	//TPP_CONNECT_INFO conn;

	ex_astr sid;

	// ���������Ϣ��ص�����Ҫ�ص�ID
	int user_id;
	int host_id;
	int acc_id;

	ex_astr user_username;// ���뱾�����ӵ��û���

	ex_astr host_ip;// ������Զ������IP�������ֱ������ģʽ������remote_host_ip��ͬ��
	ex_astr conn_ip;// Ҫ���ӵ�Զ��������IP������Ƕ˿�ӳ��ģʽ����Ϊ·��������IP��
	int conn_port;// Ҫ���ӵ�Զ�������Ķ˿ڣ�����Ƕ˿�ӳ��ģʽ����Ϊ·�������Ķ˿ڣ�
	ex_astr client_ip;

	ex_astr acc_username;	// Զ���������˺�
	ex_astr acc_secret;	// Զ�������˺ŵ����루����˽Կ��
	ex_astr username_prompt;// for telnet
	ex_astr password_prompt;// for telnet

	int protocol_type;
	int protocol_sub_type;
	int protocol_flag;
	int record_flag;
	int auth_type;

	int ref_count;// ���������Ϣ�����ü��������������������Ϣ����δ��ʹ�ã��򳬹�30����Զ�����
	ex_u64 ticket_start;// ��������Ϣ�Ĵ���ʱ�䣨���ڳ�ʱδʹ�þ����ٵĹ��ܣ�
}TS_CONNECT_INFO;

typedef std::map<ex_astr, TS_CONNECT_INFO*> ts_connections;  // sid -> TS_CONNECT_INFO

class TsSessionManager : public ExThreadBase
{
public:
	TsSessionManager();
	~TsSessionManager();

	// generate a sid for connection info.
	bool request_session(ex_astr& sid, TS_CONNECT_INFO* info);

	// ����sid�õ�������Ϣ�����������ü�����
	bool get_connect_info(const ex_astr& sid, TS_CONNECT_INFO& info);
	// �������ü����������ü���Ϊ0ʱ��ɾ��֮
	bool free_connect_info(const ex_astr& sid);

protected:
	void _thread_loop(void);

private:
	void _gen_session_id(ex_astr& sid, const TS_CONNECT_INFO* info, int len);

	// ��ʱ��飬����30��δ�������ӵ�connect-info�ᱻ�Ƴ�
	void _remove_expired_connect_info(void);

private:
	ExThreadLock m_lock;
	ts_connections m_connections;
};

extern TsSessionManager g_session_mgr;

#endif // __TS_SESSION_H__

