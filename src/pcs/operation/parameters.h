#pragma once

#include <fstream>
#include <ostream>
#include <string>
#include <unordered_map>

namespace pcs {

	class Parameters {
	private:
		std::unordered_map<std::string, std::string> parameters_;
	public:
		Parameters() = default;
		Parameters(const std::unordered_map<std::string, std::string>& params);
		Parameters(std::unordered_map<std::string, std::string>&& params);
		Parameters(const Parameters& other);
		Parameters& operator=(const Parameters& other);
		Parameters(Parameters&& other) noexcept;
		Parameters& operator=(Parameters&& other) noexcept;

		std::unordered_map<std::string, std::string>& map();
		const std::unordered_map<std::string, std::string>& map() const;

		bool empty() const;
		bool operator==(const Parameters& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Parameters& params);
	};
}