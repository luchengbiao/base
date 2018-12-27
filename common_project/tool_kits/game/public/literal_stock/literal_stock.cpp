#include "literal_stock.h"
#include <QFile>
#include "json/json.h"
#include "game/public/resourse_manager/resourse_manager.h"

GAME_NAMESPACE_BEGIN

LiteralStock::LiteralStock()
{
	ResourceManager::LoadResource();

	QFile file(R"(:/literal/res/literal/game.json)");
	if (file.open(QFile::ReadOnly))
	{
		auto content = file.readAll().toStdString();

		Json::Value root;
		Json::Reader reader;
		if (reader.parse(content, root))
		{
			auto keys = root.getMemberNames();
			for (const auto& key : keys)
			{
				const auto& value = root[key];
				literal_map_.emplace(key, value.isString() ? value.asString() : value.toStyledString());
			}
		}
	}
}

std::string	LiteralStock::StringByKey(const std::string& key) const
{
	auto it = literal_map_.find(key);

	return (it == literal_map_.cend()) ? "" : it->second;
}
	

GAME_NAMESPACE_END