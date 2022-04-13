#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <fstream>

#include "pcs/operation/guard.h"
#include "pcs/operation/operation.h"

namespace pcs {

	struct CompositeOperation {
	public:
		std::pair<Guard, std::vector<std::string>> guard; // Operation + Input parts
		std::vector<std::tuple<Operation, std::vector<std::string>, std::vector<std::string>>> sequential; // Operation, Input, Output
		std::vector<std::tuple<Operation, std::vector<std::string>, std::vector<std::string>>> parallel; // ^
	public:
		CompositeOperation() = default;
		CompositeOperation(Guard&& guard, std::vector<Operation>&& parallel_operations, std::vector<Operation>&& sequential_operations);

		bool HasGuard() const;

		bool operator==(const CompositeOperation& other) const;
		friend std::ostream& operator<<(std::ostream& os, const CompositeOperation& co);
		friend std::ofstream& operator<<(std::ofstream& os, const CompositeOperation& co);
	};

}