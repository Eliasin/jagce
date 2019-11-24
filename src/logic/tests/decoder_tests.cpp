#include <catch.hpp>

#include <map>

#include "decoder.hpp"

TEST_CASE("decoder produces correct events", "[logic], [decoder]") {
	jagce::Decoder decoder{};

	SECTION("single instruction decodes") {
		auto opcode = GENERATE(0x7F, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E);
		
		std::map<uint8_t, jagce::Event> expectedEvents{
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

		CHECK(decoder.decodeEvent(bytes) == expectedEvents[opcode]);
	}
}
