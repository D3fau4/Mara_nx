#include <fstream>
#include <string>
#include "helpers/PatchData.hpp"
#include "Program/Main.hpp"
#include "fs/fs.hpp"
#include "borealis/logger.hpp"

namespace Mara::helpers {
    PatchData::PatchData() {
        initializeConstants();
    }

    PatchData::~PatchData() {
        patch_files.clear();
        delete program;
    }

    void PatchData::initializeConstants() {
        nlohmann::json jsonData;
        this->game_found = false;

        std::ifstream inputFile(VERSIONS_FILE);
        if (!inputFile.is_open()) {
            brls::Logger::error("No se ha podido abrir el archivo de versiones %s", VERSIONS_FILE);
            return;
        }

        inputFile >> jsonData;   
        brls::Logger::info("Archivo de versiones abierto");

        if (!jsonData.contains("game_versions") || !jsonData["game_versions"].is_structured()) {
            brls::Logger::error("El archivo de versiones no contiene 'game_versions' o no es válido");
            return;
        }

        for (const auto& version : jsonData["game_versions"]) {
            if (!version.contains("game_pid") || !version.contains("base_path"))
                continue;
            
            u64 game_pid = 0;
            try {
                game_pid = std::stoull(version["game_pid"].get<std::string>(), nullptr, 16);
            } catch (...) {
                brls::Logger::error("game_pid inválido en el archivo de versiones");
                continue;
            }

            for (const auto& title : Mara::ns::getAllTitles()) {
                if (title.second && game_pid == title.second->GetTitleID()) {
                    if (program) 
                        delete program;
                    program = new ns::Title(game_pid);
                    brls::Logger::info("Juego encontrado");
                    this->base_path = version["base_path"].get<std::string>();
                    this->patch_files = Mara::fs::getFiles(ROMFS_MOUNT_NAME + this->base_path);
                    this->game_found = true;
                    if (this->patch_files.empty()) {
                        brls::Logger::error("No se han encontrado parches xdelta en la carpeta %s%s", ROMFS_MOUNT_NAME, this->base_path.c_str());
                    }

                    inputFile.close();
                    return;
                }
            }
        }
    }
}