#ifndef __JSON_DEF_H__
#define __JSON_DEF_H__

#define JSON_FIELD_AS_INT(target, field, json) { const auto& field = json[#field]; if (field.isConvertibleTo(Json::intValue)) { target = field.asInt(); } }

#define JSON_FIELD_AS_UINT(target, field, json) { const auto& field = json[#field]; if (field.isConvertibleTo(Json::uintValue)) { target = field.asUInt(); } }

#define JSON_FIELD_AS_INT64(target, field, json) { const auto& field = json[#field]; if (field.isConvertibleTo(Json::uintValue)) { target = field.asInt64(); } }

#define	JSON_FIELD_AS_UINT64(target, field, json) { const auto& field = json[#field]; if (field.isConvertibleTo(Json::uintValue)) { target = field.asUInt64(); } }

#define JSON_FIELD_AS_FLOAT(target, field, json) { const auto& field = json[#field]; if (field.isConvertibleTo(Json::realValue)) { target = field.asFloat(); } }

#define	JSON_FIELD_AS_DOUBLE(target, field, json) { const auto& field = json[#field]; if (field.isConvertibleTo(Json::realValue)) { target = field.asDouble(); } }

#define	JSON_FIELD_AS_BOOLEAN(target, field, json) { const auto& field = json[#field]; if (field.isConvertibleTo(Json::booleanValue)) { target = field.asBool(); } }

#define JSON_FIELD_AS_STRING(target, field, json) { const auto& field = json[#field]; if (field.isConvertibleTo(Json::stringValue)) { target = field.asString(); } }

#define JSON_FIELD_AS_WSTRING(target, field, json) { const auto& field = json[#field]; if (field.isConvertibleTo(Json::stringValue)) { target = nbase::UTF8ToUTF16(field.asString()); } }

#define JSON_FIELD_AS_ENUM(target, field, json, enumType) { const auto& field = json[#field]; if (field.isConvertibleTo(Json::intValue)) { target = static_cast<enumType>(field.asInt()); } }

#define FOREACH_JSON_ARRAY(variable, jsonArray) for (const auto& variable : jsonArray)

#define FOREACH_IF_JSON_IS_ARRAY(variable, json) if (json.isArray()) FOREACH_JSON_ARRAY(variable, json)

#define FOREACH_IF_JSON_FIELD_IS_ARRAY(variable, field, json) FOREACH_IF_JSON_IS_ARRAY(variable, static_cast<const Json::Value&>(json[#field]))

#endif