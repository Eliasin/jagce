#include "decoder.hpp"

#include <optional>

namespace jagce {

	Event createLoadFromImmediate8ToRegister(ByteStream& in, RegisterName r) {
		Immediate8 immediate8 = static_cast<Immediate8>(in.get());
		return {LoadEvent8{{r}, {Immediate8{immediate8}}}};
	}

	bool LoadEvent8::operator==(const LoadEvent8& other) const {
		return this->dest == other.dest && this->src == other.src;
	}

	Event Decoder::decodeEvent(ByteStream& in) const {
		uint8_t firstByte = in.get();
		std::optional<uint8_t> prefixByte{};
		uint8_t opcode{};

		if (firstByte == 0xCB || firstByte == 0xDD || firstByte == 0xED || firstByte == 0xFD) {
			prefixByte = firstByte;
			opcode = in.get();
		} else {
			opcode = firstByte;
		}

		switch (opcode) {
			// 8-bit immediate to register load
			case 0x06:
				return createLoadFromImmediate8ToRegister(in, RegisterName::B);
				break;
			case 0x0E:
				return createLoadFromImmediate8ToRegister(in, RegisterName::C);
				break;
			case 0x16:	
				return createLoadFromImmediate8ToRegister(in, RegisterName::D);
				break;
			case 0x1E:
				return createLoadFromImmediate8ToRegister(in, RegisterName::E);
				break;
			case 0x26:
				return createLoadFromImmediate8ToRegister(in, RegisterName::H);
				break;
			case 0x2E:
				return createLoadFromImmediate8ToRegister(in, RegisterName::L);
				break;
			// 8-bit register/indirect to register/indirect loads
			case 0x7F:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::A}}};
				break;
			case 0x78:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::B}}};
				break;
			case 0x79:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::C}}};
				break;
			case 0x7A:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::D}}};
				break;
			case 0x7B:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::E}}};
				break;
			case 0x7C:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::H}}};
				break;
			case 0x7D:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::L}}};
			case 0x7E:
				return {LoadEvent8{{RegisterName::A}, {Indirect::HL}}};
				break;
			default:
				return {NopEvent{}};
				break;
		}
	}

	std::vector<Event> Decoder::decodeEvents(ByteStream& in, size_t n) const {
		std::vector<Event> events{};
		for (size_t i = 0; i < n; i++) {
			events.push_back(decodeEvent(in));
		}
		return events;
	}

	std::vector<Event> Decoder::decodeUntilEmpty(ByteStream& in) const {
		return decodeEvents(in, in.size());
	}
}
