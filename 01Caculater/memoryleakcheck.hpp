#ifndef MEMORY_LEAK_CHECK_HPP
#define MEMORY_LEAK_CHECK_HPP

#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>
//����ں����а��� _CrtDumpMemoryLeaks();
//���ɼ�⵽�ڴ�й¶

// enable memrory leak check
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}
//�ú������Է���������������λ�ã�������ȷ�Ĵ����ڴ�й¶���

#endif
