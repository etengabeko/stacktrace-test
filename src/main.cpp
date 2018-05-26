#include <signal.h>

#include <memory>

#include <QApplication>
#include <QDebug>
#include <QString>

#include "common.h"
#include "crashreportwidget.h"
#include "mainwidget.h"

namespace
{
const QString dumpFileName() { return "backtrace.dump"; }

void safetyQuit(int signum)
{
    qDebug().noquote() << "Program crashed. Stacktrace dump will be save in "
                       << dumpFileName()
                       << " and will be analyzed by next run.";
    boost::stacktrace::safe_dump_to(dumpFileName().toStdString().c_str());
    qApp->quit();
}

}

int main(int argc, char* argv[])
{
    ::signal(SIGSEGV, &::safetyQuit);

    QApplication app(argc, argv);
    CrashReportWidget reporter(::dumpFileName());

    try
    {
        MainWidget wgt;
        wgt.show();
        app.exec();
    }
    catch (std::exception& e)
    {
        reporter.process(e);
    }

    return EXIT_SUCCESS;
}

/*
bool checkDump()
{
    boost::system::error_code ec;
    if (boost::filesystem::exists(boost::filesystem::path(dumpFileName()), ec))
    {
        std::ifstream in(dumpFileName());

        boost::stacktrace::stacktrace st = boost::stacktrace::stacktrace::from_dump(in);
        in.close();
        boost::filesystem::remove(dumpFileName());

        std::cout << "Previous run crashed:\n";
        displayStacktrace(st);
    }

    return (ec.value() != boost::system::errc::success);
}

void unsafelyDivide()
{
    int a = 0,
        b = 0;

    std::cout << "Input a: ";
    std::cin >> a;
    std::cout << "Input b: ";
    std::cin >> b;

    int res = a / b;

    std::cout << "Divide a/b = " << res << std::endl;
}

void unsafelyAccess()
{
    std::vector<int> v;
    size_t s = 0,
           i = 0;

    std::cout << "Input size: ";
    std::cin >> s;
    std::cout << "Input index: ";
    std::cin >> i;

    v.resize(s, 1);
    int res = v[i];

    std::cout << "v[" << i << "] = " << res << std::endl;
}
*/
