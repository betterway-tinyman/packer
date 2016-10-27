#pragma once
#include <samchon/API.hpp>

#include <string>
#include <random>
#include <boost/uuid/sha1.hpp>

#include <samchon/ByteArray.hpp>
#include <samchon/library/Base64.hpp>
#include <samchon/library/Date.hpp>

namespace samchon
{
namespace protocol
{
	class WebSocketUtil
	{
	public:
		enum OpCode : unsigned char
		{
			TEXT = 129,
			BINARY = 130,
			DISCONNECT = 136
		};

		enum SizeCode : unsigned char
		{
			TWO_BYTES = 126,
			EIGHT_BYTES = 127
		};

		static const unsigned char MASK = 128;

		static auto GUID() -> std::string
		{
			return "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
		};
		
		static auto generate_base64_certification_key() -> std::string
		{
			static std::uniform_int_distribution<unsigned short> distribution(0, 255);
			static std::random_device device;

			std::string certification_key(16, NULL);
			for (size_t i = 0; i < certification_key.size(); i++)
				certification_key[i] = (unsigned char)distribution(device);

			ByteArray byte_array;
			byte_array.assign(certification_key.begin(), certification_key.end());

			return library::Base64::encode(byte_array);
		};

		static auto encode_certification_key(const std::string &base64) -> std::string
		{
			std::string acceptKey = base64 + GUID();

			boost::uuids::detail::sha1 hash;
			hash.process_bytes(acceptKey.c_str(), acceptKey.size());

			ByteArray bytes;
			unsigned int digest[5];
			hash.get_digest(digest);

			for (size_t index = 0; index < 5; index++)
				bytes.writeReversely(digest[index]); //ENDIAN REVERSING

			return library::Base64::encode(bytes);
		};
	};
};
};