#include <fstream>
#include <string>
#include "borealis/logger.hpp"
#include "helpers/PatchData.hpp"
#include "Program/Main.hpp"

namespace Mara {
    PatchData::PatchData() {
        initializeConstants();
    }

    PatchData::~PatchData() {
        delete program;
    }

    void PatchData::initializeConstants() {
        nlohmann::json jsonData;
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