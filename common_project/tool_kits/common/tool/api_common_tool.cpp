#include "api_common_tool.h"

std::string ApiCommonTool::ComposeParamArray(std::string key, std::vector<std::string> value)
{
	std::string v("");
	bool first_ = true;
	std::string key_str = "&" + key + "=";
	for (auto item : value)
	{
		std::string item_str = item;
		if (first_)
		{
			v.append(item_str);
			first_ = false;
		}
		else
		{
			v.append(key_str).append(item_str);
		}
	}

	return v;
}