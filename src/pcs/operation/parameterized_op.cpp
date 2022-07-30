#include "pcs/operation/parameterized_op.h"

#include <memory>
#include <vector>
#include <string>

#include "pcs/operation/operation.h"
#include "pcs/operation/parameters.h"

namespace pcs {

	ParameterizedOp::ParameterizedOp(const std::string& op, const Parameters& parameters)
		: operation_(op), parameters_(parameters) {}
	
	ParameterizedOp::ParameterizedOp(std::string&& op, Parameters&& parameters)
		: operation_(std::move(op)), parameters_(std::move(parameters)) {}

	ParameterizedOp::ParameterizedOp(const ParameterizedOp& other)
		: operation_(other.operation_), parameters_(other.parameters_) {}
	
	ParameterizedOp& ParameterizedOp::operator=(const ParameterizedOp& other) {
		operation_ = other.operation_;
		parameters_ = other.parameters_;
		return *this;
	}
	
	ParameterizedOp::ParameterizedOp(ParameterizedOp&& other) noexcept
		: operation_(std::move(other.operation_)), parameters_(std::move(other.parameters_)) {}
	
	ParameterizedOp& ParameterizedOp::operator=(ParameterizedOp&& other) noexcept {
		operation_ = std::move(other.operation_);
		parameters_ = std::move(other.parameters_);
		return *this;
	}

	const std::string& ParameterizedOp::operation() const {
		return operation_;
	}

	void ParameterizedOp::set_operation(const std::string& op) {
		operation_ = op;
	}

	void ParameterizedOp::set_operation(std::string&& op) {
		operation_ = std::move(op);
	}

	const Parameters& ParameterizedOp::parameters() const {
		return parameters_;
	}

	void ParameterizedOp::set_parameters(const Parameters& parameters) {
		parameters_ = parameters;
	}

	void ParameterizedOp::set_parameters(Parameters&& parameters) {
		parameters_ = std::move(parameters);
	}

	bool ParameterizedOp::operator==(const ParameterizedOp& other) const {
		return (operation_ == other.operation_ && parameters_ == other.parameters_);
	}

	std::ostream& operator<<(std::ostream& os, const ParameterizedOp& p_op) {
		os << p_op.operation();
		if (!p_op.parameters_.empty()) {
			os << "(";
			os << p_op.parameters_;
			os << ")";
		}
		return os;
	}

	std::ofstream& operator<<(std::ofstream& os, const ParameterizedOp& p_op) {
		os << p_op.operation();
		if (!p_op.parameters_.empty()) {
			os << "(<I>";
			os << p_op.parameters_;
			os << "</I>)";
		}
		return os;
	}

}