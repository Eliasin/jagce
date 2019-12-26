#ifndef JAGCE_DECODER
#define JAGCE_ENCODER

#include <variant>
#include <vector>

#include "register_names.hpp"
#include "byte_stream.hpp"

namespace jagce {

	using Address = size_t;
	using Immediate8 = uint8_t;
	using Immediate16 = uint16_t;

	struct PartialAddress {
		std::variant<Immediate8, RegisterName> msb;
		std::variant<Immediate8, RegisterName> lsb;
		bool operator==(const PartialAddress& other) const;
	};

	using Writeable = std::variant<RegisterName, Address, Indirect, PartialAddress>;
	using Readable = std::variant<RegisterName, Address, Immediate8, Immediate16, Indirect, PartialAddress>;

	struct LoadEvent8 {
		Writeable dest;
		Readable src;
		bool operator==(const LoadEvent8& other) const;
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
	using Event = std::variant<RegisterShiftEvent, LoadEvent8, NopEvent>;

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
