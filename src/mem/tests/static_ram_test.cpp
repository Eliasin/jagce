#include <catch.hpp>

#include <cstring>

#include "static_ram.hpp"

struct vec3 {
	float x, y, z;
};

TEST_CASE("static ram methods work", "[static_ram]") {
	constexpr size_t SIZE = 100;
	jagce::StaticRAM<SIZE> sram{};

	SECTION("static ram size is correct") {
		REQUIRE(sram.size() == SIZE);
	}

	SECTION("static ram is zero initialized") {
		uint8_t const * end = sram.readBytes(sram.size() - 1, 1) + 1;
		for (uint8_t const * p = sram.readBytes(0, sram.size()); p != end; p++) {
			REQUIRE(*p == 0);
		}
	}

	SECTION("static ram can be read from and written to consistently and correctly") {
		const uint8_t MAGIC[2]{ 0xCA, 0xFE };

		for (size_t writeAreaStart = 0; writeAreaStart < sram.size() - sizeof(MAGIC); writeAreaStart++) {
			sram.writeBytes(writeAreaStart, MAGIC, sizeof(MAGIC));
		
			uint8_t const * bytes = sram.readBytes(writeAreaStart, sizeof(MAGIC));

			REQUIRE(memcmp(bytes, MAGIC, sizeof(MAGIC)) == 0);
		}
	}

	SECTION("static ram reading and writing types works consistently and correctly") {
		const vec3 v{1.f, 2.f, -1.f};

		for (size_t writeAreaStart = 0; writeAreaStart < sram.size() - sizeof(vec3); writeAreaStart++) {
			sram.writeBytesAsType<vec3>(writeAreaStart, v);
		
			vec3 const * u = sram.readBytesAsType<vec3>(writeAreaStart);

			bool same = v.x == u->x && v.y == u->y && v.z == u->z;
			REQUIRE(same);
		}
	}
}
