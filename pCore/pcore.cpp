#include "pcore.h"

PCore::PCore(){}

void PCore::initializeSystem(char **argv, int argc, PConsole *pConsole)
{
    std::shared_ptr<ParserArgsTerminal> argsTerminal(new ParserArgsTerminal);
    argsTerminal->initializeSystem(argv, argc);
    qInfo() << "Core load is succesfull";
    if(!argsTerminal->getValue("-mode-engine"))
        engine = startEngine<PGame>(pConsole, argsTerminal);
    else
    {
        // start editor, TODO
    }
}
