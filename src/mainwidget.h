#ifndef STACKTRACE_MAINWIDGET_H
#define STACKTRACE_MAINWIDGET_H

#include <memory>

#include <QWidget>

class CrashReportWidget;

namespace Ui
{
class MainWidget;
} // Ui

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent = nullptr);
    ~MainWidget();

private slots:
    void slotSafeCrash();

private:
    void divideByZeroCase();
    void dereferenceNullptrCase();
    void outOfRangeCase();
    void openNonexistentCase();

private:
    std::unique_ptr<Ui::MainWidget> m_ui;

    std::unique_ptr<CrashReportWidget> m_reportWidget;

};

#endif // STACKTRACE_MAINWIDGET_H
