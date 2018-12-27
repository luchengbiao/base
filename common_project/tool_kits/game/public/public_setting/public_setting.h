#ifndef __GAME_PUBLIC_PUBLIC_SETTING_PUBLIC_SETTING_H__
#define __GAME_PUBLIC_PUBLIC_SETTING_PUBLIC_SETTING_H__
#include <string>
#include "channel_manager/channel_manager_global.h"
#include "invite_manager/invite_manager_global.h"
#include "game/public/enums/client_type.h"
#include "game/public/enums/game_type.h"
#include "game/public/enums/usage_type.h"

GAME_NAMESPACE_BEGIN

class PublicSetting
{
public:
	inline static void						SetClientType(ClientType client_type) { client_type_ = client_type; }
	inline static void						SetUsageType(UsageType usage_type) { usage_type_ = usage_type; }
	inline static void						SetGameType(GameType game_type) { game_type_ = game_type; }

	inline static void						SetUserId(long long user_id) { user_id_ = user_id; }
	inline static void						SetAccountId(const std::string& account_id) { account_id_ = account_id; }

	inline static void						SetChannelAccount(const std::string& channel_account) { channel_account_ = channel_account; }
	inline static void						SetChannelServiceType(NetworkServiceType service_type) { channel_service_type_ = service_type; }

	inline static void						SetCourseDataIsBase64Encoded(bool base64_encoded) { course_data_is_base64_encoded_ = base64_encoded; }

	inline static void						SetDefaultDirctoryToStoreFiles(const std::wstring& dir) { default_dirctory_to_store_files_ = dir; }
	inline static void						SetDirctoryToStoreChessManual(const std::wstring& dir) { dirctory_to_store_chess_manual_ = dir; }
	inline static void						SetDirctoryToStorePlayerPortrait(const std::wstring& dir) { dirctory_to_store_player_portrait_ = dir; }
	inline static void						SetDirctoryToStoreImage(const std::wstring& dir) { dirctory_to_store_image_ = dir; }

	inline static void						SetSuffixOfHeadPortraitUrl(const std::string& suffix) { suffix_of_head_portrait_url_ = suffix; }

	inline static void						SetO2OCallInfo(const One2OneCallInfo& o2o_call_info) { o2o_call_info_ = o2o_call_info; }

private:
	PublicSetting() = delete;

private:
	static ClientType						client_type_;
	static UsageType						usage_type_;
	static GameType							game_type_;

	static long long						user_id_;
	static std::string						account_id_;

	static std::string						channel_account_;
	static NetworkServiceType				channel_service_type_;

	static bool								course_data_is_base64_encoded_;

	static std::wstring						default_dirctory_to_store_files_;
	static std::wstring						dirctory_to_store_chess_manual_;
	static std::wstring						dirctory_to_store_player_portrait_;
	static std::wstring						dirctory_to_store_image_;

	static std::string						suffix_of_head_portrait_url_;

	static One2OneCallInfo					o2o_call_info_;

	friend inline ClientType				GetClientType();
	friend inline bool						ClientTypeIs(const ClientType client_type);

	friend inline UsageType					GetUsageType();
	friend inline bool						UsageTypeIs(const UsageType usage_type);

	friend inline GameType					GetGameType();
	friend inline bool						GameTypeIs(const GameType game_type);

	friend inline long long					UserId();
	friend inline const std::string&		AccountId();

	friend inline const std::string&		ChannelAccount();
	friend inline NetworkServiceType		ChannelServiceType();

	friend inline bool						CourseDataIsBase64Encoded();

	friend inline const std::wstring&		DefaultDirctoryToStoreFiles();
	friend inline const std::wstring&		DirctoryToStoreChessManual();
	friend inline const std::wstring&		DirctoryToStorePlayerPortrait();
	friend inline const std::wstring&		DirctoryToStoreImage();

	friend inline std::string				PlayerPortraitUrl(const std::string& raw_url);

	friend inline const One2OneCallInfo&	O2OCallInfo();
};

inline ClientType							GetClientType() { return PublicSetting::client_type_; }
inline bool									ClientTypeIs(const ClientType client_type) { return PublicSetting::client_type_ == client_type; }
inline bool									ClientTypeIsTeacher() { return ClientTypeIs(ClientType::Teacher); }
inline bool									ClientTypeIsStudent() { return ClientTypeIs(ClientType::Student); }

inline UsageType							GetUsageType() { return PublicSetting::usage_type_; }
inline bool									UsageTypeIs(const UsageType usage_type) { return PublicSetting::usage_type_ == usage_type; }
inline bool									UsageTypeIsPlayback() { return UsageTypeIs(UsageType::Playback_O2O) || UsageTypeIs(UsageType::Playback_Class); }
inline bool									UsageTypeIsOnline() { return UsageTypeIs(UsageType::Online_O2O) || UsageTypeIs(UsageType::Online_Class); }
inline bool									UsageTypeIsO2O() { return UsageTypeIs(UsageType::Online_O2O) || UsageTypeIs(UsageType::Playback_O2O); }
inline bool									UsageTypeIsClass() { return UsageTypeIs(UsageType::Online_Class) || UsageTypeIs(UsageType::Playback_Class); }
inline bool									UsageTypeIsO2OOnline() { return UsageTypeIs(UsageType::Online_O2O); }
inline bool									UsageTypeIsClassOnline() { return UsageTypeIs(UsageType::Online_Class); }

inline GameType								GetGameType() { return PublicSetting::game_type_; }
inline bool									GameTypeIs(const GameType game_type) { return PublicSetting::game_type_ == game_type; }
inline bool									GameTypeIsChess() { return GameTypeIs(GameType::Chess); }
inline bool									GameTypeIsGo() { return GameTypeIs(GameType::Go); }
 
inline long long							UserId() { return PublicSetting::user_id_; }
inline const std::string&					AccountId() { return PublicSetting::account_id_; }

inline const std::string&					ChannelAccount() { return PublicSetting::channel_account_; }
inline NetworkServiceType					ChannelServiceType() { return PublicSetting::channel_service_type_; }
inline bool									ChannelMessageIsEncoded() { return ChannelServiceType() != SWITCH_NETWORK_NETEASE; }

inline bool									CourseDataIsBase64Encoded() { return PublicSetting::course_data_is_base64_encoded_; }

inline const std::wstring&					DefaultDirctoryToStoreFiles() { return PublicSetting::default_dirctory_to_store_files_; }
inline const std::wstring&					DirctoryToStoreChessManual() { return PublicSetting::dirctory_to_store_chess_manual_; }
inline const std::wstring&					DirctoryToStorePlayerPortrait() { return PublicSetting::dirctory_to_store_chess_manual_; }
inline const std::wstring&					DirctoryToStoreImage() { return PublicSetting::dirctory_to_store_chess_manual_; }

inline std::string							PlayerPortraitUrl(const std::string& raw_url) { return raw_url + PublicSetting::suffix_of_head_portrait_url_; }

inline const One2OneCallInfo&				O2OCallInfo() { return PublicSetting::o2o_call_info_; }

GAME_NAMESPACE_END

#endif