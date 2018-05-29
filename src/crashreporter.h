#ifndef STACKTRACE_CRASH_REPORTER_H
#define STACKTRACE_CRASH_REPORTER_H

#include <memory>
#include <string>
#include <utility>

#include <boost/stacktrace.hpp>

class CrashReporter
{
private:
    CrashReporter(const std::string& appName, int appPid);

public:
    ~CrashReporter();

    static CrashReporter* initialize(const std::string& appName, int appPid);
    static CrashReporter* instance();

    const std::string& crashReportFileName() const;
    void setCrashReportFileName(const std::string& outputFileName);

    std::string makeCrashReportCustomFileName() const;

    std::string makeCrashReport(const boost::stacktrace::stacktrace& st) const;

    std::pair<bool, std::string> saveCrashReport(const std::string& report) const;
    std::pair<bool, std::string> saveCrashReport(const boost::stacktrace::stacktrace& st) const;


private:
    static std::unique_ptr<CrashReporter> m_instance;

    const std::string m_appName;
    const int m_appPid = 0;

    std::string m_filename;

};

#endif // STACKTRACE_CRASH_REPORTER_H
