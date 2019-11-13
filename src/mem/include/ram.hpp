#ifndef JAGCE_RAM
#define JAGCE_RAM

#include <type_traits>
#include <memory>

namespace jagce {

	class RandomAccessMemory {
	public:
		virtual size_t size() const = 0;	
		virtual char readByte(size_t index) const = 0;
		
		template <typename T>
		T readBytesAsType(size_t index) const;

		virtual void writeByte(size_t index, char byte) = 0;
		
		template <typename T>
		void writeBytesAsType(T var);

		virtual char* getBytes(size_t index, size_t num) const = 0;
		virtual void writeBytes(char* bytes, size_t num) = 0;
	};

	template <typename T>
	T RandomAccessMemory::readBytesAsType(size_t index) const {
		constexpr size_t T_SIZE = sizeof(T);
		static_assert(std::is_trivial_v<T>, "Non-trivial types cannot be read");
		static_assert(std::is_default_constructible_v<T>, "Types must be default constructible to be read");	

		T t{};
		char* bytes = getBytes(index, T_SIZE);

		memcpy(&t, bytes, T_SIZE);

		return t;
	}

	template <typename T>
	void RandomAccessMemory::writeBytesAsType(T var) {
		constexpr size_t T_SIZE = sizeof(T);
		static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
			
		writeBytes(reinterpret_cast<char*>(&var), T_SIZE);
	}

}

#endif
