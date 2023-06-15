#include "ui/Tabs/AboutTab.hpp"
#include "Utils/json.hpp"
#include <iostream>
#include <fstream>

namespace Mara::ui {
    AboutTab::AboutTab() {
        tablist = new brls::List();

        // Cargar datos desde el archivo JSON
        nlohmann::json jsonData;
        std::ifstream inputFile(BOREALIS_ASSET("Patch/credits.json"));
        if (inputFile.is_open()) {
            inputFile >> jsonData;
            inputFile.close();
        } else {
            std::cout << "Error al abrir el archivo JSON." << std::endl;
            return;
        }

        // Crear elementos de la interfaz de usuario desde los datos del JSON
        if (jsonData.contains("categories") && jsonData["categories"].is_array()) {
            for (const auto& category : jsonData["categories"]) {
                if (!category.contains("name") || !category["name"].is_string())
                    continue;

                std::string categoryName = category["name"].get<std::string>();
                tablist->addView(new brls::Header(categoryName));

                if (category.contains("items") && category["items"].is_array()) {
                    for (const auto& item : category["items"]) {
                        if (!item.contains("title") || !item["title"].is_string())
                            continue;

                        std::string title = item["title"].get<std::string>();
                        std::string image = item.contains("image") && item["image"].is_string() ? item["image"].get<std::string>() : "";
                        std::string description = item.contains("description") && item["description"].is_string() ? item["description"].get<std::string>() : "";

                        auto* listItem = new brls::ListItem(title, "", description);
                        if (!image.empty()) {
                            listItem->setThumbnail("romfs:/" + image);
                        }

                        tablist->addView(listItem);
                    }
                }
            }
        }
    }

    brls::List* AboutTab::GetTab() {
        return this->tablist;
    }
}
