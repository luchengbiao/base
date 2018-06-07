#include "qiniu_multi_upload.h"
#include "io.h"
#include "qiniu/resumable_io.h"
#include "qiniu/rs.h"
#include "qiniu/region.h"
#include <vector>
#include "json/cJSON.h"
#include <thread>
#include "common/tool/common_tool.h"

using namespace std;

typedef struct _Multi_Qiniu_Rio_PutProgress_Recvr {

	const char *progressFilePath;
	Qiniu_Int64 fsize;
	int blkCnt;
	vector<int> progress;
	qiniu_upload_progress_cb progress_notify;
	Qiniu_Rio_BlkputRet *blkputRets;
} Multi_Qiniu_Rio_PutProgress_Recvr;


static int BLOCK_SIZE = 4 * 1024 * 1024;
static const int BLOCK_CONTEXT_LENGTH = 196;

void QiniuMultiUpload::Init()
{
	Qiniu_Global_Init(-1);
}

void QiniuMultiUpload::Clean()
{
	Qiniu_Global_Cleanup();
}

char *ResumableProgressMarshal(Qiniu_Rio_BlkputRet *putRets, int blockCnt)
{
	char *progress;
	cJSON *root = cJSON_CreateArray();
	for (int i = 0; i < blockCnt; i++)
	{
		cJSON *item = cJSON_CreateObject();
		Qiniu_Rio_BlkputRet *blk = putRets + i;
		if (blk && blk->ctx)
		{
			cJSON_AddStringToObject(item, "host", blk->host);
			cJSON_AddStringToObject(item, "ctx", blk->ctx);
			cJSON_AddStringToObject(item, "checksum", blk->checksum);
			cJSON_AddNumberToObject(item, "crc32", blk->crc32);
			cJSON_AddNumberToObject(item, "offset", blk->offset);
		}
		else
		{
			cJSON_AddNullToObject(item, "host");
			cJSON_AddNullToObject(item, "ctx");
			cJSON_AddNullToObject(item, "checksum");
			cJSON_AddNumberToObject(item, "crc32", 0);
			cJSON_AddNumberToObject(item, "offset", 0);
		}

		cJSON_AddItemToArray(root, item);
	}

	progress = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	return progress;
}

int ResumableUploadNotify(void* recvr, int blkIdx, int blkSize, Qiniu_Rio_BlkputRet* ret)
{

	Multi_Qiniu_Rio_PutProgress_Recvr *pRecvr = (Multi_Qiniu_Rio_PutProgress_Recvr*)recvr;

	if (pRecvr->progress.size() < blkIdx + 1)
	{
		pRecvr->progress.push_back(ret->offset);
	}
	else if (blkIdx < pRecvr->progress.size())
	{
		pRecvr->progress[blkIdx] = ret->offset;
	}
	int sum = 0;
	for (int i = 0; i < pRecvr->progress.size(); i++)
	{
		sum += pRecvr->progress[i];
	}

	printf("Fsize: %lld, BlkIndex: %d, Offset: %d, progress: %.2lf\n", pRecvr->fsize, blkIdx, ret->offset, (sum * 1.0) / pRecvr->fsize);
	pRecvr->progress_notify(sum * 1.0);

	if (pRecvr->progressFilePath)
	{
		if (ret->offset % BLOCK_SIZE == 0 || blkIdx == pRecvr->blkCnt - 1)
		{
			printf("Write block %d progress\n", blkIdx);
			Qiniu_Rio_BlkputRet *blk = pRecvr->blkputRets + blkIdx;
			blk->checksum = _strdup(ret->checksum);
			blk->crc32 = ret->crc32;
			blk->ctx = _strdup(ret->ctx);
			blk->host = _strdup(ret->host);
			blk->offset = ret->offset;

			FILE *progressRecordHandle = fopen(pRecvr->progressFilePath, "wb+");
			if (progressRecordHandle)
			{
				char *progress = ResumableProgressMarshal(pRecvr->blkputRets, pRecvr->blkCnt);
				fwrite(progress, strlen(progress) + 1, 1, progressRecordHandle);
				fflush(progressRecordHandle);
				fclose(progressRecordHandle);
			}
		}
	}
	return QINIU_RIO_NOTIFY_OK;
}

