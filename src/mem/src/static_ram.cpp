#include "static_ram.hpp"

namespace jagce {

	template <size_t N>
	StaticRAM<N>::StaticRAM() : arr{} {};

	template <size_t N>
	size_t StaticRAM<N>::size() const {
		return sizeof(arr);
	}

	template <size_t N>
	char StaticRAM<N>::readByte(size_t index) const {
		#ifdef MEM_ACCESS_ASSERTIONS
		assert(index >= 0 && index < N);	
		#endif
		return arr[index];
	}

	template <size_t N>
	void StaticRAM<N>::writeByte(size_t index, char byte) {
		#ifdef MEM_ACCESS_ASSERTIONS
		assert(index >= 0 && index < N);	
		#endif
		arr[index] = byte;
	}

	template <size_t N>
	char* StaticRAM<N>::readBytes(size_t index, size_t num) const {
		#ifdef MEM_ACCESS_ASSERTIONS
		assert(index >= 0 && index < N);
		assert(index + num >= 0 && index + num < N);
		#endif
		
		return arr + index;
	}

	template <size_t N>
	void StaticRAM<N>::writeBytes(size_t index, char* bytes, size_t num) {
		#ifdef MEM_ACCESS_ASSERTIONS
		assert(index >= 0 && index < N);
		assert(index + num >= 0 && index < N);
		#endif

		memmove(arr + index, bytes, num);
	}

}
