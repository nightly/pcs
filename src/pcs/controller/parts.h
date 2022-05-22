#pragma once

#include <vector>
#include <string>

namespace pcs {

	class Parts {
	private:
		std::vector<std::vector<std::string>> vector_;
	public:
		Parts(size_t num_resources);
		Parts(const Parts& other);
		Parts& operator=(const Parts& other);
		Parts(Parts&& other) noexcept;
		Parts& operator=(Parts&& other) noexcept;


	};

}