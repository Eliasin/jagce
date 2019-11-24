#include <catch.hpp>

#include <map>

#include "decoder.hpp"

TEST_CASE("decoder produces correct events", "[logic], [decoder]") {
	jagce::Decoder decoder{};

	SECTION("single instruction decodes") {
		auto opcode = GENERATE(0x7F, 0x78);
		
		std::map<uint8_t, jagce::Event> expectedEvents{
			{ 0x7F, {jagce::LoadEvent{ {jagce::RegisterName::A}, {jagce::RegisterName::A} }} },
			{ 0x78, {jagce::LoadEvent{ {jagce::RegisterName::A}, {jagce::RegisterName::B} }} }
		};
		
		jagce::ByteStream bytes{};
		bytes.add(opcode);

		REQUIRE(decoder.decodeEvent(bytes) == expectedEvents[opcode]);
	}
}