int UploadProgressNotify(void* recvr, int blkIdx, int blkSize, Qiniu_Rio_BlkputRet* ret)
{
	Multi_Qiniu_Rio_PutProgress_Recvr *pRecvr = (Multi_Qiniu_Rio_PutProgress_Recvr*)recvr;

	if (pRecvr->progress.size() < blkIdx + 1)
	{
		pRecvr->progress.push_back(ret->offset);
	}
	else if (blkIdx < pRecvr->progress.size())
	{
		pRecvr->progress[blkIdx] = ret->offset;
	}
	int sum = 0;
	for (int i = 0; i < pRecvr->progress.size(); i++)
	{
		sum += pRecvr->progress[i];
	}

	printf("Fsize: %lld, BlkIndex: %d, Offset: %d, progress: %.2lf\n", pRecvr->fsize, blkIdx, ret->offset, (sum * 1.0) / pRecvr->fsize);
	pRecvr->progress_notify((sum * 1.0) / pRecvr->fsize);

	return QINIU_RIO_NOTIFY_OK;
}

void Debuginfo(Qiniu_Client* client, Qiniu_Error err)
{
	printf("\nerror code: %d, message: %s\n", err.code, err.message);
	printf("response header:\n%s", Qiniu_Buffer_CStr(&client->respHeader));
	printf("response body:\n%s", Qiniu_Buffer_CStr(&client->b));
	printf("\n\n\n");
}

bool QiniuMultiUpload::UploadFile(QiNiuUploadInfo info, const qiniu_upload_finished_cb& finished_cb)
{
	std::string file_name = "";
	std::string file_path = "";
	commontool::UnicodeToMBCS(info.file_name_, file_name);
	commontool::UnicodeToMBCS(info.file_path_, file_path);

	DoUpload(info.token_, info.base_url_, file_name, file_path, finished_cb);
	return true;
}

void QiniuMultiUpload::DoUpload(string& token, string& base, string& name, string& file, const qiniu_upload_finished_cb& cb)
{
	Qiniu_Client client;
	Qiniu_Zero(client);
	Qiniu_Servend_Init(-1);
	Qiniu_Rgn_Disable();
	Qiniu_Client_InitNoAuth(&client, 1024);

	bool ret = false;
	string msg;
	Qiniu_Io_PutRet putRet;
	//
	Qiniu_Error error = Qiniu_Io_PutFile(&client, &putRet, token.c_str(), name.c_str(), file.c_str(), NULL);
	if (error.code != 200)
	{
		//QLOG_ERR(L"Upload File {0}, error code: {1}, message: {2}") << file << error.code << error.message;
		//printf("Upload File %s error.\n", file.c_str());
		Debuginfo(&client, error);
		ret = false;
		msg = "upload error";
	}
	else
	{
		printf("Upload File %s success.\n", file.c_str());
		ret = true;
		msg = base + name;
	}

	Qiniu_Client_Cleanup(&client);

	if (cb)
	{
		cb(ret, msg);
	}
}

bool QiniuMultiUpload::UploadFile(QiNiuUploadInfo info, const qiniu_upload_progress_cb& progress_cb, const qiniu_upload_finished_cb& finished_cb)
{

	std::string file_name = "";
	std::string file_path = "";
	commontool::UnicodeToMBCS(info.file_name_, file_name);
	commontool::UnicodeToMBCS(info.file_path_, file_path);

	DoUploadFile(info.token_, info.base_url_, file_name, file_path, progress_cb, finished_cb);
	return true;
}

void QiniuMultiUpload::DoUploadFile(string& token, string& base, string& name, string& file, const qiniu_upload_progress_cb& progress_cb, const qiniu_upload_finished_cb& finished_cb)
{
	Qiniu_Client client;
	Qiniu_Zero(client);
	Qiniu_Servend_Init(-1);
	Qiniu_Rgn_Disable();
	Qiniu_Client_InitNoAuth(&client, 1024);

	Qiniu_RS_PutPolicy policy;
	Qiniu_Zero(policy);

	bool ret = false;
	string msg;

	int blockCnt = 0;

	Multi_Qiniu_Rio_PutProgress_Recvr putProgressRecvr;
	Qiniu_Rio_PutExtra putExtra;
	Qiniu_Zero(putProgressRecvr);
	Qiniu_Zero(putExtra);

	putProgressRecvr.fsize = getFileSzie(file.c_str());
	blockCnt = Qiniu_Rio_BlockCount(putProgressRecvr.fsize);
	putProgressRecvr.blkCnt = blockCnt;
	putProgressRecvr.blkputRets = (Qiniu_Rio_BlkputRet*)malloc(sizeof(Qiniu_Rio_BlkputRet)*blockCnt);
	for (int i = 0; i < blockCnt; i++)
	{
		Qiniu_Zero(*(putProgressRecvr.blkputRets + i));
		putProgressRecvr.progress.push_back(0);
	}

	putProgressRecvr.progress_notify = progress_cb;
	putExtra.notifyRecvr = &putProgressRecvr;
	putExtra.notify = UploadProgressNotify;
	Qiniu_Io_PutRet putRet;
	Qiniu_Zero(putRet);
	Qiniu_Error error = Qiniu_Rio_PutFile(&client, &putRet, token.c_str(), name.c_str(), file.c_str(), &putExtra);
	if (error.code != 200)
	{
		//QLOG_ERR(L"Upload File {0}, error code: {1}, message: {2}") << file << error.code << error.message;
		//printf("Upload File %s error.\n", file.c_str());
		Debuginfo(&client, error);
		ret = false;
		msg = "upload error";
	}
	else
	{
		printf("Upload File %s success.\n", file.c_str());
		ret = true;
		msg = base + name;
	}

	Qiniu_Client_Cleanup(&client);
	Qiniu_Servend_Cleanup();
	free(putProgressRecvr.blkputRets);

	if (finished_cb)
	{
		finished_cb(ret, msg);
	}
}

