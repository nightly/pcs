#pragma once

#include <string>
#include <vector>

#include "pcs/operation/guard.h"
#include "pcs/operation/operation.h"

namespace pcs {

	struct CompositeOperation {
	public:
		Guard guard;
		std::vector<Operation> parallel;
		std::vector<Operation> sequential;
	public:
		CompositeOperation();
		CompositeOperation(Guard&& guard, std::vector<Operation>&& parallel_operations, std::vector<Operation>&& sequential_operations);
		CompositeOperation(const std::string& str);

		bool operator==(const CompositeOperation& other) const;
	};

}