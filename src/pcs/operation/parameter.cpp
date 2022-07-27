#include "pcs/operation/parameter.h"

#include <fstream>
#include <ostream>
#include <string>

namespace pcs {

	Parameter::Parameter(const std::string& name) : name_(name) {}
	Parameter::Parameter(std::string&& name) : name_(std::move(name)) {}

	Parameter::Parameter(const std::string& name, const std::string& value) : name_(name), value_(value) {}
	Parameter::Parameter(std::string&& name, std::string&& value) : name_(std::move(name)), value_(std::move(value)) {}

	Parameter::Parameter(const Parameter& other) : name_(other.name_), value_(other.value_) {}

	Parameter& Parameter::operator=(const Parameter& other) {
		name_ = other.name_;
		value_ = other.value_;
		return *this;
	}
	
	Parameter::Parameter(Parameter&& other) noexcept : name_(std::move(other.name_)), value_(std::move(other.value_)) {}
	
	Parameter& Parameter::operator=(Parameter&& other) noexcept {
		name_ = std::move(other.name_);
		value_ = std::move(other.value_);
		return *this;
	}

	const std::string& Parameter::name() const {
		return name_;
	}

	const std::string& Parameter::value() const {
		return value_;
	}

	bool Parameter::operator==(const Parameter& other) const {
		return (name_ == other.name_ && value_ == other.value_);
	}

	std::ostream& operator<<(std::ostream& os, const Parameter& param) {
		os << param.name_ << " = " << param.value_;
		return os;
	}

}