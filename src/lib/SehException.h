#ifndef SEHEXCEPTION_H
#define SEHEXCEPTION_H

#include <Windows.h>

struct SehException
{
	unsigned int code;
	EXCEPTION_RECORD record;
	CONTEXT context;
};

void sehTranslator(unsigned int code, EXCEPTION_POINTERS* info);
void sehInit();

#endif // SEHEXCEPTION_H
