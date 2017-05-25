#pragma once

///////////////////////////////////////////////////////////////////////////////////////
//
//辅助函数文件，本文件中含有以下方法：
//整数解析
//浮点数解析
//字符串到转义字符
//转义字符到字符串
//
//注意：
//数值解析部分不检查数字的合法性（合法性检查在Scanner中做）
//
///////////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <string>

using namespace std;

inline int char2int(char c)
{
	return c - '0';
}

// 整数解析
int ParseInt(char *p)
{
	int is_negative = (*p == '-');
	if (*p == '-' || *p == '+') p++;
	int sum = 0;
	while (*p)
		sum = sum * 10 + char2int(*p++);
	return is_negative ? -sum : sum;
}

// 浮点数（含科学计数法）解析
double ParseDouble(char *p)
{
	//char buffer[128]; strcpy_s(buffer, p); p = buffer;
	char *ip, *fp, *sp;
	ip = fp = sp = p;

	while (*fp != '.') fp++;
	while (*sp != 'e' && *sp != 'E' && *sp) sp++;

	return *sp ?
		(*fp = *sp = 0, (ParseInt(ip) + ParseInt(fp + 1) / pow(10, (sp - fp - 1)))* pow(10, ParseInt(sp + 1))) // 科学计数法
		:
		(*fp = 0, ParseInt(ip) + ParseInt(fp + 1) / pow(10, (sp - fp - 1))) // 一般浮点数
		;
}

string& replace_all_distinct(string& str, const string& old_value, const string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return str;
}

// "\\n" to "\n"
string escape2string(string &_Src)
{
	string::size_type pos(0);
	replace_all_distinct(_Src, "\\\\", "\\");
	replace_all_distinct(_Src, "\\b", "\b");
	replace_all_distinct(_Src, "\\\"", "\"");
	replace_all_distinct(_Src, "\\f", "\f");
	replace_all_distinct(_Src, "\\n", "\n");
	replace_all_distinct(_Src, "\\r", "\r");
	replace_all_distinct(_Src, "\\t", "\t");

	return _Src;
}

// "\n" to "\\n"
string string2escape(string &_Dest, char *_Src)
{
	return "";
}
