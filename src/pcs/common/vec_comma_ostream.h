#pragma once

#include <ostream>
#include <string>
#include <vector>

#include <spdlog/fmt/bundled/format.h>
#include <spdlog/fmt/ostr.h>

namespace pcs {

    /* 
     * @brief std::vector operator<< overload delimited by commas 
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
    {
        for (std::size_t i = 0; i < vec.size(); ++i) {
            os << vec[i];
            if (i != vec.size() - 1) {
                os << ",";
            }
        }
        return os;
    }

}