#include "decoder.hpp"

#include <optional>

namespace jagce {

	constexpr FlagStateChange _defaultFlagStateChange() {
		FlagStateChange f{};
		f.at(static_cast<size_t>(jagce::FlagName::S)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::Z)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::F5)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::H)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::F3)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::PV)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::N)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::C)) = jagce::FlagState::UNCH;

		return f;
	};

	constexpr FlagStateChange defaultFlagStateChange = _defaultFlagStateChange();

	Immediate16 getImmediate16FromByteStream(ByteStream& in) {
		uint8_t lsb = in.get();
		uint8_t msb = in.get();
		return (msb << (sizeof(uint8_t) * 8)) + lsb;
	};

	Address getAddressFromByteStream(ByteStream& in) {
		return static_cast<Address>(getImmediate16FromByteStream(in));
	};

	constexpr Event createAddCarry8EventFromRegister(const RegisterName8& r) {
		return {AddEvent8{RegisterNames::A, Register8PlusFlag{r, FlagName::C}}};
	}

	constexpr Event createAddCarry8EventFromIndirect(const Indirect& i) {
		return {AddEvent8{RegisterNames::A, IndirectPlusFlag{i, FlagName::C}}};
	}

	constexpr Event createSubCarry8EventFromRegister(const RegisterName8& r) {
		return {SubEvent8{Register8PlusFlag{r, FlagName::C}}};
	}

	constexpr Event createSubCarry8EventFromIndirect(const Indirect& i) {
		return {SubEvent8{IndirectPlusFlag{i, FlagName::C}}};
	}

	Event createLoadFromImmediate16ToRegister(ByteStream& in, RegisterName16 r) {
		return {LoadEvent16{{r}, {Immediate16{getImmediate16FromByteStream(in)}}}};
	}

	Event createLoadFromImmediate8ToRegister(ByteStream& in, RegisterName r) {
		const Immediate8 immediate8 = static_cast<Immediate8>(in.get());
		return {LoadEvent8{{r}, {Immediate8{immediate8}}}};
	}

	constexpr Event createAndEventFromRegister(const RegisterName8& r) {
		return {AndEvent8{r}};
	}

    constexpr Event createAndEventFromImmediate(const Immediate8& i) {
        return {AndEvent8{i}};
    }

	constexpr Event createXorEvent8FromRegister(const RegisterName8& r) {
		return {XorEvent8{r}};
	}

	constexpr Event createXorEvent8FromIndirect(const Indirect& i) {
		return {XorEvent8{i}};
	}

	Event createXorEvent8FromImmediate(ByteStream& in) {
		const Immediate8 immediate8 = static_cast<Immediate8>(in.get());
		return {XorEvent8{immediate8}};
	}

    Event createAnd8EventFromImmediate(ByteStream& in) {
        const Immediate8 immediate = static_cast<Immediate8>(in.get());
        return {AndEvent8{immediate}};
    }

	constexpr Event createOr8EventFromRegister(const RegisterName8& r) {
		return {OrEvent8{r}};
	}

	constexpr Event createOr8EventFromIndirect(const Indirect& i) {
		return {OrEvent8{i}};
	}

	Event createOr8EventFromImmediate(ByteStream& in) {
		const Immediate8 immediate8 = static_cast<Immediate8>(in.get());
		return {OrEvent8{immediate8}};
	}

	constexpr Event createCompareEvent8FromRegister(const RegisterName8& r) {
		return {CompareEvent8{r}};
	}

	constexpr Event createCompareEvent8FromIndirect(const Indirect& i) {
		return {CompareEvent8{i}};
	}

	Event createCompareEvent8FromImmediate(ByteStream& in) {
		const Immediate8 immediate8 = static_cast<Immediate8>(in.get());
		return {CompareEvent8{immediate8}};
	}

	Event createIncrementEvent8FromRegister(const RegisterName8& r) {
		return {IncrementEvent8{r}};
	}

	Event createIncrementEvent8FromIndirect(const Indirect& i) {
		return {IncrementEvent8{i}};
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
				return createLoadFromImmediate8ToRegister(in, RegisterNames::B);
			case 0x0E:
				return createLoadFromImmediate8ToRegister(in, RegisterNames::C);
			case 0x16:	
				return createLoadFromImmediate8ToRegister(in, RegisterNames::D);
			case 0x1E:
				return createLoadFromImmediate8ToRegister(in, RegisterNames::E);
			case 0x26:
				return createLoadFromImmediate8ToRegister(in, RegisterNames::H);
			case 0x2E:
				return createLoadFromImmediate8ToRegister(in, RegisterNames::L);
			case 0x3E:
				return createLoadFromImmediate8ToRegister(in, RegisterNames::A);	
			// 8-bit register/indirect to register/indirect loads
			case 0x7F:
				return {LoadEvent8{{RegisterNames::A}, {RegisterNames::A}}};
			case 0x78:
				return {LoadEvent8{{RegisterNames::A}, {RegisterNames::B}}};
			case 0x79:
				return {LoadEvent8{{RegisterNames::A}, {RegisterNames::C}}};
			case 0x7A:
				return {LoadEvent8{{RegisterNames::A}, {RegisterNames::D}}};
			case 0x7B:
				return {LoadEvent8{{RegisterNames::A}, {RegisterNames::E}}};
			case 0x7C:
				return {LoadEvent8{{RegisterNames::A}, {RegisterNames::H}}};
			case 0x7D:
				return {LoadEvent8{{RegisterNames::A}, {RegisterNames::L}}};
			case 0x7E:
				return {LoadEvent8{{RegisterNames::A}, {Indirect::HL}}};
			case 0x40:
				return {LoadEvent8{{RegisterNames::B}, {RegisterNames::B}}};
			case 0x41:
				return {LoadEvent8{{RegisterNames::B}, {RegisterNames::C}}};
			case 0x42:
				return {LoadEvent8{{RegisterNames::B}, {RegisterNames::D}}};
			case 0x43:
				return {LoadEvent8{{RegisterNames::B}, {RegisterNames::E}}};
			case 0x44:
				return {LoadEvent8{{RegisterNames::B}, {RegisterNames::H}}};
			case 0x45:
				return {LoadEvent8{{RegisterNames::B}, {RegisterNames::L}}};
			case 0x46:
				return {LoadEvent8{{RegisterNames::B}, {Indirect::HL}}};
			case 0x48:
				return {LoadEvent8{{RegisterNames::C}, {RegisterNames::B}}};
			case 0x49:
				return {LoadEvent8{{RegisterNames::C}, {RegisterNames::C}}};
			case 0x4A:
				return {LoadEvent8{{RegisterNames::C}, {RegisterNames::D}}};
			case 0x4B:
				return {LoadEvent8{{RegisterNames::C}, {RegisterNames::E}}};
			case 0x4C:
				return {LoadEvent8{{RegisterNames::C}, {RegisterNames::H}}};
			case 0x4D:
				return {LoadEvent8{{RegisterNames::C}, {RegisterNames::L}}};
			case 0x4E:
				return {LoadEvent8{{RegisterNames::C}, {Indirect::HL}}};
			case 0x50:
				return {LoadEvent8{{RegisterNames::D}, {RegisterNames::B}}};
			case 0x51:
				return {LoadEvent8{{RegisterNames::D}, {RegisterNames::C}}};
			case 0x52:
				return {LoadEvent8{{RegisterNames::D}, {RegisterNames::D}}};
			case 0x53:
				return {LoadEvent8{{RegisterNames::D}, {RegisterNames::E}}};
			case 0x54:
				return {LoadEvent8{{RegisterNames::D}, {RegisterNames::H}}};
			case 0x55:
				return {LoadEvent8{{RegisterNames::D}, {RegisterNames::L}}};
			case 0x56:
				return {LoadEvent8{{RegisterNames::D}, {Indirect::HL}}};
			case 0x58:
				return {LoadEvent8{{RegisterNames::E}, {RegisterNames::B}}};
			case 0x59:
				return {LoadEvent8{{RegisterNames::E}, {RegisterNames::C}}};
			case 0x5A:
				return {LoadEvent8{{RegisterNames::E}, {RegisterNames::D}}};
			case 0x5B:
				return {LoadEvent8{{RegisterNames::E}, {RegisterNames::E}}};
			case 0x5C:
				return {LoadEvent8{{RegisterNames::E}, {RegisterNames::H}}};
			case 0x5D:
				return {LoadEvent8{{RegisterNames::E}, {RegisterNames::L}}};
			case 0x5E:
				return {LoadEvent8{{RegisterNames::E}, {Indirect::HL}}};
			case 0x60:
				return {LoadEvent8{{RegisterNames::H}, {RegisterNames::B}}};
			case 0x61:
				return {LoadEvent8{{RegisterNames::H}, {RegisterNames::C}}};
			case 0x62:
				return {LoadEvent8{{RegisterNames::H}, {RegisterNames::D}}};
			case 0x63:
				return {LoadEvent8{{RegisterNames::H}, {RegisterNames::E}}};
			case 0x64:
				return {LoadEvent8{{RegisterNames::H}, {RegisterNames::H}}};
			case 0x65:
				return {LoadEvent8{{RegisterNames::H}, {RegisterNames::L}}};
			case 0x66:
				return {LoadEvent8{{RegisterNames::H}, {Indirect::HL}}};
			case 0x68:
				return {LoadEvent8{{RegisterNames::L}, {RegisterNames::B}}};
			case 0x69:
				return {LoadEvent8{{RegisterNames::L}, {RegisterNames::C}}};
			case 0x6A:
				return {LoadEvent8{{RegisterNames::L}, {RegisterNames::D}}};
			case 0x6B:
				return {LoadEvent8{{RegisterNames::L}, {RegisterNames::E}}};
			case 0x6C:
				return {LoadEvent8{{RegisterNames::L}, {RegisterNames::H}}};
			case 0x6D:
				return {LoadEvent8{{RegisterNames::L}, {RegisterNames::L}}};
			case 0x6E:
				return {LoadEvent8{{RegisterNames::L}, {Indirect::HL}}};
			case 0x70:
				return {LoadEvent8{{Indirect::HL}, {RegisterNames::B}}};
			case 0x71:
				return {LoadEvent8{{Indirect::HL}, {RegisterNames::C}}};
			case 0x72:
				return {LoadEvent8{{Indirect::HL}, {RegisterNames::D}}};
			case 0x73:
				return {LoadEvent8{{Indirect::HL}, {RegisterNames::E}}};
			case 0x74:
				return {LoadEvent8{{Indirect::HL}, {RegisterNames::H}}};
			case 0x75:
				return {LoadEvent8{{Indirect::HL}, {RegisterNames::L}}};
			case 0x47:
				return {LoadEvent8{{RegisterNames::B}, {RegisterNames::A}}};
			case 0x4F:
				return {LoadEvent8{{RegisterNames::C}, {RegisterNames::A}}};
			case 0x57:
				return {LoadEvent8{{RegisterNames::D}, {RegisterNames::A}}};
			case 0x5F:
				return {LoadEvent8{{RegisterNames::E}, {RegisterNames::A}}};
			case 0x67:
				return {LoadEvent8{{RegisterNames::H}, {RegisterNames::A}}};
			case 0x6F:
				return {LoadEvent8{{RegisterNames::L}, {RegisterNames::A}}};
			case 0x02:
				return {LoadEvent8{{Indirect::BC}, {RegisterNames::A}}};
			case 0x12:
				return {LoadEvent8{{Indirect::DE}, {RegisterNames::A}}};
			case 0x77:
				return {LoadEvent8{{Indirect::HL}, {RegisterNames::A}}};
			case 0x3A:
				return {LoadEvent8{{RegisterNames::A}, {Indirect::HLD}}};
			case 0x32:
				return {LoadEvent8{{Indirect::HLD}, {RegisterNames::A}}};
			case 0x2A:
				return {LoadEvent8{{RegisterNames::A}, {Indirect::HLI}}};
			case 0x22:
				return {LoadEvent8{{Indirect::HLI}, {RegisterNames::A}}};
			case 0x0A:
				return {LoadEvent8{{RegisterNames::A}, {Indirect::BC}}};
			case 0x1A:
				return {LoadEvent8{{RegisterNames::A}, {Indirect::DE}}};
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
					PartialAddress partialAddress{{0xFF}, {RegisterNames::C}};
					return {LoadEvent8{{RegisterNames::A}, {partialAddress}}};
				}
			case 0xE2:
				{
					// load from A to address at 0xFF00 + (contents of C)
					PartialAddress partialAddress{{0xFF}, {RegisterNames::C}};
					return {LoadEvent8{{partialAddress}, {RegisterNames::A}}};
				}
			// 8  bit address/register to register/address loads
			case 0xFA:
				{
					return {LoadEvent8{{RegisterNames::A}, {getAddressFromByteStream(in)}}};
				}
			case 0xEA:
				{
					return {LoadEvent8{{getAddressFromByteStream(in)}, {RegisterNames::A}}};
				}
			case 0xE0:
				{
					uint8_t lsb = in.get();
					uint8_t msb = 0xFF;
					Address address = (msb << (sizeof(uint8_t) * 8)) + lsb;
					return {LoadEvent8{{address}, {RegisterNames::A}}};
				}
			case 0xF0:
				{
					uint8_t lsb = in.get();
					uint8_t msb = 0xFF;
					Address address = (msb << (sizeof(uint8_t) * 8)) + lsb;
					return {LoadEvent8{{RegisterNames::A}, {address}}};
				}
			// 16-bit immediate to register loads
			case 0x01:
				return createLoadFromImmediate16ToRegister(in, RegisterNames::BC);
			case 0x11:
				return createLoadFromImmediate16ToRegister(in, RegisterNames::DE);
			case 0x21:
				return createLoadFromImmediate16ToRegister(in, RegisterNames::HL);
			case 0x31:
				return createLoadFromImmediate16ToRegister(in, RegisterNames::SP);
			// 16-bit register to register loads
			case 0xF9:
				return LoadEvent16{{RegisterNames::SP}, {RegisterNames::HL}, defaultFlagStateChange};
			// 16-bit Register16PlusValue to register loads
			case 0xF8:
				{
					Immediate16 val = static_cast<Immediate8>(in.get());
					FlagStateChange flagStateChange{};

					flagStateChange.at(static_cast<size_t>(FlagName::Z)) = FlagState::RESET;
					flagStateChange.at(static_cast<size_t>(FlagName::N)) = FlagState::RESET;
					flagStateChange.at(static_cast<size_t>(FlagName::H)) = FlagState::DEFER;
					flagStateChange.at(static_cast<size_t>(FlagName::C)) = FlagState::DEFER;
					
					LoadEvent16 loadEvent{{RegisterNames::HL},
						{Register16PlusValue{RegisterNames::SP, val}}, flagStateChange};

					return loadEvent;
				}
			// 16-bit register to address loads
			case 0x08:
				{
					return LoadEvent16{ Address{getAddressFromByteStream(in)}, {RegisterNames::SP}, defaultFlagStateChange };
				}
			// 16-bit register to stack pushes
			case 0xF5:
				return PushEvent{RegisterNames::AF};
			case 0xC5:
				return PushEvent{RegisterNames::BC};
			case 0xD5:
				return PushEvent{RegisterNames::DE};
			case 0xE5:
				return PushEvent{RegisterNames::HL};
			// 16-bit stack to register pops
			case 0xF1:
				return PopEvent{RegisterNames::AF};
			case 0xC1:
				return PopEvent{RegisterNames::BC};
			case 0xD1:
				return PopEvent{RegisterNames::DE};
			case 0xE1:
				return PopEvent{RegisterNames::HL};
			// 8-bit add operations
			case 0x87:
				return AddEvent8{RegisterNames::A, RegisterNames::A};
			case 0x80:
				return AddEvent8{RegisterNames::A, RegisterNames::B};
			case 0x81:
				return AddEvent8{RegisterNames::A, RegisterNames::C};
			case 0x82:
				return AddEvent8{RegisterNames::A, RegisterNames::D};
			case 0x83:
				return AddEvent8{RegisterNames::A, RegisterNames::E};
			case 0x84:
				return AddEvent8{RegisterNames::A, RegisterNames::H};
			case 0x85:
				return AddEvent8{RegisterNames::A, RegisterNames::L};
			case 0x86:
				return AddEvent8{RegisterNames::A, Indirect::HL};
			case 0xC6:
				{
					Immediate8 n = in.get();
					return AddEvent8{RegisterNames::A, Immediate8{n}};
				}
			// 8-bit add carry operations
			case 0x8F:
				return createAddCarry8EventFromRegister(RegisterNames::A);
			case 0x88:
				return createAddCarry8EventFromRegister(RegisterNames::B);
			case 0x89:
				return createAddCarry8EventFromRegister(RegisterNames::C);
			case 0x8A:
				return createAddCarry8EventFromRegister(RegisterNames::D);
			case 0x8B:
				return createAddCarry8EventFromRegister(RegisterNames::E);
			case 0x8C:
				return createAddCarry8EventFromRegister(RegisterNames::H);
			case 0x8D:
				return createAddCarry8EventFromRegister(RegisterNames::L);
			case 0x8E:
				return createAddCarry8EventFromIndirect(Indirect::HL);
			case 0xCE:
				{
					Immediate8 n = in.get();
					return AddEvent8{RegisterNames::A, Immediate8PlusFlag{n, FlagName::C}};
				}
			// 8-bit subtraction operations
			case 0x97:
				return SubEvent8{RegisterNames::A};
			case 0x90:
				return SubEvent8{RegisterNames::B};
			case 0x91:
				return SubEvent8{RegisterNames::C};
			case 0x92:
				return SubEvent8{RegisterNames::D};
			case 0x93:
				return SubEvent8{RegisterNames::E};
			case 0x94:
				return SubEvent8{RegisterNames::H};
			case 0x95:
				return SubEvent8{RegisterNames::L};
			case 0x96:
				return SubEvent8{Indirect::HL};
			case 0xD6:
				{
					Immediate8 n = in.get();
					return SubEvent8{n};
				}
			// 8-bit sub carry operations
			case 0x9F:
				return createSubCarry8EventFromRegister(RegisterNames::A);
			case 0x98:
				return createSubCarry8EventFromRegister(RegisterNames::B);
			case 0x99:
				return createSubCarry8EventFromRegister(RegisterNames::C);
			case 0x9A:
				return createSubCarry8EventFromRegister(RegisterNames::D);
			case 0x9B:
				return createSubCarry8EventFromRegister(RegisterNames::E);
			case 0x9C:
				return createSubCarry8EventFromRegister(RegisterNames::H);
			case 0x9D:
				return createSubCarry8EventFromRegister(RegisterNames::L);
			case 0x9E:
				return createSubCarry8EventFromIndirect(Indirect::HL);
			case 0xDE:
				{
					Immediate8 n = in.get();
					return SubEvent8{n};
				}
			// 8-bit and operations
			case 0xA7:
				return createAndEventFromRegister(RegisterNames::A);
            case 0xA0:
                return createAndEventFromRegister(RegisterNames::B);
            case 0xA1:
                return createAndEventFromRegister(RegisterNames::C);
            case 0xA2:
                return createAndEventFromRegister(RegisterNames::D);
            case 0xA3:
                return createAndEventFromRegister(RegisterNames::E);
            case 0xA4:
                return createAndEventFromRegister(RegisterNames::H);
            case 0xA5:
                return createAndEventFromRegister(RegisterNames::L);
			case 0xA6:
				return createAddCarry8EventFromIndirect(Indirect::HL);
			case 0xE6:
				return createAnd8EventFromImmediate(in);
			// 8-bit or operations
			case 0xB7:
				return createOr8EventFromRegister(RegisterNames::A);
			case 0xB0:
				return createOr8EventFromRegister(RegisterNames::B);
			case 0xB1:
				return createOr8EventFromRegister(RegisterNames::C);
			case 0xB2:
				return createOr8EventFromRegister(RegisterNames::D);
			case 0xB3:
				return createOr8EventFromRegister(RegisterNames::E);
			case 0xB4:
				return createOr8EventFromRegister(RegisterNames::H);
			case 0xB5:
				return createOr8EventFromRegister(RegisterNames::L);
			case 0xB6:
				return createOr8EventFromIndirect(Indirect::HL);
			case 0xF6:
				return createOr8EventFromImmediate(in);
			case 0xAF:
				return createXorEvent8FromRegister(RegisterNames::A);
			case 0xA8:
				return createXorEvent8FromRegister(RegisterNames::B);
			case 0xA9:
				return createXorEvent8FromRegister(RegisterNames::C);
			case 0xAA:
				return createXorEvent8FromRegister(RegisterNames::D);
			case 0xAB:
				return createXorEvent8FromRegister(RegisterNames::E);
			case 0xAC:
				return createXorEvent8FromRegister(RegisterNames::H);
			case 0xAD:
				return createXorEvent8FromRegister(RegisterNames::L);
			case 0xAE:
				return createXorEvent8FromIndirect(Indirect::HL);
			case 0xEE:
				return createXorEvent8FromImmediate(in);
			case 0xBF:
				return createCompareEvent8FromRegister(RegisterNames::A);
			case 0xB8:
				return createCompareEvent8FromRegister(RegisterNames::B);
			case 0xB9:
				return createCompareEvent8FromRegister(RegisterNames::C);
			case 0xBA:
				return createCompareEvent8FromRegister(RegisterNames::D);
			case 0xBB:
				return createCompareEvent8FromRegister(RegisterNames::E);
			case 0xBC:
				return createCompareEvent8FromRegister(RegisterNames::H);
			case 0xBD:
				return createCompareEvent8FromRegister(RegisterNames::L);
			case 0xBE:
				return createCompareEvent8FromIndirect(Indirect::HL);
			case 0xFE:
				return createCompareEvent8FromImmediate(in);
			case 0x3C:
				return createIncrementEvent8FromRegister(RegisterNames::A);
			case 0x04:
				return createIncrementEvent8FromRegister(RegisterNames::B);
			case 0x0C:
				return createIncrementEvent8FromRegister(RegisterNames::C);
			case 0x14:
				return createIncrementEvent8FromRegister(RegisterNames::D);
			case 0x1C:
				return createIncrementEvent8FromRegister(RegisterNames::E);
			case 0x24:
				return createIncrementEvent8FromRegister(RegisterNames::H);
			case 0x2C:
				return createIncrementEvent8FromRegister(RegisterNames::L);
			case 0x34:
				return createIncrementEvent8FromIndirect(Indirect::HL);
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
