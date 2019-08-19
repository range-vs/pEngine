#pragma once

#include "../pEntryPoint/pch.h"

// pEngine
#include "../pUtils/putils.h"
#include "lib_define.h"

class DYNLIB_PDEBUG PConsole : public QWidget
{
    Q_OBJECT

    QTextEdit* textEdit;
    QGridLayout* gridLayout;
public:
    explicit PConsole(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    void addText(const QString& str);
    void addHtml(const QString& str);
    ~PConsole();
signals:

public slots:
    void closedWindow();

};
