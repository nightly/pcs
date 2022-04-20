#pragma once

#include "pcs/operation/operation.h"

#include <string>

namespace pcs {

	enum class Transfer { in, out };

	class TransferOperation : public IOperation {
	private:
		Transfer type_;
		size_t n_;
		std::string name_;
	public:
		TransferOperation() = default;
		TransferOperation(Transfer type, size_t n);
		~TransferOperation() = default;
		
		const std::string& name() const override;
		TransferOperation Inverse() const;
		size_t n() const;
		bool IsIn() const;
		bool IsOut() const;
		
		bool operator==(const TransferOperation& other) const;
		bool operator <(const TransferOperation& other) const;

	};
}