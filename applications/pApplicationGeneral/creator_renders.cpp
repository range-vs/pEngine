#include "creator_renders.h"

CRender::CRender(QObject *parent):QObject (parent)
{

}

std::shared_ptr<IRender> CDirect3DRender::create(PConsole* pConsole)
{
    std::shared_ptr<Direct3DWidget> render(new Direct3DWidget);
    QObject::connect(render.get(), &Direct3DWidget::closedChildrenWindows, pConsole, &PConsole::closedWindow);
    return std::dynamic_pointer_cast<IRender>(render);
}

std::shared_ptr<IRender> COpenGLRender::create(PConsole* pConsole)
{
    std::shared_ptr<OpenGLRender> render(new OpenGLRender);
    QObject::connect(render.get(), &OpenGLRender::closedChildrenWindows, pConsole, &PConsole::closedWindow);
    return std::dynamic_pointer_cast<IRender>(render);
}

std::map<uint, std::shared_ptr<CRender>> CreatorRenders::initializeCreatorsRender()
{
    return std::map<uint, std::shared_ptr<CRender>>
    {
        {0, std::shared_ptr<CDirect3DRender>(new CDirect3DRender())},
        {1, std::shared_ptr<COpenGLRender>(new COpenGLRender())}
    };
}

std::map<uint, std::shared_ptr<CRender>> CreatorRenders::creatorsRender(initializeCreatorsRender());

std::shared_ptr<IRender> CreatorRenders::createRender(uint r, PConsole *pConsole)
{
    if(creatorsRender.find(r) == creatorsRender.end())
        qFatal(" no find render!");
    return creatorsRender[r]->create(pConsole);
}

