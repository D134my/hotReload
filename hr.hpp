#ifndef __hotReloader__
#define __hotReloader__

#include <source_location>
#include <string>
#include <vector>

enum class compilerType { clang, gcc };
std::vector<std::string> findFiles();
std::string removeExtension(std::string const &file);
bool searchCmake();
void compilePlz(std::string const &file = "main.cpp",
                enum compilerType compiler = compilerType::gcc,
                std::string const &options = "-std=c++23 -o ");
void checkModified(std::string const &path);
void HOTRELOAD(std::source_location location = std::source_location::current());

#endif
