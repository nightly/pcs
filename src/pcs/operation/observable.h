#pragma once 

#include <string>
#include <vector>
#include <span>

#include "pcs/operation/operation.h"

namespace pcs {

	/**
	 * @brief Represents an Observable operation within a Resource (e.g. engrave, separate, join) 
	 */
	class Observable : public IOperation {
	public:
		std::string name_;
	public:
		Observable() = default;
		Observable(const std::string& name);
		~Observable() = default;

		const std::string& name() const override;

		bool operator==(const Observable& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Observable& operation);
		virtual Observable* clone() const override;
	};

}