#pragma once

#include "../pEntryPoint/pch.h"

// pEngine
#include "lib_define.h"
#include "../pUtils/putils.h"
#include "../pDebug/pdebug.h"
#include "../applications/pGame/pgame.h"

class DYNLIB_PCORE PCore
{
    std::shared_ptr<IApplication> engine;
public:
    PCore();
    void initializeSystem(char** argv, int argc, PConsole* pConsole);
    template<class Mode> std::shared_ptr<IApplication> startEngine(PConsole* pConsole, const std::shared_ptr<ParserArgsTerminal> &argsParser);
};

template<class Mode>
std::shared_ptr<IApplication> PCore::startEngine(PConsole* pConsole, const std::shared_ptr<ParserArgsTerminal> &argsParser)
{
    std::shared_ptr<Mode> engine(new Mode);
    engine->initializeSystem(pConsole, argsParser);
    return engine;
}

// TODO:  тут принимаем ссылку на консоль
// получаем информацию об оборудовании
// выводим её в лог
// вызываем pGame/pEditor и передаём туда ссылку на консоль
// передаем туда парсер с аргументами - TODO
