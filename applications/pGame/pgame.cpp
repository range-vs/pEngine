#include "pgame.h"

PGame::PGame(){}

void PGame::initializeSystem(PConsole *pConsole, const std::shared_ptr<ParserArgsTerminal> &argsParser)
{
    std::shared_ptr<CreatorRenders> cr(new CreatorRenders);
    render = cr->createRender(static_cast<uint>(argsParser->getValue("-mode-render")), pConsole);
    // TODO: передать парсер аргументов
    render->initializeRender(800, 650);
    render->showRender();
}
