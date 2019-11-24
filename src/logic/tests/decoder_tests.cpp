#include <catch.hpp>

#include <array>

#include "decoder.hpp"

TEST_CASE("decoder produces correct events", "[logic], [decoder]") {
	jagce::Decoder decoder{};

	SECTION("load instructions") {
		jagce::ByteStream bytes{};

		constexpr uint8_t LOAD_A_A = 0x7F;
		bytes.add(LOAD_A_A);

		jagce::Event event{};
		event = decoder.decodeEvent(bytes);
		REQUIRE(std::holds_alternative<jagce::LoadEvent>(event));

		jagce::LoadEvent loadAA = std::get<jagce::LoadEvent>(event);
		constexpr jagce::LoadEvent EXPECTED_LOAD_A_A{{jagce::RegisterName::A}, {jagce::RegisterName::A}};
		REQUIRE(loadAA == EXPECTED_LOAD_A_A);
	}
}
