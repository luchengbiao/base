#include "public_setting.h"

GAME_NAMESPACE_BEGIN

ClientType			PublicSetting::client_type_{ ClientType::None };
UsageType			PublicSetting::usage_type_{ UsageType::None };
GameType			PublicSetting::game_type_{ GameType::None };
long long			PublicSetting::user_id_{ 0 };
std::string			PublicSetting::account_id_;
std::string			PublicSetting::channel_account_;
NetworkServiceType	PublicSetting::channel_service_type_{ SWITCH_NETWORK_AGORA };
bool				PublicSetting::course_data_is_base64_encoded_{ true };
std::wstring		PublicSetting::default_dirctory_to_store_files_;
std::wstring		PublicSetting::dirctory_to_store_chess_manual_;
std::wstring		PublicSetting::dirctory_to_store_player_portrait_;
std::wstring		PublicSetting::dirctory_to_store_image_;
std::string			PublicSetting::suffix_of_head_portrait_url_;
One2OneCallInfo		PublicSetting::o2o_call_info_;

GAME_NAMESPACE_END