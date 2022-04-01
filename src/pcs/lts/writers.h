#pragma once

#include <string>
#include <ostream>
#include <filesystem>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"
#include "pcs/common/directory.h"

namespace pcs {

	/*
	 * @brief Exports a given LTS to the provided path, which can be visualised with GraphViz
	 * @param lts: labelled transition system in question to write to file
	 * @param path: export filepath, missing directories will be created. Filepath should include file extension.
	 * @exception Propagates std::ofstream::failure
	 */
	template <typename TransitionT = std::string>
	void ExportToFile(const LabelledTransitionSystem<TransitionT>& lts, const std::filesystem::path& path) {
		std::ofstream stream;
		stream.exceptions(std::ofstream::badbit);
		CreateDirectoryForPath(path);
		try {
			stream.open(path, std::ios::out | std::ios::trunc);
			stream << lts;
		} catch (const std::ofstream::failure& e) {
			throw;
		}
	}

	// @Note: ostream & ofstream overloads are inlined in lts.h and state.h

}