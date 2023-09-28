#pragma once

#include <switch.h>
#include <borealis.hpp>

namespace Mara::ns
{
    class Title
    {
    public:
        Title(u64 program_id);

        std::string GetTitleName();
        std::string GetTitleAuthor();
        std::string GetTitleVersion();
        u64 GetTitleID();
        void Launch();
    private:
        u64 m_programid;
        
        std::string m_titlename;
        std::string m_author;
        std::string m_version;
    };
    
}
