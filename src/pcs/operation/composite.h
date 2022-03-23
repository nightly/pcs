#pragma once

#include <string>
#include <vector>

namespace pcs {

	struct CompositeOperation {
	public:
		std::pair<std::string, std::vector<std::string>> guard; // Also referred to as "test"
		std::vector<std::tuple<std::string, std::vector<std::string>, std::vector<std::string>>> parallel_ops;
		std::vector<std::tuple<std::string, std::vector<std::string>, std::vector<std::string>>> sequential_ops;
	public:
		CompositeOperation();
		CompositeOperation(const std::string& str);

		bool operator==(const CompositeOperation& other) const;
	};

}