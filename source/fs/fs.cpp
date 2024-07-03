#include <regex>
#include "fs/fs.hpp"
#include "Program/Main.hpp"

namespace Mara::fs {

    bool checkdirexist(std::string path)
    {
        bool exist = false;
        DIR *dir = opendir(path.c_str());
        if (dir)
        {
            closedir(dir);
            exist = true;
        }
        return exist;
    }

    Result createdir(std::string path)
    {
        if (checkdirexist(path) == false)
        {
            std::filesystem::create_directories(path.c_str());
            mkdir(path.c_str(), 0777);
            return 0;
        }
        return 0;
    }

    Result DeleteDir(std::string path)
    {
        auto dir = getFiles(path);
        for (auto f : dir){
            DeleteFile(f);
        }
        //rmdir(path.c_str());

        return 0;
    }

    Result createFile(std::string path)
    {
        std::filesystem::path p(path);
        std::string parent_path = p.parent_path().string();
        if(!checkdirexist(parent_path)) {
            std::filesystem::create_directories(parent_path);
        }

        FILE *f;
        f = fopen(path.c_str(), "w");
        if (f)
        {
            fclose(f);
            return 0;
        }
        return 1;
    }

    Result checkFile(std::string path)
    {
        std::ifstream is(path, std::ios_base::binary);

        if (is.good()){
            is.close();
            return 0;
        } else {
            is.close();
            return 1;
        }
    }

    bool copy_file(const char* From, const char* To, std::size_t MaxBufferSize)
    {
        std::ifstream is(From, std::ios_base::binary);
        std::ofstream os(To, std::ios_base::binary);

        std::vector<char> buffer(MaxBufferSize);

        while (is.good() && os) {
            is.read(buffer.data(), buffer.size());
            os.write(buffer.data(), is.gcount());
        }

        if (os.fail()) return false;
        if (is.eof()) return true;
        return false;
    }


    Result DeleteFile(std::string path)
    {
        std::filesystem::remove(path);
        return 0;
    }

    std::vector<std::string> getDirectories(const std::string &s)
    {
        std::vector<std::string> r;
        for (auto &p : std::filesystem::recursive_directory_iterator(s))
            if (p.is_directory())
                r.push_back(p.path().filename());
        return r;
    }

    std::vector<std::string> getFiles(const std::string &s)
    {
        std::vector<std::string> r;
        for (auto &p : std::filesystem::recursive_directory_iterator(s))
            if (!p.is_directory()){
                std::string path = p.path();
                r.push_back(std::regex_replace(path, std::regex("romfs:/Patch/[a-zA-Z]+/"), std::string()));
            }
        return r;
    }

}