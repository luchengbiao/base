#pragma once
#include "base\callback\callback.h"
#include "invite_manager_global.h"

struct BaseInviteDetail
{
	std::string nim_channel_id;

	BaseInviteDetail() : nim_channel_id("")
	{

	}
};

class BaseInvite :public nbase::SupportWeakCallback
{
public:
	BaseInvite();
	~BaseInvite();

	virtual void OnSetCallInfo(One2OneCallInfo info) = 0;
	virtual void OnInitInvite(One2OneCallInfo info, bool b_force_join = false) = 0;	//��ʼ������	
	virtual void StartInvite() = 0;							//��������
	virtual void StartInviteByAccount(std::string account) = 0;						//����ʧ�ܺ�����������������Է�
	virtual void AcceptInvite() = 0;						//��������
	virtual void RefuseInvite() = 0;						//�ܾ�����
	virtual void CancelInvite() = 0;						//����ȡ������
	virtual void EndInvite(std::string peer_account) = 0;	//��ʱ�������� 
	virtual void EndFailInvite(bool b_creator) = 0;			//���벻�ɹ����� 
	virtual void CancelAllService() = 0;					//��Ҫ�Ǵ���ʱ��
	virtual bool GetDetail(BaseInviteDetail& detail) = 0;
};