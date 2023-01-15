#include "fs/fs.hpp"

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
            mkdir(path.c_str(), 0777);
            return 0;
        }
        return 0;
    }

    Result DeleteDir(std::string path)
    {
        std::filesystem::remove_all(path);
        rmdir(path.c_str());
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
        FILE *f;
        f = fopen(path.c_str(), "rb");
        if (f)
        {
            fclose(f);
            return 1;
        }
        return 0;
    }

    bool copy_file(const char* From, const char* To, std::size_t MaxBufferSize)
    {
        std::ifstream is(From, std::ios_base::binary);
        std::ofstream os(To, std::ios_base::binary);

        std::pair<char*,std::ptrdiff_t> buffer;
        buffer = std::get_temporary_buffer<char>(MaxBufferSize);

        //Note that exception() == 0 in both file streams,
        //so you will not have a memory leak in case of fail.
        while(is.good() and os)
        {
            is.read(buffer.first, buffer.second);
            os.write(buffer.first, is.gcount());
        }

        std::return_temporary_buffer(buffer.first);

        if(os.fail()) return false;
        if(is.eof()) return true;
        return false;
    }
    Result copyFile(std::string source_file, std::string destination_file) {
        std::filesystem::path p(destination_file);
        std::string parent_path = p.parent_path().string();

        if(!checkdirexist(parent_path)) {
            brls::Logger::info("Carpeta creada: %s", parent_path);
            std::filesystem::create_directories(parent_path);
        }

        if(checkFile(source_file)){
            brls::Logger::error("Archivo no existe:", source_file.c_str());
            brls::Logger::error(source_file.c_str());
            return 1;
        }

        if(std::filesystem::copy_file(source_file, destination_file))
            return 0;

        return 1;
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
}