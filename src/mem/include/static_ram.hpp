#ifndef JAGCE_STATIC_RAM
#define JAGCE_STATIC_RAM

#include "ram.hpp"

namespace jagce {
	
	template <size_t N>
	class StaticRAM : public RandomAccessMemory {
	public:
		StaticRAM();
		size_t size() const override;
		char readByte(size_t index) const override;
		void writeByte(size_t index, char byte) override;
		char* readBytes(size_t index, size_t num) const override;
		void writeBytes(size_t index, char* bytes, size_t num) override;
	private:
		char arr[N];
	};
}

#endif
