#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <fstream>

#include "pcs/operation/guard.h"
#include "pcs/operation/observable.h"
#include "pcs/operation/task_expression.h"

namespace pcs {

	struct CompositeOperation {
	public:
		TaskExpression guard;
		std::vector<TaskExpression> sequential;
		std::vector<TaskExpression> parallel;
	public:
		CompositeOperation() = default;
		CompositeOperation(Guard&& guard, std::vector<Observable>&& parallel_operations, std::vector<Observable>&& sequential_operations);

		bool HasGuard() const;
		const TaskExpression& CurrentTask(size_t seq_id) const;
		bool IsLastOp(size_t seq_id) const;

		bool operator==(const CompositeOperation& other) const;
		friend std::ostream& operator<<(std::ostream& os, const CompositeOperation& co);
		friend std::ofstream& operator<<(std::ofstream& os, const CompositeOperation& co);
	};

}