#ifndef JAGCE_DECODER
#define JAGCE_ENCODER

#include <variant>
#include <vector>
#include <array>

#include "register_names.hpp"
#include "byte_stream.hpp"

namespace jagce {

	using Address = size_t;
	using Immediate8 = uint8_t;
	using Immediate16 = uint16_t;

	struct PartialAddress {
		std::variant<Immediate8, RegisterName8> msb;
		std::variant<Immediate8, RegisterName8> lsb;
		constexpr bool operator==(const PartialAddress& other) const {
			return this->msb == other.msb && this->lsb == other.lsb;
		}
	};

	struct Register8PlusFlag {
		RegisterName8 r;
		FlagName flag;
		constexpr bool operator==(const Register8PlusFlag& other) const {
			return this->r == other.r && this->flag == other.flag;
		}
	};

	struct IndirectPlusFlag {
		Indirect i;
		FlagName flag;
		constexpr bool operator==(const IndirectPlusFlag& other) const {
			return this->i == other.i && this->flag == other.flag;
		}
	};

	struct Immediate8PlusFlag {
		Immediate8 n;
		FlagName flag;
		constexpr bool operator==(const Immediate8PlusFlag& other) const {
			return this->n == other.n && this->flag == other.flag;
		}
	};

	struct Register16PlusValue {
		RegisterName reg;
		Immediate16 val;
		constexpr bool operator==(const Register16PlusValue& other) const {
			return this->reg == other.reg && this->val == other.val;
		}
	};

	using Writeable16 = std::variant<RegisterName16, Address, Indirect, PartialAddress>;
	using Writeable = std::variant<RegisterName, Address, Indirect, PartialAddress>;
	using Readable8 = std::variant<RegisterName8, Address, Immediate8, Indirect, PartialAddress, Register8PlusFlag, IndirectPlusFlag, Immediate8PlusFlag>;
	using Readable = std::variant<RegisterName, Address, Immediate8, Immediate16, Indirect, PartialAddress, Register16PlusValue, Register8PlusFlag, IndirectPlusFlag, Immediate8PlusFlag>;

	struct LoadEvent8 {
		Writeable dest;
		Readable8 src;
		constexpr bool operator==(const LoadEvent8& other) const {
			return this->dest == other.dest && this->src == other.src;
		}
	};

	using FlagStateChange = std::array<FlagState, 8>;

	constexpr FlagStateChange unionFlagStateChange(FlagStateChange a, FlagStateChange b) {
		FlagStateChange result{a};
		for (size_t i = 0; i < b.size(); i++) {
			if (b.at(i) != FlagState::UNCH) {
				if (result.at(i) != FlagState::UNCH) {
					throw std::logic_error("Attempted union of incompatible FlagStateChange objects");
				}
				result.at(i) = b.at(i);
			}
		}

		return result;
	};

