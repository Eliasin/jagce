#include <catch.hpp>

#include <array>

#include "byte_stream.hpp"

TEST_CASE("byte stream tests work", "[byte_stream]") {
	jagce::ByteStream byteStream{};

	SECTION("single insertions and empty work correctly") {
		uint8_t MAGIC_BYTE = 0x13;
		REQUIRE(byteStream.empty());

		byteStream.add(MAGIC_BYTE);

		REQUIRE(!byteStream.empty());

		uint8_t byte = byteStream.peek();

		REQUIRE(byte == MAGIC_BYTE);

		byte = byteStream.get();

		REQUIRE(byte == MAGIC_BYTE);
		REQUIRE(byteStream.empty());
	}

	SECTION("multiple insertions and empty work") {
		constexpr size_t IN_SIZE = 10;
		constexpr std::array<uint8_t, IN_SIZE> in{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

		byteStream.addBytes<10, 10>(in);
		REQUIRE(!byteStream.empty());

		std::array<uint8_t, 10> out{};

		byteStream.getBytes<10, 10>(out);

		REQUIRE(out == in);
		REQUIRE(byteStream.empty());

		byteStream.addBytes<10, 10>(in);

		std::array<uint8_t, 5> outHalf{};
		byteStream.getBytes<5, 5>(outHalf);

		std::array<uint8_t, 5> halfIn{};
		std::copy(in.begin(), in.begin() + 5, halfIn.begin());
		REQUIRE(outHalf == halfIn);
	}
}
