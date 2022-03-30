#pragma once

#include <string>
#include <ostream>
#include <filesystem>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"

namespace pcs {
	void ExportToFile(const LabelledTransitionSystem<>& lts, const std::filesystem::path& path);

	std::ostream& operator<<(std::ostream& os, const LabelledTransitionSystem<>& lts);
	std::ostream& operator<<(std::ostream& os, const State<>& state);

	std::ofstream& operator<<(std::ofstream& os, const State<>& state);
	std::ofstream& operator<<(std::ofstream& os, const LabelledTransitionSystem<>& lts);
}