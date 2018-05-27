#include "crashreportwidget.h"
#include "ui_crashreportwidget.h"

#include <sstream>

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "common.h"

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
    QObject::connect(m_ui->fileNameLineEdit, &QLineEdit::textChanged,
                     this, &CrashReportWidget::slotChangeFileName);
}

CrashReportWidget::~CrashReportWidget()
{

}

void CrashReportWidget::slotChangeFileName(const QString& fileName)
{
    m_ui->saveReportButton->setEnabled(!fileName.isEmpty());
}

void CrashReportWidget::slotSelectFileName()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Dump file as..."),
                                                    qApp->applicationDirPath());
    if (!fileName.isEmpty())
    {
        m_ui->fileNameLineEdit->setText(fileName);
    }
}

void CrashReportWidget::slotSaveReport()
{
    QFile file(m_ui->fileNameLineEdit->text());
    if (file.open(QFile::WriteOnly))
    {
        file.write(m_ui->reportTextEdit->toPlainText().toUtf8());
    }
    else
    {
        QMessageBox::warning(this,
                             tr("Save Report Error"),
                             file.errorString());
    }
}

void CrashReportWidget::process(const boost::stacktrace::stacktrace& st)
{
    m_ui->reportTextEdit->setPlainText(makeCrashReport(st));
    m_ui->saveReportButton->setEnabled(true);
}

QString CrashReportWidget::makeCrashReport(const boost::stacktrace::stacktrace& st) const
{
    std::stringstream out;
    out << st;
    // TODO
    //    for (const boost::stacktrace::frame& each : st)
    //    {
    //        std::cout << each << " at "
    //                  << each.source_file() << ":"
    //                  << each.source_line() << "\n";
    //    }

    return QString::fromStdString(out.str());
}
