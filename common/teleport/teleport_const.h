#ifndef __TELEPORT_CONST_H__
#define __TELEPORT_CONST_H__

// ע��ͬ������������ͬ���Ե�const�ļ�

// ���ļ��趨teleport����ģ��֮��ͨѶʱ�Ĵ���ֵ��JSON���ݣ���������
//  - WEB����������
//  - WEB������WEB��̨
//  - WEB��̨��CORE���ķ���

//=======================================================
// Urlprotocol���
//=======================================================
#define TP_URLPROTO_APP_NAME		"teleport"

//=======================================================
// Զ��������֤��ʽ
//=======================================================
#define TP_AUTH_TYPE_NONE			0
#define TP_AUTH_TYPE_PASSWORD		1
#define TP_AUTH_TYPE_PRIVATE_KEY	2

//=======================================================
// Զ������Э��
//=======================================================
#define TP_PROTOCOL_TYPE_RDP		1
#define TP_PROTOCOL_TYPE_SSH		2
#define TP_PROTOCOL_TYPE_TELNET		3

//=======================================================
// Զ��������Э��
//=======================================================
#define TP_PROTOCOL_TYPE_RDP_DESKTOP		100
#define TP_PROTOCOL_TYPE_SSH_SHELL		    200
#define TP_PROTOCOL_TYPE_SSH_SFTP	    	201
#define TP_PROTOCOL_TYPE_TELNET_SHELL		300


//=======================================================
// Զ����������ϵͳ
//=======================================================
#define TP_OS_TYPE_WINDOWS	1
#define TP_OS_TYPE_LINUX	2

//=======================================================
// Զ�����ӻỰ״̬
//=======================================================
#define TP_SESS_STAT_RUNNING				0		// �Ự��ʼ�ˣ���������
#define TP_SESS_STAT_END					9999	// �Ự�ɹ�����
#define TP_SESS_STAT_ERR_AUTH_DENIED		1		// �Ự��������Ϊ��֤ʧ��
#define TP_SESS_STAT_ERR_CONNECT			2		// �Ự��������Ϊ�޷����ӵ�Զ������
#define TP_SESS_STAT_ERR_BAD_SSH_KEY		3		// �Ự��������Ϊ�޷�ʶ��SSH˽Կ
#define TP_SESS_STAT_ERR_INTERNAL			4		// �Ự��������Ϊ�ڲ�����
#define TP_SESS_STAT_ERR_UNSUPPORT_PROTOCOL	5		// �Ự��������ΪЭ�鲻֧��(RDP)
#define TP_SESS_STAT_ERR_BAD_PKG			6		// �Ự��������Ϊ�յ�����ı���
#define TP_SESS_STAT_ERR_RESET				7		// �Ự��������Ϊteleport���ķ���������
#define TP_SESS_STAT_ERR_IO					8		// �Ự��������Ϊ�����ж�
#define TP_SESS_STAT_ERR_SESSION			9		// �Ự��������Ϊ��Ч�ĻỰID
#define TP_SESS_STAT_ERR_AUTH_TYPE			10		// �Ự��������Ϊ�����������֤��ʽ
#define TP_SESS_STAT_STARTED                100     // �Ѿ����ӳɹ��ˣ���ʼ��¼¼����
#define TP_SESS_STAT_ERR_START_INTERNAL     104     // �Ự��������Ϊ�ڲ�����
#define TP_SESS_STAT_ERR_START_BAD_PKG      106     // �Ự��������Ϊ�յ�����ı���
#define TP_SESS_STAT_ERR_START_RESET        107     // �Ự��������Ϊteleport���ķ���������
#define TP_SESS_STAT_ERR_START_IO           108     // �Ự��������Ϊ�����ж�


