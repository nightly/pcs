#include "pcs/product/parser.h"

#include "pcs/product/recipe.h"
#include "pcs/operation/guard.h"
#include "pcs/operation/operation.h"

namespace pcs {

	/*
	 * @brief ReadFromFile will parse a JSON input file into an instance of the LTS<Transition = CompositeOperation> class.
	 *
	 * The expected form consists of: initialState as a string, and an array of transitions
	 * consisting of startState, label, and endState strings.
	 * Label consists of the following objects: guard, parallelOperations (name, input), sequentialOperations (name, input, output).
	 *
	 * @param lts: Labelled Transition System to parse into
	 * @param filepath: path to the file containing a LTS, examples contained within the data folder.
	 * @exception Propagates std::ifstream::failure
	 */
	void ReadFromJsonFile(LabelledTransitionSystem<CompositeOperation>& lts, const std::filesystem::path& filepath) {
		nlohmann::json j;
		try {
			std::ifstream stream(filepath);
			stream >> j;
		}
		catch (std::ifstream::failure& e) {
			throw;
		}
		ParseJson(lts, j);
	}

	/*
	 * @brief ParseJson will read data into a LTS<Transition = CompositeOperation> instance from a JSON object instance.
	 * @param lts: Labelled Transition System to parse into
	 * @param j: json object containing the correct object layout as previously defined
	 */
	void ParseJson(LabelledTransitionSystem<CompositeOperation>& lts, const nlohmann::json& j) {
		lts.SetInitialState(j["initialState"], true);
		for (const auto& t : j["transitions"]) {
			CompositeOperation co;
			co.guard.name = t["guard"]["name"];
			for (const auto& g : t["guard"]["input"]) {
				co.guard.input.emplace_back(g);
			}
			for (const auto& seq_op : t["sequential"]) {
				Operation o;
				o.name = seq_op["name"];
				for (const auto& in : seq_op["input"]) {
					o.input.emplace_back(in);
				}
				for (const auto& out : seq_op["output"]) {
					o.input.emplace_back(out);
				}
			}
			for (const auto& par_op : t["parallel"]) {
				Operation o;
				o.name = par_op["name"];
				for (const auto& in : par_op["input"]) {
					o.input.emplace_back(in);
				}
				for (const auto& out : par_op["output"]) {
					o.input.emplace_back(out);
				}
			}
			lts.AddTransition(t["startState"], std::move(co), t["endState"], true);
		}
	}
}