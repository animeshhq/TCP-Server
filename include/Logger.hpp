#pragma once

#include <string>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

#define BUFFER_SIZE 4096

void info(const std::string& msg);
void success(const std::string& msg);
void warning(const std::string& msg);
void error(const std::string& msg);
