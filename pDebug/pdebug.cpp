#include "pdebug.h"

void PDebug::pDebugMessage(QtMsgType msgType, const QMessageLogContext & logContext, const QString & message)
{
    QString formatLine = QDateTime::currentDateTime().toString("dd.MM.yy-hh:mm");
    QTextStream stream(&PDebug::currentPDebug->log);
    switch(msgType)
    {
    case QtInfoMsg:
        stream << message << "\n";
        PDebug::currentPDebug->console->addText(message);
        break;

    case QtDebugMsg:
        stream << "[" << formatLine << "] Message: " << message << "\n";
        PDebug::currentPDebug->console->addText("[" + formatLine + "] Message: " + message);
        break;

    case QtWarningMsg: // TODO: добавить цвет строки красным!
        stream << "[" << formatLine << "] Warning: " << message << "\n";
        PDebug::currentPDebug->console->addHtml("<br><font color=\"Red\">[" + formatLine + "] Warning: " + message + "</font>");
        break;

    case QtFatalMsg:
        stream << "[" << formatLine << "] Error: " << message << "\n";
        PDebug::currentPDebug->console->addText("[" + formatLine + "] Error: " + message);
        PDebug::currentPDebug->log.close();
        abort();
    }

}

PDebug* PDebug::currentPDebug(nullptr);

PDebug::PDebug(bool isConsole):isConsole(isConsole){}

bool PDebug::initializeSystem()
{
    QMessageBox error;

    currentPDebug = this;
    log.setFileName("plog.ptext");
    if(!log.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        error.setText("Error create file plog.ptext!");
        error.exec();
        return false;
    }
    if(!log.resize(0))
    {
        error.setText("Error clear file plog.ptext!");
        error.exec();
        return false;
    }

//    SmartThunkCreator thunk(new ThunkCreator);
//    thunk->createNonStaticCallbackFunction(this, &PDebug::pDebugMessage);
//    auto result = qInstallMessageHandler(reinterpret_cast<QtMessageHandler>(thunk->getCallbackMethod()));

    qInstallMessageHandler(&PDebug::pDebugMessage);

    if(isConsole)
       console.reset(new PConsole);
    return true;
}

PConsole *PDebug::getDebugConsole() const
{
    return console.get();
}

