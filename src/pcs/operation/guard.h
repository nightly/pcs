#pragma once

#include <vector>
#include <string>
#include <ostream>
#include <span>

#include "pcs/operation/operation.h"

namespace pcs {

	class Guard : public IOperation {
	public:
		std::string name_;
		std::vector<std::string> input_;
	public:
		Guard() = default;
		Guard(const std::string& name, const std::span<std::string>& input_parts);
		// Guard(std::string&& name, std::vector<std::string>&& input_parts);

		bool operator==(const Guard& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Guard& guard);
	};
}