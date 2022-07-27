#include "pcs/product/parsers/recipe.h"

#include <unordered_set>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "pcs/product/recipe.h"
#include "pcs/operation/guard.h"
#include "pcs/operation/operation.h"
#include "pcs/operation/parameter.h"

namespace pcs {

	/**
	 * @brief ReadFromFile will parse a JSON input file into an instance of the LTS<Transition = CompositeOperation> class.
	 *
	 * The expected form consists of: `initialState` as a string, and an array of `transitions`
	 * consisting of `startState`, `label`, and `endState` strings.
	 * Label consists of the following objects: `guard`, `parallelOperations` (`name`, `input`), 
	 * `sequentialOperations` (`name`, `input`, `output`).
	 *
	 * @param lts: Labelled Transition System to parse into
	 * @param filepath: path to the file containing a LTS, examples contained within the data folder.
	 * @exception Propagates std::ifstream::failure
	 */
	void ReadFromJsonFile(nightly::LTS<std::string, CompositeOperation>& lts, const std::filesystem::path& filepath) {
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

	/**
	 * @brief ParseJson will read data into a LTS<KeyType = String, TransitionType = CompositeOperation> instance from a JSON object instance.
	 * @param lts: Labelled Transition System to parse into
	 * @param j: json object containing the correct object layout as previously defined
	 */
	void ParseJson(nightly::LTS<std::string, CompositeOperation>& lts, const nlohmann::json& j) {
		lts.set_initial_state(j["initialState"]);
		for (const auto& t : j["transitions"]) {
			CompositeOperation co;

			if (t["label"]["guard"] != nlohmann::json::object()) {
				std::unordered_set<std::string> input;
				co.guard.set_operation(pcs::Observable(t["label"]["guard"]["name"]));
				for (const auto& g : t["label"]["guard"]["input"]) {
					input.emplace(g);
				}
				co.guard.set_input(std::move(input));
			}

			for (const auto& seq_op : t["label"]["sequential"]) {
				Observable o;
				o.name_ = seq_op["name"];
				std::unordered_set<std::string> input;
				std::vector<Parameter> parameters;
				std::vector<std::string> output;
				for (const auto& in : seq_op["input"]) {
					input.emplace(in);
				}
				for (const auto& param : seq_op["parameters"]) {
					parameters.emplace_back(param["name"], param["value"]);
				}
				for (const auto& out : seq_op["output"]) {
					output.emplace_back(out);
				}
				co.sequential.emplace_back(std::move(o), std::move(input), std::move(parameters), std::move(output));
			}

			for (const auto& par_op : t["label"]["parallel"]) {
				Observable o;
				o.name_ = par_op["name"];
				std::unordered_set<std::string> input;
				std::vector<Parameter> parameters;
				std::vector<std::string> output;
				for (const auto& in : par_op["input"]) {
					input.emplace(in);
				}
				for (const auto& param : par_op["parameters"]) {
					parameters.emplace_back(param["name"], param["value"]);
				}
				for (const auto& out : par_op["output"]) {
					output.emplace_back(out);
				}
				co.parallel.emplace_back(std::move(o), std::move(input), std::move(parameters), std::move(output));
			}

			lts.AddTransition(t["startState"], std::move(co), t["endState"]);
		}
	}
}