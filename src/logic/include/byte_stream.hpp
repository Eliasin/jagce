#ifndef JAGCE_BYTE_STREAM
#define JAGCE_BYTE_STREAM

#include <array>
#include <queue>

namespace jagce {

	class ByteStream {
	public:
		uint8_t get();
		uint8_t peek() const;
		void add(uint8_t byte);
		bool empty() const;

		template <size_t N, size_t BufSize>
		std::enable_if_t<BufSize >= N, void> getBytes(std::array<uint8_t, BufSize>& buf) {
			for (size_t i = 0; i < N; i++) {
				buf[i] = queue.front();
				queue.pop();
			}
		}

		template <size_t N, size_t BufSize>
		std::enable_if_t<BufSize >= N, void> addBytes(const std::array<uint8_t, BufSize>& buf) {
			for (size_t i = 0; i < N; i++) {
				queue.push(buf[i]);
			}
		}

	private:
		std::queue<uint8_t> queue;
	};

}

#endif
