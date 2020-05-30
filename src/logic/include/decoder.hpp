#ifndef JAGCE_DECODER
#define JAGCE_ENCODER

#include <variant>
#include <vector>
#include <array>

#include "register_names.hpp"
#include "byte_stream.hpp"

namespace jagce {

	//TODO Make flag state changes static on most operations
	using Address = size_t;
	using Immediate8 = uint8_t;
	using Immediate16 = uint16_t;

	struct PartialAddress {
		std::variant<Immediate8, RegisterName8> msb;
		std::variant<Immediate8, RegisterName8> lsb;
		bool operator==(const PartialAddress& other) const;
	};

	struct Register8PlusFlag {
		RegisterName8 r;
		FlagName flag;
		bool operator==(const Register8PlusFlag& other) const;
	};

	struct IndirectPlusFlag {
		Indirect i;
		FlagName flag;
		bool operator==(const IndirectPlusFlag& other) const;
	};

	struct Immediate8PlusFlag {
		Immediate8 n;
		FlagName flag;
		bool operator==(const Immediate8PlusFlag& other) const;
	};

	struct Register16PlusValue {
		RegisterName reg;
		Immediate16 val;
		bool operator==(const Register16PlusValue& other) const;
	};

	using Writeable16 = std::variant<RegisterName16, Address, Indirect, PartialAddress>;
	using Writeable = std::variant<RegisterName, Address, Indirect, PartialAddress>;
	using Readable8 = std::variant<RegisterName8, Address, Immediate8, Indirect, PartialAddress, Register8PlusFlag, IndirectPlusFlag, Immediate8PlusFlag>;
	using Readable = std::variant<RegisterName, Address, Immediate8, Immediate16, Indirect, PartialAddress, Register16PlusValue, Register8PlusFlag, IndirectPlusFlag, Immediate8PlusFlag>;

	struct LoadEvent8 {
		Writeable dest;
		Readable8 src;
		bool operator==(const LoadEvent8& other) const;
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

	struct LoadEvent16 {
		Writeable16 dest;
		Readable src;
		FlagStateChange flagStates;
		bool operator==(const LoadEvent16& other) const;
	};

	struct PushEvent {
		Readable src;
		bool operator==(const PushEvent& other) const;
	};

	struct PopEvent {
		Writeable16 dest;
		bool operator==(const PopEvent& other) const;
	};

	struct AddEvent8 {
		Readable8 a;
		Readable8 b;
		constexpr static FlagStateChange flagStates = _add8FlagStateChanges();
		bool operator==(const AddEvent8& other) const;
	};

	struct SubEvent8 {
		Readable8 r;
		constexpr static FlagStateChange flagStates = _sub8FlagStateChanges();
		bool operator==(const SubEvent8& other) const;
	};

	struct AndEvent8 {
		Readable8 r;
		constexpr static FlagStateChange flagStates = _and8FlagStateChanges();
		bool operator==(const AndEvent8& other) const;
	};

	struct OrEvent8 {
		Readable8 r;
		constexpr static FlagStateChange flagStates = _or8FlagStateChanges();
		bool operator==(const OrEvent8& other) const;
	};

	struct XorEvent8 {
		Readable8 r;
		constexpr static FlagStateChange flagStates = _xor8FlagStateChanges();
		bool operator==(const XorEvent8& other) const;
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
		bool operator==(const RegisterShiftEvent& other) const;
	};

	using NopEvent = std::monostate;

	using Event = std::variant<XorEvent8, OrEvent8, AndEvent8, SubEvent8, AddEvent8, PushEvent, PopEvent, RegisterShiftEvent, LoadEvent8, LoadEvent16, NopEvent>;

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
