#pragma once

#include "platform.h"

#include <ostream>
#include <string>


namespace Platform {

#ifdef PLATFORM_LINUX

enum TerminalColorCode {
    FG_BLACK    = 30,
    FG_RED      = 31,
    FG_GREEN    = 32,
    FG_YELLOW   = 33,
    FG_BLUE     = 34,
    FG_MAGENTA  = 35,
    FG_CYAN     = 36,
    FG_WHITE    = 37,

    FG_DEFAULT  = 39,

    BG_BACK     = 40,
    BG_RED      = 41,
    BG_GREEN    = 42,
    BG_YELLOW   = 43,
    BG_BLUE     = 44,
    BG_MAGENTA  = 45,
    BG_CYAN     = 46,
    BG_WHITE    = 47,

    BG_DEFAULT  = 49
};

#endif

void Print(std::string _message);
void Print(std::string _message, TerminalColorCode foreground);
void Print(std::string _message, TerminalColorCode foreground, TerminalColorCode background);

void PrintLn(std::string _message);
void PrintLn(std::string _message, TerminalColorCode foreground);
void PrintLn(std::string _message, TerminalColorCode foreground, TerminalColorCode background);



//TODO : Implement these on Windows too

class TerminalModifier
{
public:
    TerminalModifier(TerminalColorCode _code);
    ~TerminalModifier() = default;

private:
    TerminalColorCode code;

    #ifdef PLATFORM_LINUX

    friend std::ostream&
    operator<<(std::ostream& os, const TerminalModifier& mod) {
        return os << "\033[" << mod.code << "m";
    }

    #endif
};

}