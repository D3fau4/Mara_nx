#pragma once

#include <string>
#include "json.hpp"
#include "types.h"
#include "ns/ns.hpp"

namespace Mara {
    class PatchData {
    private:
        void initializeConstants();

    public:
        ns::Title *program;
        std::string *ori_files;
        std::string *patch_files;

        PatchData();

        virtual ~PatchData();
    };
}