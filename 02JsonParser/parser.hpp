#pragma once

#include "json.hpp"
#include "scanner.hpp"

namespace json
{

	class Parser
	{
		vector<Json>::iterator m_curser;
		Scanner m_scanner;
		vector<string> m_error_msgs;
		vector<pair<short, short>> m_tok_postions;
		vector<pair<short, short>>::iterator m_current_pos;

	private:
		inline const vector<Json>::iterator get()
		{
			m_current_pos++;
			return m_curser++;
		}

		void ReportErrorMsg(const char *_Format, char *para = NULL)
		{
			throw LexemeOrSyntaxException(m_current_pos[-1].first, m_current_pos[-1].second, _Format, para);
		}

	private:
		Json object()
		{
			if (m_curser->type == JsonType::JS_LBRACE)
			{
				get();
				if (m_curser->type == JsonType::JS_RBRACE)
				{
					return JsonType::JS_OBJECT;
				}
				else
				{
					auto _members = members();
					if (m_curser->type == JsonType::JS_RBRACE)
						return _members;
					else
						ReportErrorMsg("expected <}>");
				}
			}
			else
			{
				ReportErrorMsg("expected <{>");
			}
		}

		vector<pair<string, Json>> members()
		{
			vector<pair<string, Json>> _members;
			_members.push_back(Pair());
			while (m_curser->type == JsonType::JS_COMMA)
			{
				get();
				_members.push_back(Pair());
			}
			return _members;
		}

		pair<string, Json> Pair()
		{
			if (m_curser->type == JsonType::JS_STRING)
			{
				string name = m_curser->s;
				get();
				if (m_curser->type == JsonType::JS_COLON)
				{
					get();
					return make_pair(name, value());
				}
				else
				{
					ReportErrorMsg("expected <:>");
				}
			}
			else
			{
				ReportErrorMsg("expected string as key");
			}
		}

		vector<Json> array()
		{
			if (m_curser->type == JsonType::JS_LBRACKET)
			{
				get();
				vector<Json> jslist;
				if (m_curser->type == JsonType::JS_RBRACKET)
					return jslist;
				else
				{
					jslist = elements();
					if (m_curser->type == JsonType::JS_RBRACKET)
					{
						return jslist;
					}
					else
					{
						ReportErrorMsg("expected <]>");
					}
				}
			}
		}

		vector<Json> elements()
		{
			vector<Json> jslist;
			jslist.push_back(value());
			while (m_curser->type == JsonType::JS_COMMA)
			{
				get();
				jslist.push_back(value());
			}
			return jslist;
		}

		Json value()
		{
			Json js = *m_curser;
			switch (m_curser->type)
			{
			case JsonType::JS_STRING:
			case JsonType::JS_CONST_INT:
			case JsonType::JS_CONST_DOUBLE:
			case JsonType::JS_CONST_TRUE:
			case JsonType::JS_CONST_FALSE:
			case JsonType::JS_CONST_NULL:
				break;
			case JsonType::JS_LBRACE:
				js = object();
				break;
			case JsonType::JS_LBRACKET:
				js = array();
				break;
			default:
				js = JsonType::JS_NULL;
				break;
			}
			get();
			return js;
		}

	public:
		Json Parse(ifstream &_In)
		{
			vector<Json> token_list;
			Json js;

			m_scanner.Init(_In);

			// 词法分析，获得tokenlist，利用try 捕捉所有的词法错误
			int is_lexeme_exception_exist = 0;
			while (1)
			{
				try
				{
					++is_lexeme_exception_exist;
					js = m_scanner.GetToken();
					--is_lexeme_exception_exist;
					m_tok_postions.push_back(m_scanner.GetCurrenPosition());
				}
				catch (LexemeOrSyntaxException &e)
				{
					m_error_msgs.push_back(e.what());
				}
				if (js.type == JsonType::JS_NULL) break;
				token_list.push_back(js);
			}
			token_list.push_back(JsonType::JS_NULL);

			// 词法分析失败则不进行语法分析
			if (is_lexeme_exception_exist)
				return JsonType::JS_NULL;

			try
			{
				m_curser = token_list.begin();
				m_current_pos = m_tok_postions.begin();
				js = object();
				return js;
			}
			catch (LexemeOrSyntaxException &e)
			{
				m_error_msgs.push_back(e.what());
				return JsonType::JS_NULL;
			}

		}

		const vector<string> GetErrorMsgs()
		{
			return m_error_msgs;
		}

	};

}
