#pragma once
#include <samchon/API.hpp>

#include <string>
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
	class SAMCHON_FRAMEWORK_API Base64
	{
	public:
		/**
		 * @brief Encode from binary data to base64-string
		 * @details Encodes a binary data to base64-string
		 *
		 * @param byteArray binary data you want to encode
		 * @return A base64-string encoded from binary data
		 */
		static auto encode(const ByteArray &) -> std::string;

		/**
		 * @brief Decode from base64-string to binary data
		 * @details Decodes a base64 encoded string to binary data
		 *
		 * @note If the encoded string is not whole but a part of the binary data,
		 *		 Base64Decoder will be better
		 * @param str A encoded string representing binary data as base64 format
		 * @return A binary data decoded from string
		 */
		static auto decode(const std::string &) -> ByteArray;
	};
};
};