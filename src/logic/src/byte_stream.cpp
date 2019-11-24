#include "byte_stream.hpp"

namespace jagce {

	uint8_t ByteStream::get() {
		uint8_t byte = queue.front();
		queue.pop();
		return byte;
	}

	uint8_t ByteStream::peek() const {
		return queue.front();
	}

	size_t ByteStream::size() const {
		return queue.size();
	}

	void ByteStream::add(uint8_t byte) {
		return queue.push(byte);
	}

	bool ByteStream::empty() const {
		return queue.empty();
	}

}
