#ifndef JAGCE_REGISTER_NAMES
#define JAGCE_REGISTER_NAMES

#include <cstddef>
#include <utility>
#include <array>

namespace jagce {

	struct RegisterName {
		constexpr bool operator==(const RegisterName& other) const { return id == other.id; };
	private:
		int id;
	protected:
		constexpr RegisterName(int id) : id(id) {};
	};

	struct RegisterNames;
	class RegisterName8 : public RegisterName {
		friend RegisterNames;
		constexpr RegisterName8(int id) : RegisterName{id} {};
	};

	class RegisterName16 : public RegisterName {
		friend RegisterNames;
		constexpr RegisterName16(int id) : RegisterName{id} {};
	};

	struct RegisterNames {
		constexpr static RegisterName8 A{0}, B{1}, C{2}, D{3}, E{4}, H{5}, L{6};
		constexpr static RegisterName16 AF{7}, BC{8}, DE{9}, HL{10}, SP{11}, PC{12};
	};

	// DEFER refers to FlagState events that cannot be described at the current time. For example,
	// the H or C flag during the decoding step, as the contents of the registers is not known at
	// that time.
	enum class FlagState : uint8_t {
		UNCH = 0,
		SET = 1,
		RESET = 2,
		DEFER = 3
	};

	enum class FlagName : size_t {
		S = 0, Z = 1, F5 = 2, H = 3, F3 = 4, PV = 5, N = 6, C = 7
	};

	using SingleFlagEvent = std::pair<FlagName, FlagState>;

	template <size_t N>
	using FlagEvents = std::array<SingleFlagEvent, N>;

	enum class Indirect {
		HL,
		HLI,
		HLD,
		BC,
		DE
	};

}
#endif
