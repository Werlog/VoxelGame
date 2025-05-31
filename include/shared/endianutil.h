#include <type_traits>

namespace endian
{
	bool isLittleEndian()
	{
		int num = 1;
		if (*(char*)&num == 1)
		{
			return true;
		}

		return false;
	}

	template<typename T>
	T swapEndian(T value)
	{
		static_assert(std::is_integral<T>::value, "Swap Endian only supports integral types");
		if constexpr (sizeof(T) == 4)
		{
			return ((value >> 24) & 0x000000FF) |
				((value >> 8) & 0x0000FF00) |
				((value << 8) & 0x00FF0000) |
				((value << 24) & 0xFF000000);
		}
		else if constexpr (sizeof(T) == 2)
		{
			return (value & 0xFF) << 8 | ((value >> 8) & 0xFF);
		}
		else if constexpr (sizeof(T) == 8)
		{
			return ((value >> 56) & 0x00000000000000FFULL) |
				((value >> 40) & 0x000000000000FF00ULL) |
				((value >> 24) & 0x0000000000FF0000ULL) |
				((value >> 8) & 0x00000000FF000000ULL) |
				((value << 8) & 0x000000FF00000000ULL) |
				((value << 24) & 0x0000FF0000000000ULL) |
				((value << 40) & 0x00FF000000000000ULL) |
				((value << 56) & 0xFF00000000000000ULL);
		}
		else
		{
			static_assert(false, "Swap Endian does not support this type");
		}
	}

	float swapFloatEndian(float value)
	{
		int val;
		std::memcpy(&val, &value, sizeof(int));
		val = swapEndian(val);

		float result;
		std::memcpy(&result, &val, sizeof(float));
		return result;
	}

	template<typename T>
	T toHostEndian(T value)
	{
		if (isLittleEndian())
			return swapEndian(value);
		return value;
	}

	template<typename T>
	T toBigEndian(T value)
	{
		if (!isLittleEndian())
			return value;
		return swapEndian(value);
	}
}
