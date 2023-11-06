#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"
#include "types.h"
#include "borealis/logger.hpp"

namespace Mara::helpers
{
  Helper::Helper()
  {
    initializeConstants()
  }

  void Helper::initializeConstants()
  {
    nlohmann::json jsonData;
    std::ifstream inputFile(VERSIONS_FILE);
    if (inputFile.is_open())
    {
      inputFile >> jsonData;
      brls::Logger::info("Archivo de versiones abierto");
      // Crear elementos de la interfaz de usuario desde los datos del JSON
      if (jsonData.contains("game_versions") && jsonData["game_versions"].is_structured())
      {
        for (const auto &version : jsonData['game_versions'])
        {
          for (auto &title : Mara::ns::getAllTitles())
          {
            u64 game_pid = std::stoull(version['game_pid'], 0, 16);
            if (std::stoull(version['game_pid'], 0, 16) == title.second->GetTitleID())
            {
              std::vector<std::string> patch_files = Mara::fs::GetFiles(version['base_path']);
              std::vector<std::string> ori_files = for (std::string filename : patch_files)
              {
                filename = filename.substr('.xdelta', '');
              }
              this->program_id = game_pid;
              this->patch_files = patch_files this->ori_files = ori_fileu
            }
          }
        }
      }

      inputFile.close();
    }
    else
    {
      brls::Logger::error("Archivo no abierto");
      return;
    }
  }
}