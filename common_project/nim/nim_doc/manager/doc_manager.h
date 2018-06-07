#pragma once
#include "base\callback\callback.h"
#include "base\memory\singleton.h"
#include "nim_sdk_manager\helper\nim_doc_trans_helper.h"
#include <list>
#include <stdint.h>
#include "nim_sdk_manager\api\nim_cpp_nos.h"
#include "base\util\auto_unregister.h"
#include "nim_sdk_manager\api\nim_cpp_doc_trans.h"
#include <string>
#include "nim_sdk_manager\api\nim_cpp_user.h"
#include <map>
#include <memory>
#include <vector>
#include <set>

namespace nim_comp
{

typedef std::function<void(nim::DocTransInfo info)> OnUseDoc;	//ʹ��ĳ���ĵ��Ļص�����
typedef std::function<void(int32_t count, const std::list<nim::DocTransInfo>& doc_infos)> OnGetDocInfos;	//��ȡ�ĵ���Ϣ�Ļص�����
typedef std::function<void(bool b_downloaded, int download_pages)> OnCheckDocDownloaded;			//����ĵ��Ƿ����سɹ��Ļص�����

/** @class DocManager
  * @brief �ĵ����������
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2016/12/15
  */
class DocManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(DocManager);
	DocManager();
	~DocManager();

	/**
	* ��ȡ�ĵ��б�
	* @param[in] ��ѯ����ʼdocId����Ϊ�գ���ʾ��ͷ��ʼ���ң������ĵ�ת��ķ���ʱ�併������
	* @return void	�޷���ֵ
	*/
	void GetDocInfoList(const std::string& doc_id);

	/**
	* �ĵ�ת�����֪ͨ�Ļص�����
	* @param[in] code ������
	* @param[in] doc_info �ĵ���Ϣ
	* @return void	�޷���ֵ
	*/
	void OnDocInfoNotify(int32_t code, const nim::DocTransInfo& doc_info);

	/**
	* ��ȡ�ĵ���Ϣ�Ļص�����
	* @param[in] code ������
	* @param[in] count ������������
	* @param[in] doc_infos �ĵ���Ϣ
	* @return void	�޷���ֵ
	*/
	void OnGetDocInfosCallback(int32_t code, int32_t count, const std::list<nim::DocTransInfo>& doc_infos);

	/**
	* ɾ���ĵ��Ļص�����
	* @param[in] doc_id �ĵ�id
	* @param[in] extern_info ����ɾ����Ϣ
	* @param[in] code ������
	* @param[in] doc_infos �ĵ���Ϣ
	* @return void	�޷���ֵ
	*/
	void OnDeleteDocInfoCallback(const std::string& doc_id, const std::string& extern_info, int32_t code, const nim::DocTransInfo& doc_info);

	/**
	* ע���ȡת��֪ͨ�Ļص�
	* @param[in] callback �ص�����
	* @return UnregisterCallback ��ע�����
	*/
	UnregisterCallback RegDocInfoNotifyCallback(const nim::DocTrans::DocInfoCallback& callback);

	/**
	* ע���ȡ�ĵ���Ϣ�Ļص�
	* @param[in] callback �ص�����
	* @return UnregisterCallback ��ע�����
	*/
	UnregisterCallback RegGetDocInfosCallback(const OnGetDocInfos& callback);

	/**
	* ע��ʹ���ĵ��Ļص�
	* @param[in] callback �ص�����
	* @return UnregisterCallback ��ע�����
	*/
	UnregisterCallback RegUseDocCallback(const OnUseDoc& callback);

	/**
	* ����ʹ���ĵ��ص�����
	* @param[in] doc_info �ĵ���Ϣ
	* @return void	�޷���ֵ
	*/
	bool InvokeUseDocCallback(const nim::DocTransInfo &doc_info);

public:
	/**
	* �ϴ��ĵ�
	* @param[in] doc_id �ĵ�id������ǵ�һ���ϴ�����Ϊ�գ����Ϊ����������д��Ҫ�������ĵ�id
	* @param[in] file_path ��Ҫ�ϴ����ĵ�·��
	* @param[in] file_name �ĵ�����
	* @param[in] file_type �ĵ�����
	* @return std::string �ϴ�id(��demo�����ڱ�ʶһ���ϴ�����)
	*/
	std::string UploadDoc(const std::string& doc_id, std::string& file_path, const std::string& file_name, nim::NIMDocTranscodingFileType file_type);

	/**
	* �����ĵ�id��ȡ�����ϴ����ļ����ϴ�id
	* @param[in] doc_id �ĵ�id
	* @return std::string �ϴ�id��ʧ���򷵻ؿ�
	*/
	std::string GetUploadId(const std::string& doc_id);

