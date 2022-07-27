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
#include <memory>

// LTS
#include "lts/lts.h"
#include "lts/state.h"
#include "lts/transition.h"
#include "lts/export.h"
#include "lts/writers.h"
#include "lts/parsers/parsers.h"

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