	constexpr FlagStateChange _add8FlagStateChanges() {
		FlagStateChange f{};
		f.at(static_cast<size_t>(jagce::FlagName::S)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::Z)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F5)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::H)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F3)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::PV)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::N)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::C)) = jagce::FlagState::DEFER;

		return f;
	}

	constexpr FlagStateChange _sub8FlagStateChanges() {
		FlagStateChange f{};
		f.at(static_cast<size_t>(jagce::FlagName::S)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::Z)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F5)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::H)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F3)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::PV)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::N)) = jagce::FlagState::SET;
		f.at(static_cast<size_t>(jagce::FlagName::C)) = jagce::FlagState::DEFER;

		return f;
	}

	constexpr FlagStateChange _and8FlagStateChanges() {
		FlagStateChange f{};
		f.at(static_cast<size_t>(jagce::FlagName::S)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::Z)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F5)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::H)) = jagce::FlagState::SET;
		f.at(static_cast<size_t>(jagce::FlagName::F3)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::PV)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::N)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::C)) = jagce::FlagState::RESET;

		return f;
	}

	constexpr FlagStateChange _or8FlagStateChanges() {
		FlagStateChange f{};
		f.at(static_cast<size_t>(jagce::FlagName::S)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::Z)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F5)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::H)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::F3)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::PV)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::N)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::C)) = jagce::FlagState::RESET;

		return f;
	}

	constexpr FlagStateChange _xor8FlagStateChanges() {
		FlagStateChange f{};
		f.at(static_cast<size_t>(jagce::FlagName::S)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::Z)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F5)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::H)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::F3)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::PV)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::N)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::C)) = jagce::FlagState::RESET;

		return f;
	}

	constexpr FlagStateChange _compare8FlagStateChanges() {
		FlagStateChange f{};
		f.at(static_cast<size_t>(jagce::FlagName::S)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::Z)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F5)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::H)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F3)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::PV)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::N)) = jagce::FlagState::SET;
		f.at(static_cast<size_t>(jagce::FlagName::C)) = jagce::FlagState::DEFER;

		return f;
	}

	constexpr FlagStateChange _increment8FlagStateChanges() {
		FlagStateChange f{};
		f.at(static_cast<size_t>(jagce::FlagName::S)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::Z)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F5)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::H)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F3)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::PV)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::N)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::C)) = jagce::FlagState::UNCH;

		return f;
	}

	constexpr FlagStateChange _decrement8FlagStateChanges() {
		FlagStateChange f{};
		f.at(static_cast<size_t>(jagce::FlagName::S)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::Z)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F5)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::H)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F3)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::PV)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::N)) = jagce::FlagState::SET;
		f.at(static_cast<size_t>(jagce::FlagName::C)) = jagce::FlagState::UNCH;

		return f;
	}

	constexpr FlagStateChange _addHLFlagStateChanges() {
		FlagStateChange f{};
		f.at(static_cast<size_t>(jagce::FlagName::S)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::Z)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::F5)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::H)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F3)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::PV)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::N)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::C)) = jagce::FlagState::DEFER;

		return f;
	}

	constexpr FlagStateChange _addSPFlagStateChanges() {
		FlagStateChange f{};
		f.at(static_cast<size_t>(jagce::FlagName::S)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::Z)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::F5)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::H)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F3)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::PV)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::N)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::C)) = jagce::FlagState::DEFER;

		return f;
	}

	struct LoadEvent16 {
		Writeable16 dest;
		Readable src;
		FlagStateChange flagStates;
		constexpr bool operator==(const LoadEvent16& other) const {
			return this->dest == other.dest && this->src == other.src && this->flagStates == other.flagStates;
		}
	};

	struct PushEvent {
		Readable src;
		constexpr bool operator==(const PushEvent& other) const {
			return this->src == other.src;
		}
	};

	struct PopEvent {
		Writeable16 dest;
		constexpr bool operator==(const PopEvent& other) const {
			return this->dest == other.dest;
		}
	};

	struct AddEvent8 {
		Readable8 a;
		Readable8 b;
		constexpr static FlagStateChange flagStates = _add8FlagStateChanges();
		constexpr bool operator==(const AddEvent8& other) const {
			return this->a == other.a && this->b == other.b;
		}
	};

	struct AddHLEvent {
		RegisterName16 r;
		constexpr static FlagStateChange flagStates = _addHLFlagStateChanges();
		constexpr bool operator==(const AddHLEvent& other) const {
			return this->r == other.r;
		}
	};

	struct AddSPEvent {
		Immediate8 i;
		constexpr static FlagStateChange flagStates = _addSPFlagStateChanges();
		constexpr bool operator==(const AddSPEvent& other) const {
			return this->i == other.i;
		}
	};

	struct SubEvent8 {
		Readable8 r;
		constexpr static FlagStateChange flagStates = _sub8FlagStateChanges();
		constexpr bool operator==(const SubEvent8& other) const {
			return this->r == other.r;
		}
	};

	struct AndEvent8 {
		Readable8 r;
		constexpr static FlagStateChange flagStates = _and8FlagStateChanges();
		constexpr bool operator==(const AndEvent8& other) const {
			return this->r == other.r;
		}
	};

	struct OrEvent8 {
		Readable8 r;
		constexpr static FlagStateChange flagStates = _or8FlagStateChanges();
		constexpr bool operator==(const OrEvent8& other) const {
			return this->r == other.r;
		}
	};

	struct XorEvent8 {
		Readable8 r;
		constexpr static FlagStateChange flagStates = _xor8FlagStateChanges();
		constexpr bool operator==(const XorEvent8& other) const {
			return this->r == other.r;
		}
	};

	struct CompareEvent8 {
		Readable8 r;
		constexpr static FlagStateChange flagStates = _compare8FlagStateChanges();
		constexpr bool operator==(const CompareEvent8& other) const {
			return this->r == other.r;
		}
	};

	struct IncrementEvent8 {
		Writeable r;
		constexpr static FlagStateChange flagStates = _increment8FlagStateChanges();
		constexpr bool operator==(const IncrementEvent8& other) const {
			return this->r == other.r;
		}
	};

	struct IncrementEvent16 {
		RegisterName16 r;
		constexpr bool operator==(const IncrementEvent16& other) const {
			return this->r == other.r;
		}
	};

	struct DecrementEvent16 {
		RegisterName16 r;
		constexpr bool operator==(const DecrementEvent16& other) const {
			return this->r == other.r;
		}
	};

	struct DecrementEvent8 {
		Writeable r;
		constexpr static FlagStateChange flagStates = _decrement8FlagStateChanges();
		constexpr bool operator==(const DecrementEvent8& other) const {
			return this->r == other.r;
		}
	};


	enum class ShiftDirection {
		LEFT,
		RIGHT
	};

	enum class ShiftType {
		LOGICAL,
		ARITHMETIC,
		ROTATE
	};

	struct RegisterShiftEvent {
		RegisterName registerName;
		ShiftDirection direction;
		ShiftType type;
		unsigned int amount;
		constexpr bool operator==(const RegisterShiftEvent& other) const {
			return this->registerName == other.registerName && this->direction == other.direction && this->type == other.type && this->amount == other.amount;
		}
	};

	using NopEvent = std::monostate;

	using Event = std::variant<DecrementEvent8, IncrementEvent8, CompareEvent8, XorEvent8, OrEvent8, AndEvent8, SubEvent8, AddEvent8, PushEvent, PopEvent, RegisterShiftEvent, LoadEvent8, LoadEvent16, NopEvent, AddHLEvent, AddSPEvent, IncrementEvent16, DecrementEvent16>;

	/** 
	 * The decoder class consumes bytes from a byte stream as it's input
	 * and produces 'events' from it. Events are state changes to RAM,
	 * video memory or registers.
	 */
	class Decoder {
	public:
		Event decodeEvent(ByteStream& in) const;
		std::vector<Event> decodeEvents(ByteStream& in, size_t n) const;
		std::vector<Event> decodeUntilEmpty(ByteStream& in) const;
	};

}

#endif
