#pragma once

#include "../../pEntryPoint/pch.h"

// pEngine
#include "../../renders/pgeneralrender.h"
#include "../../renders/pDirect3DRender/direct3dwidget.h"
#include "../../renders/pOpenGLRender/openglwidget.h"
#include "../../pDebug/pconsole.h"

class CRender: public QObject
{
    Q_OBJECT

public:
    explicit CRender(QObject *parent = nullptr);
    virtual std::shared_ptr<IRender> create(PConsole* pConsole) = 0;
};

class CDirect3DRender: public CRender
{
    Q_OBJECT

public:
    std::shared_ptr<IRender> create(PConsole* pConsole) override;
};

class COpenGLRender: public CRender
{
    Q_OBJECT

public:
    std::shared_ptr<IRender> create(PConsole* pConsole) override;
};

class CreatorRenders
{
    static std::map<uint, std::shared_ptr<CRender>> creatorsRender;
    static std::map<uint, std::shared_ptr<CRender>> initializeCreatorsRender();

public:
    std::shared_ptr<IRender> createRender(uint r, PConsole* pConsole);
};
