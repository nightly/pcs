#pragma once

#include <memory>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>

#include "pcs/operation/operation.h"
#include "pcs/operation/parameter.h"

namespace pcs {

	class ParameterizedOp {
	private:
		std::string operation_;
		std::vector<Parameter> parameters_;
	public:
		ParameterizedOp() = default;
		ParameterizedOp(const std::string& op, const std::vector<Parameter>& parameters);
		ParameterizedOp(std::string&& op, std::vector<Parameter>&& parameters);

		ParameterizedOp(const ParameterizedOp& other);
		ParameterizedOp& operator=(const ParameterizedOp& other);
		ParameterizedOp(ParameterizedOp&& other) noexcept;
		ParameterizedOp& operator=(ParameterizedOp&& other) noexcept;

		const std::string& operation() const;
		void set_operation(const std::string& op);
		void set_operation(std::string&& op);

		const std::vector<Parameter>& parameters() const;
		void set_parameters(const std::vector<Parameter>& parameters);
		void set_parameters(std::vector<Parameter>&& parameters);

		bool operator==(const ParameterizedOp& other) const;

		friend std::ostream& operator<<(std::ostream& os, const ParameterizedOp& p_op);
		friend std::ofstream& operator<<(std::ofstream& os, const ParameterizedOp& p_op);

	};

}