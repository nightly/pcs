#pragma once

#include <string>
#include <span>
#include <vector>

#include <boost/container_hash/hash.hpp>

#include "lts/state.h"
#include "lts/lts.h"
#include "pcs/operation/parameterized_op.h"

namespace pcs {

	/**
	 * @brief Interface for Topology
	 */
	class ITopology {
	public:
		virtual ~ITopology() = default;

		virtual const nightly::LTS<std::vector<std::string>, std::pair<size_t, ParameterizedOp>, boost::hash<std::vector<std::string>>>& lts() const = 0;

		virtual const std::vector<std::string>& initial_state() const = 0;

		virtual const nightly::State<std::vector<std::string>, std::pair<size_t, ParameterizedOp>>& at(const std::vector<std::string>& key) = 0;

		virtual operator const nightly::LTS<std::vector<std::string>, std::pair<size_t, ParameterizedOp>, boost::hash<std::vector<std::string>>>& () const = 0;
	};

}