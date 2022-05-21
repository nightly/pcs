#pragma once

namespace pcs {
	 
	 /**
	 * @brief Basically a std::pair, to represent State transitions but with more domain-focused naming to avoid pair inception confusion
	 * @tparam KeyT: the type of the State which the transition points to
	 * @tparam TransitionT: the label of the given transition
	 */
	template <typename TransitionT = std::string, typename KeyT = std::string>
	class Transition {
	private:
		TransitionT label_;
		KeyT to_;
	public:

		// ===========================
		// Constructors & destructor
		// ===========================

		Transition() = default;

		Transition(const TransitionT& label, const KeyT& to)
			: label_(label), to_(to) {}

		Transition(TransitionT&& label, KeyT&& to)
			: label_(std::move(label)), to_(std::move(to)) {}


		Transition(const Transition& other)
			: label_(other.label_), to_(other.to_) {}

		Transition& operator=(const Transition& other) {
			label_ = other.label_;
			to_ = other.to_;
			return *this;
		}

		Transition(Transition&& other) noexcept 
		: label_(std::move(other.label_)), to_(std::move(other.to_)) {}

		Transition& operator=(Transition&& other) noexcept {
			label_ = std::move(other.label_);
			to_ = std::move(other.to_);
			return *this;
		}

		~Transition() = default;

		// ===========================
		// Getters/setters
		// ===========================

		const TransitionT& label() const {
			return label_;
		}

		void set_label(const TransitionT& label) {
			label_ = label;
		}

		void set_label(TransitionT&& label) {
			label_ = std::move(label);
		}

		const KeyT& to() const {
			return to_;
		}

		void set_to(const KeyT& to) {
			to_ = to;
		}

		void set_to(KeyT&& to) {
			to_ = std::move(to);
		}

		// ===========================
		// Operator overloads
		// ===========================

		bool operator==(const Transition& other) const {
			return (label_ == other.label_) && (to_ == other.to_);
		}

	};
}