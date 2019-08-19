#include "putils.h"

void InspectHRESULT::debugging(HRESULT hr, const char* file, int line)
{
    if(FAILED(hr))
    {
        _com_error err(hr);
        std::string out("\nFile: ");
        out += std::string(file, file + strlen(file)) + ";\nLine: " + std::to_string(line) + ";\nError: " +
        std::string(err.ErrorMessage(), err.ErrorMessage() + wcslen(err.ErrorMessage()));
        qFatal(out.c_str());
    }
}



void ParserArgsTerminal::defaultArgs()
{
    args.insert({"-mode-engine", 0}); // 0 - game, 1 - editor
    args.insert({"-mode-render", 0}); // 0 - d3d, 1 - opengl, etc - TODO
}

void ParserArgsTerminal::initializeSystem(char **argv, int argc)
{
    for(int i(1); i < argc;i+=2)
    {
        std::string key(argv[i], argv[i] + strlen(argv[i]));
        if(i + 1 >= argc)
            qFatal(std::string("Error parse argument for key: " + key).c_str());
        int value = QVariant(argv[i + 1]).toInt();
        if(args.find(key) == args.end())
            args.insert({key, value});
        else
            args[key] = value;
    }
}

int &ParserArgsTerminal::getValue(const std::string &key)
{
    if(args.find(key) == args.end())
        qFatal(std::string("No find value for arg terminal: " + key).c_str());
    return args[key];
}
