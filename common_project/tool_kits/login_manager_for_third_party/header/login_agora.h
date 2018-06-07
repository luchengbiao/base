#pragma once
#include "login_base.h"
#include "base\memory\singleton.h"
#define SUCCESS 0
#define LOGOUT_E_OTHER  100 //δ֪ԭ�����˳���¼, ͨ��������������������ġ�
#define LOGOUT_E_USER   101	//�û��ǳ�
#define LOGOUT_E_KICKED 103	//���˻����ڱ𴦵�¼

#define LOGIN_E_NET		201	//��������	
#define LOGIN_E_TOKEN_KICKED 207	//�û���ʹ�ø��º�� Signaling Key �ڱ𴦵�¼��ϵͳ��
#define LOGIN_E_FAILED 202	//���������ܾ�
/*
	����CODE
	SUCCESS				0				û�д���
	LOGOUT_E_OTHER		100				δ֪ԭ�����˳���¼, ͨ��������������������ġ�
	LOGOUT_E_USER		101				�û��ǳ�
	LOGOUT_E_NET		102				��������
	LOGOUT_E_KICKED		103				���˻����ڱ𴦵�¼
	LOGOUT_E_PACKET		104				�ѱ�����
	LOGOUT_E_TOKENEXPIRED	105			Signaling Key����
	LOGOUT_E_OLDVERSION	106				�ѱ�����
	LOGOUT_E_TOKENWRONG	107				�ѱ�����
	LOGIN_E_OTHER		200				ԭ��δ֪
	LOGIN_E_NET			201				��������
	LOGIN_E_FAILED	202	���������ܾ�
	LOGIN_E_CANCEL	203	�û���ȡ����¼
	LOGIN_E_TOKENEXPIRED	204	Signaling Key���ڣ���¼����
	LOGIN_E_OLDVERSION	205	�ѱ�����
	LOGIN_E_TOKENWRONG	206	Signaling Key ��Ч����� ����LOGIN_E_TOKENWRONG(206)���� ��
	LOGIN_E_TOKEN_KICKED	207	�û���ʹ�ø��º�� Signaling Key �ڱ𴦵�¼��ϵͳ��
	LOGIN_E_ALREADY_LOGIN	208	�û��ѵ�¼���ٴη����¼�ᴥ���ô��󡣿��Ժ��Ӹô���
	JOINCHANNEL_E_OTHER	300	����ý������ʧ�ܡ�
	SENDMESSAGE_E_OTHER	400	����ý������ʧ�ܡ�
	SENDMESSAGE_E_TIMEOUT	401	������Ϣ��ʱ
	QUERYUSERNUM_E_OTHER	500	��ѯƵ���û�����ʧ��
	QUERYUSERNUM_E_TIMEOUT	501	��ѯƵ���û����볬ʱ
	QUERYUSERNUM_E_BYUSER	501	�ѱ�����
	LEAVECHANNEL_E_OTHER	600	δ֪ԭ�����˳�Ƶ��
	LEAVECHANNEL_E_KICKED	601	������Ա�߳�Ƶ��
	LEAVECHANNEL_E_BYUSER	602	�û�����Ƶ���ڡ�
	LEAVECHANNEL_E_LOGOUT	603	�ǳ�ʱ���߳�Ƶ��
	LEAVECHANNEL_E_DISCONN	604	�������⵼���˳�Ƶ��
	INVITE_E_OTHER	700	����ʧ��
	INVITE_E_REINVITE	701	�ظ�����
	INVITE_E_NET	702	��������
	INVITE_E_PEEROFFLINE	703	�Է���������״�塣
	INVITE_E_TIMEOUT	704	���г�ʱ
	INVITE_E_CANTRECV	705	�ѱ�����
	GENERAL_E	1000	һ�����
	GENERAL_E_FAILED	1001	һ����� - ʧ��
	GENERAL_E_UNKNOWN	1002	һ����� - δ֪
	GENERAL_E_NOT_LOGIN	1003	һ����� - ����ǰû�е�¼
	GENERAL_E_WRONG_PARAM	1004	һ����� - ��������ʧ��
*/

using AgoraLoginCb = std::function<void(int, std::wstring, uint32_t)>;//code,msg,u_id
using AgoraLogoutCb = std::function<void(int, std::wstring)>;
class LoginAgora :public LoginBase
{
public:
	SINGLETON_DEFINE(LoginAgora);
	LoginAgora();
	~LoginAgora();
	void Login(std::string app_key,std::string account, std::string pwd, AgoraLoginCb cb);
	void Logout(AgoraLogoutCb);
	void Logout();

	void RegLogoutCb(AgoraLogoutCb);	//ע��ǳ�����Ļص�
	
	bool CheckStatus(std::string app_id,std::string account, std::string pwd, AgoraLoginCb cb);	//�����Ƿ����ߣ�������δ���ߵ�������Զ���¼


	bool IsOnline();	//ֻ�����Ƿ�����
	void SetAppid(std::string id);

};