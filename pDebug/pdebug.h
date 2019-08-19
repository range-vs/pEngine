#pragma once

#include "../pEntryPoint/pch.h"

// pEngine
#include "../pUtils/putils.h"
#include "pconsole.h"

class PConsole;

using SmartPConsole = std::shared_ptr<PConsole>;

class DYNLIB_PDEBUG PDebug
{
     QFile log;
     bool isConsole;
     SmartPConsole console;

     static PDebug* currentPDebug;
     static void pDebugMessage(QtMsgType, const QMessageLogContext&, const QString&); // crutch?
public:
    PDebug(bool isConsole = true);
    bool initializeSystem();
    PConsole* getDebugConsole()const;
};

