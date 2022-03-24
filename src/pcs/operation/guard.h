#pragma once

#include <vector>
#include <string>

namespace pcs {

	struct Guard {
	public:
		std::string name;
		std::vector<std::string> input;
	public:
		Guard();
		Guard(std::string&& name, std::vector<std::string>&& input_parts);

		bool operator==(const Guard& other) const;

	};
}