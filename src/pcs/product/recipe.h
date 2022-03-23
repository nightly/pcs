#pragma once

#include <vector>
#include <unordered_map>

#include "pcs/product/operation.h"
#include "pcs/lts/lts.h"

namespace pcs::product {

	using LabelledTransitionSystem = pcs::lts::LabelledTransitionSystem;
	using State = pcs::lts::State;

	class Recipe {
	public:
		LabelledTransitionSystem lts_;
		std::unordered_map<std::string, Operation> operations_;
	public:
		Recipe();
	};

	// Recipes are also formalised as LTS'
	//	- Nodes represent state of parts
	//  - Edges represent the composite operations required to transistion between states
	//      - CompositeOp = {Test} {";" {Step}}*  - where steps are the sequential or parallel operations
	//		- Operations can be combined in sequence (denoted by ';' or in parallel denoted by '||')

}