//bool QiniuMultiUpload::ResumableUploadFile(UploadVideoFile videofile, const qiniu_upload_progress_cb& progress_cb, const qiniu_upload_finished_cb& finished_cb)
//{
//	string upload_name = nbase::UTF16ToUTF8(videofile.fileName_);
//	string file = "";
//	nbase::win32::UnicodeToMBCS(videofile.filePath_, file);
//	auto cb = [=](ApiResult& api_result)
//	{
//		bool ret = false;
//		string msg;
//		if (api_result.Success())
//		{
//			string token = api_result.GetDataString("token");
//			std::thread task(std::bind(&QiniuMultiUpload::DoResumableUpload, this, token, upload_name, file, progress_cb, finished_cb));
//			task.detach();
//		}
//		else
//		{
//			ret = false;
//			msg = file;
//			if (finished_cb)
//			{
//				finished_cb(ret, msg);
//			}
//		}
//
//	};
//
//	//从服务器获取token
//	SS_MAP param;
//	param["courseId"] = nbase::Uint64ToString(videofile.m_course_id_);
//	if (videofile.s_course_type_ == "series-course")
//	{
//		param["itemId"] = nbase::Uint64ToString(videofile.m_item_id_);
//	}
//	param["filename"] = upload_name;
//	param["index"] = nbase::IntToString(videofile.video_index_);
//	//string ret_s_ = ApiCaller().CallGet(API_COURSE_VIDEO_UPLOAD_TOKEN, param, cb);
//
//	return true;
//}

