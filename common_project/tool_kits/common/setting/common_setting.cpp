#include "common_setting.h"
#include <string>
#include <algorithm>
#include "nim_base\util.h"
#include "base\file\file_util.h"
#include "log\log.h"
#include "base\system\api_setting.h"
#include "base\util\string_number_conversions.h"

#define EMOTICON_EMOTICON_NAME	"Name"
#define EMOTICON_EMOTICON_VALUE	"Value"

CommonSetting::CommonSetting()
{

}

CommonSetting::~CommonSetting()
{

}

void CommonSetting::Load()
{
	QDomDocument doc("ProjectSetting");
	QFile file("res\\setting.xml");

	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}

	do 
	{
		if (!doc.setContent(&file))
		{
			LOG_ERR(L"load xml fail");
			break;
		}

		QDomElement root = doc.documentElement();

		QDomElement usingElement = root.firstChildElement(QString("Using"));
		if (!usingElement.isElement())
		{
			break;
		}

		QString usingName = usingElement.text();

		QDomNode node = root.firstChild();

		while (!node.isNull())
		{
			if (node.isElement() && node.toElement().tagName() == usingName)
			{
				ParseKey(node.firstChild());
				break;
			}

			node = node.nextSibling();
		}

		auto iter = m_setting_pair.find("API_HOST");
		if (iter != m_setting_pair.end())
		{
			ApiSetting::api_host_ = iter->second;
		}

		iter = m_setting_pair.find("API_PORT");
		if (iter != m_setting_pair.end())
		{
			nbase::StringToInt(iter->second, &ApiSetting::api_port_);
		}

		iter = m_setting_pair.find("API_PPT_HOST");
		if (iter != m_setting_pair.end())
		{
			ApiSetting::SetOtherApiHost("API_PPT_HOST", iter->second);
		}

		iter = m_setting_pair.find("API_SERVER_HOST");
		if (iter != m_setting_pair.end())
		{
			ApiSetting::SetOtherApiHost("API_SERVER_HOST", iter->second);
		}

		iter = m_setting_pair.find("NIM_APPKEY");
		if (iter != m_setting_pair.end())
		{
			ApiSetting::SetAppKey("NIM_APPKEY", iter->second);
		}
		iter = m_setting_pair.find("AGORA_APP_ID");
		if (iter != m_setting_pair.end())
		{
			ApiSetting::SetAppKey("AGORA_APP_ID", iter->second);
		}
		iter = m_setting_pair.find("SOCKET_HOST");
		if (iter != m_setting_pair.end())
		{
			ApiSetting::api_socket_ = iter->second;
		}
		iter = m_setting_pair.find("URL_HEAD");
		if (iter != m_setting_pair.end())
		{
			ApiSetting::url_head_ = iter->second;
		}

	} while (false);

	file.close();
}

void CommonSetting::ParseKey(QDomNode childs)
{
	while (!childs.isNull())
	{
		QDomElement element = childs.toElement();
		QString name = element.attribute(EMOTICON_EMOTICON_NAME);
		QString value = element.attribute(EMOTICON_EMOTICON_VALUE);
		m_setting_pair[name.toStdString()] = value.toStdString();
		childs = childs.nextSibling();
	}
	
}
