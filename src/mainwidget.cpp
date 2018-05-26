#include "mainwidget.h"
#include "ui_mainwidget.h"

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

}

void MainWidget::dereferenceNullptrCase()
{

}

void MainWidget::outOfRangeCase()
{

}

void MainWidget::openNonexistentCase()
{

}
