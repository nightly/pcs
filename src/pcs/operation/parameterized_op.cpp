#include "pcs/operation/parameterized_op.h"

#include <memory>
#include <vector>
#include <string>

#include "pcs/operation/operation.h"
#include "pcs/operation/parameter.h"

namespace pcs {

	ParameterizedOp::ParameterizedOp(const std::string& op, const std::vector<Parameter>& parameters)
		: operation_(op), parameters_(parameters) {}
	
	ParameterizedOp::ParameterizedOp(std::string&& op, std::vector<Parameter>&& parameters)
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

	const std::vector<Parameter>& ParameterizedOp::parameters() const {
		return parameters_;
	}

	void ParameterizedOp::set_parameters(const std::vector<Parameter>& parameters) {
		parameters_ = parameters;
	}

	void ParameterizedOp::set_parameters(std::vector<Parameter>&& parameters) {
		parameters_ = std::move(parameters);
	}

	bool ParameterizedOp::operator==(const ParameterizedOp& other) const {
		return (operation_ == other.operation_ && parameters_ == other.parameters_);
	}

	std::ostream& operator<<(std::ostream& os, const ParameterizedOp& p_op) {
		os << p_op.operation();
		if (!p_op.parameters_.empty()) {
			os << "(";
			for (const auto& p : p_op.parameters()) {
				os << p;
			}
			os << ")";
		}
		return os;
	}

	std::ofstream& operator<<(std::ofstream& os, const ParameterizedOp& p_op) {
		os << p_op.operation();
		if (!p_op.parameters_.empty()) {
			os << "(<I>";
			for (const auto& p : p_op.parameters()) {
				os << p;
			}
			os << "</I>)";
		}
		return os;
	}

}