	/**
	* �����ϴ�idע��ĳ���ĵ��ϴ�����Ļص�
	* @param[in] upload_id �ϴ�id
	* @param[in] compelete_cb �ϴ���ɵĻص�����
	* @param[in] progress_cb �ϴ����ȵĻص�����
	* @return bool true �ɹ���false ʧ��
	*/
	bool RegUploadDocCallback(const std::string& upload_id, nim::NOS::UploadMediaExCallback compelete_cb, nim::NOS::ProgressExCallback progress_cb);

	/**
	* �ĵ��ϴ���ϵĻص�����
	* @param[in] upload_id �ϴ�id
	* @param[in] res_code ������
	* @param[in] result �ϴ���ɵĽ��
	* @return void	�޷���ֵ
	*/
	void OnDocUploadCompeleteCallback(const std::string& upload_id, nim::NIMResCode res_code, const nim::UploadMediaResult& result);

	/**
	* �ĵ��ϴ����ȵĻص�����
	* @param[in] upload_id �ϴ�id
	* @param[in] completed_size �Ѿ��ϴ��Ĵ�С
	* @param[in] file_size �ϴ��ļ����ܴ�С
	* @param[in] result ������̵�����
	* @return void	�޷���ֵ
	*/
	void OnDocUploadProgressCallback(const std::string& upload_id, int64_t completed_size, int64_t file_size, const nim::ProgressData& result);

public:
	/**
	* ����ĵ��Ƿ��������
	* @param[in] doc_info �ĵ���Ϣ
	* @param[in] quality �ĵ�����
	* @param[in] page_num Ҫ����ҳ��(���Ϊ0�������ȫ��)
	* @param[in] cb ������ص�����
	* @return void	�޷���ֵ
	*/
	void IsDownloadedDoc(const nim::DocTransInfo& doc_info, nim::NIMDocTranscodingQuality quality, int32_t page_num, const OnCheckDocDownloaded& cb);

	/**
	* ɾ�����������ص��ĵ�
	* @param[in] doc_info �ĵ���Ϣ
	* @param[in] quality �ĵ�����
	* @return void	�޷���ֵ
	*/
	void DeleteDoc(const nim::DocTransInfo& doc_info, nim::NIMDocTranscodingQuality quality = nim::kNIMDocTranscodingQualityHigh);

	/**
	* ��ȡ�ĵ�ҳ�ı���·��������������Ҫ���أ�
	* @param[in] doc_info �ĵ���Ϣ
	* @param[in] page_num ҳ��
	* @param[in] quality �ĵ�����
	* @return std::string	�ļ�·��
	*/
	std::wstring GetDocPagePath(const nim::DocTransInfo& doc_info, int32_t page_num, nim::NIMDocTranscodingQuality quality = nim::kNIMDocTranscodingQualityHigh);

	/**
	* �����ĵ�ҳ
	* @param[in] doc_info �ĵ���Ϣ
	* @param[in] page_num ҳ��
	* @param[in] quality �ĵ�����
	* @param[in] cb ���ؽ���ص�
	* @return bool	���ص����Ƿ�ɹ�
	*/
	bool DownloadDocPage(const nim::DocTransInfo& doc_info, int32_t page_num, nim::NIMDocTranscodingQuality quality, nim::NOS::DownloadMediaCallback cb);
	
	/**
	* �����ĵ�ҳ�Ľ���ص�
	* @param[in] download_id ���صĶ���id
	* @param[in] save_path ����·��
	* @param[in] res_code ������
	* @param[in] file_path ʵ������·��
	* @param[in] call_id ����id
	* @param[in] res_id ��Դid
	* @return void	�޷���ֵ
	*/
	void DownloadDocPageCb(const std::string& download_id, const std::wstring& save_path, nim::NIMResCode res_code, const std::string& file_path, const std::string& call_id, const std::string& res_id);

private:
	std::map<int, std::unique_ptr<nim::DocTrans::DocInfoCallback>> doc_info_notify_cb_list_; //�ĵ�ת��֪ͨ�Ļص��б�
	std::map<int, std::unique_ptr<OnGetDocInfos>> get_doc_infos_cb_list_;	//��ȡ�ĵ���Ϣ�Ļص��б�
	std::map<int, std::unique_ptr<OnUseDoc>> use_doc_cb_list_;				//�û���ʼʹ���ĵ��Ļص��б�

	std::map<std::string, std::pair<nim::NOS::UploadMediaExCallback, nim::NOS::ProgressExCallback>> doc_upload_cb_list_; //�û��ĵ��ϴ��ص��б�<upload_id, cbs>
	std::set<std::string> doc_upload_ids_; //�û��ĵ��ϴ�id�б�
	std::map<std::string, std::string> doc_id_upload_id_list_; //�û��ĵ�id���ϴ�idӳ��<doc_id, upload_id>

	std::map<std::string, std::vector<nim::NOS::DownloadMediaCallback>> doc_page_download_cb_list_; //�û��ĵ�ҳ���ػص��б� 
};

}