#include <QApplication>
#include <QDebug>

#include "common.h"
#include "crashreporter.h"
#include "mainwidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    CrashReporter* reporter = CrashReporter::initialize(app.applicationName().toStdString(),
                                                        app.applicationPid());
    if (reporter == nullptr)
    {
        qCritical().noquote() << app.tr("Can not create Crash Reporter. Application will be closed.");
        return EXIT_FAILURE;
    }

    try
    {
        MainWidget wgt;
        wgt.show();
        return app.exec();
    }
    catch (std::exception& e)
    {
        const boost::stacktrace::stacktrace* st = boost::get_error_info<traced>(e);
        if (st != nullptr)
        {
            reporter->setCrashReportFileName(reporter->makeCrashReportCustomFileName());
            reporter->saveCrashReport(*st);
        }
    }
    return EXIT_FAILURE;
}
