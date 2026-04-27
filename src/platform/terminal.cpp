#include "terminal.h"

#include <iostream>

Platform::TerminalModifier::TerminalModifier(Platform::TerminalColorCode _code) : code(_code) 
{

}

void Platform::Print(std::string message)
{
    std::cout << message;
}

void Platform::Print(std::string message, TerminalColorCode foreground)
{
    std::cout << TerminalModifier(foreground) << message << 
    TerminalModifier(TerminalColorCode::FG_DEFAULT);
}

void Platform::Print(std::string message, TerminalColorCode foreground, TerminalColorCode background)
{
    std::cout << TerminalModifier(foreground) << TerminalModifier(background) << 
        message << 
        TerminalModifier(TerminalColorCode::FG_DEFAULT) << 
        TerminalModifier(TerminalColorCode::BG_DEFAULT);
}

void Platform::PrintLn(std::string message)
{
    std::cout << message << "\n";
}

void Platform::PrintLn(std::string message, TerminalColorCode foreground)
{
    std::cout << TerminalModifier(foreground) << message << TerminalModifier(TerminalColorCode::FG_DEFAULT) << "\n";
}

void Platform::PrintLn(std::string message, TerminalColorCode foreground, TerminalColorCode background)
{
    std::cout << TerminalModifier(foreground) << TerminalModifier(background) << 
        message << 
        TerminalModifier(TerminalColorCode::FG_DEFAULT) << 
        TerminalModifier(TerminalColorCode::BG_DEFAULT) << "\n";
}