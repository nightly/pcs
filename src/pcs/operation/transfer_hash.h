#pragma once

#include "pcs/operation/transfer.h"

namespace std {

	/**
	 * @brief Template specialization for std::hash<Transfer> for usage with unordered_map 
	 */
	template<>
	struct hash<pcs::TransferOperation> {

		size_t operator() (const pcs::TransferOperation& transfer_op) {
			return 0;
		}

	};

}