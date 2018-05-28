#include "signalhandler.h"

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

    const SignalHandler* handler = SignalHandler::instance();
    assert(handler != nullptr);

    const std::string crashReport = handler->makeCrashReport(boost::stacktrace::stacktrace());
    auto res = handler->saveCrashReport(crashReport);

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
                   << handler->crashReportFileName()
                   << std::endl;
    }

    std::exit(signum);
}

}

std::unique_ptr<SignalHandler> SignalHandler::m_instance(nullptr);

SignalHandler::SignalHandler()
{
    std::signal(SIGABRT, &::abortHandler);
    std::signal(SIGSEGV, &::abortHandler);
    std::signal(SIGILL,  &::abortHandler);
    std::signal(SIGFPE,  &::abortHandler);
}

SignalHandler* SignalHandler::instance()
{
    if (m_instance == nullptr)
    {
        m_instance.reset(new SignalHandler());
    }

    return m_instance.get();
}

const std::string& SignalHandler::crashReportFileName() const
{
    return m_filename;
}

void SignalHandler::setCrashReportFileName(const std::string& outputFileName)
{
    m_filename = outputFileName;
}

std::string SignalHandler::makeCrashReport(const boost::stacktrace::stacktrace& st) const
{
    std::stringstream stream;
    stream << st;
    return stream.str();
}

std::pair<bool, std::string> SignalHandler::saveCrashReport(const boost::stacktrace::stacktrace& st) const
{
    return saveCrashReport(makeCrashReport(st));
}

std::pair<bool, std::string> SignalHandler::saveCrashReport(const std::string& report) const
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
