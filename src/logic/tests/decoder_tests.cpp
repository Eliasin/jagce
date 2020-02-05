#include <catch.hpp>

#include <map>

#include "decoder.hpp"

namespace TestConstants {

	constexpr jagce::FlagStateChange _ldhlFlags() {
		jagce::FlagStateChange f{};
		f.at(static_cast<size_t>(jagce::FlagName::S)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::Z)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::F5)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::H)) = jagce::FlagState::DEFER;
		f.at(static_cast<size_t>(jagce::FlagName::F3)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::PV)) = jagce::FlagState::UNCH;
		f.at(static_cast<size_t>(jagce::FlagName::N)) = jagce::FlagState::RESET;
		f.at(static_cast<size_t>(jagce::FlagName::C)) = jagce::FlagState::DEFER;

		return f;
	};

	constexpr jagce::FlagStateChange ldhlFlags = _ldhlFlags();

};

TEST_CASE("decoder produces correct events", "[logic], [decoder]") {
	jagce::Decoder decoder{};

	SECTION("16 bit register plus value to register loads") {
		uint8_t opcode = GENERATE(0xF8);

		uint16_t immediate = GENERATE(0x21, 0xA5, 0xF2);

		std::map<uint8_t, jagce::Event> expectedEvents{
			{ 0xF8, {jagce::LoadEvent16{ {jagce::RegisterNames::HL}, {jagce::RegisterPlusValue{jagce::RegisterNames::SP, immediate}}, {TestConstants::ldhlFlags} }}}};

		jagce::ByteStream bytes{};
		bytes.add(opcode);
		bytes.add(immediate);

		CHECK(decoder.decodeEvent(bytes) == expectedEvents.at(opcode));
	}

	SECTION(" 16 bit register to address loads") {
		uint8_t opcode = GENERATE(0x08);

		uint8_t addressLSB = GENERATE(0x97, 0xF5, 0x01);
		uint8_t addressMSB = GENERATE(0x42, 0x15, 0x51);

		uint16_t address = (addressMSB << sizeof(uint8_t) * 8) + addressLSB;

		std::map<uint8_t, jagce::Event> expectedEvents{
			{ 0x08, {jagce::LoadEvent16{ {jagce::Address{address}}, {jagce::RegisterNames::SP}, {} }}}
		};

		jagce::ByteStream bytes{};
		bytes.add(opcode);
		bytes.add(addressLSB);
		bytes.add(addressMSB);

		CHECK(decoder.decodeEvent(bytes) == expectedEvents.at(opcode));
	}

	SECTION("16 bit immediate to register loads") {
		uint8_t opcode = GENERATE(0x01);//0x11, 0x21, 0x31);

		uint8_t immediateLSB = GENERATE(0x11, 0x76, 0x12);
		uint8_t immediateMSB = GENERATE(0x66, 0x44, 0x21);
		std::array<uint8_t, 2> immediate{ immediateLSB, immediateMSB };

		jagce::Immediate16 immediate16 = (immediateMSB << (sizeof(uint8_t) * 8)) + immediateLSB;

		std::map<uint8_t, jagce::Event> expectedEvents{
			{ 0x01, {jagce::LoadEvent16{{jagce::RegisterNames::BC}, {immediate16} }} }
		};

		jagce::ByteStream bytes{};
		bytes.add(opcode);
		bytes.addBytes<2>(immediate);

		CHECK(decoder.decodeEvent(bytes) == expectedEvents.at(opcode));
	}

	SECTION("8 bit loads with partial addresses") {
		uint8_t opcode =  GENERATE(0xF2, 0xE2);

		uint8_t partialByte = GENERATE(0x00, 0xFF, 0xA2, 0x45);

		std::map<uint8_t, jagce::Event> expectedEvents{
			{ 0xF2, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::PartialAddress{{0xFF}, {jagce::RegisterNames::C}}} }} },
			{ 0xE2, {jagce::LoadEvent8{ {jagce::PartialAddress{{0xFF}, {jagce::RegisterNames::C}}}, {jagce::RegisterNames::A} }} }
		};

		jagce::ByteStream bytes{};
		bytes.add(opcode);
		bytes.add(partialByte);

		CHECK(decoder.decodeEvent(bytes) == expectedEvents.at(opcode));
	}

	SECTION("8 bit address/register to register/address load instruction decodes") {
		uint8_t opcode = GENERATE(0xFA, 0xEA, 0xE0, 0xF0);

		uint8_t addressLSB = GENERATE(0xFF, 0xAF, 0x11);
		uint8_t addressMSB = GENERATE(0x96, 0xAF, 0x13);

		jagce::Address address = (addressMSB << (sizeof(uint8_t) * 8)) + addressLSB;

		jagce::Address ffLSB = 0xFF00 + addressLSB;

		std::map<uint8_t, jagce::Event> expectedEvents {
			{ 0xFA, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {address} }} },
			{ 0xEA, {jagce::LoadEvent8{ {address}, {jagce::RegisterNames::A} }} },
			{ 0xE0, {jagce::LoadEvent8{ jagce::Address{ffLSB}, {jagce::RegisterNames::A} }} },
			{ 0xF0, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, jagce::Address{ffLSB} }} }
		};

		jagce::ByteStream bytes{};
		bytes.add(opcode);
		bytes.add(addressLSB);
		bytes.add(addressMSB);

		CHECK(decoder.decodeEvent(bytes) == expectedEvents.at(opcode));
	}

	SECTION("8 bit register/indirect to indirect/register load instruction decodes") {
		uint8_t opcode = GENERATE(0x02, 0x12, 0x22, 0x2A, 0x32, 0x3A, 0x7F, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C,
				0x7D, 0x7E, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 
				0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D,
				0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,
				0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x0A, 0x1A, 0x7E);

		uint8_t secondByte = 0x5A;
		uint8_t thirdByte = 0x44;
		jagce::Address address = (thirdByte << (sizeof(uint8_t) * 8)) + secondByte;
		
		std::map<uint8_t, jagce::Event> expectedEvents {
			{ 0x02, {jagce::LoadEvent8{ {jagce::Indirect::BC}, {jagce::RegisterNames::A} }} },
			{ 0x12, {jagce::LoadEvent8{ {jagce::Indirect::DE}, {jagce::RegisterNames::A} }} },
			{ 0x22, {jagce::LoadEvent8{ {jagce::Indirect::HLI}, {jagce::RegisterNames::A} }} },
			{ 0x2A, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::Indirect::HLI} }} },
			{ 0x32, {jagce::LoadEvent8{ {jagce::Indirect::HLD}, {jagce::RegisterNames::A} }} },
			{ 0x3A, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::Indirect::HLD} }} },
			{ 0x7F, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::RegisterNames::A} }} },
			{ 0x77, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterNames::A} }} },
			{ 0x78, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::RegisterNames::B} }} },
			{ 0x79, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::RegisterNames::C} }} },
			{ 0x7A, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::RegisterNames::D} }} },
			{ 0x7B, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::RegisterNames::E} }} },
			{ 0x7C, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::RegisterNames::H} }} },
			{ 0x7D, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::RegisterNames::L} }} },
			{ 0x7E, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::Indirect::HL} }} },
			{ 0x40, {jagce::LoadEvent8{ {jagce::RegisterNames::B}, {jagce::RegisterNames::B} }} },
			{ 0x41, {jagce::LoadEvent8{ {jagce::RegisterNames::B}, {jagce::RegisterNames::C} }} },
			{ 0x42, {jagce::LoadEvent8{ {jagce::RegisterNames::B}, {jagce::RegisterNames::D} }} },
			{ 0x43, {jagce::LoadEvent8{ {jagce::RegisterNames::B}, {jagce::RegisterNames::E} }} },
			{ 0x44, {jagce::LoadEvent8{ {jagce::RegisterNames::B}, {jagce::RegisterNames::H} }} },
			{ 0x45, {jagce::LoadEvent8{ {jagce::RegisterNames::B}, {jagce::RegisterNames::L} }} },
			{ 0x46, {jagce::LoadEvent8{ {jagce::RegisterNames::B}, {jagce::Indirect::HL} }} },
			{ 0x47, {jagce::LoadEvent8{ {jagce::RegisterNames::B}, {jagce::RegisterNames::A} }} },
			{ 0x48, {jagce::LoadEvent8{ {jagce::RegisterNames::C}, {jagce::RegisterNames::B} }} },
			{ 0x49, {jagce::LoadEvent8{ {jagce::RegisterNames::C}, {jagce::RegisterNames::C} }} },
			{ 0x4A, {jagce::LoadEvent8{ {jagce::RegisterNames::C}, {jagce::RegisterNames::D} }} },
			{ 0x4B, {jagce::LoadEvent8{ {jagce::RegisterNames::C}, {jagce::RegisterNames::E} }} },
			{ 0x4C, {jagce::LoadEvent8{ {jagce::RegisterNames::C}, {jagce::RegisterNames::H} }} },
			{ 0x4D, {jagce::LoadEvent8{ {jagce::RegisterNames::C}, {jagce::RegisterNames::L} }} },
			{ 0x4E, {jagce::LoadEvent8{ {jagce::RegisterNames::C}, {jagce::Indirect::HL} }} },
			{ 0x4F, {jagce::LoadEvent8{ {jagce::RegisterNames::C}, {jagce::RegisterNames::A} }} },
			{ 0x50, {jagce::LoadEvent8{ {jagce::RegisterNames::D}, {jagce::RegisterNames::B} }} },
			{ 0x51, {jagce::LoadEvent8{ {jagce::RegisterNames::D}, {jagce::RegisterNames::C} }} },
			{ 0x52, {jagce::LoadEvent8{ {jagce::RegisterNames::D}, {jagce::RegisterNames::D} }} },
			{ 0x53, {jagce::LoadEvent8{ {jagce::RegisterNames::D}, {jagce::RegisterNames::E} }} },
			{ 0x54, {jagce::LoadEvent8{ {jagce::RegisterNames::D}, {jagce::RegisterNames::H} }} },
			{ 0x55, {jagce::LoadEvent8{ {jagce::RegisterNames::D}, {jagce::RegisterNames::L} }} },
			{ 0x56, {jagce::LoadEvent8{ {jagce::RegisterNames::D}, {jagce::Indirect::HL} }} },
			{ 0x57, {jagce::LoadEvent8{ {jagce::RegisterNames::D}, {jagce::RegisterNames::A} }} },
			{ 0x58, {jagce::LoadEvent8{ {jagce::RegisterNames::E}, {jagce::RegisterNames::B} }} },
			{ 0x59, {jagce::LoadEvent8{ {jagce::RegisterNames::E}, {jagce::RegisterNames::C} }} },
			{ 0x5A, {jagce::LoadEvent8{ {jagce::RegisterNames::E}, {jagce::RegisterNames::D} }} },
			{ 0x5B, {jagce::LoadEvent8{ {jagce::RegisterNames::E}, {jagce::RegisterNames::E} }} },
			{ 0x5C, {jagce::LoadEvent8{ {jagce::RegisterNames::E}, {jagce::RegisterNames::H} }} },
			{ 0x5D, {jagce::LoadEvent8{ {jagce::RegisterNames::E}, {jagce::RegisterNames::L} }} },
			{ 0x5E, {jagce::LoadEvent8{ {jagce::RegisterNames::E}, {jagce::Indirect::HL} }} },
			{ 0x5F, {jagce::LoadEvent8{ {jagce::RegisterNames::E}, {jagce::RegisterNames::A} }} },
			{ 0x60, {jagce::LoadEvent8{ {jagce::RegisterNames::H}, {jagce::RegisterNames::B} }} },
			{ 0x61, {jagce::LoadEvent8{ {jagce::RegisterNames::H}, {jagce::RegisterNames::C} }} },
			{ 0x62, {jagce::LoadEvent8{ {jagce::RegisterNames::H}, {jagce::RegisterNames::D} }} },
			{ 0x63, {jagce::LoadEvent8{ {jagce::RegisterNames::H}, {jagce::RegisterNames::E} }} },
			{ 0x64, {jagce::LoadEvent8{ {jagce::RegisterNames::H}, {jagce::RegisterNames::H} }} },
			{ 0x65, {jagce::LoadEvent8{ {jagce::RegisterNames::H}, {jagce::RegisterNames::L} }} },
			{ 0x66, {jagce::LoadEvent8{ {jagce::RegisterNames::H}, {jagce::Indirect::HL} }} },
			{ 0x67, {jagce::LoadEvent8{ {jagce::RegisterNames::H}, {jagce::RegisterNames::A} }} },
			{ 0x68, {jagce::LoadEvent8{ {jagce::RegisterNames::L}, {jagce::RegisterNames::B} }} },
			{ 0x69, {jagce::LoadEvent8{ {jagce::RegisterNames::L}, {jagce::RegisterNames::C} }} },
			{ 0x6A, {jagce::LoadEvent8{ {jagce::RegisterNames::L}, {jagce::RegisterNames::D} }} },
			{ 0x6B, {jagce::LoadEvent8{ {jagce::RegisterNames::L}, {jagce::RegisterNames::E} }} },
			{ 0x6C, {jagce::LoadEvent8{ {jagce::RegisterNames::L}, {jagce::RegisterNames::H} }} },
			{ 0x6D, {jagce::LoadEvent8{ {jagce::RegisterNames::L}, {jagce::RegisterNames::L} }} },
			{ 0x6E, {jagce::LoadEvent8{ {jagce::RegisterNames::L}, {jagce::Indirect::HL} }} },
			{ 0x6F, {jagce::LoadEvent8{ {jagce::RegisterNames::L}, {jagce::RegisterNames::A} }} },
			{ 0x70, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterNames::B} }} },
			{ 0x71, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterNames::C} }} },
			{ 0x72, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterNames::D} }} },
			{ 0x73, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterNames::E} }} },
			{ 0x74, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterNames::H} }} },
			{ 0x75, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterNames::L} }} },
			{ 0x0A, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::Indirect::BC} }} },
			{ 0x1A, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::Indirect::DE} }} },
			{ 0x7E, {jagce::LoadEvent8{ {jagce::RegisterNames::A}, {jagce::Indirect::HL} }} },
		};
		
		jagce::ByteStream bytes{};
		bytes.add(opcode);
		bytes.add(secondByte);
		bytes.add(thirdByte);


		CHECK(decoder.decodeEvent(bytes) == expectedEvents.at(opcode));
	}

	SECTION("8-bit immediate to register load instruction decodes") {
		uint8_t opcode = GENERATE(0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E, 0x36);
		uint8_t immediate = GENERATE(0x00, 0xFF, 0x21, 0xAC);

		std::map<uint8_t, jagce::Event> expectedEvents {
			{ 0x06, {jagce::LoadEvent8{ {jagce::RegisterNames::B}, {jagce::Immediate8{immediate}} }} },
			{ 0x0E, {jagce::LoadEvent8{ {jagce::RegisterNames::C}, {jagce::Immediate8{immediate}} }} },
			{ 0x16, {jagce::LoadEvent8{ {jagce::RegisterNames::D}, {jagce::Immediate8{immediate}} }} },
			{ 0x1E, {jagce::LoadEvent8{ {jagce::RegisterNames::E}, {jagce::Immediate8{immediate}} }} },
			{ 0x26, {jagce::LoadEvent8{ {jagce::RegisterNames::H}, {jagce::Immediate8{immediate}} }} },
			{ 0x2E, {jagce::LoadEvent8{ {jagce::RegisterNames::L}, {jagce::Immediate8{immediate}} }} },
			{ 0x36, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::Immediate8{immediate}} }} }
		};

		jagce::ByteStream bytes{};
		bytes.add(opcode);
		bytes.add(immediate);

		CHECK(decoder.decodeEvent(bytes) == expectedEvents.at(opcode));
	}
}
