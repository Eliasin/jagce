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
			case 0x0E:
				return createLoadFromImmediate8ToRegister(in, RegisterName::C);
			case 0x16:	
				return createLoadFromImmediate8ToRegister(in, RegisterName::D);
			case 0x1E:
				return createLoadFromImmediate8ToRegister(in, RegisterName::E);
			case 0x26:
				return createLoadFromImmediate8ToRegister(in, RegisterName::H);
			case 0x2E:
				return createLoadFromImmediate8ToRegister(in, RegisterName::L);
			case 0x3E:
				return createLoadFromImmediate8ToRegister(in, RegisterName::A);	
			// 8-bit register/indirect to register/indirect loads
			case 0x7F:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::A}}};
			case 0x78:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::B}}};
			case 0x79:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::C}}};
			case 0x7A:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::D}}};
			case 0x7B:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::E}}};
			case 0x7C:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::H}}};
			case 0x7D:
				return {LoadEvent8{{RegisterName::A}, {RegisterName::L}}};
			case 0x7E:
				return {LoadEvent8{{RegisterName::A}, {Indirect::HL}}};
			case 0x40:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::B}}};
			case 0x41:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::C}}};
			case 0x42:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::D}}};
			case 0x43:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::E}}};
			case 0x44:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::H}}};
			case 0x45:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::L}}};
			case 0x46:
				return {LoadEvent8{{RegisterName::B}, {Indirect::HL}}};
			case 0x48:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::B}}};
			case 0x49:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::C}}};
			case 0x4A:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::D}}};
			case 0x4B:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::E}}};
			case 0x4C:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::H}}};
			case 0x4D:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::L}}};
			case 0x4E:
				return {LoadEvent8{{RegisterName::C}, {Indirect::HL}}};
			case 0x50:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::B}}};
			case 0x51:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::C}}};
			case 0x52:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::D}}};
			case 0x53:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::E}}};
			case 0x54:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::H}}};
			case 0x55:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::L}}};
			case 0x56:
				return {LoadEvent8{{RegisterName::D}, {Indirect::HL}}};
			case 0x58:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::B}}};
			case 0x59:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::C}}};
			case 0x5A:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::D}}};
			case 0x5B:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::E}}};
			case 0x5C:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::H}}};
			case 0x5D:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::L}}};
			case 0x5E:
				return {LoadEvent8{{RegisterName::E}, {Indirect::HL}}};
			case 0x60:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::B}}};
			case 0x61:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::C}}};
			case 0x62:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::D}}};
			case 0x63:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::E}}};
			case 0x64:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::H}}};
			case 0x65:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::L}}};
			case 0x66:
				return {LoadEvent8{{RegisterName::H}, {Indirect::HL}}};
			case 0x68:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::B}}};
			case 0x69:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::C}}};
			case 0x6A:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::D}}};
			case 0x6B:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::E}}};
			case 0x6C:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::H}}};
			case 0x6D:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::L}}};
			case 0x6E:
				return {LoadEvent8{{RegisterName::L}, {Indirect::HL}}};
			case 0x70:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::B}}};
			case 0x71:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::C}}};
			case 0x72:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::D}}};
			case 0x73:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::E}}};
			case 0x74:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::H}}};
			case 0x75:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::L}}};
			case 0x47:
				return {LoadEvent8{{RegisterName::B}, {RegisterName::A}}};
			case 0x4F:
				return {LoadEvent8{{RegisterName::C}, {RegisterName::A}}};
			case 0x57:
				return {LoadEvent8{{RegisterName::D}, {RegisterName::A}}};
			case 0x5F:
				return {LoadEvent8{{RegisterName::E}, {RegisterName::A}}};
			case 0x67:
				return {LoadEvent8{{RegisterName::H}, {RegisterName::A}}};
			case 0x6F:
				return {LoadEvent8{{RegisterName::L}, {RegisterName::A}}};
			case 0x02:
				return {LoadEvent8{{Indirect::BC}, {RegisterName::A}}};
			case 0x12:
				return {LoadEvent8{{Indirect::DE}, {RegisterName::A}}};
			case 0x77:
				return {LoadEvent8{{Indirect::HL}, {RegisterName::A}}};
			case 0x3A:
				return {LoadEvent8{{RegisterName::A}, {Indirect::HLD}}};
			case 0x32:
				return {LoadEvent8{{Indirect::HLD}, {RegisterName::A}}};
			case 0x2A:
				return {LoadEvent8{{RegisterName::A}, {Indirect::HLI}}};
			case 0x22:
				return {LoadEvent8{{Indirect::HLI}, {RegisterName::A}}};
			case 0x0A:
				return {LoadEvent8{{RegisterName::A}, {Indirect::BC}}};
			case 0x1A:
				return {LoadEvent8{{RegisterName::A}, {Indirect::DE}}};
			// 8 bit immediate/indirect to indirect/immediate loads
			case 0x36:
				{
					Immediate8 n = in.get();
					return {LoadEvent8{{Indirect::HL}, {n}}};
				}
			// 8 bit loads with partial addresses
			case 0xF2:
				{
					// load from address at 0xFF00 + (contents of C) to A
					PartialAddress partialAddress{{0xFF}, {RegisterName::C}};
					return {LoadEvent8{{RegisterName::A}, {partialAddress}}};
				}
			case 0xE2:
				{
					// load from A to address at 0xFF00 + (contents of C)
					PartialAddress partialAddress{{0xFF}, {RegisterName::C}};
					return {LoadEvent8{{partialAddress}, {RegisterName::A}}};
				}
			// 8  bit address/register to register/address loads
			case 0xFA:
				{
					uint8_t lsb = in.get();
					uint8_t msb = in.get();
					Address address = (msb << (sizeof(uint8_t) * 8)) + lsb;
					return {LoadEvent8{{RegisterName::A}, {address}}};
				}
			case 0xEA:
				{
					uint8_t lsb = in.get();
					uint8_t msb = in.get();
					Address address = (msb << 8) + lsb;
					return {LoadEvent8{{address}, {RegisterName::A}}};
				}
			case 0xE0:
				{
					uint8_t lsb = in.get();
					uint8_t msb = 0xFF;
					Address address = (msb << (sizeof(uint8_t) * 8)) + lsb;
					return {LoadEvent8{{address}, {RegisterName::A}}};
				}
			case 0xF0:
				{
					uint8_t lsb = in.get();
					uint8_t msb = 0xFF;
					Address address = (msb << (sizeof(uint8_t) * 8)) + lsb;
					return {LoadEvent8{{RegisterName::A}, {address}}};
				}
			default:
				return {NopEvent{}};
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
