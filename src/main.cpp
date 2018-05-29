#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QString>

#include "crashreporter.h"
#include "mainwidget.h"

namespace
{

const QString customCrashReportFileName()
{
    return "crash_report.dump";
}

}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("stacktrace");
    app.setApplicationVersion("1.0");

    QCommandLineParser argparser;
    argparser.setApplicationDescription(app.tr("Test for generating safe-handling exceptions with stack trace."));
    argparser.addHelpOption();
    argparser.addVersionOption();

    QCommandLineOption outputFileNameOption(QStringList({"f", "outfile"}),
                                            app.tr("Crash report filename"),
                                            app.tr("filename"));
    argparser.addOption(outputFileNameOption);

    if (!argparser.parse(app.arguments()))
    {
        qWarning().noquote() << (argparser.errorText());
        argparser.showHelp(EXIT_FAILURE);
    }
    if (argparser.isSet("help"))
    {
        argparser.showHelp();
    }
    else if (argparser.isSet("version"))
    {
        argparser.showVersion();
    }

    const QString outputFileName = argparser.isSet(outputFileNameOption) ? argparser.value(outputFileNameOption)
                                                                         : ::customCrashReportFileName();
    CrashReporter* reporter = CrashReporter::instance();
    Q_CHECK_PTR(reporter);
    reporter->setCrashReportFileName(outputFileName.toStdString());

    MainWidget wgt;
    wgt.show();

    return app.exec();
}
