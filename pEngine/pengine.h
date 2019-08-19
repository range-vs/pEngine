#pragma once

#include "../pEntryPoint/pch.h"

// pEngine
#include "lib_define.h"

class FPSCounter;

class DYNLIB_PENGINE SystemTimer
{
    std::shared_ptr<QTime> timer;
    float oldTime;
    float newTime;
    float allTime;

    void clear();

    friend class FPSCounter;
public:
    SystemTimer();
    void start();
    void setBeforeTime();
    void setAfterTime();
    bool isSecond() const;
    bool isSecond_1_10() const;
    bool isSecond_1_100() const;
};

class DYNLIB_PENGINE FPSCounter
{
    std::shared_ptr<SystemTimer> timerFps;
    size_t frameCount;

    void clear();
public:
    FPSCounter();
    void start();
    void setBeforeTime();
    void setAfterTime();
    bool isReady() const;
    size_t getFps();

    FPSCounter& operator++();
};
