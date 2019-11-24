#include <catch.hpp>

#include <map>

#include "decoder.hpp"

TEST_CASE("decoder produces correct events", "[logic], [decoder]") {
	jagce::Decoder decoder{};

	SECTION("load instructions") {
		auto opcode = GENERATE(0x7F, 0x78);
		
		std::map<uint8_t, jagce::LoadEvent> expectedEvents{
			{ 0x7F, { {jagce::RegisterName::A}, {jagce::RegisterName::A} }},
			{ 0x78, { {jagce::RegisterName::A}, {jagce::RegisterName::B} }}
		};
		
		jagce::ByteStream bytes{};
		bytes.add(opcode);

		jagce::Event event = decoder.decodeEvent(bytes);
		REQUIRE(std::get<jagce::LoadEvent>(event) == expectedEvents[opcode]);
	}
}
