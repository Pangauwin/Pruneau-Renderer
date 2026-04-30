#include "log.h"

#include "core/application.h"
#include "platform/terminal.h"

#include <stdlib.h>

#include <ctime>
#include <time.h>


namespace Core 
{

// TODO: Colorize console Outputs

void LogMessage(std::string _message)
{
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);

    char output[50];

    std::strftime(output, 50, "%Y-%m-%d %H:%M:%S", &datetime);

    Platform::Print("[" + (std::string)(output) + "] ");

    Platform::PrintLn(_message);
}

void LogMessageDebug(std::string _message)
{
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);

    char output[50];

    std::strftime(output, 50, "%Y-%m-%d %H:%M:%S", &datetime);

    Platform::Print("[" + (std::string)(output) + "] ");


    Platform::Print((std::string)"[DBG]" + "\t", Platform::TerminalColorCode::FG_GREEN);

    Platform::PrintLn(_message);
}

void LogMessageInfo(std::string _message)
{
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);

    char output[50];

    std::strftime(output, 50, "%Y-%m-%d %H:%M:%S", &datetime);

    Platform::Print("[" + (std::string)(output) + "] ");
    

    Platform::Print((std::string)"[INF]" + "\t", Platform::TerminalColorCode::FG_BLUE);

    Platform::PrintLn(_message);
}

void LogMessageWarning(std::string _message)
{
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);

    char output[50];

    std::strftime(output, 50, "%Y-%m-%d %H:%M:%S", &datetime);

    Platform::Print("[" + (std::string)(output) + "] ");


    Platform::Print((std::string)"[WRN]" + "\t", Platform::TerminalColorCode::FG_YELLOW);

    Platform::PrintLn(_message);
}

void LogMessageError(std::string _message)
{
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);

    char output[50];

    std::strftime(output, 50, "%Y-%m-%d %H:%M:%S", &datetime);

    Platform::Print("[" + (std::string)(output) + "] ");


    Platform::Print((std::string)"[ERR]" + "\t", Platform::TerminalColorCode::FG_RED);

    Platform::PrintLn(_message);
}

void LogMessageCritical(std::string _message)
{
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);

    char output[50];

    std::strftime(output, 50, "%Y-%m-%d %H:%M:%S", &datetime);

    Platform::Print("[" + (std::string)(output) + "] ");


    Platform::Print((std::string)"[CRIT]" + "\t", 
        Platform::TerminalColorCode::FG_BLACK,
        Platform::TerminalColorCode::BG_WHITE);

    Platform::PrintLn(_message);
}

}