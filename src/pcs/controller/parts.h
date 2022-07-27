#pragma once

#include <vector>
#include <unordered_set>
#include <string>

#include "pcs/operation/parameterized_op.h"

namespace pcs {

	class Parts {
	public:
		using TopologyTransition = std::pair<size_t, ParameterizedOp>;
	private:
		std::vector<std::unordered_set<std::string>> parts_;
	public:
		Parts() = default;
		Parts(size_t num_resources);
		Parts(const Parts& other);
		Parts& operator=(const Parts& other);
		Parts(Parts&& other) noexcept;
		Parts& operator=(Parts&& other) noexcept;

		const std::unordered_set<std::string>& AtResource(size_t resource) const;

		void Add(const TopologyTransition& transition, const std::vector<std::string>& output);
		bool Synchronize(size_t in, size_t out, const std::unordered_set<std::string>& input);
		bool Allocate(const TopologyTransition& transition, const std::unordered_set<std::string>& input);

		bool operator==(const Parts& other) const;

		friend std::ostream& operator<<(std::ostream& os, const Parts& parts);
	};

}