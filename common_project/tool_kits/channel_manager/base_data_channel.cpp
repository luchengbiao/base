#include "base_data_channel.h"

BaseDataChannel::BaseDataChannel(bool masetr, QObject* parent) :QObject(parent)
{
	b_master_ = masetr;
}

BaseDataChannel::~BaseDataChannel()
{

}
