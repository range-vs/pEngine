#include "pengine.h"

void SystemTimer::clear()
{
    oldTime = newTime = allTime = 0.f;
}

SystemTimer::SystemTimer()
{
    timer.reset(new QTime);
    clear();
}

void SystemTimer::start()
{
    timer->start();
    clear();
}

void SystemTimer::setBeforeTime()
{
    oldTime = timer->elapsed();
}

void SystemTimer::setAfterTime()
{
    newTime = timer->elapsed();
    allTime = newTime - oldTime;
}

bool SystemTimer::isSecond() const
{
    return allTime >= 1000.f;
}

bool SystemTimer::isSecond_1_10() const
{
    return allTime >= 100.f;
}

bool SystemTimer::isSecond_1_100() const
{
    return allTime >= 10.f;
}

void FPSCounter::clear()
{
    timerFps->clear();
    frameCount = 0;
}

FPSCounter::FPSCounter()
{
    timerFps.reset(new SystemTimer);
    clear();
}

void FPSCounter::start()
{
    timerFps->start();
    clear();
}

void FPSCounter::setBeforeTime()
{
    timerFps->setBeforeTime();
}

void FPSCounter::setAfterTime()
{
    timerFps->setAfterTime();
}

bool FPSCounter::isReady() const
{
    return timerFps->allTime >= 500.f;
}

size_t FPSCounter::getFps()
{
    size_t fps = 1000 * frameCount / timerFps->allTime;
    clear();
    return fps;
}

FPSCounter &FPSCounter::operator++()
{
    frameCount++;
    return *this;
}

