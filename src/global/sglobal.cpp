/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sglobal.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>

static SLogMsgHandler globalHandler = NULL;

void s_assert(const char *expression, const char *src, int line, const char *msg)
{
	if (msg)
		sFatal("ASSERT failure: expression: \"%s\", file %s, line %d, message=\"%s\"",
               expression, src, line, msg);
    else
        sFatal("ASSERT failure: expression: \"%s\", file %s, line %d",
               expression, src, line);
}

void sDebug(const char *msg, ...)
{
	if (strlen(msg) > APPLICATION_LOG_LINE_BUFFER)
        return;

	char tmp[APPLICATION_LOG_LINE_BUFFER];
	tmp[0] = '\0';
	tmp[APPLICATION_LOG_LINE_BUFFER - 1] = '\0';

    va_list vl;
    va_start(vl, msg);
    vsprintf(tmp, msg, vl);
    printMessageOnStdErr(SMsgLevelDebug, tmp);
    va_end(vl);
}

void sWarning(const char *msg, ...)
{
	if (strlen(msg) > APPLICATION_LOG_LINE_BUFFER)
        return;

	char tmp[APPLICATION_LOG_LINE_BUFFER];
	tmp[0] = '\0';
	tmp[APPLICATION_LOG_LINE_BUFFER - 1] = '\0';

    va_list vl;
    va_start(vl, msg);
    vsprintf(tmp, msg, vl);
    printMessageOnStdErr(SMsgLevelWarning, tmp);
    va_end(vl);
}

void sCritical(const char *msg, ...)
{
	if (strlen(msg) > APPLICATION_LOG_LINE_BUFFER)
        return;

	char tmp[APPLICATION_LOG_LINE_BUFFER];
	tmp[0] = '\0';
	tmp[APPLICATION_LOG_LINE_BUFFER - 1] = '\0';

    va_list vl;
    va_start(vl, msg);
    vsprintf(tmp, msg, vl);
    printMessageOnStdErr(SMsgLevelCritical, tmp);
    va_end(vl);
}

void sFatal(const char *msg, ...)
{
	if (strlen(msg) > APPLICATION_LOG_LINE_BUFFER)
    {
        abort();
        return;
    }

	char tmp[APPLICATION_LOG_LINE_BUFFER];
	tmp[0] = '\0';
	tmp[APPLICATION_LOG_LINE_BUFFER - 1] = '\0';

    va_list vl;
    va_start(vl, msg);
    vsprintf(tmp, msg, vl);
    printMessageOnStdErr(SMsgLevelFatal, tmp);
    va_end(vl);

	if (globalHandler == NULL)
		abort();
}

void printMessageOnStdErr(SMsgLevel messageLevel, const char* msg)
{
    const char *prefix = NULL;

    switch (messageLevel)
    {
    case SMsgLevelDebug:
        prefix = "DEBUG    ";
        break;
    case SMsgLevelWarning:
        prefix = "WARNING  ";
        break;
    case SMsgLevelCritical:
        prefix = "CRITICAL ";
        break;
    case SMsgLevelFatal:
        prefix = "FATAL    ";
        break;

    default:
        prefix = "UNKNOWN  ";
    };

	if (globalHandler == NULL)
	{
        std::cerr << prefix << msg << std::endl;
        fflush(stderr);
    }
    else
        (*globalHandler)(messageLevel, msg);      //Call the custom message handler
}

void sInstallCustomMsgLogHandler(SLogMsgHandler handler)
{
    globalHandler = handler;
}
