
#include <borealis.hpp>
#include "hos/savefs.hpp"
#include "Program/Main.hpp"
#include <fatfs.hpp>

FsStorage g_FatFsDumpBisStorage;


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

Mara::es::Cert readCertificate(const char* expected_issuer, FIL &file) {
    Mara::es::Cert read_cert = {};
    u32 tmp_size = 0;
    u8 tmp_cert_buf[0x700];  // Tamaño suficiente para un bloque de certificado



    while (true) {
        const auto fr = f_read(&file, &tmp_cert_buf, sizeof(tmp_cert_buf), &tmp_size);
        brls::Logger::info("f_read result: %d, tmp_size: %u", fr, tmp_size);
        if (fr != FR_OK) {
            brls::Logger::error("Read error: %d", fr);
            break;
        }
        if (tmp_size == 0) {
            brls::Logger::info("End of file reached or no bytes read, tmp_size %u", tmp_size);
            break;
        }

        const auto cert_sig = *reinterpret_cast<Mara::es::SignatureBlock*>(tmp_cert_buf);
        if (IsValidCertificateSignature(cert_sig)) {
            brls::Logger::info("IsValidCertificateSignature %c", cert_sig.issuer);
            Mara::es::Cert cert = { .signature_block = cert_sig };

            const auto cert_sig_size = GetCertificateSignatureSize(cert.signature_block);
            memcpy(cert.signature_block.issuer, tmp_cert_buf + cert_sig_size, sizeof(cert.signature_block.issuer));
            memcpy(&cert.cert_header, tmp_cert_buf + cert_sig_size + sizeof(cert.signature_block.issuer), sizeof(cert.cert_header));

            // Comparar el issuer con el esperado
            if (strncmp(cert.signature_block.issuer, expected_issuer, sizeof(cert.signature_block.issuer)) != 0) {
                brls::Logger::info("%s", cert.signature_block.issuer);
                continue;
            }

            // Manejo del bloque de clave pública según el tipo de clave pública
            size_t header_offset = cert_sig_size + sizeof(cert.signature_block.issuer) + sizeof(cert.cert_header);
            switch (cert.cert_header.pub_key_type) {
                case Mara::es::PubKeyType_Rsa4096:
                    memcpy(cert.public_key_block.rsa4096.public_key, tmp_cert_buf + header_offset, sizeof(cert.public_key_block.rsa4096.public_key));
                    memcpy(&cert.public_key_block.rsa4096.public_exponent, tmp_cert_buf + header_offset + sizeof(cert.public_key_block.rsa4096.public_key), sizeof(cert.public_key_block.rsa4096.public_exponent));
                    memcpy(cert.public_key_block.rsa4096.reserved_1, tmp_cert_buf + header_offset + sizeof(cert.public_key_block.rsa4096.public_key) + sizeof(cert.public_key_block.rsa4096.public_exponent), sizeof(cert.public_key_block.rsa4096.reserved_1));
                    break;
                case Mara::es::PubKeyType_Rsa2048:
                    memcpy(cert.public_key_block.rsa2048.public_key, tmp_cert_buf + header_offset, sizeof(cert.public_key_block.rsa2048.public_key));
                    memcpy(&cert.public_key_block.rsa2048.public_exponent, tmp_cert_buf + header_offset + sizeof(cert.public_key_block.rsa2048.public_key), sizeof(cert.public_key_block.rsa2048.public_exponent));
                    memcpy(cert.public_key_block.rsa2048.reserved_1, tmp_cert_buf + header_offset + sizeof(cert.public_key_block.rsa2048.public_key) + sizeof(cert.public_key_block.rsa2048.public_exponent), sizeof(cert.public_key_block.rsa2048.reserved_1));
                    break;
                case Mara::es::PubKeyType_Ecc480:
                    memcpy(cert.public_key_block.ecc480.public_key, tmp_cert_buf + header_offset, sizeof(cert.public_key_block.ecc480.public_key));
                    memcpy(cert.public_key_block.ecc480.reserved_1, tmp_cert_buf + header_offset + sizeof(cert.public_key_block.ecc480.public_key), sizeof(cert.public_key_block.ecc480.reserved_1));
                    break;
                default:
                    brls::Application::crash("Tipo de clave pública no soportado.");
                    break;
            }

            read_cert = cert;
            break;
        }
    }
    return read_cert;
}

Mara::es::TicketFile Mara::hos::ReadTicket(const u64 app_id){
    Mara::es::TicketFile read_tik_file = {};

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

void Mara::hos::ReadCert(const char* issuer){
    if (R_SUCCEEDED(fsOpenBisStorage(&g_FatFsDumpBisStorage, FsBisPartitionId_System))) {
        FATFS fs;
        FIL save;
        if(f_mount(&fs, SYSTEM_MOUNT_NAME, 1) == FR_OK && f_open(&save, CERT_SAVE_FILE, FA_READ | FA_OPEN_EXISTING) == FR_OK){
            Mara::es::Cert cert = readCertificate(issuer, save);

            if(cert.cert_header.date == 0) {
                brls::Logger::error("algo va mal");
            }

            f_close(&save);
            f_mount(nullptr, SYSTEM_MOUNT_NAME, 1);
        } else {
            brls::Logger::error("ReadCert fail");
        }
        fsStorageClose(&g_FatFsDumpBisStorage);
    } else {
        brls::Application::crash("fsOpenBisStorage fail");
    }
}