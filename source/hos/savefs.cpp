#include <borealis.hpp>
#include "hos/savefs.hpp"
#include "Program/Main.hpp"
#include <fatfs.hpp>

FsStorage g_FatFsDumpBisStorage;

Mara::es::TicketFile getTicket(const u64 app_id, FIL &save) {
    Mara::es::TicketFile read_tik_file = {};
    u32 tmp_size = 0;
    u8 tmp_tik_buf[0x400];

    while (true) {
        const auto fr = f_read(&save, tmp_tik_buf, sizeof(tmp_tik_buf), &tmp_size);
        if (fr != FR_OK) {
            brls::Logger::error("Failed to read from file: %d", fr);
            break;
        }
        if (tmp_size == 0) {
            brls::Logger::debug("End of file reached");
            break;
        }

        const auto tik_sig = *reinterpret_cast<Mara::es::TicketSignature*>(tmp_tik_buf);
        if (Mara::es::IsValidTicketSignature(tik_sig)) {
            Mara::es::TicketFile tik_file = { .signature = tik_sig };

            const auto tik_sig_size = Mara::es::GetTicketSignatureSize(tik_file.signature);
            memcpy(tik_file.signature_data, tmp_tik_buf + sizeof(tik_file.signature), Mara::es::GetTicketSignatureDataSize(tik_file.signature));
            memcpy(&tik_file.data, tmp_tik_buf + tik_sig_size, sizeof(tik_file.data));

            if (app_id == tik_file.data.rights_id.GetApplicationId()) {
                read_tik_file = tik_file;
                break;
            } else {
                //brls::Logger::debug("app_id %u != tik_file %u", app_id, tik_file.data.rights_id.GetApplicationId());
            }
        }
    }

    return read_tik_file;
}

Mara::es::Cert readCertificate(const char* expected_issuer, FIL &file) {
    Mara::es::Cert read_cert = {};
    u32 tmp_size = 0;
    u8 tmp_cert_buf[0x400];

    f_rewind(&file);

    while (true) {
        const auto fr = f_read(&file, &tmp_cert_buf, sizeof(tmp_cert_buf), &tmp_size);

        if (fr != FR_OK) {
            brls::Logger::error("Failed to read from file: %d", fr);
            break;
        }

        if (tmp_size == 0) {
            brls::Logger::debug("End of file reached");
            break;
        }

        const auto cert_sig = *reinterpret_cast<Mara::es::SignatureBlock*>(tmp_cert_buf);
        if (IsValidCertificateSignature(cert_sig)) {
            Mara::es::Cert cert = {.signature_block = cert_sig};

            const auto sig_nature_block_size = Mara::es::GetCertificateSignatureSize(cert.signature_block) +
                                               sizeof(Mara::es::SignatureBlock::sig_type) +
                                               sizeof(Mara::es::SignatureBlock::issuer);

            memcpy(&cert.cert_header, tmp_cert_buf + sig_nature_block_size, sizeof(Mara::es::CertHeader));
            memcpy(&cert.public_key_block, tmp_cert_buf + sig_nature_block_size + sizeof(Mara::es::CertHeader),
                   Mara::es::GetCertificatePublicKeyBlockSize(cert.cert_header));

            if (strcmp(expected_issuer, cert.cert_header.subject) == 0) {
                read_cert = cert;
                break;
            }
        }
    }

    return read_cert;
}

Mara::es::TicketFile Mara::hos::ReadTicket(const u64 app_id) {
    Mara::es::TicketFile read_tik_file = {};
    appletSetCpuBoostMode(ApmCpuBoostMode_FastLoad);

    if (R_SUCCEEDED(fsOpenBisStorage(&g_FatFsDumpBisStorage, FsBisPartitionId_System))) {
        FATFS fs;
        FIL save;
        if (f_mount(&fs, SYSTEM_MOUNT_NAME, 1) == FR_OK) {
            if (f_open(&save, COMMON_TICKET_FILE, FA_READ | FA_OPEN_EXISTING) == FR_OK) {
                brls::Logger::debug("f_open %s", COMMON_TICKET_FILE);
                read_tik_file = getTicket(app_id, save);
                f_close(&save);
            }

            if (read_tik_file.data.ticket_version == 0) {
                if (f_open(&save, PERSONALIZED_TICKET_FILE, FA_READ | FA_OPEN_EXISTING) == FR_OK) {
                    brls::Logger::debug("f_open %s", PERSONALIZED_TICKET_FILE);
                    read_tik_file = getTicket(app_id, save);
                    f_close(&save);
                }
            }

            f_mount(nullptr, SYSTEM_MOUNT_NAME, 1);
        } else {
            brls::Logger::error("Failed to mount filesystem");
        }

        fsStorageClose(&g_FatFsDumpBisStorage);
    } else {
        brls::Application::crash("Failed to open BIS storage");
    }

    appletSetCpuBoostMode(ApmCpuBoostMode_Normal);
    return read_tik_file;
}

void Mara::hos::ReadCert(const char* issuer) {
    if (R_SUCCEEDED(fsOpenBisStorage(&g_FatFsDumpBisStorage, FsBisPartitionId_System))) {
        FATFS fs;
        FIL save;
        if (f_mount(&fs, SYSTEM_MOUNT_NAME, 1) == FR_OK && f_open(&save, CERT_SAVE_FILE, FA_READ | FA_OPEN_EXISTING) == FR_OK) {
            brls::Logger::debug("f_open %s", CERT_SAVE_FILE);
            Mara::es::Cert cert = readCertificate(issuer, save);

            if (!Mara::es::IsValidCertificateSignature(cert.signature_block)) {
                brls::Logger::error("Invalid certificate signature");
            }

            f_close(&save);
            f_mount(nullptr, SYSTEM_MOUNT_NAME, 1);
        } else {
            brls::Logger::error("Failed to read certificate");
        }

        fsStorageClose(&g_FatFsDumpBisStorage);
    } else {
        brls::Application::crash("Failed to open BIS storage");
    }
}
