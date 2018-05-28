#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <fstream>
#include <vector>

#include <QDebug>

#include <boost/filesystem.hpp>

#include "common.h"
#include "crashreportwidget.h"

MainWidget::MainWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::MainWidget()),
    m_reportWidget(new CrashReportWidget())
{
    m_ui->setupUi(this);

    QObject::connect(m_ui->checkButton, &QPushButton::clicked,
                     this, &MainWidget::slotSafeCrash);
    QObject::connect(m_ui->cancelButton, &QPushButton::clicked,
                     this, &MainWidget::close);

    QObject::connect(m_ui->catchedRadioButton, &QRadioButton::toggled,
                     m_ui->catchedFrame, &QFrame::setEnabled);
    QObject::connect(m_ui->nonCatchedRadioButton, &QRadioButton::toggled,
                     m_ui->nonCatchedFrame, &QFrame::setEnabled);
}

MainWidget::~MainWidget()
{

}

void MainWidget::slotSafeCrash()
{
    try
    {
        if (m_ui->catchedRadioButton->isChecked())
        {
            if (m_ui->outOfRangeRadioButton->isChecked())
            {
                outOfRangeCase();
            }
            else if (m_ui->openNonexistRadioButton->isChecked())
            {
                openNonexistentCase();
            }
        }
        else if (m_ui->nonCatchedRadioButton->isChecked())
        {
            if (m_ui->divideZeroRadioButton->isChecked())
            {
                divideByZeroCase();
            }
            else if (m_ui->derefNullRadioButton->isChecked())
            {
                dereferenceNullptrCase();
            }
        }
    }
    catch (std::exception& e)
    {
        const boost::stacktrace::stacktrace* st = boost::get_error_info<traced>(e);
        if (st != nullptr)
        {
            m_reportWidget->process(*st);
            m_reportWidget->show();
        }
    }
}

void MainWidget::divideByZeroCase()
{
    try
    {
        std::vector<int> v { 1, 0 };
        qDebug() << (v[0]/v[1]);
    }
    catch (std::exception& e)
    {
        throwWithTrace(e);
    }
}

void MainWidget::dereferenceNullptrCase()
{
    try
    {
        std::unique_ptr<int> invalid;
        qDebug() << *invalid;
    }
    catch (std::exception& e)
    {
        throwWithTrace(e);
    }
}

void MainWidget::outOfRangeCase()
{
    try
    {
        enum { InvalidIndex = 1 };
        std::vector<int> v;
        qDebug() << v.at(InvalidIndex);
    }
    catch (std::exception& e)
    {
        throwWithTrace(e);
    }
}

void MainWidget::openNonexistentCase()
{
    try
    {
        std::string nonExistentFileName;
        do
        {
            static int index = 0;
            nonExistentFileName = "invalid_filename_" + std::to_string(++index);
        }
        while (boost::filesystem::exists(boost::filesystem::path(nonExistentFileName)));

        std::ifstream stream;
        stream.exceptions(std::ios_base::failbit | std::ios_base::badbit);
        stream.open(nonExistentFileName);
        qDebug() << (stream ? "True" : "False");
    }
    catch (std::exception& e)
    {
        throwWithTrace(e);
    }
}
