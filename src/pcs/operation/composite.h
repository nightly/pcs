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
		Guard guard;
		std::vector<Operation> parallel;
		std::vector<Operation> sequential;
	public:
		CompositeOperation() = default;
		CompositeOperation(Guard&& guard, std::vector<Operation>&& parallel_operations, std::vector<Operation>&& sequential_operations);

		bool HasGuard() const;

		bool operator==(const CompositeOperation& other) const;
		friend std::ostream& operator<<(std::ostream& os, const CompositeOperation& co);
		friend std::ofstream& operator<<(std::ofstream& os, const CompositeOperation& co);
	};

}