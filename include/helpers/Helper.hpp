#pragma once
#include "../types.h"
#include <string>
namespace Mara::helpers
{
    class Helper
    {
    private:
        void initializeConstants();
    public:
        u64 program_id;
        std::string *ori_files;
        std::string *patch_files;
    };
}