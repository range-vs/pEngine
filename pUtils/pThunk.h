#pragma once

#include "../pEntryPoint/pch.h"

// pEngine
#include "putils.h"

class ThunkCreator;

using SmartThunkCreator = std::shared_ptr<ThunkCreator>;

struct Thunk
{
#pragma pack(push, 1)
	unsigned  short stub1; // lea ecx,
	unsigned  long nThisPtr; // this (class)
	unsigned  char stub2; // mov eax,
	unsigned  long nJumpProc; // pointer to destination function
	unsigned  short stub3; // jmp eax
#pragma pack(pop)
};

class ThunkCreator
{
	Thunk* callbackMethod;

public:
	ThunkCreator():callbackMethod(nullptr){}
	ThunkCreator(const ThunkCreator&);
	ThunkCreator& operator=(const ThunkCreator&);
	template<class This, class Method> void createNonStaticCallbackFunction(This thisClass, Method callbackMethod);
	Thunk* getCallbackMethod();
	~ThunkCreator();
};

template<class This, class Method>
inline void ThunkCreator::createNonStaticCallbackFunction(This thisClass, Method callMeth)
{
    // search analog Qt(crossplatform) function VirtualAlloc or run function for OS
    LPVOID memory = VirtualAlloc(NULL, sizeof(Thunk), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    callbackMethod = reinterpret_cast<Thunk*>(memory);
	if (!callbackMethod)
        throw DebugSystemException(L"Error alloc memory for static debug QT proc!");
	callbackMethod->stub1 = 0x0D8D;
	callbackMethod->nThisPtr = reinterpret_cast<unsigned long>(thisClass);
	callbackMethod->stub2 = 0xB8;
    callbackMethod->nJumpProc = CustomCast::brute_cast<unsigned long>(callMeth);
	callbackMethod->stub3 = 0xE0FF;
    FlushInstructionCache(GetCurrentProcess(), callbackMethod, sizeof(Thunk));
}





