#include "crashreportwidget.h"
#include "ui_crashreportwidget.h"

#include "common.h"

//void displayStacktrace(const boost::stacktrace::stacktrace& st)
//{
//    std::cout << st;

//    for (const boost::stacktrace::frame& each : st)
//    {
//        std::cout << each << " at "
//                  << each.source_file() << ":"
//                  << each.source_line() << "\n";
//    }
//}

CrashReportWidget::CrashReportWidget(const QString& dumpFileName,
                                     QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::CrashReportWidget())
{
    m_ui->setupUi(this);

    m_ui->fileNameLineEdit->setText(dumpFileName);

    QObject::connect(m_ui->cancelButton, &QPushButton::clicked,
                     this, &CrashReportWidget::close);
    QObject::connect(m_ui->selectFileNameButton, &QPushButton::clicked,
                     this, &CrashReportWidget::slotSelectFileName);
    QObject::connect(m_ui->saveReportButton, &QPushButton::clicked,
                     this, &CrashReportWidget::slotSaveReport);
}

CrashReportWidget::~CrashReportWidget()
{

}

void CrashReportWidget::slotSelectFileName()
{

}

void CrashReportWidget::slotSaveReport()
{

}

void CrashReportWidget::process(std::exception& e)
{
    Q_UNUSED(e);
}
