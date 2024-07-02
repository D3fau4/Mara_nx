
#include "hos/savefs.hpp"
#include <switch/services/fs.h>
#include <borealis.hpp>
#include "fatfs/fatfs.hpp"
#include "tc/io/IStream.h"

FsStorage g_FatFsDumpBisStorage;

Mara::es::TicketFile Mara::hos::ReadTicket(u64 app_id){
    Mara::es::TicketFile read_tik_file = {};

    if (R_SUCCEEDED(fsOpenBisStorage(&g_FatFsDumpBisStorage, FsBisPartitionId_System))){
        FATFS fs;
        FIL save;
        if(f_mount(&fs, "0", 1) == FR_OK && f_open(&save, "0:/save/80000000000000e1", FA_READ | FA_OPEN_EXISTING) == FR_OK) {
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
                        brls::Logger::error("ticket encontrado");
                        read_tik_file = tik_file;
                        break;
                    }
                    brls::Logger::info("%016X", tik_file.data.rights_id.GetApplicationId());
                    brls::Logger::info("Original %016X", app_id);
                }
            }

            f_close(&save);
            f_mount(nullptr, "0", 1);
            fsStorageClose(&g_FatFsDumpBisStorage);
        } else {
            brls::Application::crash("");
        }
    }
    return read_tik_file;
}

void Mara::WriteToStream(tc::io::IStream& stream, const Mara::es::TicketFile& tik_file) {
    // Escribir la firma
    stream.write(reinterpret_cast<const uint8_t*>(&tik_file.signature), sizeof(tik_file.signature));

    // Escribir los datos de la firma
    size_t signature_data_size = Mara::es::GetTicketSignatureDataSize(tik_file.signature);
    stream.write(reinterpret_cast<const uint8_t*>(tik_file.signature_data), signature_data_size);

    // Escribir los datos
    stream.write(reinterpret_cast<const uint8_t*>(&tik_file.data), sizeof(tik_file.data));
}