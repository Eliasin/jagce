#include "decoder.hpp"

#include <optional>

namespace jagce {

	Event createLoadFromImmediate8ToRegister(ByteStream& in, RegisterName r) {
		Immediate8 immediate8 = static_cast<Immediate8>(in.get());
		return {LoadEvent8{{r}, {Immediate8{immediate8}}}};
	}

	bool PartialAddress::operator==(const PartialAddress& other) const {
		return this->msb == other.msb && this->lsb == other.lsb;
	}

	bool LoadEvent8::operator==(const LoadEvent8& other) const {
		return this->dest == other.dest && this->src == other.src;
	}

	bool RegisterShiftEvent::operator==(const RegisterShiftEvent& other) const {
		return this->registerName == other.registerName && this->direction == other.direction && this->type == other.type && this->amount == other.amount;
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
			case 0x40:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::B}}};
				break;
			case 0x41:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::C}}};
				break;
			case 0x42:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::D}}};
				break;
			case 0x43:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::E}}};
				break;
			case 0x44:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::H}}};
				break;
			case 0x45:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::L}}};
				break;
			case 0x46:
				return {LoadEvent8{{RegisterName::B}, {Indirect::HL}}};
				break;
			case 0x48:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::B}}};
				break;
			case 0x49:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::C}}};
				break;
			case 0x4A:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::D}}};
				break;
			case 0x4B:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::E}}};
				break;
			case 0x4C:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::H}}};
				break;
			case 0x4D:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::L}}};
				break;
			case 0x4E:
				return {LoadEvent8{{RegisterName::C}, {Indirect::HL}}};
				break;
			case 0x50:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::B}}};
				break;
			case 0x51:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::C}}};
				break;
			case 0x52:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::D}}};
				break;
			case 0x53:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::E}}};
				break;
			case 0x54:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::H}}};
				break;
			case 0x55:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::L}}};
				break;
			case 0x56:
				return {LoadEvent8{{RegisterName::D}, {Indirect::HL}}};
				break;
			case 0x58:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::B}}};
				break;
			case 0x59:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::C}}};
				break;
			case 0x5A:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::D}}};
				break;
			case 0x5B:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::E}}};
				break;
			case 0x5C:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::H}}};
				break;
			case 0x5D:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::L}}};
				break;
			case 0x5E:
				return {LoadEvent8{{RegisterName::E}, {Indirect::HL}}};
				break;
			case 0x60:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::B}}};
				break;
			case 0x61:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::C}}};
				break;
			case 0x62:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::D}}};
				break;
			case 0x63:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::E}}};
				break;
			case 0x64:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::H}}};
				break;
			case 0x65:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::L}}};
				break;
			case 0x66:
				return {LoadEvent8{{RegisterName::H}, {Indirect::HL}}};
				break;
			case 0x68:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::B}}};
				break;
			case 0x69:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::C}}};
				break;
			case 0x6A:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::D}}};
				break;
			case 0x6B:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::E}}};
				break;
			case 0x6C:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::H}}};
				break;
			case 0x6D:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::L}}};
				break;
			case 0x6E:
				return {LoadEvent8{{RegisterName::L}, {Indirect::HL}}};
				break;
			case 0x70:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::B}}};
				break;
			case 0x71:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::C}}};
				break;
			case 0x72:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::D}}};
				break;
			case 0x73:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::E}}};
				break;
			case 0x74:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::H}}};
				break;
			case 0x75:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::L}}};
				break;
			case 0x36:
				{
					Immediate8 n = in.get();
					return {LoadEvent8{{Indirect::HL}, {n}}};
					break;
				}
			case 0xFA:
				{
					unsigned char LS = in.get();
					unsigned char MS = in.get();
					Address address = (MS << 8) + LS;
					return {LoadEvent8{{RegisterName::A}, {address}}};
					break;
				}
			case 0x3E:
				return {RegisterShiftEvent{RegisterName::A, ShiftDirection::RIGHT, ShiftType::LOGICAL, 1}};
				break;
			case 0x47:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::A}}};
				break;
			case 0x4F:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::A}}};
				break;
			case 0x57:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::A}}};
				break;
			case 0x5F:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::A}}};
				break;
			case 0x67:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::A}}};
				break;
			case 0x6F:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::A}}};
				break;
			case 0x02:
				return {LoadEvent8{{Indirect::BC}, {RegisterName::A}}};
				break;
			case 0x12:
				return {LoadEvent8{{Indirect::DE}, {RegisterName::A}}};
				break;
			case 0x77:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::A}}};
				break;
			case 0xEA:
				{
					unsigned char LSB = in.get();
					unsigned char MSB = in.get();
					Address address = (MSB << 8) + LSB;
					return {LoadEvent8{{address}, {RegisterName::A}}};
					break;
				}
			case 0xF2:
				{
					// load from address at 0xFF00 + (contents of C) to A
					PartialAddress partialAddress{{0xFF}, {RegisterName::C}};
					return {LoadEvent8{{RegisterName::A}, {partialAddress}}};
					break;
				}
			case 0xE2:
				{
					// load from A to address at 0xFF00 + (contents of C)
					PartialAddress partialAddress{{0xFF}, {RegisterName::C}};
					return {LoadEvent8{{partialAddress}, {RegisterName::A}}};
					break;
				}
			case 0x3A:
				return {LoadEvent8{{RegisterName::A}, {Indirect::HLD}}};
				break;
			case 0x32:
				return {LoadEvent8{{Indirect::HLD}, {RegisterName::A}}};
				break;
			case 0x2A:
				return {LoadEvent8{{RegisterName::A}, {Indirect::HLI}}};
				break;
			case 0x22:
				return {LoadEvent8{{Indirect::HLI}, {RegisterName::A}}};
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
