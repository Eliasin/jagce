#include <catch.hpp>

#include <array>

#include "byte_stream.hpp"

TEST_CASE("byte stream tests work", "[byte_stream]") {
	jagce::ByteStream byteStream{};

	SECTION("single insertions, empty and size work correctly") {
		uint8_t MAGIC_BYTE = 0x13;
		REQUIRE(byteStream.empty());

		byteStream.add(MAGIC_BYTE);

		REQUIRE(!byteStream.empty());
		REQUIRE(byteStream.size() == 1);

		uint8_t byte = byteStream.peek();

		REQUIRE(byte == MAGIC_BYTE);
		REQUIRE(!byteStream.empty());
		REQUIRE(byteStream.size() == 1);

		byte = byteStream.get();

		REQUIRE(byte == MAGIC_BYTE);
		REQUIRE(byteStream.empty());
		REQUIRE(byteStream.size() == 0);
	}

	SECTION("multiple insertions and empty work") {
		constexpr size_t IN_SIZE = 10;
		constexpr std::array<uint8_t, IN_SIZE> in{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

		byteStream.addBytes<10, IN_SIZE>(in);
		REQUIRE(!byteStream.empty());
		REQUIRE(byteStream.size() == IN_SIZE);

		std::array<uint8_t, IN_SIZE> out{};

		byteStream.getBytes<10, IN_SIZE>(out);

		REQUIRE(out == in);
		REQUIRE(byteStream.empty());
		REQUIRE(byteStream.size() == 0);

		byteStream.addBytes<10, IN_SIZE>(in);

		std::array<uint8_t, 5> outHalf{};
		byteStream.getBytes<5, 5>(outHalf);

		std::array<uint8_t, 5> halfIn{};
		std::copy(in.begin(), in.begin() + 5, halfIn.begin());
		REQUIRE(outHalf == halfIn);
		REQUIRE(!byteStream.empty());
		REQUIRE(byteStream.size() == 5);
	}
}
