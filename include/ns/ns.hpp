#pragma once

#include <switch.h>
#include <map>
#include <borealis.hpp>
#include "ns/title.hpp"

namespace Mara::ns
{
    std::map<u64,Mara::ns::title*>& getAllTitles();
}
