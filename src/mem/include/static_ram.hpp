#ifndef JAGCE_STATIC_RAM
#define JAGCE_STATIC_RAM

#include <cstring>

#include "ram.hpp"

namespace jagce {
	
	template <size_t N>
	class StaticRAM : public RandomAccessMemory {
	public:
		StaticRAM() : arr{} {};
		size_t size() const override { return sizeof(arr); }

		uint8_t readByte(size_t index) const override {
			#ifdef MEM_ACCESS_ASSERTIONS
			assert(index >= 0 && index < N);	
			#endif
			return arr[index];
		} 

		void writeByte(size_t index, uint8_t byte) override {
			#ifdef MEM_ACCESS_ASSERTIONS
			assert(index >= 0 && index < N);	
			#endif
			arr[index] = byte;
		}

		uint8_t const * readBytes(size_t index, size_t num) const override {
			#ifdef MEM_ACCESS_ASSERTIONS
			assert(index >= 0 && index < N);
			assert(index + num - 1 >= 0 && index + num - 1 < N);
			#endif
			
			return arr + index;
		}

		void writeBytes(size_t index, uint8_t const * bytes, size_t num) override {
			#ifdef MEM_ACCESS_ASSERTIONS
			assert(index >= 0 && index < N);
			assert(index + num - 1 >= 0 && index + num - 1 < N);
			#endif

			memmove(arr + index, bytes, num);
		}

	private:
		uint8_t arr[N];
	};

}

#endif
