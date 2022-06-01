#pragma once

#include <unordered_set>
#include <vector>
#include <string>

#include "pcs/operation/observable.h"

namespace pcs {

	class TaskExpression {
	private:
		Observable operation_;
		std::unordered_set<std::string> input_;
		std::vector<std::string> output_;
	public:
		TaskExpression() = default;
		TaskExpression(Observable&& operation, std::unordered_set<std::string>&& input, std::vector<std::string>&& output);
		TaskExpression(const Observable& operation, const std::unordered_set<std::string>& input, const std::vector<std::string>& output);
		TaskExpression(const TaskExpression& other);
		TaskExpression& operator=(const TaskExpression& other);
		TaskExpression(TaskExpression&& other) noexcept;
		TaskExpression& operator=(TaskExpression&& other) noexcept;
		~TaskExpression() = default;

		const Observable& operation() const;
		void set_operation(Observable&& operation);
		void set_operation(const Observable& operation);
		
		const std::unordered_set<std::string>& input() const;
		void set_input(std::unordered_set<std::string>&& input);
		void set_input(const std::unordered_set<std::string>& input);

		const std::vector<std::string>& output() const;
		void set_output(std::vector<std::string>&& output);
		void set_output(const std::vector<std::string>& output);

		bool operator==(const TaskExpression& other) const;
	};

}

/*
* @Todo: use this in CompositeOperation.
*/