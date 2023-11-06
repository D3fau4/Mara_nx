#pragma once

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <filesystem>
#include <vector>
#include <borealis.hpp>

namespace Mara::fs {
    bool copy_file(const char* From, const char* To, std::size_t MaxBufferSize = 1048576);
    Result checkFile(std::string path);
    Result createFile(std::string path);
    Result DeleteDir(std::string path);
    Result createdir(std::string path);
    bool checkdirexist(std::string path);
    std::vector<std::string> getDirectories(const std::string &s);
    std::vector<std::string> getFiles(const std::string &s);
    Result DeleteFile(std::string path);
}