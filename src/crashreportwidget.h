#ifndef STACKTRACE_CRASH_REPORT_WIDGET_H
#define STACKTRACE_CRASH_REPORT_WIDGET_H

#include <memory>
#include <exception>

#include <QWidget>

#include <boost/stacktrace.hpp>

class QString;

namespace Ui
{
class CrashReportWidget;
} // Ui

class CrashReportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CrashReportWidget(const QString& dumpFileName,
                               QWidget* parent = nullptr);
    ~CrashReportWidget();

    void process(const boost::stacktrace::stacktrace& st);

private slots:
    void slotChangeFileName(const QString& fileName);
    void slotSelectFileName();
    void slotSaveReport();

private:
    QString makeCrashReport(const boost::stacktrace::stacktrace& st) const;

private:
    std::unique_ptr<Ui::CrashReportWidget> m_ui;

};

#endif // STACKTRACE_CRASH_REPORT_WIDGET_H
