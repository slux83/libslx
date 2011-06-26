/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sglobal.h"
#include <cstdarg>
#include <iostream>
#include <cstdio>
#include <cstring>

void s_assert(const char *expression, const char *src, int line, const char *msg)
{
    if (msg)
        sFatal("ASSERT failure: expression: \"%s\", file %s, line %d, message= \"%s\"",
               expression, src, line, msg);
    else
        sFatal("ASSERT failure: expression: \"%s\", file %s, line %d",
               expression, src, line);
}

void sDebug(const char *msg, ...)
{
    if (strlen(msg) > 512)
        return;

    char tmp[512];
    tmp[0] = '\0';
    tmp[511] = '\0';

    va_list vl;
    va_start(vl, msg);
    vsprintf(tmp, msg, vl);
    std::cerr << "DEBUG: " << tmp << std::endl; //FIXME
    va_end(vl);
}

void sWarning(const char *msg, ...)
{

}

void sCritical(const char *msg, ...)
{

}

void sFatal(const char *msg, ...)
{

}
