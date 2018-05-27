#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <fstream>
#include <vector>

#include <QDebug>

#include <boost/filesystem.hpp>

#include "common.h"

MainWidget::MainWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::MainWidget())
{
    m_ui->setupUi(this);

    QObject::connect(m_ui->checkButton, &QPushButton::clicked,
                     this, &MainWidget::slotCrashCase);
    QObject::connect(m_ui->cancelButton, &QPushButton::clicked,
                     this, &MainWidget::close);
}

MainWidget::~MainWidget()
{

}

void MainWidget::slotCrashCase()
{
    if (m_ui->divideZeroRadioButton->isChecked())
    {
        divideByZeroCase();
    }
    else if (m_ui->derefNullRadioButton->isChecked())
    {
        dereferenceNullptrCase();
    }
    else if (m_ui->outOfRangeRadioButton->isChecked())
    {
        outOfRangeCase();
    }
    else if (m_ui->openNonexistRadioButton->isChecked())
    {
        openNonexistentCase();
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

        qDebug() << (std::ifstream(nonExistentFileName) ? "True" : "False");
    }
    catch (std::exception& e)
    {
        throwWithTrace(e);
    }
}
