#include "ui/Tabs/AboutTab.hpp"
#include <iostream>
#include <fstream>
#include "json.hpp"

namespace Mara::ui {
    AboutTab::AboutTab() {
        tablist = new brls::List();
        brls::ListItem *cabeza = new brls::ListItem("main/translation/title"_i18n, "", "main/translation/subtitle"_i18n);
        tablist->addView(cabeza);

        // Creditos
        nlohmann::json jsonData;
        std::ifstream inputFile(ABOUT_TAB_INFO);
        if (inputFile.is_open()) {
            inputFile >> jsonData;
            brls::Logger::info("Archivo abierto");
            // Crear elementos de la interfaz de usuario desde los datos del JSON

            if(jsonData.contains("team_logo_path") && jsonData["team_logo_path"].is_string()){

                std::string team_logo_path = jsonData["team_logo_path"].get<std::string>();

                if(!team_logo_path.empty()){
                    brls::Label* patchdescription = new brls::Label(
                            brls::LabelStyle::REGULAR,
                            "main/translation/translatedby"_i18n,
                            true);

                    patchdescription->setHorizontalAlign(NVG_ALIGN_CENTER);
                    tablist->addView(patchdescription);
                    brls::Image* a = new brls::Image(ROMFS_MOUNT_NAME + team_logo_path);
                    a->setHeight(150);
                    tablist->addView(a);
                }
            }

            if (jsonData.contains("categories") && jsonData["categories"].is_structured()) {

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
                                listItem->setThumbnail(ROMFS_MOUNT_NAME + image);
                            }

                            tablist->addView(listItem);
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

    brls::List* AboutTab::GetTab() {
        return this->tablist;
    }
}