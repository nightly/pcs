#pragma once

#include <vector>
#include <span>
#include <optional>
#include <string>
#include <unordered_set>

#include <boost/container_hash/hash.hpp>

#include "lts/lts.h"
#include "pcs/topology/topology.h"
#include "pcs/topology/core.h"

namespace pcs {

	class CompleteTopology : public ITopology {
	private:
		nightly::LTS<std::vector<std::string>, std::pair<size_t, ParameterizedOp>, boost::hash<std::vector<std::string>>> topology_;

		const std::vector<nightly::LTS<std::string, ParameterizedOp>>& ltss_;
		std::unordered_set<std::vector<std::string>, boost::hash<std::vector<std::string>>> visited_;
	public:
		CompleteTopology(const std::vector<nightly::LTS<std::string, ParameterizedOp>>& ltss, bool recursive=true);
		const nightly::LTS<std::vector<std::string>, std::pair<size_t, ParameterizedOp>, boost::hash<std::vector<std::string>>>& lts() const override;
		operator const nightly::LTS<std::vector<std::string>, std::pair<size_t, ParameterizedOp>, boost::hash<std::vector<std::string>>>& () const;
		const std::vector<std::string>& initial_state() const override;
		const nightly::State<std::vector<std::string>, std::pair<size_t, ParameterizedOp>>& at(const std::vector<std::string>& key) override;
	private:
		void CombineRecursive(std::vector<std::string>& states_vec);
		void CombineIterative(std::vector<std::string>& initial_key);
	};

}