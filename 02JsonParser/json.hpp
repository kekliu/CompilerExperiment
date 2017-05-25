#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <exception>
#include "json_type.hpp"
#include "LexemeOrSyntaxException.hpp"

using namespace std;

namespace json
{

	struct Json
	{
		JsonType type;
		// INTEGER
		int i;
		// DOUBLE
		double d;
		// STRING
		string s;
		// ARRAY
		vector<Json> elements;
		// OBJECT
		vector<pair<string, Json>> members;

		Json() { }
		Json(JsonType _type) :type(_type) { }
		Json(int _i) :type(JsonType::JS_CONST_INT), i(_i) { }
		Json(double _d) :type(JsonType::JS_CONST_DOUBLE), d(_d) { }
		Json(string _s) :type(JsonType::JS_STRING), s(_s) { }
		Json(vector<Json> _elements) :type(JsonType::JS_ARRAY), elements(_elements) { }
		Json(vector<pair<string, Json>> _members) :type(JsonType::JS_OBJECT), members(_members) { }

		string GetTypeName() const { return string(JsonTypeNames(type)); }

		string GetValue() const
		{
			char buffer[64];
			switch (type)
			{
			case json::JsonType::JS_OBJECT:                                                 return "not defined yet";
			case json::JsonType::JS_ARRAY:                                                  return "not defined yet";
			case json::JsonType::JS_CONST_INT: sprintf_s(buffer, "%i", i);				    return buffer;
			case json::JsonType::JS_CONST_DOUBLE: sprintf_s(buffer, "%g", d);               return buffer;
			case json::JsonType::JS_STRING:				                                    return s;
			default:				                                                        return "";
			}
		}
	};

}
