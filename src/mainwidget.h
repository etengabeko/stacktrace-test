#ifndef STACKTRACE_MAINWIDGET_H
#define STACKTRACE_MAINWIDGET_H

#include <memory>

#include <QWidget>

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
    void slotCrashCase();

private:
    void divideByZeroCase();
    void dereferenceNullptrCase();
    void outOfRangeCase();
    void openNonexistentCase();

private:
    std::unique_ptr<Ui::MainWidget> m_ui;

};

#endif // STACKTRACE_MAINWIDGET_H
