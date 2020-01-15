#include <catch.hpp>

#include <map>

#include "decoder.hpp"

TEST_CASE("decoder produces correct events", "[logic], [decoder]") {
	jagce::Decoder decoder{};

	SECTION("8 bit loads with partial addresses") {
		uint8_t opcode =  GENERATE(0xF2, 0xE2);

		uint8_t partialByte = GENERATE(0x00, 0xFF, 0xA2, 0x45);

		std::map<uint8_t, jagce::Event> expectedEvents{
			{ 0xF2, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::PartialAddress{{0xFF}, {jagce::RegisterName::C}}} }} },
			{ 0xE2, {jagce::LoadEvent8{ {jagce::PartialAddress{{0xFF}, {jagce::RegisterName::C}}}, {jagce::RegisterName::A} }} }
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
			{ 0xFA, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {address} }} },
			{ 0xEA, {jagce::LoadEvent8{ {address}, {jagce::RegisterName::A} }} },
			{ 0xE0, {jagce::LoadEvent8{ jagce::Address{ffLSB}, {jagce::RegisterName::A} }} },
			{ 0xF0, {jagce::LoadEvent8{ {jagce::RegisterName::A}, jagce::Address{ffLSB} }} }
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
			{ 0x02, {jagce::LoadEvent8{ {jagce::Indirect::BC}, {jagce::RegisterName::A} }} },
			{ 0x12, {jagce::LoadEvent8{ {jagce::Indirect::DE}, {jagce::RegisterName::A} }} },
			{ 0x22, {jagce::LoadEvent8{ {jagce::Indirect::HLI}, {jagce::RegisterName::A} }} },
			{ 0x2A, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::Indirect::HLI} }} },
			{ 0x32, {jagce::LoadEvent8{ {jagce::Indirect::HLD}, {jagce::RegisterName::A} }} },
			{ 0x3A, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::Indirect::HLD} }} },
			{ 0x7F, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::A} }} },
			{ 0x77, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterName::A} }} },
			{ 0x78, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::B} }} },
			{ 0x79, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::C} }} },
			{ 0x7A, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::D} }} },
			{ 0x7B, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::E} }} },
			{ 0x7C, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::H} }} },
			{ 0x7D, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::L} }} },
			{ 0x7E, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::Indirect::HL} }} },
			{ 0x40, {jagce::LoadEvent8{ {jagce::RegisterName::B}, {jagce::RegisterName::B} }} },
			{ 0x41, {jagce::LoadEvent8{ {jagce::RegisterName::B}, {jagce::RegisterName::C} }} },
			{ 0x42, {jagce::LoadEvent8{ {jagce::RegisterName::B}, {jagce::RegisterName::D} }} },
			{ 0x43, {jagce::LoadEvent8{ {jagce::RegisterName::B}, {jagce::RegisterName::E} }} },
			{ 0x44, {jagce::LoadEvent8{ {jagce::RegisterName::B}, {jagce::RegisterName::H} }} },
			{ 0x45, {jagce::LoadEvent8{ {jagce::RegisterName::B}, {jagce::RegisterName::L} }} },
			{ 0x46, {jagce::LoadEvent8{ {jagce::RegisterName::B}, {jagce::Indirect::HL} }} },
			{ 0x47, {jagce::LoadEvent8{ {jagce::RegisterName::B}, {jagce::RegisterName::A} }} },
			{ 0x48, {jagce::LoadEvent8{ {jagce::RegisterName::C}, {jagce::RegisterName::B} }} },
			{ 0x49, {jagce::LoadEvent8{ {jagce::RegisterName::C}, {jagce::RegisterName::C} }} },
			{ 0x4A, {jagce::LoadEvent8{ {jagce::RegisterName::C}, {jagce::RegisterName::D} }} },
			{ 0x4B, {jagce::LoadEvent8{ {jagce::RegisterName::C}, {jagce::RegisterName::E} }} },
			{ 0x4C, {jagce::LoadEvent8{ {jagce::RegisterName::C}, {jagce::RegisterName::H} }} },
			{ 0x4D, {jagce::LoadEvent8{ {jagce::RegisterName::C}, {jagce::RegisterName::L} }} },
			{ 0x4E, {jagce::LoadEvent8{ {jagce::RegisterName::C}, {jagce::Indirect::HL} }} },
			{ 0x4F, {jagce::LoadEvent8{ {jagce::RegisterName::C}, {jagce::RegisterName::A} }} },
			{ 0x50, {jagce::LoadEvent8{ {jagce::RegisterName::D}, {jagce::RegisterName::B} }} },
			{ 0x51, {jagce::LoadEvent8{ {jagce::RegisterName::D}, {jagce::RegisterName::C} }} },
			{ 0x52, {jagce::LoadEvent8{ {jagce::RegisterName::D}, {jagce::RegisterName::D} }} },
			{ 0x53, {jagce::LoadEvent8{ {jagce::RegisterName::D}, {jagce::RegisterName::E} }} },
			{ 0x54, {jagce::LoadEvent8{ {jagce::RegisterName::D}, {jagce::RegisterName::H} }} },
			{ 0x55, {jagce::LoadEvent8{ {jagce::RegisterName::D}, {jagce::RegisterName::L} }} },
			{ 0x56, {jagce::LoadEvent8{ {jagce::RegisterName::D}, {jagce::Indirect::HL} }} },
			{ 0x57, {jagce::LoadEvent8{ {jagce::RegisterName::D}, {jagce::RegisterName::A} }} },
			{ 0x58, {jagce::LoadEvent8{ {jagce::RegisterName::E}, {jagce::RegisterName::B} }} },
			{ 0x59, {jagce::LoadEvent8{ {jagce::RegisterName::E}, {jagce::RegisterName::C} }} },
			{ 0x5A, {jagce::LoadEvent8{ {jagce::RegisterName::E}, {jagce::RegisterName::D} }} },
			{ 0x5B, {jagce::LoadEvent8{ {jagce::RegisterName::E}, {jagce::RegisterName::E} }} },
			{ 0x5C, {jagce::LoadEvent8{ {jagce::RegisterName::E}, {jagce::RegisterName::H} }} },
			{ 0x5D, {jagce::LoadEvent8{ {jagce::RegisterName::E}, {jagce::RegisterName::L} }} },
			{ 0x5E, {jagce::LoadEvent8{ {jagce::RegisterName::E}, {jagce::Indirect::HL} }} },
			{ 0x5F, {jagce::LoadEvent8{ {jagce::RegisterName::E}, {jagce::RegisterName::A} }} },
			{ 0x60, {jagce::LoadEvent8{ {jagce::RegisterName::H}, {jagce::RegisterName::B} }} },
			{ 0x61, {jagce::LoadEvent8{ {jagce::RegisterName::H}, {jagce::RegisterName::C} }} },
			{ 0x62, {jagce::LoadEvent8{ {jagce::RegisterName::H}, {jagce::RegisterName::D} }} },
			{ 0x63, {jagce::LoadEvent8{ {jagce::RegisterName::H}, {jagce::RegisterName::E} }} },
			{ 0x64, {jagce::LoadEvent8{ {jagce::RegisterName::H}, {jagce::RegisterName::H} }} },
			{ 0x65, {jagce::LoadEvent8{ {jagce::RegisterName::H}, {jagce::RegisterName::L} }} },
			{ 0x66, {jagce::LoadEvent8{ {jagce::RegisterName::H}, {jagce::Indirect::HL} }} },
			{ 0x67, {jagce::LoadEvent8{ {jagce::RegisterName::H}, {jagce::RegisterName::A} }} },
			{ 0x68, {jagce::LoadEvent8{ {jagce::RegisterName::L}, {jagce::RegisterName::B} }} },
			{ 0x69, {jagce::LoadEvent8{ {jagce::RegisterName::L}, {jagce::RegisterName::C} }} },
			{ 0x6A, {jagce::LoadEvent8{ {jagce::RegisterName::L}, {jagce::RegisterName::D} }} },
			{ 0x6B, {jagce::LoadEvent8{ {jagce::RegisterName::L}, {jagce::RegisterName::E} }} },
			{ 0x6C, {jagce::LoadEvent8{ {jagce::RegisterName::L}, {jagce::RegisterName::H} }} },
			{ 0x6D, {jagce::LoadEvent8{ {jagce::RegisterName::L}, {jagce::RegisterName::L} }} },
			{ 0x6E, {jagce::LoadEvent8{ {jagce::RegisterName::L}, {jagce::Indirect::HL} }} },
			{ 0x6F, {jagce::LoadEvent8{ {jagce::RegisterName::L}, {jagce::RegisterName::A} }} },
			{ 0x70, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterName::B} }} },
			{ 0x71, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterName::C} }} },
			{ 0x72, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterName::D} }} },
			{ 0x73, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterName::E} }} },
			{ 0x74, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterName::H} }} },
			{ 0x75, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::RegisterName::L} }} },
			{ 0x0A, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::Indirect::BC} }} },
			{ 0x1A, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::Indirect::DE} }} },
			{ 0x7E, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::Indirect::HL} }} },
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
			{ 0x06, {jagce::LoadEvent8{ {jagce::RegisterName::B}, {jagce::Immediate8{immediate}} }} },
			{ 0x0E, {jagce::LoadEvent8{ {jagce::RegisterName::C}, {jagce::Immediate8{immediate}} }} },
			{ 0x16, {jagce::LoadEvent8{ {jagce::RegisterName::D}, {jagce::Immediate8{immediate}} }} },
			{ 0x1E, {jagce::LoadEvent8{ {jagce::RegisterName::E}, {jagce::Immediate8{immediate}} }} },
			{ 0x26, {jagce::LoadEvent8{ {jagce::RegisterName::H}, {jagce::Immediate8{immediate}} }} },
			{ 0x2E, {jagce::LoadEvent8{ {jagce::RegisterName::L}, {jagce::Immediate8{immediate}} }} },
			{ 0x36, {jagce::LoadEvent8{ {jagce::Indirect::HL}, {jagce::Immediate8{immediate}} }} }
		};

		jagce::ByteStream bytes{};
		bytes.add(opcode);
		bytes.add(immediate);

		CHECK(decoder.decodeEvent(bytes) == expectedEvents.at(opcode));
	}
}
