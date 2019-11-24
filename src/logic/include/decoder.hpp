#ifndef JAGCE_DECODER
#define JAGCE_ENCODER

#include <variant>
#include <vector>

#include "register_names.hpp"
#include "byte_stream.hpp"

namespace jagce {

	using Address = size_t;
	using Immediate = uint16_t;
	using Writeable = std::variant<RegisterName, Address, Indirect>;
	using Readable = std::variant<RegisterName, Address, Immediate, Indirect>;

	struct LoadEvent8 {
		Writeable dest;
		Readable src;
		bool operator==(const LoadEvent8& other) const;
	};

	using NopEvent = std::monostate;
	using Event = std::variant<LoadEvent8, NopEvent>;

	/* The decoder class consumes bytes from a byte stream as it's input
	 * and produces 'events' from it. Events are state changes to RAM,
	 * video memory or registers.
	 */
	class Decoder {
	public:
		Event decodeEvent(ByteStream& in);
		std::vector<Event> decodeEvents(ByteStream& in, size_t n);
		std::vector<Event> decodeUntilEmpty(ByteStream& in);
	};

}

#endif
