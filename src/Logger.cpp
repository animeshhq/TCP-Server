#include "../include/Logger.hpp"
#include <iostream>

void info(const std::string& msg)
{
    std::cout << CYAN << "[INFO] " << RESET << msg << '\n';
}

void success(const std::string& msg)
{
    std::cout << GREEN << "[SUCCESS] " << RESET << msg << '\n';
}

void warning(const std::string& msg)
{
    std::cout << YELLOW << "[WARNING] " << RESET << msg << '\n';
}

void error(const std::string& msg)
{
    std::cout << RED << "[ERROR] " << RESET << msg << '\n';
}
