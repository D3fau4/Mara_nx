#pragma once

#include <string>
#include <vector>
#include "json.hpp"
#include "types.h"
#include "ns/ns.hpp"

namespace Mara::helpers {
    class PatchData {
    private:
        void initializeConstants();

    public:
        ns::Title *program;
        std::vector<std::string> patch_files;
        std::string base_path;

        bool game_found;

        PatchData();

        virtual ~PatchData();
    };
}