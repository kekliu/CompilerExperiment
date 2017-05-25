#pragma once

namespace json
{

#define __REGESTER_ALL_JSON_TYPES \
    TOKEN(JS_NULL,              "NULL")      \
	TOKEN(JS_OBJECT,			"object")    \
	TOKEN(JS_ARRAY,				"array")	 \
	/* constant */                           \
    TOKEN(JS_CONST_INT,         "int")		 \
    TOKEN(JS_CONST_DOUBLE,      "float")	 \
    TOKEN(JS_STRING,            "string")	 \
    TOKEN(JS_CONST_TRUE,        "true")	     \
    TOKEN(JS_CONST_FALSE,       "false")     \
    TOKEN(JS_CONST_NULL,		"null")	     \
	/* punctuators */                        \
    TOKEN(JS_LBRACE,            "{")		 \
    TOKEN(JS_RBRACE,            "}")		 \
    TOKEN(JS_LBRACKET,          "[")		 \
    TOKEN(JS_RBRACKET,          "]")		 \
    TOKEN(JS_COMMA,             ",")		 \
    TOKEN(JS_COLON,             ":")		 \


	enum class JsonType
	{
#define TOKEN(k, s) k,
		__REGESTER_ALL_JSON_TYPES
#undef TOKEN
	};

	const char *JsonTypeNames(JsonType type)
	{
#define TOKEN(k, s) if(type==JsonType::k) return s;
		__REGESTER_ALL_JSON_TYPES
#undef TOKEN
			return NULL;
	};

#undef __REGESTER_ALL_JSON_TYPES

}