void QiniuMultiUpload::DoResumableUpload(string uploadtoken, string name, string file, const qiniu_upload_progress_cb& progress_cb, const qiniu_upload_finished_cb& cb)
{
	Qiniu_Client client;
	Qiniu_Zero(client);
	Qiniu_Servend_Init(-1);
	Qiniu_Rgn_Disable();
	Qiniu_Client_InitNoAuth(&client, 1024);

	Qiniu_RS_PutPolicy policy;
	Qiniu_Zero(policy);

	bool ret = false;
	string msg;

	int blockCnt = 0;
	int blockIndex = 0;
	int progressFileLen = 0;
	int recordBlockCnt = 0;

	Multi_Qiniu_Rio_PutProgress_Recvr putProgressRecvr;
	Qiniu_Rio_PutExtra putExtra;
	Qiniu_Zero(putProgressRecvr);
	Qiniu_Zero(putExtra);

	putProgressRecvr.fsize = getFileSzie(file.c_str());
	blockCnt = Qiniu_Rio_BlockCount(putProgressRecvr.fsize);
	putProgressRecvr.blkCnt = blockCnt;
	putProgressRecvr.blkputRets = (Qiniu_Rio_BlkputRet*)malloc(sizeof(Qiniu_Rio_BlkputRet)*blockCnt);
	for (int i = 0; i < blockCnt; i++)
	{
		Qiniu_Zero(*(putProgressRecvr.blkputRets + i));
		putProgressRecvr.progress.push_back(0);
	}

	char *progressFilePath = GetProgressFilePath("jyxb-media", name.c_str(), file.c_str());
	putProgressRecvr.progressFilePath = progressFilePath;

	printf("Local progress file is %s\n", progressFilePath);
	//尝试读取本地进度
	FILE *progressRecordHandle = fopen(progressFilePath, "rb+");
	if (progressRecordHandle)
	{
		fseek(progressRecordHandle, 0l, SEEK_END);
		progressFileLen = ftell(progressRecordHandle);
		char *progressBuffer = (char*)malloc(sizeof(char)*(progressFileLen + 1));
		//reset
		fseek(progressRecordHandle, 0L, SEEK_SET);
		while (!feof(progressRecordHandle))
		{
			fread(progressBuffer, sizeof(char), progressFileLen, progressRecordHandle);
		}
		fclose(progressRecordHandle);
		progressBuffer[progressFileLen] = '\0';

		cJSON *root = cJSON_Parse(progressBuffer);
		int ctxCnt = cJSON_GetArraySize(root);
		for (int i = 0; i < ctxCnt; i++)
		{
			cJSON *item = cJSON_GetArrayItem(root, i);
			Qiniu_Rio_BlkputRet bputRet;
			bputRet.ctx = _strdup(Qiniu_Json_GetString(item, "ctx", NULL));
			bputRet.host = _strdup(Qiniu_Json_GetString(item, "host", NULL));
			bputRet.checksum = _strdup(Qiniu_Json_GetString(item, "checksum", NULL));
			bputRet.crc32 = Qiniu_Json_GetInt64(item, "crc32", 0);
			bputRet.offset = Qiniu_Json_GetInt64(item, "offset", 0);
			putProgressRecvr.blkputRets[i] = bputRet;

			if (i < putProgressRecvr.progress.size())
			{
				putProgressRecvr.progress[i] = bputRet.offset;
			}
		}

		putExtra.progresses = putProgressRecvr.blkputRets;
		putExtra.blockCnt = ctxCnt;
		cJSON_Delete(root);
		free(progressBuffer);
	}

	putProgressRecvr.progress_notify = progress_cb;
	putExtra.notifyRecvr = &putProgressRecvr;
	putExtra.notify = ResumableUploadNotify;
	Qiniu_Io_PutRet putRet;
	Qiniu_Zero(putRet);
	Qiniu_Error error = Qiniu_Rio_PutFile(&client, &putRet, uploadtoken.c_str(), name.c_str(), file.c_str(), &putExtra);
	if (error.code != 200)
	{
		//QLOG_ERR(L"Upload File {0}, error code: {1}, message: {2}") << file << error.code << error.message;
		//printf("Upload File %s error.\n", file.c_str());
		Debuginfo(&client, error);
		ret = false;
	}
	else
	{
		printf("Upload File %s success.\n", file.c_str());
		ret = true;
	}

	Qiniu_Client_Cleanup(&client);
	Qiniu_Servend_Cleanup();
	free(putProgressRecvr.blkputRets);

	msg = file;
	if (cb)
	{
		cb(ret, msg);
	}
	
}

__int64 QiniuMultiUpload::getFileSzie(const char *localFile)
{
	Qiniu_FileInfo fi;
	__int64 fsize = 0;
	Qiniu_Error error;
	Qiniu_File *f;

	error = Qiniu_File_Open(&f, localFile);
	if (error.code != 200)
	{
		fsize = 0;
	}
	else
	{
		error = Qiniu_File_Stat(f, &fi);
		if (error.code == 200) {
			fsize = Qiniu_FileInfo_Fsize(fi);
		}
	}

	return fsize;
}


char* QiniuMultiUpload::GetProgressFilePath(const char* bucket, const char* key, const char * localFilePath)
{
	//MD5_CTX ctx;
	//int dataLen = strlen(bucket) + strlen(key) + strlen(localFilePath) + 3;
	//char *tmpData = (char*)malloc(sizeof(char)*dataLen);
	//unsigned char md[16];
	char *buf = (char*)malloc(sizeof(char)* 256);
	/*memset(buf, 0, 256);
	char tmp[3] = { '\0' };
	int i = 0;
	sprintf(tmpData, "%s:%s:%s", bucket, key, localFilePath);
	tmpData[dataLen - 1] = '\0';

	MD5_Init(&ctx);
	MD5_Update(&ctx, tmpData, strlen(tmpData));
	MD5_Final(md, &ctx);
	string path(localFilePath);
	wstring wpath;
	nbase::win32::MBCSToUnicode(path, wpath);
	wstring dir = L"";
	nbase::FilePathApartDirectory(wpath, dir);
	path = "";
	nbase::win32::UnicodeToMBCS(dir, path);
	strcpy(buf, path.c_str());

	for (i = 0; i < 16; i++)
	{
		sprintf(tmp, "%02x", md[i]);
		strcat(buf, tmp);
	}
	strcat(buf, ".progress");

	free(tmpData);*/

	return buf;
}

