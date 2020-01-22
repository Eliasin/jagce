#ifndef JAGCE_REGISTER_NAMES
#define JAGCE_REGISTER_NAMES

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
	enum class FlagState {
		UNCH,
		SET,
		RESET,
		DEFER
	};

	enum class Indirect {
		HL,
		HLI,
		HLD,
		BC,
		DE
	};

}
#endif
