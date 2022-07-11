#include "pcs/controller/controller.h"
#include <gtest/gtest.h>

#include <vector>
#include <unordered_set>

#include <boost/container_hash/hash.hpp>

static pcs::Environment LoadPadMachine() {
	pcs::Environment machine;
	try {
		machine.AddResource("../../data/best/Resource1.txt", false);
		machine.AddResource("../../data/best/Resource2.txt", false);
		machine.AddResource("../../data/best/Resource3.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	return machine;
}

// First test output of DFS, which should result in a larger controller
TEST(Best_Controller, DFS) {
	pcs::LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>,
		boost::hash<std::pair<std::string, std::vector<std::string>>>> expected;
	pcs::ReadFromFile(expected, "../../tests/controller/testdata/best/dfs.txt");

	pcs::Environment machine = LoadPadMachine();
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/best/recipe.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	machine.Complete();

	pcs::Controller con(&machine, machine.topology(), &recipe);
	auto opt = con.Generate();
	ASSERT_EQ(opt.has_value(), true);
	auto got = opt.value();
	ASSERT_EQ(*got, expected);
}

TEST(Best_Controller, BreadthFirst) {
	pcs::LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>,
		boost::hash<std::pair<std::string, std::vector<std::string>>>> expected;
	pcs::ReadFromFile(expected, "../../tests/controller/testdata/best/best.txt");

	pcs::Environment machine = LoadPadMachine();
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/best/recipe.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	machine.Complete();
	
	pcs::MinimizeOpt opt_sync{ pcs::MinimizeOpt::Transitions };
	pcs::MinimizeOpt opt_resources{ pcs::MinimizeOpt::Resources };



}
