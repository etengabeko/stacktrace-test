#ifndef STACKTRACE_CRASH_REPORT_WIDGET_H
#define STACKTRACE_CRASH_REPORT_WIDGET_H

#include <memory>
#include <exception>

#include <QWidget>

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

    void process(std::exception& e);

private slots:
    void slotSelectFileName();
    void slotSaveReport();

private:
    std::unique_ptr<Ui::CrashReportWidget> m_ui;

};

#endif // STACKTRACE_CRASH_REPORT_WIDGET_H
