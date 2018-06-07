#include "doc_manager.h"
#include "callback/doc_callback.h"
#include <string>
#include <stdint.h>
#include "nim_sdk_manager/api/nim_cpp_doc_trans.h"
#include "base/callback/callback.h"
#include "nim_sdk_manager/helper/nim_doc_trans_helper.h"
#include "log_manager/log.h"
#include <assert.h>
#include "qthread_manager/closure.h"
#include "base/util/string_util.h"
#include "base\file\file_util.h"
#include "nim_base/util.h"
#include "base/win32/path_util.h"

namespace nim_comp
{

DocManager::DocManager()
{

}

DocManager::~DocManager()
{

}

void DocManager::GetDocInfoList(const std::string& doc_id)
{
	nim::DocTrans::GetInfoList(doc_id, 30, "", nbase::Bind(DocTransCallback::DocInfosCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void DocManager::OnDocInfoNotify(int32_t code, const nim::DocTransInfo& doc_info)
{
	for (auto& it : doc_info_notify_cb_list_)
	{
		(*(it.second))(code, doc_info);
	}
}

void DocManager::OnGetDocInfosCallback(int32_t code, int32_t count, const std::list<nim::DocTransInfo>& doc_infos)
{
	if (code == nim::kNIMResSuccess)
	{
		for (auto& it : get_doc_infos_cb_list_)
		{
			(*(it.second))(count, doc_infos);
		}
	}
}

void DocManager::OnDeleteDocInfoCallback(const std::string& doc_id, const std::string& extern_info, int32_t code, const nim::DocTransInfo& doc_info)
{
	if (doc_id.empty())
	{
		QLOG_PRO(L"DocManager::OnDeleteDocInfoCallback operate={0}, code={1}, doc_id={2}") << extern_info << code << doc_info.id_;
	}
}

UnregisterCallback DocManager::RegDocInfoNotifyCallback(const nim::DocTrans::DocInfoCallback& callback)
{
	nim::DocTrans::DocInfoCallback* new_callback = new nim::DocTrans::DocInfoCallback(callback);
	int cb_id = (int)new_callback;

	doc_info_notify_cb_list_[cb_id].reset(new_callback);
	auto cb = ToWeakCallback([this, cb_id]() {
		doc_info_notify_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback DocManager::RegGetDocInfosCallback(const OnGetDocInfos& callback)
{
	OnGetDocInfos* new_callback = new OnGetDocInfos(callback);
	int cb_id = (int)new_callback;
	get_doc_infos_cb_list_[cb_id].reset(new_callback);
	auto cb = ToWeakCallback([this, cb_id]() {
		get_doc_infos_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback DocManager::RegUseDocCallback(const OnUseDoc& callback)
{
	OnUseDoc* new_callback = new OnUseDoc(callback);
	int cb_id = (int)new_callback;
	use_doc_cb_list_[cb_id].reset(new_callback);
	auto cb = ToWeakCallback([this, cb_id]() {
		use_doc_cb_list_.erase(cb_id);
	});
	return cb;
}

bool DocManager::InvokeUseDocCallback(const nim::DocTransInfo &doc_info)
{
	for (auto& it : use_doc_cb_list_)
	{
		(*(it.second))(doc_info);
	}
	bool flag = use_doc_cb_list_.size() > 0;
	return flag;
}

// �ϴ��ĵ�����
std::string DocManager::UploadDoc(const std::string& doc_id, std::string& file_path, const std::string& file_name, nim::NIMDocTranscodingFileType file_type)
{
	Json::Value value;
	value[nim::kNIMNosUploadType] = nim::kNIMNosUploadTypeDocTrans;
	value[nim::kNIMNosDocTransName] = file_name;
	value[nim::kNIMNosDocTransSourceType] = (nim::NIMDocTranscodingFileType)file_type;
	value[nim::kNIMNosDocTransPicType] = nim::kNIMDocTranscodingImageTypeJPG;
	value[nim::kNIMNosDocTransExt] = "";
	// �����ĵ���������
	value[nim::kNIMNosNeedContinueTrans] = true;
	// ִ������id������ǵ�һ���ϴ�����Ϊ�գ�sdk�ڻ�ȡtoken����Զ��������id
	value[nim::kNIMNosTaskId] = doc_id; 
	Json::FastWriter writer;

	std::string upload_id = QNimString::GetGUID();
	doc_upload_ids_.insert(upload_id);
	// ����������ϴ�����doc_id��Ϊ�գ���ʱ�ĵ�id���ϴ�id��
	if (!doc_id.empty())
	{
		assert(doc_id_upload_id_list_.find(doc_id) == doc_id_upload_id_list_.end());
		doc_id_upload_id_list_[doc_id] = upload_id;
	}

	nim::NOS::UploadResourceEx(file_path, writer.write(value),
		nbase::Bind(&DocManager::OnDocUploadCompeleteCallback, this, upload_id, std::placeholders::_1, std::placeholders::_2),
		nbase::Bind(&DocManager::OnDocUploadProgressCallback, this, upload_id, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	return upload_id;
}

std::string DocManager::GetUploadId(const std::string& doc_id)
{
	auto it = doc_id_upload_id_list_.find(doc_id);
	if (it != doc_id_upload_id_list_.end())
	{
		return it->second;
	}

	return "";
}

bool DocManager::RegUploadDocCallback(const std::string& upload_id, nim::NOS::UploadMediaExCallback compelete_cb, nim::NOS::ProgressExCallback progress_cb)
{
	auto it = doc_upload_ids_.find(upload_id);
	if (it != doc_upload_ids_.end())
	{
		doc_upload_cb_list_[upload_id] = std::make_pair(compelete_cb, progress_cb);
		return true;
	}
	else
	{
		return false;
	}
}

void DocManager::OnDocUploadCompeleteCallback(const std::string& upload_id, nim::NIMResCode res_code, const nim::UploadMediaResult& result)
{
	auto it = doc_upload_cb_list_.find(upload_id);
	assert(it != doc_upload_cb_list_.end());
	if (it != doc_upload_cb_list_.end())
	{
		auto callbacks = it->second;
		callbacks.first(res_code, result);
	}

	doc_upload_cb_list_.erase(upload_id);
	doc_upload_ids_.erase(upload_id);
	bool succeed = false;
	for (auto &iter:doc_id_upload_id_list_)
	{
		if (iter.second == upload_id)
		{
			succeed = true;
			doc_id_upload_id_list_.erase(iter.first);
			break;
		}
	}
	assert(succeed);
}

void DocManager::OnDocUploadProgressCallback(const std::string& upload_id, int64_t completed_size, int64_t file_size, const nim::ProgressData& data)
{
	auto it = doc_upload_cb_list_.find(upload_id);
	assert(it != doc_upload_cb_list_.end());
	if (it != doc_upload_cb_list_.end())
	{
		// ����ǵ�һ���ϴ�����completed_sizeΪ0ʱ��ȡ�ĵ�id���ϴ�id��
		if (completed_size == 0 && !data.res_id_.empty())
		{
			doc_id_upload_id_list_[data.res_id_] = upload_id;
		}

		auto callbacks = it->second;
		callbacks.second(completed_size, file_size, data);
	}
}

//�������
void DocManager::IsDownloadedDoc(const nim::DocTransInfo& doc_info, nim::NIMDocTranscodingQuality quality, int32_t page_num, const OnCheckDocDownloaded& cb)
{
	if (doc_info.id_.empty() || doc_info.page_num_ == 0)
	{
		qtbase::Post2Task(kThreadUIHelper, nbase::Bind(cb, false, 0));
		return;
	}

	StdClosure callback = ToWeakCallback([=]()
	{
		//std::wstring doc_dir = QPath::GetUserAppDataDir(LoginManager::GetInstance()->GetAccount());
		std::wstring doc_dir = QPath::GetUserAppDataDir("");
		doc_dir = doc_dir.append(L"doc_trans\\");
		doc_dir = doc_dir.append(nbase::UTF8ToUTF16(doc_info.id_));
		doc_dir = doc_dir.append(L"\\");
		if (!nbase::FilePathIsExist(doc_dir, true))
		{
			qtbase::Post2Task(kThreadUIHelper, nbase::Bind(cb, false, 0));
			return;
		}

		std::wstring ext;
		switch (doc_info.pic_type_)
		{
		case nim::kNIMDocTranscodingImageTypeJPG:
			ext = L".jpg";
			break;
		case nim::kNIMDocTranscodingImageTypePNG:
			ext = L".png";
			break;
		default:
		{
			qtbase::Post2Task(kThreadUIHelper, nbase::Bind(cb, false, 0));
			return;
		}
		}

		std::wstring path;
		if (page_num > 0)
		{
			path = nbase::StringPrintf(L"%s%d-%d%s", doc_dir.c_str(), quality, page_num, ext.c_str());
			if (!nbase::FilePathIsExist(path, false))
			{
				qtbase::Post2Task(kThreadUIHelper, nbase::Bind(cb, false, 0));
				return;
			}
		}
		else
		{
			int page_num = 0;
			for (int i = 1; i <= doc_info.page_num_; ++i)
			{
				path = nbase::StringPrintf(L"%s%d-%d%s", doc_dir.c_str(), quality, i, ext.c_str());
				if (nbase::FilePathIsExist(path, false))
				{
					page_num++;
				}
			}

			qtbase::Post2Task(kThreadUIHelper, nbase::Bind(cb, doc_info.page_num_ == page_num, page_num));
			return;
		}

		qtbase::Post2Task(kThreadUIHelper, nbase::Bind(cb, true, 1));
	});

	qtbase::Post2Task(kThreadUIHelper, callback);
}

void DocManager::DeleteDoc(const nim::DocTransInfo& doc_info, nim::NIMDocTranscodingQuality quality)
{
	StdClosure callback = ToWeakCallback([=]()
	{
		if (doc_info.id_.empty())
			return;

		//std::wstring doc_dir = QPath::GetUserAppDataDir(LoginManager::GetInstance()->GetAccount());
		std::wstring doc_dir = QPath::GetUserAppDataDir("");
		doc_dir = doc_dir.append(L"doc_trans\\");
		doc_dir = doc_dir.append(nbase::UTF8ToUTF16(doc_info.id_));
		doc_dir = doc_dir.append(L"\\");
		if (!nbase::FilePathIsExist(doc_dir, true))
			return;

		std::wstring ext;
		switch (doc_info.pic_type_)
		{
		case nim::kNIMDocTranscodingImageTypeJPG:
			ext = L".jpg";
			break;
		case nim::kNIMDocTranscodingImageTypePNG:
			ext = L".png";
			break;
		default:
			return;
		}

		std::wstring path;
		for (int i = 1; i <= doc_info.page_num_; ++i)
		{
			path = nbase::StringPrintf(L"%s%d-%d%s", doc_dir.c_str(), quality, i, ext.c_str());
			nbase::DeleteFileW(path);
		}

		::RemoveDirectory(doc_dir.c_str());
	});

	qtbase::Post2Task(kThreadUIHelper, callback);
}

std::wstring DocManager::GetDocPagePath(const nim::DocTransInfo& doc_info, int32_t page_num, nim::NIMDocTranscodingQuality quality/* = kNIMDocTranscodingQualityHigh*/)
{
	if (doc_info.id_.empty())
	{
		return L"";
	}
	//std::wstring doc_dir = QPath::GetUserAppDataDir(LoginManager::GetInstance()->GetAccount());
	std::wstring doc_dir = QPath::GetUserAppDataDir("");
	doc_dir = doc_dir.append(L"doc_trans\\");
	if (!nbase::FilePathIsExist(doc_dir, true))
	{
		nbase::win32::CreateDirectoryRecursively(doc_dir.c_str());
	}
	doc_dir = doc_dir.append(nbase::UTF8ToUTF16(doc_info.id_));
	doc_dir = doc_dir.append(L"\\");
	if (!nbase::FilePathIsExist(doc_dir, true))
	{
		nbase::win32::CreateDirectoryRecursively(doc_dir.c_str());
	}

	std::wstring ext;
	switch (doc_info.pic_type_)
	{
	case nim::kNIMDocTranscodingImageTypeJPG:
		ext = L".jpg";
		break;
	case nim::kNIMDocTranscodingImageTypePNG:
		ext = L".png";
		break;
	default:
		return L"";
	}
	std::wstring path = nbase::StringPrintf(L"%s%d-%d%s", doc_dir.c_str(), quality, page_num, ext.c_str());
	return path;
}

bool DocManager::DownloadDocPage(const nim::DocTransInfo& doc_info, int32_t page_num, nim::NIMDocTranscodingQuality quality, nim::NOS::DownloadMediaCallback cb)
{
	std::wstring path = GetDocPagePath(doc_info, page_num, quality);
	if (!path.empty() && nbase::FilePathIsExist(path, false))
	{
		if (cb)
		{
			qtbase::Post2Task(kThreadUIHelper, nbase::Bind(cb, nim::kNIMResSuccess, nbase::UTF16ToUTF8(path), "", ""));
		}
		return true;
	}
	std::string download_id = nbase::StringPrintf("%d_%d_%s", page_num, quality, doc_info.id_.c_str());
	auto it = doc_page_download_cb_list_.find(download_id);
	if (it != doc_page_download_cb_list_.end())
	{
		it->second.push_back(cb);
		return true;
	}
	if (doc_info.id_.empty() || doc_info.url_prefix_.empty() || doc_info.state_ != nim::kNIMDocTranscodingStateCompleted  || page_num < 1 || page_num > doc_info.page_num_)
	{
		return false;
	}
	std::string url = nim::DocTrans::GetPageUrl(doc_info.url_prefix_, doc_info.pic_type_, quality, page_num);
	if (url.empty())
	{
		return false;
	}
	nim::NOS::DownloadMediaCallback dl_cb = nbase::Bind(&DocManager::DownloadDocPageCb, this, download_id, path, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	std::vector<nim::NOS::DownloadMediaCallback> cb_list;
	cb_list.push_back(cb);
	doc_page_download_cb_list_[download_id] = cb_list;
	nim::NOS::DownloadResource(url, dl_cb);
	return true;
}

void DocManager::DownloadDocPageCb(const std::string& download_id, const std::wstring& save_path, nim::NIMResCode res_code, const std::string& file_path, const std::string& call_id, const std::string& res_id)
{
	if (res_code == nim::kNIMResSuccess)
	{
		bool ret = nbase::CopyFileW(nbase::UTF8ToUTF16(file_path), save_path);
		if (ret)
		{
			nbase::DeleteFileW(nbase::UTF8ToUTF16(file_path));
		}
	}
	auto it = doc_page_download_cb_list_.find(download_id);
	if (it != doc_page_download_cb_list_.end())
	{
		std::vector<nim::NOS::DownloadMediaCallback>& cb_list = it->second;
		for (auto cb : cb_list)
		{
			qtbase::Post2Task(kThreadUIHelper, nbase::Bind(cb, res_code, file_path, call_id, res_id));
		}
		doc_page_download_cb_list_.erase(it);
	}
}

}