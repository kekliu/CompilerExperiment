#pragma once

#include "json.hpp"

namespace json
{

	class JsonFormat
	{

	public:
		//Json到字符串
		// type =0 tab缩进 =-1 无换行和缩进 >0 type个空格缩进
		// intentCount为缩进量
		static void json2string(Json &_json, string &_out_string, const int _type = 0, int _intent_count = 0)
		{
			switch (_json.type)
			{
			case json::JsonType::JS_OBJECT:
			{
				if (_json.members.size() == 0)
					_out_string.append("{}");
				else
				{
					_out_string.append("{");
					++_intent_count;
					for each (auto var in _json.members)
					{
						AddIntentChars(_out_string, _intent_count, _type);
						_out_string.append('"' + var.first + "\": ");
						json2string(var.second, _out_string, _type, _intent_count);
						_out_string.append(",");
					}
					// 删除最后一个逗号
					DeleteTheLastComma(_out_string);
					AddIntentChars(_out_string, --_intent_count, _type);
					_out_string.append("}");
				}
			}
			break;
			case json::JsonType::JS_ARRAY:
				if (_json.elements.size() == 0)
					_out_string.append("[]");
				else
				{
					_out_string.append("[");
					++_intent_count;
					for each (auto ele in _json.elements)
					{
						AddIntentChars(_out_string, _intent_count, _type);
						json2string(ele, _out_string, _type, _intent_count);
						_out_string.append(",");
					}
					// delete the last comma
					DeleteTheLastComma(_out_string);
					AddIntentChars(_out_string, --_intent_count, _type);
					_out_string.append("]");
				}
				break;
			case json::JsonType::JS_CONST_INT:
			case json::JsonType::JS_CONST_DOUBLE:
				_out_string.append(_json.GetValue());
				break;
			case json::JsonType::JS_STRING:
				_out_string.append('"' + _json.GetValue() + '"');
				break;
			case json::JsonType::JS_CONST_TRUE:
			case json::JsonType::JS_CONST_FALSE:
			case json::JsonType::JS_CONST_NULL:
				_out_string.append(_json.GetTypeName());
				break;
			default:
				break;
			}
		}

	private:
		// add newline and intent characters
		static void AddIntentChars(string &_strings, int _count, int _type = 0)
		{
			string s;
			if (_type < 0)	return;
			else if (_type == 0) s = "\t";
			else while (_type--) s.append(" ");

			_strings.append("\n");
			while (_count--)
				_strings.append(s);
		}

		// remove the last comma(,)
		static void DeleteTheLastComma(string &_string)
		{
			_ASSERT(_string.find(',') != string::npos);
			auto it = _string.end();
			while (*--it != ',');
			_string.erase(it);
		}

	};

}
