// qt
#include <QApplication>

// pEngine
#include "../pCore/pcore.h"
#include "../pDebug/pdebug.h"

extern "C"
{
    __declspec(dllexport) unsigned int NvOptimusEnablement;
}

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    std::shared_ptr<PDebug> debug(new PDebug);
    if(!debug->initializeSystem())
        return -1;
    std::shared_ptr<PCore> core(new PCore);
    core->initializeSystem(argv, argc, debug->getDebugConsole());

    return a.exec();

}

// TODO:  сначала пишем шапку о создателе, информации о движке, информации об оборудовании и дате

