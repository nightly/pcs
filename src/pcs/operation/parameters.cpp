#include "pcs/operation/parameters.h"

#include <fstream>
#include <ostream>
#include <string>

namespace pcs {

	Parameters::Parameters(const std::unordered_map<std::string, std::string>& params) : parameters_(params) {}
	Parameters::Parameters(std::unordered_map<std::string, std::string>&& params) : parameters_(std::move(params)) {}

	Parameters::Parameters(const Parameters& other) : parameters_(other.parameters_) {}

	Parameters& Parameters::operator=(const Parameters& other) {
		parameters_ = other.parameters_;
		return *this;
	}
	
	Parameters::Parameters(Parameters&& other) noexcept : parameters_(std::move(other.parameters_)) {}
	
	Parameters& Parameters::operator=(Parameters&& other) noexcept {
		parameters_ = std::move(other.parameters_);
		return *this;
	}

	std::unordered_map<std::string, std::string>& Parameters::map() {
		return parameters_;
	}

	const std::unordered_map<std::string, std::string>& Parameters::map() const {
		return parameters_;
	}

	bool Parameters::empty() const {
		return parameters_.empty();
	}

	bool Parameters::operator==(const Parameters& other) const {
		return (parameters_ == other.parameters_);
	}

	std::ostream& operator<<(std::ostream& os, const Parameters& param) {
		for (const auto& [k, v] : param.parameters_) {
			os << k << " = " << v;
		}
		return os;
	}

}