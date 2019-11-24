#include "decoder.hpp"

#include <optional>

namespace jagce {

	Event Decoder::decodeEvent(ByteStream& in) {
		uint8_t firstByte = in.get();
		std::optional<uint8_t> prefixByte{};
		uint8_t opcode{};

		if (firstByte == 0xCB || firstByte == 0xDD || firstByte == 0xED || firstByte == 0xFD) {
			prefixByte = firstByte;
			opcode = in.get();
		} else {
			opcode = firstByte;
		}

		switch (opcode) {
			case 0x7F:
				return {LoadEvent{{RegisterName::A}, {RegisterName::A}}};
			default:
				return {NopEvent{}};
		}
	}

	std::vector<Event> Decoder::decodeEvents(ByteStream& in, size_t n) {
		std::vector<Event> events{};
		for (size_t i = 0; i < n; i++) {
			events.push_back(decodeEvent(in));
		}
		return events;
	}

	std::vector<Event> Decoder::decodeUntilEmpty(ByteStream& in) {
		return decodeEvents(in, in.size());
	}
}
