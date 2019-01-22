#ifndef __TS_HTTP_RPC_H__
#define __TS_HTTP_RPC_H__

#include "../../external/mongoose/mongoose.h"

#include "ts_const.h"

#include <vector>
#include <string>
#include <map>

#include <ex.h>
#include <json/json.h>


/*
//=================================================================
�ӿ�ʹ��˵����

�����������󣬼��� localhost:50022������http���������ʽҪ�����£�

GET ��ʽ
http://localhost:50022/method/json_param
����json_param��ʹ��url_encode���б�����json��ʽ�ַ���

POST ��ʽ
http://localhost:50022/method
post������������json_param

���У�URI��Ϊ�������֣�
method			����ִ�е����񷽷���
json_param		�����񷽷��ĸ��Ӳ��������û�и��Ӳ������ⲿ�ֿ���ʡ�ԡ�

���ظ�ʽ��ִ�н����󣬷���һ��json��ʽ���ַ����������ߣ���ʽ���£�

{"code":0,"data":varb}

���У�code�Ǳ��еģ���ֵ��һ��������룬0��ʾ�ɹ������ʧ�ܣ������û��data�򡣲����ɹ�ʱ��data�����
�����ķ������ݣ����ʽ���ݾ���ִ�е����񷽷���ͬ����ͬ��

*/

void http_rpc_main_loop(bool is_https);
void http_rpc_stop(bool is_https);

typedef std::map<ex_astr, ex_astr> content_type_map;

// for https server, see
// http://www.xiaovdiy.cn/?post=284

class TsHttpRpc {
public:
    TsHttpRpc();
    ~TsHttpRpc();

    bool init_http();
    bool init_https();
    void run(void);
    void stop(void);
    void _rpc_func_url_protocol(const ex_astr& func_args, ex_astr& buf);

    ex_astr get_content_type(ex_astr file_suffix) {
        content_type_map::iterator it=m_content_type_map.find(file_suffix);
        if (it != m_content_type_map.end()) {
            return it->second;
        } else {
            return "application/octet-stream";
        }
    };

private:
    bool _on_init();
    int _parse_request(struct http_message* req, ex_astr& func_cmd, ex_astr& func_args);
    void _process_js_request(const ex_astr& func_cmd, const ex_astr& func_args, ex_astr& buf);
    void _create_json_ret(ex_astr& buf, int errcode);
    void _create_json_ret(ex_astr& buf, Json::Value& jr_root);

    void _rpc_func_run_client(const ex_astr& func_args, ex_astr& buf);
    // 	void _rpc_func_check(const ex_astr& func_args, ex_astr& buf);
    void _rpc_func_rdp_play(const ex_astr& func_args, ex_astr& buf);
    void _rpc_func_get_config(const ex_astr& func_args, ex_astr& buf);
    void _rpc_func_set_config(const ex_astr& func_args, ex_astr& buf);
    void _rpc_func_file_action(const ex_astr& func_args, ex_astr& buf);
    void _rpc_func_get_version(const ex_astr& func_args, ex_astr& buf);

    static void _mg_event_handler(struct mg_connection *nc, int ev, void *ev_data);

private:
    content_type_map m_content_type_map;
    struct mg_mgr m_mg_mgr;
    bool m_stop;
};

#endif // __TS_HTTP_RPC_H__
