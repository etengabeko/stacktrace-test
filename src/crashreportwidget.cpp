#include "crashreportwidget.h"
#include "ui_crashreportwidget.h"

#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include "common.h"
#include "crashreporter.h"

namespace
{

const QString makeCrashReportFileName()
{
    return QString("%1_%2_%3.dump")
            .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"))
            .arg(qApp->applicationName())
            .arg(qApp->applicationPid());
}

}

CrashReportWidget::CrashReportWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::CrashReportWidget())
{
    m_ui->setupUi(this);

    CrashReporter* reporter = CrashReporter::instance();
    Q_CHECK_PTR(reporter);
    m_ui->fileNameLineEdit->setText(QString::fromStdString(reporter->crashReportFileName()));

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
    m_ui->saveReportButton->setEnabled(   !fileName.isEmpty()
                                       && !m_ui->reportTextEdit->toPlainText().isEmpty());
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
    CrashReporter* reporter = CrashReporter::instance();
    Q_CHECK_PTR(reporter);

    reporter->setCrashReportFileName(m_ui->fileNameLineEdit->text().toStdString());
    auto res = reporter->saveCrashReport(m_ui->reportTextEdit->toPlainText().toStdString());
    if (res.first)
    {
        QMessageBox::information(this,
                                 tr("Saving report successful"),
                                 tr("Crash report saved to %1.").arg(m_ui->fileNameLineEdit->text()),
                                 QMessageBox::StandardButton::Ok);
    }
    else
    {
        QMessageBox::warning(this,
                             tr("Saving report failure"),
                             QString::fromStdString(res.second),
                             QMessageBox::StandardButton::Ok);
    }
}

void CrashReportWidget::process(const boost::stacktrace::stacktrace& st)
{
    const QString report = QString::fromStdString(CrashReporter::instance()->makeCrashReport(st));
    m_ui->reportTextEdit->setPlainText(report);

    m_ui->fileNameLineEdit->setText(::makeCrashReportFileName());
    m_ui->saveReportButton->setFocus();
}
