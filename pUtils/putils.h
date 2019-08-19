#pragma once

#include "../pEntryPoint/pch.h"

// pEngine
#include "pThunk.h"

// new types
using uint = unsigned int;


#if defined(DYNAMIC_LIBRARY)
#  define DYNLIB_EXPORT Q_DECL_EXPORT
#else
#  define DYNLIB_EXPORT Q_DECL_IMPORT
#endif

class InspectHRESULT;

using DebugHR = InspectHRESULT;


class InspectHRESULT
{
public:
    static void debugging(HRESULT hr, const char* file = __FILE__, int line = __LINE__);
    template<class T>
    static void debuggingDynamic(T obj, const char* file = __FILE__, int line = __LINE__);
};

template<class T>
void InspectHRESULT::debuggingDynamic(T obj, const char *file, int line)
{
    if(!obj)
    {
        std::string out("\nFile: ");
        out += std::string(file, file + strlen(file)) + ";\nLine: " + std::to_string(line) + ";\nError: address of object is nullptr!";
        qFatal(out.c_str());
    }
}

class CustomCast
{
public:
    template<typename Target, typename Source>
    static Target brute_cast(const Source);
};

template<typename Target, typename Source>
inline Target CustomCast::brute_cast(const Source s)
{
    if (abs((int)(sizeof(Target) - sizeof(Source))) > 4) // size adress method - 8 bit (???)
        qFatal("Types size is are identical!");
    union { Target t; Source s; } u;
    u.s = s;
    return u.t;
}

class ParserArgsTerminal
{
    std::map<std::string, int> args;

    void defaultArgs();
public:
    ParserArgsTerminal(){defaultArgs();}
    void initializeSystem(char** argv, int argc);
    int& getValue(const std::string& key);
};

class IApplication
{

};


