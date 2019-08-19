#include "pconsole.h"

PConsole::PConsole(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    textEdit = new QTextEdit;
    gridLayout = new QGridLayout;
    gridLayout->addWidget(textEdit, 0, 0);
    gridLayout->setMargin(0);
    setLayout(gridLayout);
    resize(400, 250);
    show();
}

void PConsole::addText(const QString &str)
{
    textEdit->append(str);
}

void PConsole::addHtml(const QString &str)
{
    textEdit->insertHtml(str);
}

PConsole::~PConsole()
{
    delete textEdit;
    delete gridLayout;
}

void PConsole::closedWindow()
{
    close();
}
