#include "pcs/operation/transfer.h"

#include <string>

namespace pcs {

	TransferOperation::TransferOperation(TransferType type, size_t n) 
		: type_(type), n_(n) {
		if (type_ == TransferType::in) {
			name_ += "in:";
		} else if (type_ == TransferType::out) {
			name_ += "out:";
		}
		name_ += std::to_string(n);
	}

	const std::string& TransferOperation::name() const {
		return name_;
	}

	size_t TransferOperation::n() const {
		return n_;
	}

	TransferType TransferOperation::type() const {
		return type_;
	}

	bool TransferOperation::IsIn() const {
		return type_ == TransferType::in;
	}
	
	bool TransferOperation::IsOut() const {
		return !IsIn();
	}

	/**
	 * @brief Returns the inverse of the TransferOperation, useful for checking matching operations. 
	 * @example in:2 has an inverse of out:2
	 */
	TransferOperation TransferOperation::Inverse() const {
		return TransferOperation(type_ == TransferType::in ? TransferType::out : TransferType::in, n_);
	}
	
	bool TransferOperation::operator<(const TransferOperation& other) const {
		return ((static_cast<size_t>(type_) + n_) < (static_cast<size_t>(other.type_) + other.n_));
	}

	bool TransferOperation::operator==(const TransferOperation& other) const {
		return (name_ == other.name_) && (n_ == other.n_) && (type_ == other.type_);
	}

}