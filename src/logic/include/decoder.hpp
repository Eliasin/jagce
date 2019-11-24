#ifndef JAGCE_DECODER
#define JAGCE_ENCODER

#include <variant>
#include <vector>

#include "register_names.hpp"
#include "byte_stream.hpp"

namespace jagce {

	using Address = size_t;
	using Immediate = uint16_t;
	using Writeable = std::variant<RegisterName, Address>;
	using Readable = std::variant<RegisterName, Address, Immediate>;

	struct LoadEvent {
		Writeable dest;
		Readable src;
	};

	using NopEvent = std::monostate;
	using Event = std::variant<LoadEvent, NopEvent>;

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
