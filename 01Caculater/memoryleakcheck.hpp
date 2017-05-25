#ifndef MEMORY_LEAK_CHECK_HPP
#define MEMORY_LEAK_CHECK_HPP

#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>
//在入口函数中包含 _CrtDumpMemoryLeaks();
//即可检测到内存泄露

// enable memrory leak check
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}
//该函数可以放在主函数的任意位置，都能正确的触发内存泄露输出

#endif
