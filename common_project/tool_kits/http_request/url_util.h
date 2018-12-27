#pragma once
#include <string>
#include <unordered_map>

std::string urlencode(const std::string& input);

template<typename Str2StrMapT>  // Str2StrMapT can be map<string, string>, multimap<string, string>, unordered_map<string, string>, unordered_multimap<string, string>, etc...
std::string formatToUrlParams(Str2StrMapT&& ss_map)
{
	std::string url_params;

	int index = 0;
	for (const auto& pair : std::forward<Str2StrMapT>(ss_map))
	{
		url_params.append((index == 0) ? "?" : "&");

		url_params.append(pair.first).append("=").append(pair.second);

		index += 1;
	}

	return url_params;
}