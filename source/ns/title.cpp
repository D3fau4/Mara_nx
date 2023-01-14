#include "ns/title.hpp"

namespace Mara::ns
{
    title::title(u64 program_id) : m_programid(program_id)
    {
        NsApplicationControlData appControlData;
        size_t appControlDataSize = 0;
        NacpLanguageEntry *languageEntry = nullptr;

        std::memset(&appControlData, 0x00, sizeof(NsApplicationControlData));

        if(R_FAILED(nsGetApplicationControlData(NsApplicationControlSource_Storage, program_id, &appControlData, sizeof(NsApplicationControlData), &appControlDataSize)))
        {
            brls::Logger::error("Fallo al obtener informacion del juego.");
            return;
        }

        if(R_FAILED(nacpGetLanguageEntry(&appControlData.nacp, &languageEntry)))
        {
            brls::Logger::error("Fallo al obtener informacion del juego.");
            return;
        }

        this->m_titlename = std::string(languageEntry->name);
        this->m_author = std::string(languageEntry->author);
        this->m_version = std::string(appControlData.nacp.display_version);
    }

    std::string title::GetTitleName()
    {
        return this->m_titlename;
    }
    
    std::string title::GetTitleAuthor()
    {
        return this->m_author;
    }
    
    std::string title::GetTitleVersion()
    {
        return this->m_version;
    }

    u64 title::GetTitleID()
    {
        return this->m_programid;
    }
}
