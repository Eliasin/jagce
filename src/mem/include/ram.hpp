#ifndef JAGCE_RAM
#define JAGCE_RAM

#include <type_traits>
#include <memory>

namespace jagce {

	class RandomAccessMemory {
	public:
		virtual size_t size() const = 0;	
		virtual uint8_t readByte(size_t index) const = 0;
		
		template <typename T>
		T const * readBytesAsType(size_t index) const;

		virtual void writeByte(size_t index, uint8_t byte) = 0;
		
		template <typename T>
		void writeBytesAsType(size_t index, const T& var);

		virtual uint8_t const * readBytes(size_t index, size_t num) const = 0;
		virtual void writeBytes(size_t index, uint8_t const * bytes, size_t num) = 0;
	};

	template <typename T>
	T const * RandomAccessMemory::readBytesAsType(size_t index) const {
		static_assert(std::is_trivial_v<T>, "Non-trivial types cannot be read");
		static_assert(std::is_default_constructible_v<T>, "Types must be default constructible to be read");	

		return reinterpret_cast<T const *>(readBytes(index, sizeof(T)));
	}

	template <typename T>
	void RandomAccessMemory::writeBytesAsType(size_t index, const T& var) {
		constexpr size_t T_SIZE = sizeof(T);
		static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");

		writeBytes(index, reinterpret_cast<uint8_t const *>(&var), T_SIZE);
	}

}

#endif
