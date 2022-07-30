#include "pcs/operation/task_expression.h"

#include <unordered_set>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>

#include "pcs/operation/observable.h"
#include "pcs/common/strings.h"

namespace pcs {

	TaskExpression::TaskExpression(Observable&& operation, std::unordered_set<std::string>&& input, Parameters&& parameters,
		                          std::vector<std::string>&& output) 
		: operation_(std::move(operation)), input_(std::move(input)), parameters_(std::move(parameters)), output_(std::move(output)) {}

	TaskExpression::TaskExpression(const Observable& operation, const std::unordered_set<std::string>& input, 
		                           const Parameters& parameters, const std::vector<std::string>& output)
		: operation_(operation), input_(input), parameters_(parameters), output_(output) {}

	TaskExpression::TaskExpression(TaskExpression&& other) noexcept
		: operation_(std::move(other.operation_)), input_(std::move(other.input_)), 
		  parameters_(std::move(other.parameters_)), output_(std::move(other.output_)) {}

	TaskExpression::TaskExpression(const TaskExpression& other)
		: operation_(other.operation_), input_(other.input_), parameters_(other.parameters_), output_(other.output_){}

	TaskExpression& TaskExpression::operator=(TaskExpression&& other) noexcept {
		operation_ = std::move(other.operation_);
		input_ = std::move(other.input_);
		parameters_ = std::move(other.parameters_);
		output_ = std::move(other.output_);
		return *this;
	}

	TaskExpression& TaskExpression::operator=(const TaskExpression& other) {
		operation_ = other.operation_;
		input_ = other.input_;
		parameters_ = other.parameters_;
		output_ = other.output_;
		return *this;
	}

	const Observable& TaskExpression::operation() const {
		return operation_;
	}

	void TaskExpression::set_operation(Observable&& operation) {
		operation_ = std::move(operation);
	}

	void TaskExpression::set_operation(const Observable& operation) {
		operation_ = operation;
	}

	const std::unordered_set<std::string>& TaskExpression::input() const {
		return input_;
	}

	void TaskExpression::set_input(std::unordered_set<std::string>&& input) {
		input_ = std::move(input_);
	}

	void TaskExpression::set_input(const std::unordered_set<std::string>& input) {
		input_ = input;
	}

	const std::vector<std::string>& TaskExpression::output() const {
		return output_;
	}

	void TaskExpression::set_output(std::vector<std::string>&& output) {
		output_ = std::move(output_);
	}

	void TaskExpression::set_output(const std::vector<std::string>& output) {
		output_ = output_;
	}

	const Parameters& TaskExpression::parameters() const {
		return parameters_;
	}

	void TaskExpression::set_parameters(Parameters&& parameters) {
		parameters_ = std::move(parameters_);
	}
	
	void TaskExpression::set_parameters(const Parameters& parameters) {
		parameters_ = parameters;
	}

	bool TaskExpression::operator==(const TaskExpression& other) const {
		return (operation_ == other.operation_) && (input_ == other.input_) && (parameters_ == other.parameters_) && (output_ == other.output_);
	}

	std::ostream& operator<<(std::ostream& os, const TaskExpression& task) {
		os << task.operation().name() << "(" << USetToString(task.input()) << ")" << "(" << VectorToString(task.output()) << ")";
		if (!task.parameters().empty()) {
			os << " {";
			os << task.parameters_;
			os << "}";
		}
		return os;
	}
	
	std::ofstream& operator<<(std::ofstream& os, const TaskExpression& task) {
		os << task.operation().name() << "(" << USetToString(task.input()) << ")" << "(" << VectorToString(task.output()) << ")";
		if (!task.parameters().empty()) {
			os << "{<I>";
			os << task.parameters_;
			os << "</I>}";
		}
		return os;
	}

}