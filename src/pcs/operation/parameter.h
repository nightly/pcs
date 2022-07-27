#pragma once

#include <fstream>
#include <ostream>
#include <string>

namespace pcs {

	class Parameter {
	private:
		std::string name_;
		std::string value_;
	public:
		Parameter() = default;
		Parameter(const std::string& name);
		Parameter(std::string&& name);
		Parameter(const std::string& name, const std::string& value);
		Parameter(std::string&& name, std::string&& value);
		Parameter(const Parameter& other);
		Parameter& operator=(const Parameter& other);
		Parameter(Parameter&& other) noexcept;
		Parameter& operator=(Parameter&& other) noexcept;

		const std::string& name() const;
		const std::string& value() const;

		bool operator==(const Parameter& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Parameter& param);
	};
}