//=======================================================
// ��Ȩ���
//=======================================================
#define TP_FLAG_ALL					0xFFFFFFFF
// �Ự��¼���
#define TP_FLAG_RECORD_REPLAY		0x00000001	// �����¼��ʷ��¼��طţ�
#define TP_FLAG_RECORD_REAL_TIME	0x00000002	// ����ʵʱ���
// RDP���
#define TP_FLAG_RDP_DESKTOP			0x00000001	// ����Զ������
#define TP_FLAG_RDP_CLIPBOARD		0x00000002	// ���������
#define TP_FLAG_RDP_DISK			0x00000004	// �������ӳ��
#define TP_FLAG_RDP_APP				0x00000008	// ����Զ��APP����δʵ�֣�
#define TP_FLAG_RDP_CONSOLE			0x00001000	//�������ӵ�����Ա�Ự��RDP��consoleѡ�
// SSH���
#define TP_FLAG_SSH_SHELL			0x00000001	// ����SHELL
#define TP_FLAG_SSH_SFTP			0x00000002	// ����SFTP
#define TP_FLAG_SSH_X11				0x00000004	// ����X11ת������δʵ�֣�
#define TP_FLAG_SSH_EXEC			0x00000008	// ����execִ��Զ�������δʵ�֣�
#define TP_FLAG_SSH_TUNNEL			0x00000010	// allow ssh tunnel. (not impl.)


//=======================================================
// ����ֵ
//=======================================================
#define TPE_OK						0		// �ɹ�
//-------------------------------------------------------
// ͨ�ô���ֵ
//-------------------------------------------------------
#define TPE_NEED_MORE_DATA			1		// ��Ҫ�������ݣ���һ���Ǵ���
#define TPE_NEED_LOGIN				2		// ��Ҫ��¼
#define TPE_PRIVILEGE				3		// û�в���Ȩ��
#define TPE_NOT_IMPLEMENT			7		// ������δʵ��
#define TPE_EXISTS					8		// Ŀ���Ѿ�����
#define TPE_NOT_EXISTS              9		// Ŀ�겻����

// 100~299��ͨ�ô���ֵ

#define TPE_FAILED					100		// �ڲ�����
#define TPE_NETWORK					101		// �������
#define TPE_DATABASE				102		// ���ݿ����ʧ��

// HTTP������ش���
#define TPE_HTTP_METHOD				120		// ��Ч�����󷽷�������GET/POST�ȣ������ߴ�������󷽷���������ҪPOST��ȴʹ��GET��ʽ����
#define TPE_HTTP_URL_ENCODE			121		// URL��������޷����룩
//#define TPE_HTTP_URI				122		// ��Ч��URI

#define TPE_UNKNOWN_CMD				124		// δ֪������
#define TPE_JSON_FORMAT				125		// �����JSON��ʽ����ҪJSON��ʽ���ݣ�����ȴ�޷���JSON��ʽ���룩
#define TPE_PARAM					126		// ��������
#define TPE_DATA					127		// ���ݴ���

// #define TPE_OPENFILE_ERROR			0x1007	// �޷����ļ�
// #define TPE_GETTEMPPATH_ERROR		0x1007
#define TPE_OPENFILE				300


//-------------------------------------------------------
// WEB����ר�ô���ֵ
//-------------------------------------------------------
#define TPE_CAPTCHA_EXPIRED			10000	// ��֤���ѹ���
#define TPE_CAPTCHA_MISMATCH		10001	// ��֤�����
#define TPE_OATH_MISMATCH			10002	// �����֤����̬��֤�����
#define TPE_SYS_MAINTENANCE			10003	// ϵͳά����

#define TPE_USER_LOCKED				10100	// �û��Ѿ���������������δ������룩
#define TPE_USER_DISABLED			10101	// �û��Ѿ�������
#define TPE_USER_AUTH				10102	// �����֤ʧ��

//-------------------------------------------------------
// ���ֳ���ר�ô���ֵ
//-------------------------------------------------------
#define TPE_NO_ASSIST				100000	// δ�ܼ�⵽���ֳ���
#define TPE_OLD_ASSIST				100001	// ���ֳ���汾̫��
#define TPE_START_CLIENT			100002	// �޷������ͻ��˳����޷��������̣�



//-------------------------------------------------------
// ���ķ���ר�ô���ֵ
//-------------------------------------------------------
#define TPE_NO_CORE_SERVER			200000	// δ�ܼ�⵽���ķ���



#endif // __TELEPORT_CONST_H__
