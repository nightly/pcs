#pragma once

#include <vector>
#include <string>

#include "pcs/operation/operation.h"

namespace pcs {

	struct Guard : public IOperation {
	public:
		std::string name;
		std::vector<std::string> input;
	public:
		Guard();
		Guard(std::string&& name, std::vector<std::string>&& input_parts);

		bool operator==(const Guard& other) const;

	};
}