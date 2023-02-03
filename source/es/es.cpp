//
// Created by raul2 on 21/01/2023.
//

#include "es/es.hpp"

namespace Mara {
    es::es(int securityLevel) {
        smGetService(&this->g_EsService, "es");

        if(&this->g_EsService == nullptr)
            brls::Logger::error("Failed to get es service");
        this->securityLevel = static_cast<SecurityLevel>(securityLevel);
    }

    es::~es() {
    }

    Result es::CountCommonTicket(s32 *out_count){
        struct {
            s32 num_tickets;
        } out;

        Result rc = serviceDispatchOut(&this->g_EsService, 9, out);
        if (R_SUCCEEDED(rc) && out_count) *out_count = out.num_tickets;

        return rc;
    }

    Result es::CountPersonalizedTicket(s32 *out_count){
        struct {
            s32 num_tickets;
        } out;

        Result rc = serviceDispatchOut(&this->g_EsService, 10, out);
        if (R_SUCCEEDED(rc) && out_count) *out_count = out.num_tickets;

        return rc;
    }

    Result es::CountPrepurchaseRecord(s32 *out_count){
        struct {
            s32 num_tickets;
        } out;

        Result rc = serviceDispatchOut(&this->g_EsService, 27, out);
        if (R_SUCCEEDED(rc) && out_count) *out_count = out.num_tickets;

        return rc;
    }

    Result es::ListCommonTicketRightsIds(s32 *out_entries_written, FsRightsId *out_ids, s32 count){
        struct {
            s32 num_rights_ids_written;
        } out;

        Result rc = serviceDispatchInOut(&this->g_EsService, 11, *out_entries_written, out,
                                         .buffer_attrs = { SfBufferAttr_HipcMapAlias | SfBufferAttr_Out },
                                         .buffers = { { out_ids, (size_t)count * sizeof(FsRightsId) } }
        );

        if (R_SUCCEEDED(rc) && out_entries_written) *out_entries_written = out.num_rights_ids_written;

        return rc;
    }

    Result es::ListPersonalizedTicketRightsIds(s32 *out_entries_written, FsRightsId *out_ids, s32 count){
        struct {
            s32 num_rights_ids_written;
        } out;

        Result rc = serviceDispatchInOut(&this->g_EsService, 12, *out_entries_written, out,
                                         .buffer_attrs = { SfBufferAttr_HipcMapAlias | SfBufferAttr_Out },
                                         .buffers = { { out_ids, (size_t)count * sizeof(FsRightsId) } }
        );

        if (R_SUCCEEDED(rc) && out_entries_written) *out_entries_written = out.num_rights_ids_written;

        return rc;
    }

    Result es::ListPrepurchaseRecordRightsIds(s32 *out_entries_written, FsRightsId *out_ids, s32 count){
        struct {
            s32 num_rights_ids_written;
        } out;

        Result rc = serviceDispatchInOut(&this->g_EsService, 28, *out_entries_written, out,
                                         .buffer_attrs = { SfBufferAttr_HipcMapAlias | SfBufferAttr_Out },
                                         .buffers = { { out_ids, (size_t)count * sizeof(FsRightsId) } }
        );

        if (R_SUCCEEDED(rc) && out_entries_written) *out_entries_written = out.num_rights_ids_written;

        return rc;
    }

    bool es::isRightIdPurchased(u64 right_id){

        u32 entrys;
        FsRightsId *ids = nullptr;
        u32 count;
        this->CountPersonalizedTicket((s32*)&count);
        ids = static_cast<FsRightsId *>(calloc(count, sizeof(FsRightsId)));
        brls::Logger::debug("CountPersonalizedTicket: {}", count);
        this->ListPersonalizedTicketRightsIds((s32*)&entrys, ids, (s32)count);

        for (u32 i = 0; i < count; i++){
            auto v = ids[i];
            char buffer [17];
            buffer[16] = 0;
            for(int j = 0; j < 8; j++)
                sprintf(&buffer[2*j], "%02X", v.c[j]);

            auto program_id = to64b(v.c);

            if(program_id == right_id){
                return true;
            }

            //brls::Logger::debug("RightId: {}", buffer);
        }

        return false;
    }

    bool es::isRightIdPirated(u64 right_id){

        u32 entrys;
        FsRightsId *ids = nullptr;
        u32 count;
        this->CountCommonTicket((s32*)&count);
        ids = static_cast<FsRightsId *>(calloc(count, sizeof(FsRightsId)));
        brls::Logger::debug("CountCommonTicket: {}", count);
        this->ListCommonTicketRightsIds((s32*)&entrys, ids, (s32)count);

        for (u32 i = 0; i < count; i++){
            auto v = ids[i];
            char buffer [17];
            buffer[16] = 0;
            for(int j = 0; j < 8; j++)
                sprintf(&buffer[2*j], "%02X", v.c[j]);

            auto program_id = to64b(v.c);

            if(right_id == program_id){
                brls::Logger::info("Program id: {}", right_id);
                return true;
            }

            //brls::Logger::debug("RightId: {}", buffer);
        }

        return false;
    }
}