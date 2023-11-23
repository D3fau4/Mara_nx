#include <string>
#include "helpers/StringsUtils.hpp"

namespace Mara::helpers::String {
    std::string replace(std::string s, std::string search, std::string replacement) {
        size_t pos = 0;

        while ((pos = s.find(search, pos)) != std::string::npos) {
            s.replace(pos, search.length(), replacement);
            pos += replacement.length();
        }

        return s;
    }
}