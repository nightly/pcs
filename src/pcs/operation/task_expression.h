#pragma once

#include <unordered_set>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>

#include "pcs/operation/observable.h"
#include "pcs/operation/parameter.h"

namespace pcs {

	class TaskExpression {
	private:
		Observable operation_;
		std::unordered_set<std::string> input_;
		std::vector<Parameter> parameters_;
		std::vector<std::string> output_;
	public:
		TaskExpression() = default;
		TaskExpression(Observable&& operation, std::unordered_set<std::string>&& input, std::vector<Parameter>&& parameters, std::vector<std::string>&& output);
		TaskExpression(const Observable& operation, const std::unordered_set<std::string>& input, 
			const std::vector<Parameter>& parameters, const std::vector<std::string>& output);
		TaskExpression(const TaskExpression& other);
		TaskExpression& operator=(const TaskExpression& other);
		TaskExpression(TaskExpression&& other) noexcept;
		TaskExpression& operator=(TaskExpression&& other) noexcept;
		~TaskExpression() = default;

		const Observable& operation() const;
		void set_operation(Observable&& operation);
		void set_operation(const Observable& operation);
		
		const std::unordered_set<std::string>& input() const;
		void set_input(std::unordered_set<std::string>&& input);
		void set_input(const std::unordered_set<std::string>& input);

		const std::vector<Parameter>& parameters() const;
		void set_parameters(std::vector<Parameter>&& parameters);
		void set_parameters(const std::vector<Parameter>& parameters);

		const std::vector<std::string>& output() const;
		void set_output(std::vector<std::string>&& output);
		void set_output(const std::vector<std::string>& output);

		template <std::size_t N>
		decltype(auto) get() const {
			if constexpr (N == 0) return operation();
			else if constexpr (N == 1) return input();
			else if constexpr (N == 2) return parameters();
			else if constexpr (N == 3) return output();
		}

		bool operator==(const TaskExpression& other) const;
		friend std::ostream& operator<<(std::ostream& os, const TaskExpression& task);
		friend std::ofstream& operator<<(std::ofstream& os, const TaskExpression& task);
	};

}

namespace std {

	template<>
	struct tuple_size<pcs::TaskExpression> : std::integral_constant<std::size_t, 4> {};

	template<std::size_t N>
	struct tuple_element<N, pcs::TaskExpression> {
		using type = decltype(std::declval<pcs::TaskExpression>().get<N>());
	};
}