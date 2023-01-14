#pragma once

#include <borealis.hpp>
#include <memory>
#include "types.h"

namespace Mara::ui
{
    class ProgramidListItem : public brls::ListItem
    {
    public:
        ProgramidListItem(u64 program_id, std::string label, std::string description = "", std::string subLabel = "") :
        brls::ListItem(label, description, subLabel), m_program_id(program_id){}

        u64 getTitle() {
            return this->m_program_id;
        }
    private:
        u64 m_program_id;
    };
}
