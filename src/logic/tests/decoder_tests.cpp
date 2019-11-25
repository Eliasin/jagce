#include <catch.hpp>

#include <map>

#include "decoder.hpp"

TEST_CASE("decoder produces correct events", "[logic], [decoder]") {
	jagce::Decoder decoder{};

	SECTION("register/immediate to register/immediate load instruction decodes") {
		uint8_t opcode = GENERATE(0x7F, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E);
		
		std::map<uint8_t, jagce::Event> expectedEvents {
			{ 0x7F, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::A} }} },
			{ 0x78, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::B} }} },
			{ 0x79, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::C} }} },
			{ 0x7A, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::D} }} },
			{ 0x7B, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::E} }} },
			{ 0x7C, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::H} }} },
			{ 0x7D, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::RegisterName::L} }} },
			{ 0x7E, {jagce::LoadEvent8{ {jagce::RegisterName::A}, {jagce::Indirect::HL} }} }
		};
		
		jagce::ByteStream bytes{};
		bytes.add(opcode);

		CHECK(decoder.decodeEvent(bytes) == expectedEvents.at(opcode));
	}
	SECTION("8-bit immediate to register load instruction decodes") {
		uint8_t opcode = GENERATE(0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E);
		uint8_t immediate = GENERATE(0x00, 0xFF, 0x21, 0xAC);

		std::map<uint8_t, jagce::Event> expectedEvents {
			{ 0x06, {jagce::LoadEvent8{ {jagce::RegisterName::B}, {jagce::Immediate8{immediate}} }} },
			{ 0x0E, {jagce::LoadEvent8{ {jagce::RegisterName::C}, {jagce::Immediate8{immediate}} }} },
			{ 0x16, {jagce::LoadEvent8{ {jagce::RegisterName::D}, {jagce::Immediate8{immediate}} }} },
			{ 0x1E, {jagce::LoadEvent8{ {jagce::RegisterName::E}, {jagce::Immediate8{immediate}} }} },
			{ 0x26, {jagce::LoadEvent8{ {jagce::RegisterName::H}, {jagce::Immediate8{immediate}} }} },
			{ 0x2E, {jagce::LoadEvent8{ {jagce::RegisterName::L}, {jagce::Immediate8{immediate}} }} }
		};

		jagce::ByteStream bytes{};
		bytes.add(opcode);
		bytes.add(immediate);

		CHECK(decoder.decodeEvent(bytes) == expectedEvents.at(opcode));
	}
}
