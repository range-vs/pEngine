#pragma once

class IRender
{

public:
    virtual void initializeRender(int w, int h) = 0;
    virtual void resizeRender(int w, int h) = 0;
    virtual void updateRender() = 0;
    virtual void drawRender() = 0;
    virtual void showRender() = 0;
    virtual void updateFps() = 0;
    virtual ~IRender() {};
};
