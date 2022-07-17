#include "pcs/controller/controller.h"
#include <gtest/gtest.h>

#include <vector>
#include <unordered_set>

#include <boost/container_hash/hash.hpp>

static pcs::Environment LoadPadMachine() {
	pcs::Environment machine;
	try {
		machine.AddResource("../../data/pad/Resource1.txt", false);
		machine.AddResource("../../data/pad/Resource2.txt", false);
		machine.AddResource("../../data/pad/Resource3.txt", false);
		machine.AddResource("../../data/pad/Resource4.txt", false);
		machine.AddResource("../../data/pad/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	return machine;
}

TEST(Controller, Pad) {
	nightly::LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>, 
		     boost::hash<std::pair<std::string,std::vector<std::string>>>> expected;
	nightly::ReadFromFile(expected, "../../tests/controller/testdata/pad/controller.txt");

	pcs::Environment machine = LoadPadMachine();
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/pad/recipe.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	machine.Complete();

	pcs::Controller con(&machine, machine.topology(), &recipe);
	auto opt = con.Generate();
	ASSERT_EQ(opt.has_value(), true);
	auto got = opt.value();
	ASSERT_EQ(got, expected);
}

TEST(Controller, Pad_Incremental) {
	nightly::LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>,
		boost::hash<std::pair<std::string, std::vector<std::string>>>> expected;
	nightly::ReadFromFile(expected, "../../tests/controller/testdata/pad/controller.txt");

	pcs::Environment machine = LoadPadMachine();
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/pad/recipe.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	machine.Incremental();

	pcs::Controller con(&machine, machine.topology(), &recipe);
	auto opt = con.Generate();
	ASSERT_EQ(opt.has_value(), true);
	auto got = opt.value();
	ASSERT_EQ(got, expected);
}