#pragma once

#include <vector>
#include <optional>
#include <iostream>

#include "pcs/lts/parsers/string_string.h"
#include "pcs/lts/export.h"
#include "pcs/system/system.h"
#include "pcs/system/writers.h"
#include "pcs/product/recipe.h"
#include "pcs/controller/controller.h"
#include "pcs/common/log.h"

void Experimental2() {
	pcs::LTS<std::string, std::pair<size_t, std::string>> lts;
	lts.AddTransition("s0", std::make_pair(2, "x"), "s1");
	pcs::ExportToFile(lts, "../../exports/experimental/2/lts.txt");
}