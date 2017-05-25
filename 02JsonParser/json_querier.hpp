#pragma once

/////////////////////////////////////////////////////////////////////////
//
//JsonԪ�ز�ѯ��
//����Query()�������ո��ڵ�Ͳ�ѯ·����������ӦjsonԪ�أ���ѯʧ�ܷ���JsonType::JS_NULL
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
		/// ����·���ݹ��ѯjsonԪ�أ���ʧ�ܷ���NULL
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
		/// ѭ������·��
		/// e.g. ����·�� /abc/d[24] ������˳��Ϊ
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

	// ����Ҫ������ⲿ������̬���������򽫻�������´���
	/*
	������	����	˵��	��Ŀ	�ļ�	��	��ֹ��ʾ״̬
����	LNK2001	�޷��������ⲿ���� "private: static char * json::JsonQuery::buffer" (?buffer@JsonQuery@json@@0PADA)	JsonParser	D:\Documents\Visual Studio\Projects\CompilerExperiment\02JsonParser\main.obj	1
	*/
	char JsonQuery::buffer[64];

}
