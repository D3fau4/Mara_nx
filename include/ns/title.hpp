#pragma once

#include <switch.h>
#include <borealis.hpp>

namespace Mara::ns
{
    class title
    {
    public:
        title(u64 program_id);

        std::string GetTitleName();
        std::string GetTitleAuthor();
        std::string GetTitleVersion();
        u64 GetTitleID();
    private:
        u64 m_programid;
        
        std::string m_titlename;
        std::string m_author;
        std::string m_version;
    };
    
}
