#include <switch/services/fs.h>
#include <borealis.hpp>
#include <fatfs.hpp>
#include "hos/savefs.hpp"
#include "Program/Main.hpp"

Mara::es::TicketFile getTicket(const u64 app_id, FIL &save) {
    Mara::es::TicketFile read_tik_file = {};
    u32 tmp_size = 0;
    u8 tmp_tik_buf[0x400];
    while (true){
        const auto fr = f_read(&save, tmp_tik_buf, sizeof(tmp_tik_buf), &tmp_size);
        if(fr != FR_OK) {
            break;
        }
        if(tmp_size == 0) {
            break;
        }

        const auto tik_sig = *reinterpret_cast<Mara::es::TicketSignature*>(tmp_tik_buf);
        if(Mara::es::IsValidTicketSignature(tik_sig)){
            Mara::es::TicketFile tik_file = { .signature = tik_sig };

            const auto tik_sig_size = Mara::es::GetTicketSignatureSize(tik_file.signature);
            memcpy(tik_file.signature_data, tmp_tik_buf + sizeof(tik_file.signature), Mara::es::GetTicketSignatureDataSize(tik_file.signature));
            memcpy(&tik_file.data, tmp_tik_buf + tik_sig_size, sizeof(tik_file.data));

            if(app_id == tik_file.data.rights_id.GetApplicationId()) {
                read_tik_file = tik_file;
                break;
            }
        }
    }
    return read_tik_file;
}

Mara::es::TicketFile Mara::hos::ReadTicket(const u64 app_id){
    Mara::es::TicketFile read_tik_file = {};
    FsStorage g_FatFsDumpBisStorage;

    if (R_SUCCEEDED(fsOpenBisStorage(&g_FatFsDumpBisStorage, FsBisPartitionId_System))){
        FATFS fs;
        FIL save;
        if(f_mount(&fs, SYSTEM_MOUNT_NAME, 1) == FR_OK) {
            if (f_open(&save, COMMON_TICKET_FILE, FA_READ | FA_OPEN_EXISTING) == FR_OK) {
                read_tik_file = getTicket(app_id, save);
                f_close(&save);
            }

            if (read_tik_file.data.ticket_version == 0) {
                if (f_open(&save, PERSONALIZED_TICKET_FILE, FA_READ | FA_OPEN_EXISTING) == FR_OK) {
                    read_tik_file = getTicket(app_id, save);
                    f_close(&save);
                }
            }

            f_mount(nullptr, SYSTEM_MOUNT_NAME, 1);
        }

        fsStorageClose(&g_FatFsDumpBisStorage);
    }
    else {
        brls::Application::crash("");
    }
    return read_tik_file;
}

