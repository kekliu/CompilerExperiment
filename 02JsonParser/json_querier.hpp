#pragma once

/////////////////////////////////////////////////////////////////////////
//
//Json元素查询类
//其中Query()方法接收根节点和查询路径，返回相应json元素，查询失败返回JsonType::JS_NULL
//
/////////////////////////////////////////////////////////////////////////

#include "Json.hpp"

using namespace std;

namespace json
{

	class JsonQuery
	{
		static char buffer[64];

	public:
		//////////////////////////////////////////////////////////////
		/// 根据路径递归查询json元素，若失败返回NULL
		//////////////////////////////////////////////////////////////
		static Json Query(Json &root, const char *_Path)
		{
			int isInt = 0;
			char *buffer;
			if (buffer = GetNextPathEle(_Path, isInt))
			{
				if (isInt)
				{
					size_t i = atoi(buffer);
					if (root.type == JsonType::JS_ARRAY && i > 0 && i <= root.elements.size())
						return *_Path ? Query(root.elements[i], _Path) : root.elements[i];
				}
				else
				{
					if (root.type == JsonType::JS_OBJECT)
					{
						for (auto it = root.members.begin(); it != root.members.end(); ++it)
							if (it->first == string(buffer))
								return *_Path ? Query(it->second, _Path) : it->second;
					}
				}
			}
			return JsonType::JS_NULL;
		}

	private:
		/////////////////////////////////////////////////////////
		/// 循环解析路径
		/// e.g. 对于路径 /abc/d[24] ，解析顺序为
		///		abc
		///		d
		///		24
		////////////////////////////////////////////////////////
		static char *GetNextPathEle(const char *&p, int &isInteger)
		{

			int i = 0;
			if (*p == '[')
			{
				++p;
				while ((buffer[i++] = *p++) != ']')
					if (!isdigit(buffer[i - 1]))
						return NULL;
				buffer[i - 1] = 0;
				isInteger = true;
				return buffer;
			}
			else if (*p == '/')
			{
				while (*p == '/') p++;
				while (*p != '/' && *p != '[' && *p)
					buffer[i++] = *p++;
				buffer[i++] = 0;
				isInteger = false;
				return buffer;
			}
			else
				return NULL;
		}

	};

	// 必须要在类的外部声明静态变量，否则将会出现以下错误：
	/*
	严重性	代码	说明	项目	文件	行	禁止显示状态
错误	LNK2001	无法解析的外部符号 "private: static char * json::JsonQuery::buffer" (?buffer@JsonQuery@json@@0PADA)	JsonParser	D:\Documents\Visual Studio\Projects\CompilerExperiment\02JsonParser\main.obj	1
	*/
	char JsonQuery::buffer[64];

}
