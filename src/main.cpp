#include <QApplication>
#include <QDebug>

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

    MainWidget wgt;
    wgt.show();

    return app.exec();
}
