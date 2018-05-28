#ifndef STACKTRACE_SIGNAL_HANDLER_H
#define STACKTRACE_SIGNAL_HANDLER_H

#include <memory>
#include <string>
#include <utility>

#include <boost/stacktrace.hpp>

class SignalHandler
{
private:
    SignalHandler();

public:
    ~SignalHandler() = default;

    static SignalHandler* instance();

    const std::string& crashReportFileName() const;
    void setCrashReportFileName(const std::string& outputFileName);

    std::string makeCrashReport(const boost::stacktrace::stacktrace& st) const;

    std::pair<bool, std::string> saveCrashReport(const std::string& report) const;
    std::pair<bool, std::string> saveCrashReport(const boost::stacktrace::stacktrace& st) const;

private:
    static std::unique_ptr<SignalHandler> m_instance;

    std::string m_filename;

};

#endif // STACKTRACE_SIGNAL_HANDLER_H
