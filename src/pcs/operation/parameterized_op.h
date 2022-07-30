#pragma once

#include <memory>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>

#include "pcs/operation/operation.h"
#include "pcs/operation/parameters.h"

namespace pcs {

	class ParameterizedOp {
	private:
		std::string operation_;
		Parameters parameters_;
	public:
		ParameterizedOp() = default;
		ParameterizedOp(const std::string& op, const Parameters& parameters);
		ParameterizedOp(std::string&& op, Parameters&& parameters);

		ParameterizedOp(const ParameterizedOp& other);
		ParameterizedOp& operator=(const ParameterizedOp& other);
		ParameterizedOp(ParameterizedOp&& other) noexcept;
		ParameterizedOp& operator=(ParameterizedOp&& other) noexcept;

		const std::string& operation() const;
		void set_operation(const std::string& op);
		void set_operation(std::string&& op);

		const Parameters& parameters() const;
		void set_parameters(const Parameters& parameters);
		void set_parameters(Parameters&& parameters);

		bool operator==(const ParameterizedOp& other) const;

		friend std::ostream& operator<<(std::ostream& os, const ParameterizedOp& p_op);
		friend std::ofstream& operator<<(std::ofstream& os, const ParameterizedOp& p_op);

	};

}