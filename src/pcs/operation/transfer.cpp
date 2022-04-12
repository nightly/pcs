#include "pcs/operation/transfer.h"

namespace pcs {

	TransferOperation::TransferOperation(Transfer type, size_t n) 
		: type(type), n(n) {}

	/*
	 * @brief Returns the inverse of the TransferOperation, useful for checking matching operations. 
	 */
	TransferOperation TransferOperation::Inverse() const {
		return TransferOperation(type == Transfer::in ? Transfer::out : Transfer::in, n);
	}


}