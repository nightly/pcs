#pragma once

#include <string>
#include <span>
#include <vector>

#include <boost/container_hash/hash.hpp>

#include "pcs/lts/state.h"
#include "pcs/lts/lts.h"

namespace pcs {

	/**
	 * @brief Interface for Topology
	 */
	class ITopology {
	public:
		virtual ~ITopology() = default;

		virtual const LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& lts() const = 0;

		virtual const std::vector<std::string>& initial_state() const = 0;

		virtual const State<std::vector<std::string>, std::pair<size_t, std::string>>& at(const std::vector<std::string>& key) = 0;
	};

}