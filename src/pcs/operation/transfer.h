#pragma once

#include "pcs/operation/operation.h"

#include <string>

namespace pcs {

	/**
	 * @brief: Enum type that represents Transfer/Sync operations in terms of In or Out transition labels
	 */
	enum class Transfer { in, out };


	/**
	 * @brief: TransferOperation is a resource synchronization operation in the form of `in:n` or `out:n`
	 */
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
		Transfer type() const;
		bool IsIn() const;
		bool IsOut() const;
		
		bool operator==(const TransferOperation& other) const;
		bool operator <(const TransferOperation& other) const;

	};
}

#include "pcs/operation/transfer_hash.h"
