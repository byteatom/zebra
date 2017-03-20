#include "SehException.h"

#include <eh.h>

void sehTranslator(unsigned int code, EXCEPTION_POINTERS* info)
{
	throw SehException{code, *info->ExceptionRecord, *info->ContextRecord};
}

void sehInit()
{
	_set_se_translator(&sehTranslator);
}
