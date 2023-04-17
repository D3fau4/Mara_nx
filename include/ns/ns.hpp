#pragma once

#include <switch.h>
#include <map>
#include <borealis.hpp>
#include "ns/Title.hpp"

namespace Mara::ns
{
    std::map<u64,Mara::ns::Title*>& getAllTitles();
}
