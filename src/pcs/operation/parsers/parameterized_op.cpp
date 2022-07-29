#include "lts/parsers/transition.h"

#include <vector>
#include <string>
#include <string_view>
#include <sstream>

#include "pcs/operation/parameterized_op.h"
#include "pcs/operation/parsers/label.h"
#include "pcs/operation/parameter.h"

namespace pcs {

	std::vector<pcs::Parameter> StringToParameters(const std::string& str) {
		std::vector<pcs::Parameter> ret;
		std::stringstream ss(str);
		std::string line;
		while (std::getline(ss, line, ' ')) {
			std::stringstream ss2(line);
			std::string name, val;
			std::getline(ss2, name, '=');
			std::getline(ss2, val);
			ret.emplace_back(name, val);
		}
		return ret;
	}

	ParameterizedOp StringToParameterizedOp(const std::string& str) {
		std::string op_str, params_str;
		std::stringstream ss(str);
		std::getline(ss, op_str, '(');
		std::getline(ss, params_str);

		std::vector<pcs::Parameter> parameters = pcs::StringToParameters(params_str);
		
		return pcs::ParameterizedOp(std::move(op_str), std::move(parameters));
	}

}

namespace nightly {

	template <>
	pcs::ParameterizedOp ParseTransitionString(const std::string& str) {
		std::string op_str, params_str;
		std::stringstream ss(str);
		std::getline(ss, op_str, '(');
		std::getline(ss, params_str, ')');

		std::vector<pcs::Parameter> parameters = pcs::StringToParameters(params_str);
		
		return pcs::ParameterizedOp(std::move(op_str), std::move(parameters));
	}

	template <>
	std::pair<size_t, pcs::ParameterizedOp> ParseTransitionString(const std::string& str) {
		std::string n;
		std::string action;

		std::stringstream ss(str);
		std::getline(ss, n, '/');
		std::getline(ss, action);

		std::string op_str, params_str;
		std::stringstream action_stream(action);
		std::getline(action_stream, op_str, '(');
		std::getline(action_stream, params_str, ')');
		std::vector<pcs::Parameter> parameters = pcs::StringToParameters(params_str);

		return { stoul(n), pcs::ParameterizedOp(std::move(op_str), std::move(parameters)) };
	}

}