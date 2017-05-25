#include <conio.h>
#include <fstream>
#include "Parser.hpp"
#include "json_querier.hpp"
#include "json_formater.hpp"
#include "assist_functions.hpp"

using namespace std;
using namespace json;


Parser parser;
Json root;

void PrintUsage();
int CheckJsonTextIsValid();
void WritePrettyFile(string _FileName, string _String);

int main(int argc, char *argv[])
{
	argc--; argv++;
	if (argc >= 1 && argc <= 3)
	{
		// 打开文件
		ifstream infile(argv[argc - 1]);
		if (!infile) cout << "open file error" << endl;

		// 解析Json文本
		root = parser.Parse(infile);

		// 通过解析结果查看Json文件是否合法，若不合法，输出错误信息、停止格式化和查询操作
		if (CheckJsonTextIsValid())
		{
			// tab 缩进
			if (argc == 2 && argv[0] == string("-pretty"))
			{
				string s;
				JsonFormat::json2string(root, s);
				WritePrettyFile(argv[argc - 1], s);
			}
			// 指定宽度 空格缩进
			else if (argc == 3 && argv[0] == string("-pretty") && string(argv[1]).find("-w") == 0)
			{
				string _width = argv[1];
				_width.erase(_width.begin());
				_width.erase(_width.begin());
				string s;
				JsonFormat::json2string(root, s, atoi(_width.c_str()));
				WritePrettyFile(argv[argc - 1], s);
			}
			// 指定路径查询
			else if (argc == 3 && argv[0] == string("-query"))
			{
				Json query_result = JsonQuery::Query(root, argv[1]);

				cout << "query path: " << argv[1] << endl;
				if (query_result.type == JsonType::JS_NULL)
					cout << "not found" << endl;
				else
				{
					string out_string;
					if (query_result.type == JsonType::JS_ARRAY || query_result.type == JsonType::JS_OBJECT)
					{
						JsonFormat::json2string(query_result, out_string, 2);
						cout << "type: " << query_result.GetTypeName() << "\nvalue:\n" << out_string << endl;
					}
					else
					{
						cout << "type: " << query_result.GetTypeName() << "\nvalue: " << escape2string(query_result.GetValue()) << endl;
					}
				}
			}
			else
			{
				if (argc >= 2)
					PrintUsage();
			}
		}
	}
	else
	{
		PrintUsage();
	}
}

void PrintUsage()
{
	cout << "Usage: .exe [option] filename" << endl;
	cout << "option:" << endl;
	cout << "\t-pretty           format JSON text to a pretty format, the intent width can given" << endl;
	cout << "\t                  by -w options(\\t is the default intent char)." << endl;
	cout << "\t                  e.g. option '-pretty -w2' means program use 2 blanks to intent" << endl;
	cout << "\t-w                give the intent width, '-pretty' must be specified before" << endl;
	cout << "\t-query path       query data by path" << endl;
}

int CheckJsonTextIsValid()
{
	if (root.type != JsonType::JS_NULL)
	{
		cout << "valid" << endl;
		return 1;
	}
	else
	{
		for each (auto msg in parser.GetErrorMsgs())
			cout << msg << endl;
	}
	return 0;
}

void WritePrettyFile(string _FileName, string _String)
{
	int a = _FileName.find_last_of('\\');
	int b = _FileName.find_last_of('/');
	int index = a > b ? a : b;
	_FileName.erase(0, (index >= 0 ? index : 0) + 1);
	if (_FileName.find(".") != string::npos)
		_FileName.insert(_FileName.find_last_of('.'), ".pretty");

	ofstream outfile(_FileName);
	if (!outfile)
	{
		cout << "write file " << _FileName << " failed!!" << endl;
		cout << _String << endl;
	}
	else
	{
		cout << "write file " << _FileName << " success!" << endl;
		outfile << _String;
		outfile.close();
	}
}
