#pragma once

#include <string>
#include <array>

#include <samchon/ByteArray.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief Utility class for base64 format's en-decoding
	 *
	 * @details
	 * Base64 supports
	 *	\li Encoding from binary data to base64-string 
	 *	\li Decoding from base64-string to binary data
	 * 
	 * @image html cpp/subset/library_string.png
	 * @image latex cpp/subset/library_string.png
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Base64
	{
	public:
		/**
		 * @brief Encode from binary data to base64-string
		 * @details Encodes a binary data to base64-string
		 *
		 * @param byte_array binary data you want to encode
		 * @return A base64-string encoded from binary data
		 */
		static auto encode(const ByteArray &byte_array) -> std::string
		{
			static const std::array<char, 64> BASE64_CHAR_ARRAY =
			{
				'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
				'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
				'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
				'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
				'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
				'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
				'w', 'x', 'y', 'z', '0', '1', '2', '3',
				'4', '5', '6', '7', '8', '9', '+', '/'
			};

			unsigned char *bytes = (unsigned char*)byte_array.data();
			std::string str;
			str.reserve(byte_array.size() * 4);

			long long left_size = (long long)byte_array.size();

			size_t i = 0;
			size_t j = 0;
			std::array<unsigned char, 3> input;
			std::array<unsigned char, 4> output;

			while (left_size--)
			{
				input[i++] = *(bytes++);
				if (i == 3)
				{
					output[0] = (input[0] & 0xfc) >> 2;
					output[1] = ((input[0] & 0x03) << 4) + ((input[1] & 0xf0) >> 4);
					output[2] = ((input[1] & 0x0f) << 2) + ((input[2] & 0xc0) >> 6);
					output[3] = input[2] & 0x3f;

					for (i = 0; (i < 4); i++)
						str += BASE64_CHAR_ARRAY[output[i]];
					i = 0;
				}
			}

			if (i)
			{
				for (j = i; j < 3; j++)
					input[j] = '\0';

				output[0] = (input[0] & 0xfc) >> 2;
				output[1] = ((input[0] & 0x03) << 4) + ((input[1] & 0xf0) >> 4);
				output[2] = ((input[1] & 0x0f) << 2) + ((input[2] & 0xc0) >> 6);
				output[3] = input[2] & 0x3f;

				for (j = 0; (j < i + 1); j++)
					str += BASE64_CHAR_ARRAY[output[j]];

				while ((i++ < 3))
					str += '=';

			}
			return str;
		};

		/**
		 * @brief Decode from base64-string to binary data
		 * @details Decodes a base64 encoded string to binary data
		 *
		 * @note If the encoded string is not whole but a part of the binary data,
		 *		 Base64Decoder will be better
		 * @param str A encoded string representing binary data as base64 format
		 * @return A binary data decoded from string
		 */
		static auto decode(const std::string &str) -> ByteArray
		{
			static const std::array<int, 256> BASE64_DECODE_ARRAY =
			{
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 00-0F */
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 10-1F */
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,  /* 20-2F */
				52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,  /* 30-3F */
				-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,  /* 40-4F */
				15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,  /* 50-5F */
				-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,  /* 60-6F */
				41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,  /* 70-7F */
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 80-8F */
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 90-9F */
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* A0-AF */
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* B0-BF */
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* C0-CF */
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* D0-DF */
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* E0-EF */
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1   /* F0-FF */
			};

			ByteArray data;
			data.reserve(str.size() * 3);

			int space_idx = 0, phase;
			int d, prev_d = 0;
			unsigned char c;

			space_idx = 0;
			phase = 0;

			for (size_t i = 0; i < str.size(); i++)
			{
				d = BASE64_DECODE_ARRAY[(int)str[i]];

				if (d != -1)
				{
					switch (phase)
					{
					case 0:
						++phase;
						break;
					case 1:
						c = ((prev_d << 2) | ((d & 0x30) >> 4));
						data.push_back(c);
						++phase;
						break;
					case 2:
						c = (((prev_d & 0xf) << 4) | ((d & 0x3c) >> 2));
						data.push_back(c);
						++phase;
						break;
					case 3:
						c = (((prev_d & 0x03) << 6) | d);
						data.push_back(c);
						phase = 0;
						break;
					}
					prev_d = d;
				}
			}
			return data;
		};
	};
};
};