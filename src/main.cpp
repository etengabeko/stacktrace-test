#include <signal.h>

#include <memory>

#include <QApplication>
#include <QString>

#include "common.h"
#include "crashreportwidget.h"
#include "mainwidget.h"

namespace
{
const QString dumpFileName() { return "backtrace.dump"; }

static CrashReportWidget* crashReportSinglton = nullptr;

void safetyQuit(int signum)
{
    Q_UNUSED(signum);

    if (crashReportSinglton != nullptr)
    {
        crashReportSinglton->process(boost::stacktrace::stacktrace());
        crashReportSinglton->show();
    }
    else
    {
        boost::stacktrace::safe_dump_to(dumpFileName().toStdString().c_str());
    }

    qApp->quit();
}

}

int main(int argc, char* argv[])
{
    ::signal(SIGSEGV, &::safetyQuit);
    ::signal(SIGFPE,  &::safetyQuit);

    QApplication app(argc, argv);
    CrashReportWidget reporter(::dumpFileName());
    crashReportSinglton = &reporter;

    try
    {
        MainWidget wgt;
        wgt.show();
        app.exec();
    }
    catch (std::exception& e)
    {
        const boost::stacktrace::stacktrace* st = boost::get_error_info<traced>(e);
        if (st != nullptr)
        {
            reporter.process(*st);
            reporter.show();
        }
    }

    return EXIT_SUCCESS;
}
