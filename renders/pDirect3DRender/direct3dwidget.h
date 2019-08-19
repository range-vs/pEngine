#pragma once

// pEngine
#include "coredirect3d.h"

class DYNLIB_PRENDER_DIRECT3D Direct3DWidget: public CoreDirect3D
{
    Q_OBJECT

public:
    Direct3DWidget(QWidget* obj = nullptr);

public:
    void initializeRender(int w, int h) override;
    void updateRender() override;
    void drawRender() override;
    ~Direct3DWidget();
};
