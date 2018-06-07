#include "api_result.h"
#include "log_manager\log.h"

class ApiResult::ApiResultPrivate
{
	friend class ApiResult;

private:
	ApiResultPrivate() 
		:b_parse_success(false)
	{};

	ApiResultPrivate(const Json::Value& value)
		: code(value["code"].asInt())
		, msg(value["msg"].asString())
		, data(value["data"])
		, otherMsg(value["otherMsg"].asString())
		, extraData(value["extraData"])
		, b_parse_success(true)
	{}

public:
	~ApiResultPrivate()
	{};

private:
	bool				Success(){ return code == API_SUCCESS && b_parse_success; }
	bool				ParseSuccess(){ return b_parse_success; }
	int					GetCode(){ return code; }
	std::string			GetMsg(){ return msg; }
	std::string			GetOtherMsg(){ return otherMsg; }
	const Json::Value&	GetData()const{ return data; }
	const Json::Value&	GetExtraData()const{ return extraData; }

	int					GetDataInt(const char* key)const{ return data[key].asInt(); }
	double				GetDataDouble(const char* key)const{ return data[key].asDouble(); }
	bool				GetDataBool(const char* key){ return data[key].asBool(); }
	std::string			GetDataString(const char* key)const{ return data[key].asString(); }
	const Json::Value&	GetDataJsonValue(const char* key) const { return data[key]; }
	uint64_t			GetDataLong(const char* key)const{ return data[key].asInt64(); }

	bool				KeyDataNull(const char* key){ return data[key].isNull(); }
	bool				HasKey(const char* key){ return data.isMember(key); }

	void				SetMsg(std::wstring msg_) { msg = nbase::UTF16ToUTF8(msg_); }
	void				SetCode(int c_) { code = c_; }
	bool				NetFailed(){ return code < 0; }

private:
	int					code;
	std::string			msg;
	std::string			otherMsg;
	Json::Value			data;
	Json::Value			extraData;
	bool				b_parse_success;
};

ApiResult::ApiResult()
	: _ptr(new ApiResultPrivate())
{}

ApiResult::ApiResult(const Json::Value& value)
	: _ptr(new ApiResultPrivate(value))
{
}

ApiResult::ApiResult(ApiResult&& right)
	: _ptr(right.Release())
{}

ApiResult::ApiResult(const ApiResult& right, bool moveRight/* = true*/)
	: _ptr(moveRight ? (const_cast<ApiResult&>(right).Release()) : (right._ptr ? (new ApiResultPrivate(*right._ptr)) : (new ApiResultPrivate())))
{}

ApiResult::~ApiResult()
{
	if (_ptr)
	{
		delete _ptr;
	}
}

ApiResult& ApiResult::operator=(ApiResult&& right)
{
	if (this != &right)
	{
		this->Reset(right.Release());
	}

	return (*this);
}

ApiResult::ApiResultPrivate* ApiResult::Get()
{
	return _ptr;
}

void ApiResult::Reset(ApiResultPrivate* ptr)
{
	if (_ptr != ptr)
	{
		delete _ptr;

		_ptr = ptr;
	}
}

ApiResult::ApiResultPrivate* ApiResult::Release()
{
	auto ptr = _ptr;
	_ptr = nullptr;

	return ptr;
}

void ApiResult::Parse(std::string s_response_, ApiResult& api_result)
{
	Json::Reader j_reader;
	Json::Value j_result_;

	auto ptr = api_result.Get();
	if (ptr == nullptr)
	{
		api_result.Reset(ptr = new ApiResultPrivate);
	}

	if (ptr)
	{
		if (j_reader.parse((char*)s_response_.c_str(), j_result_))
		{
			ptr->code = j_result_["code"].asInt();
			ptr->msg = j_result_["msg"].asString();
			ptr->data = j_result_["data"];
			ptr->otherMsg = j_result_["otherMsg"].asString();
			ptr->extraData = j_result_["extraData"];
			ptr->b_parse_success = true;
		}
		else
		{
			QLOG_ERR(L"parse failed: {0}") << s_response_;
			ptr->code = -1;
			ptr->SetMsg(L"·þÎñÆ÷Î´ÏìÓ¦");
			ptr->b_parse_success = false;
		}
	}
}

bool ApiResult::Success()
{
	return _ptr ? _ptr->Success() : false;
}

bool ApiResult::ParseSuccess()
{ 
	return _ptr ? _ptr->ParseSuccess() : false;
}

int ApiResult::GetCode()
{
	return _ptr ? _ptr->GetCode() : 0;
}

std::string ApiResult::GetMsg()
{
	return _ptr ? _ptr->GetMsg() : "";
}

std::string ApiResult::GetOtherMsg()
{
	return _ptr ? _ptr->GetOtherMsg() : "";
}

static const Json::Value s_emptyJson;
const Json::Value&	ApiResult::GetData() const
{
	return _ptr ? _ptr->GetData() : s_emptyJson;
}

const Json::Value& ApiResult::GetExtraData()const
{
	return _ptr ? _ptr->GetExtraData() : s_emptyJson;
}

int ApiResult::GetDataInt(const char* key)const
{
	return _ptr ? _ptr->GetDataInt(key) : 0;
}

double ApiResult::GetDataDouble(const char* key)const
{
	return _ptr ? _ptr->GetDataDouble(key) : 0;
}

bool ApiResult::GetDataBool(const char* key)
{
	return _ptr ? _ptr->GetDataBool(key) : false;
}

std::string ApiResult::GetDataString(const char* key)const
{
	return _ptr ? _ptr->GetDataString(key) : "";
}

const Json::Value& ApiResult::GetDataJsonValue(const char* key) const
{
	return _ptr ? _ptr->GetDataJsonValue(key) : s_emptyJson;
}

uint64_t ApiResult::GetDataLong(const char* key)const
{
	return _ptr ? _ptr->GetDataLong(key) : 0;
}


bool ApiResult::KeyDataNull(const char* key)
{
	return _ptr ? _ptr->KeyDataNull(key) : false;
}

bool ApiResult::HasKey(const char* key)
{
	return _ptr ? _ptr->HasKey(key) : false;
}


void ApiResult::SetMsg(std::wstring msg_)
{
	if (_ptr)
	{
		_ptr->SetMsg(msg_);
	}
}

void ApiResult::SetCode(int c_)
{
	if (_ptr)
	{
		_ptr->SetCode(c_);
	}
}

bool ApiResult::NetFailed()
{
	return _ptr ? _ptr->NetFailed() : false;
}
