#ifndef STACKTRACE_COMMON_H
#define STACKTRACE_COMMON_H

#include <boost/exception/all.hpp>
#include <boost/stacktrace.hpp>

using traced = boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace>;

template <class T>
void throwWithTrace(const T& e)
{
    throw boost::enable_error_info(e) << traced(boost::stacktrace::stacktrace());
}

#endif // STACKTRACE_COMMON_H
