#pragma once 

#include <string>
#include <vector>
#include <utility>

namespace pcs::lts {

	class State {
	public:
		std::vector<std::pair<std::string, std::string>> transitions_;
	private:
		std::string name_;
	public:
		State(const std::string& name);
		~State() = default;

		std::string GetName() const;
		void AddTransistion(std::string label, std::string end_state);

		bool operator==(const State& other) const;

	};

}