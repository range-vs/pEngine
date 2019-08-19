#pragma once

#include "../../pEntryPoint/pch.h"

// pEngine
#include "lib_define.h"
#include "../../pUtils/putils.h"
#include "../../pDebug/pdebug.h"
#include "../pApplicationGeneral/creator_renders.h"

class DYNLIB_PGAME PGame: public IApplication
{
    std::shared_ptr<IRender> render;
public:
    PGame();
    void initializeSystem(PConsole* pConsole, const std::shared_ptr<ParserArgsTerminal> &argsParser);
};

// TODO: загружаем здесь рендер на выбор(пока только Dx12!!!!)
// сюда должна прийти консоль из pCore и парсер аргументов
// передаём консоль и парсер аргументов в pDirect3D12
