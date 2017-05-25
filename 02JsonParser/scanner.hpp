#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"

using namespace std;

namespace json
{

	class Scanner
	{

	private:
		// a buffer restore all the characters from input stream
		string buffer;
		// point to current character
		char *cursor;
		// current position
		short curr_row, curr_col;
		// position of next character
		short next_row, next_col;
		// width of tab
		const int TAB_WIDTH = 4;

	public:
		void Init(ifstream &_In)
		{
			istreambuf_iterator<char> beg(_In), end;
			buffer = string(beg, end);
			_In.close();

			cursor = (char*)buffer.c_str();
			next_row = next_col = 1;
		}

	private:
		void count() {
			curr_row = next_row;
			curr_col = next_col;
			switch (*cursor)
			{
			case '\t':
				next_col += TAB_WIDTH + 1 - (next_col % TAB_WIDTH);
				break;
			case '\n':
				next_row++;
				next_col = 1;
				break;
			default:
				next_col++;
				break;
			}
		}

		inline char input() {
			count();
			return *cursor++;
		}

		inline void unput(char c) {
			next_row = curr_row;
			next_col = curr_col;
			--cursor;
		}

		void ReportErrorMsg(const char *_Format, char *para = NULL)
		{
			throw LexemeOrSyntaxException(curr_row, curr_col, _Format, para);
		}

		JsonType GetTokenType()
		{
			char c;
			int status = 0;
			while (true)
			{
				switch (status)
				{
				case 0:
					if ('"' == (c = input())) status = 1;
					else if ('+' == c || '-' == c) status = 3;
					else if (isdigit(c)) status = 4;
					else if (isalpha(c)) status = 10;
					else
						switch (c)
						{
						case '{': return JsonType::JS_LBRACE;
						case '}': return JsonType::JS_RBRACE;
						case '[': return JsonType::JS_LBRACKET;
						case ']': return JsonType::JS_RBRACKET;
						case ',': return JsonType::JS_COMMA;
						case ':': return JsonType::JS_COLON;
						case '\0': return JsonType::JS_NULL;
						case '.':
							if (isdigit(cursor[-1]) || isdigit(cursor[-2]) || isdigit(cursor[1]))
								ReportErrorMsg("parsing number error");
						default: ReportErrorMsg("unknown lexeme error", NULL); break;
						}
					break;
				case 1:
					if ('\\' == (c = input())) status = 2;
					else if ('"' == c) return JsonType::JS_STRING;
					else if ('\n' == c) ReportErrorMsg("expected <\">");
					else status = 1;
					break;
				case 2:
					if ('\n' == (c = input())) ReportErrorMsg("expected <\">");
					else status = 1;
					break;
				case 3:
					if (isdigit(c = input())) status = 4;
					else ReportErrorMsg("parse integer error");
					break;
				case 4:
					if (isdigit(c = input())) status = 4;
					else if ('.' == c) status = 5;
					else if ('e' == c || 'E' == c) status = 7;
					else { unput(c); return JsonType::JS_CONST_INT; }
					break;
				case 5:
					if (isdigit(c = input())) status = 6;
					else ReportErrorMsg("parse float error");
					break;
				case 6:
					if (isdigit(c = input())) status = 6;
					else if ('e' == c || 'E' == c) status = 7;
					else { unput(c); return JsonType::JS_CONST_DOUBLE; }
					break;
				case 7:
					if ('+' == (c = input()) || '-' == c) status = 8;
					else if (isdigit(c)) status = 9;
					else ReportErrorMsg("parse scientific number error");
					break;
				case 8:
					if (isdigit(c = input())) status = 9;
					else ReportErrorMsg("parse scientific number error");
					break;
				case 9:
					if (isdigit(c = input())) status = 9;
					else { unput(c); return JsonType::JS_CONST_DOUBLE; }
					break;
				case 10:
					if (isalpha(c = input())) status = 10;
					else { unput(c); return JsonType::JS_CONST_TRUE; }
					break;
				}
			}
		}

		string GetTokenString(const char *token_start)
		{
			char c = *cursor;
			*cursor = 0;
			string s = string(token_start);
			*cursor = c;
			return s;
		}

	public:
		Json GetToken()
		{
			char c;
			while ((c = input()) == '\t' || c == '\n' || c == ' ');
			unput(c);

			const char *token_start = cursor;

			JsonType type;
			string s;
			switch (type = GetTokenType())
			{
			case JsonType::JS_CONST_TRUE:
				s = GetTokenString(token_start);
				if (s == "true") return JsonType::JS_CONST_TRUE;
				else if (s == "false") return JsonType::JS_CONST_FALSE;
				else if (s == "null") return JsonType::JS_CONST_NULL;
				else ReportErrorMsg("unrecognized token <%s>", (char*)s.c_str());
			case JsonType::JS_CONST_INT:
				s = GetTokenString(token_start);
				return atoi(s.c_str());
			case JsonType::JS_CONST_DOUBLE:
				s = GetTokenString(token_start);
				return atof(s.c_str());
			case JsonType::JS_STRING:// -1  是为了去掉双引号
				c = cursor[-1]; cursor[-1] = 0; s = string(token_start + 1); cursor[-1] = c;
				return Json(s);
			case JsonType::JS_LBRACE:
			case JsonType::JS_RBRACE:
			case JsonType::JS_LBRACKET:
			case JsonType::JS_RBRACKET:
			case JsonType::JS_COMMA:
			case JsonType::JS_COLON:
				return type;
			default:
				return JsonType::JS_NULL;
			}
		}

		pair<short, short> GetCurrenPosition()
		{
			return make_pair(curr_row, curr_col);
		}

	};

}
