#pragma once

/* 
* Precompiled Headers
*/

// std
#include <span>
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <algorithm>
#include <ostream>
#include <filesystem>
#include <fstream>

// LTS
#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"
#include "pcs/lts/transition.h"
#include "pcs/lts/export.h"
#include "pcs/lts/writers.h"
#include "pcs/lts/parsers/parsers.h"

#include <boost/container_hash/hash.hpp>

// Log
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/ranges.h>
#include <spdlog/fmt/bundled/color.h>

// Common
#include "pcs/common/directory.h"

// Operations
#include "pcs/operation/parsers/parsers.h"
#include "pcs/operation/operation.h"
#include "pcs/operation/observable.h"
#include "pcs/operation/transfer.h"
#include "pcs/operation/composite.h"
#include "pcs/operation/guard.h"
#include "pcs/operation/task_expression.h"

// Product
#include "pcs/product/recipe.h"
#include "pcs/product/parsers/recipe.h"

// Parse
#include "nlohmann/json.hpp"