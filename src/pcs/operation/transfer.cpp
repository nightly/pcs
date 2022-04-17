#include "pcs/operation/transfer.h"

#include <string>

namespace pcs {

	TransferOperation::TransferOperation(Transfer type, size_t n) 
		: type_(type), n_(n) {
		if (type_ == Transfer::in) {
			name_ += "in:";
		} else if (type_ == Transfer::out) {
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

	bool TransferOperation::In() const {
		return type_ == Transfer::in;
	}
	
	bool TransferOperation::Out() const {
		return !In();
	}

	/*
	 * @brief Returns the inverse of the TransferOperation, useful for checking matching operations. 
	 */
	TransferOperation TransferOperation::Inverse() const {
		return TransferOperation(type_ == Transfer::in ? Transfer::out : Transfer::in, n_);
	}
	
	bool TransferOperation::operator<(const TransferOperation& other) const {
		return (type_ < other.type_) && (n_ < other.n_);
	}

	bool TransferOperation::operator==(const TransferOperation& other) const {
		return (name_ == other.name_) && (n_ == other.n_) && (type_ == other.type_);
	}



}