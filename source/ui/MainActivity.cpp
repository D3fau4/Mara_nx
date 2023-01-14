#include "ui/MainActivity.hpp"

#include "ns/ns.hpp"

namespace Mara::ui
{
    MainActivity::MainActivity()
    {
        brls::TabFrame* rootFrame = new brls::TabFrame();
        rootFrame->setTitle("main/title"_i18n);

        brls::List *mainlist = new brls::List();
        for (auto &title : Mara::ns::getAllTitles())
        {
            brls::Logger::info("juego añadido");
            brls::ListItem *titleItem = new Mara::ui::ProgramidListItem(title.second->GetTitleID(), title.second->GetTitleName(),title.second->GetTitleAuthor());
            mainlist->addView(titleItem);
        }
        rootFrame->addTab("main/maintab"_i18n, mainlist);
        view = rootFrame;
    }

    brls::View* MainActivity::GetView()
    {
        return view;
    }
}
