#include "base_chat_channel.h"

BaseChatChannel::BaseChatChannel(bool master)
{
	master_ = master;
	channel_id_ = "";
}

BaseChatChannel::~BaseChatChannel()
{

}
