#include "ns/ns.hpp"

namespace Mara::ns
{
    std::map<u64,Mara::ns::Title*>& getAllTitles()
    {
        static std::map<u64, Mara::ns::Title*> titles;
        NsApplicationRecord *appRecords = new NsApplicationRecord[1024];
        s32 actualAppRecordCnt = 0;
        
        if(R_FAILED(nsListApplicationRecord(appRecords, 1024, 0, &actualAppRecordCnt)))
        {
            brls::Logger::error("No se pudo obtener los registros");
            return titles;
        }

        for(int i = 0; i < actualAppRecordCnt; i++)
        {
            titles.insert({appRecords[i].application_id, new Mara::ns::Title(appRecords[i].application_id)});
        }
        
        return titles;
    }
}
