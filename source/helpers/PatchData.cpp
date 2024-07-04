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
        if (inputFile.is_open()) {
            inputFile >> jsonData;
            brls::Logger::info("Archivo de versiones abierto");
            // Cargamos los datos del programa
            if (jsonData.contains("game_versions") && jsonData["game_versions"].is_structured()) {
                for (const auto &version: jsonData["game_versions"]) {
                    for (auto &title: Mara::ns::getAllTitles()) {
                        u64 game_pid = std::stoull(version["game_pid"].get<std::string>(), nullptr, 16);
                        if (game_pid == title.second->GetTitleID()) {
                            program = new ns::Title(game_pid);
                            brls::Logger::info("Juego encontrado");
                            this->base_path = version["base_path"].get<std::string>();
                            this->patch_files = Mara::fs::getFiles(ROMFS_MOUNT_NAME + version["base_path"].get<std::string>());
                            this->game_found = true;
                            if(this->patch_files.empty()){
                                brls::Logger::error("No se han encontrado parches xdelta en la carpeta %s%s", ROMFS_MOUNT_NAME, version["base_path"].get<std::string>().c_str());
                            }
                            break;
                        }
                    }
                }
            }

            inputFile.close();
        } else {
            brls::Logger::error("Archivo no abierto");
            return;
        }
    }
}