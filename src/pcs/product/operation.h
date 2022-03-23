#pragma once

#include <vector>
#include <string>

namespace pcs::product {

	struct Operation {
	public:
		std::vector<std::string> consumed_parts;
		std::vector<std::string> produced_parts;
	};
}