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
	public:
		Guard() = default;
		Guard(const std::string& name);
		~Guard() = default;

		const std::string& name() const override;

		bool operator==(const Guard& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Guard& guard);
	};
}