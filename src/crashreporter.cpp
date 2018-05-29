#include "crashreporter.h"

#include <cassert>
#include <chrono>
#include <csignal>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace
{

const std::tm currentTime()
{
    using namespace std::chrono;

    const time_t now = system_clock::to_time_t(system_clock::now());
    std::tm result = *std::localtime(&now);

    return result;
}

std::string timeToString(const std::tm& time)
{
    std::stringstream stream;
    stream << std::put_time(&time, "%Y%m%d_%H%M%S");

    return stream.str();
}

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

    CrashReporter* reporter = CrashReporter::instance();
    assert(reporter != nullptr);

    reporter->setCrashReportFileName(reporter->makeCrashReportCustomFileName());
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

CrashReporter::CrashReporter(const std::string& appName, int appPid) :
    m_appName(appName),
    m_appPid(appPid),
    m_filename(makeCrashReportCustomFileName())
{
    std::signal(SIGABRT, &::abortHandler);
    std::signal(SIGSEGV, &::abortHandler);
    std::signal(SIGILL,  &::abortHandler);
    std::signal(SIGFPE,  &::abortHandler);
}

CrashReporter::~CrashReporter()
{

}

CrashReporter* CrashReporter::initialize(const std::string& appName, int appPid)
{
    if (m_instance == nullptr)
    {
        m_instance.reset(new CrashReporter(appName, appPid));
    }

    return instance();
}

CrashReporter* CrashReporter::instance()
{
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

std::string CrashReporter::makeCrashReportCustomFileName() const
{
    std::string result = ::timeToString(::currentTime());
    if (!m_appName.empty())
    {
        result += "_" + m_appName;
    }
    if (m_appPid > 0)
    {
        result += "_" + std::to_string(m_appPid);
    }
    result += ".dump";

    return result;
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
