/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SGLOBAL_H
#define SGLOBAL_H

#ifndef NULL
#   define NULL 0
#endif

/*!
    Calls the message handler with the fatal debug message \a msg. If no
    message handler has been installed, the message is printed to
    stderr.

    If you pass the function a format string and a list of arguments,
    it works in similar way to the C printf() function. The format
    should be a Latin-1 string.

    \warning String greather than 512 bytes are discarded
*/
void sDebug(const char *msg, ...);


/*!
    Calls the message handler with the fatal debug message \a msg. If no
    message handler has been installed, the message is printed to
    stderr.

    If you pass the function a format string and a list of arguments,
    it works in similar way to the C printf() function. The format
    should be a Latin-1 string.

    \warning String greather than 512 bytes are discarded
*/
void sWarning(const char *msg, ...);


/*!
    Calls the message handler with the fatal debug message \a msg. If no
    message handler has been installed, the message is printed to
    stderr.

    If you pass the function a format string and a list of arguments,
    it works in similar way to the C printf() function. The format
    should be a Latin-1 string.

    \warning String greather than 512 bytes are discarded
*/
void sCritical(const char *msg, ...);


/*!
    Calls the message handler with the fatal debug message \a msg. If no
    message handler has been installed, the message is printed to
    stderr.

    If you pass the function a format string and a list of arguments,
    it works in similar way to the C printf() function. The format
    should be a Latin-1 string.

    sFatal raise an application abort.

    \warning String greather than 512 bytes are discarded
*/
void sFatal(const char *msg, ...);

/*! \internal
    This function is used for internal usage
    \endinternal
*/
void s_assert(const char *expression, const char *src, int line, const char *msg = NULL);

#ifdef S_DISABLE_ASSERT
#   define S_ASSERT(expr) s_noop();
#   define S_ASSERT_HR(expr, where, what) s_noop();
#else
#   define S_ASSERT(expr) ((expr)? s_noop() : s_assert(#expr, __FILE__, __LINE__))
#   define S_ASSERT_MSG(expr, msg) ((expr)? s_noop() : s_assert(#expr, __FILE__, __LINE__, msg))
#endif

/*! \internal
    This function is used for internal usage
    \endinternal
*/
inline void s_noop() {}

#endif // SGLOBAL_H
