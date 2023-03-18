#include "hr.hpp"
#include <filesystem>
#include <iostream>
#include <regex>
#include <source_location>
#include <thread>
#include <unistd.h>

std::string dir;

std::vector<std::string> findFiles() {

  std::vector<std::string> files{};
  for (const auto &entry : std::filesystem::directory_iterator(dir)) {
    if (entry.is_regular_file()) {
      std::string extension = entry.path().extension().string();
      if (extension == ".cpp" || extension == ".h" || extension == ".hpp") {
        files.push_back(entry.path().filename());
      }
    }
  }
  return files;
}

std::string removeExtension(std::string const &file) {
  std::regex pattern(R"((\w+)\.cpp)");
  std::smatch matches;

  if (std::regex_search(file, matches, pattern)) {
    std::string name = matches[1];
    // std::cout << "File name: " << name << std::endl;
    //  std::cout << "File extension: .cpp" << std::endl;
    return name;
  } else {
    // std::cout << "Invalid file name." << std::endl;
    return "";
  }
}

void compilePlz(std::string const &file, enum compilerType compiler,
                std::string const &options) {

  std::string command;
  // compiling with cmake
  chdir("../");
  bool result = searchCmake();
  if (result) {
    command = "cmake -Bbuild && make -C build";
    removeExtension(file);
    system(command.c_str());
    chdir("./build");
  } else {
    std::filesystem::current_path(dir);
    std::string comp;

    switch (compiler) {
    case compilerType::gcc:
      comp = "g++ ";
      break;
    case compilerType::clang:
      comp = "clang++ ";
    }

    command = comp + options + " " + removeExtension(file) + " ";

    auto files = findFiles();
    for (auto const &f : files)
      command += f + " ";

    //  std::cout << "compile without cmake: " << command << "\n\n\n";

    system(command.c_str());
  }
}

bool searchCmake() {
  std::string cmake_file = "CMakeLists.txt";

  // std::cout << "Search for Cmake in " << std::filesystem::current_path()
  //          << "\n\n\n";
  for (auto const &entry :
       std::filesystem::directory_iterator(std::filesystem::current_path())) {
    std::cout << entry.path().filename() << "\n\n";
    if (entry.is_regular_file() && entry.path().filename() == cmake_file) {
      //  std::cout << "Cmake file founded" << entry.path().filename();
      return true;
    }
  }
  // std::cout << "Cmake nottttt found\n\n";
  return false;
}
void HOTRELOAD(std::source_location location) {

  std::endl(std::cout);
  dir = std::filesystem::current_path();
  checkModified(location.file_name());
}

void checkModified(const std::string &path) {
  auto file_path = std::filesystem::path(path);

  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // check if the file exist
    if (std::filesystem::exists(file_path)) {
      // Get the last modified time of the file
      auto last_modified = std::filesystem::last_write_time(file_path);

      // Wait for a few seconds
      std::this_thread::sleep_for(std::chrono::seconds(3));

      // Check if the file has been modified
      if (last_modified != std::filesystem::last_write_time(file_path)) {
        // std::cout << "File has been modified\n";
        compilePlz(path);
      } else {
        //   std::cout << "File hasn't modified\n";
      }

    } else {
      std::cout << file_path;
      // std::cout << " File does not exist." << std::endl;
    }
  }
}
