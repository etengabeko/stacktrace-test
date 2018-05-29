#include "crashreporter.h"

#include <cassert>
#include <csignal>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

namespace
{

void abortHandler(int signum)
{
    const char* name = NULL;
    switch (signum)
    {
    case SIGABRT:
        name = "SIGABRT";
        break;
    case SIGSEGV:
        name = "SIGSEGV";
        break;
    case SIGILL:
        name = "SIGILL";
        break;
    case SIGFPE:
        name = "SIGFPE";
        break;
    }

    const CrashReporter* reporter = CrashReporter::instance();
    assert(reporter != nullptr);

    const std::string crashReport = reporter->makeCrashReport(boost::stacktrace::stacktrace());
    auto res = reporter->saveCrashReport(crashReport);

    if (name != nullptr)
    {
        std::cerr << "Caught signal " << signum << " (" << name << "):\n";
    }
    else
    {
        std::cerr << "Caught signal " << signum << ":\n";
    }

    std::cerr << crashReport;
    if (res.first)
    {
        std::cerr  << "\nCrash repost saved to "
                   << reporter->crashReportFileName()
                   << std::endl;
    }

    std::exit(signum);
}

}

std::unique_ptr<CrashReporter> CrashReporter::m_instance(nullptr);

CrashReporter::CrashReporter()
{
    std::signal(SIGABRT, &::abortHandler);
    std::signal(SIGSEGV, &::abortHandler);
    std::signal(SIGILL,  &::abortHandler);
    std::signal(SIGFPE,  &::abortHandler);
}

CrashReporter* CrashReporter::instance()
{
    if (m_instance == nullptr)
    {
        m_instance.reset(new CrashReporter());
    }

    return m_instance.get();
}

const std::string& CrashReporter::crashReportFileName() const
{
    return m_filename;
}

void CrashReporter::setCrashReportFileName(const std::string& outputFileName)
{
    m_filename = outputFileName;
}

std::string CrashReporter::makeCrashReport(const boost::stacktrace::stacktrace& st) const
{
    std::stringstream stream;
    stream << st;
    return stream.str();
}

std::pair<bool, std::string> CrashReporter::saveCrashReport(const boost::stacktrace::stacktrace& st) const
{
    return saveCrashReport(makeCrashReport(st));
}

std::pair<bool, std::string> CrashReporter::saveCrashReport(const std::string& report) const
{
    std::ofstream stream(m_filename);
    if (!stream)
    {
        return std::make_pair(false, std::string(strerror(errno)));
    }

    stream << report;
    stream.close();

    return std::make_pair(true, std::string());
}
