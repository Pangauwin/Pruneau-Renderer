#pragma once

#include <string>

namespace Core {

void LogMessage(std::string _message);
void LogMessageDebug(std::string _message);
void LogMessageInfo(std::string _message);
void LogMessageWarning(std::string _message);
void LogMessageError(std::string _message);
void LogMessageCritical(std::string _critical);

}