#pragma once
#include <samchon/API.hpp>

#include <thread>
#include <boost/asio.hpp>
#include <samchon/ByteArray.hpp>
#include <samchon/library/URLVariables.hpp>

#include <array>
#include <random>
#include <chrono>
#include <samchon/library/Date.hpp>
#include <samchon/library/StringUtil.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A http, web-page loader
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/library_data.png)
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class HTTPLoader
	{
	private:
		std::string url;

		/**
		 * @brief Method, Get or Post.
		 */
		int method;

		/**
		 * @brief Cookies got from remote web server.
		 */
		static HashMap<std::string, std::string>& cookie_map()
		{
			static HashMap<std::string, std::string> map;
			return map;
		};

	public:
		enum METHOD : int
		{
			GET = 1,
			POST = 2
		};

	public:
		/* ------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------ */
		/**
		 * @brief Construct from request url and method.
		 *
		 * @param url Target url of remote web server.
		 * @param method Get or Post
		 */
		HTTPLoader(const std::string &url, int method = POST)
		{
			this->url = url;
			this->method = method;
		};
		virtual ~HTTPLoader() = default;

		/* ------------------------------------------------------------
			SETTERS & GETTERS
		------------------------------------------------------------ */
		/**
		 * @brief Set url.
		 */
		void setURL(const std::string &val)
		{
			this->url = val;
		};

		/**
		 * @brief Set method.
		 */
		void setMethod(int val)
		{
			this->method = method;
		};

		/**
		 * @brief Get url.
		 */
		auto getURL() const -> std::string
		{
			return url;
		};

		/**
		 * @brief Get method.
		 */
		auto getMethod() const -> int
		{
			return method;
		};

		/**
		 * @brief Get cookie.
		 */
		auto getCookie(const std::string &key) const -> std::string
		{
			auto it = cookie_map().find(key);

			if (it == cookie_map().end())
				return "";
			else
				return it->second;
		};

		/* ------------------------------------------------------------
			LOADERS
		------------------------------------------------------------ */
		/**
		 * @brief Load data from target url.
		 *
		 * @details Loads binary data from target web server and address.
		 *
		 * @param data URLVariables containing parameters to request.
		 * @return Binary data fetched from remote web server.
		 */
		auto load(const URLVariables &parameters = {}) const -> ByteArray
		{
			//////////////////////////////////////////////////
			//	SENDING REQUEST HEADER
			//////////////////////////////////////////////////
			// FOR HEADER
			WeakString host = url;
			std::string path;

			if (host.find("://") != std::string::npos)
				host = host.between("://");
			if (host.find("/") != std::string::npos)
			{
				path = "/" + host.between("/").str();
				host = host.between("", "/");
			}

			// ENCODIG PATH
			{
				size_t idx = path.find('?');
				if (idx == std::string::npos)
					path = URLVariables::encode(path);
				else
				{
					std::string &front = path.substr(0, idx);
					std::string &back = path.substr(idx + 1);

					path = URLVariables::encode(front) + "?" + back;
				}
			}

			std::string header;
			if (method == GET)
			{
				header = StringUtil::substitute
				(
					std::string("") +
					"GET {2}{3} HTTP/1.1\n" +
					"Host: {1}\n" +
					"Accept: */*\n" +
					"Accept-Encoding: gzip, deflate\n"

					"Connection: Keep-Alive\n" +
					"Cookie: {4}\n"
					"\n",

					host.str(), path,
					((parameters.empty() == true)
						? std::string("")
						: "?" + parameters.toString()),
					getCookie(host)
				);
			}
			else
			{
				std::string &parameterStr = parameters.toString();

				header = StringUtil::substitute
				(
					std::string("") +
					"POST {2} HTTP/1.1\n" +
					"Host: {1}\n" +
					"Accept: */*\n" +
					"Connection: Keep-Alive\n" +

					/*"Accept-Language: en-US\n" +
					"Accept-Encoding: gzip, deflate\n" +
					"User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; Trident/7.0; rv:11.0) like Gecko\n" +*/

					"Content-Type: application/x-www-form-urlencoded\n" +
					"Content-Length: {3}\n" +
					"Cookie: {5}\n"
					"\n" +
					"{4}",

					host.str(), path,
					parameterStr.size(), parameterStr,
					getCookie(host)
				);
			}

			// PREPARE & GET IP ADDRESS
			boost::asio::io_service ioService;

			boost::asio::ip::tcp::resolver resolver(ioService);
			boost::asio::ip::tcp::resolver::query query(host, "http");
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

			// SOCKET - CONNECT AND SEND HEADER
			boost::asio::ip::tcp::socket socket(ioService);
			socket.connect(*endpoint_iterator);

			socket.write_some(boost::asio::buffer(header));

			//////////////////////////////////////////////////
			//	LISTEN HEADER FROM SERVER
			//////////////////////////////////////////////////
			HashMap<std::string, std::string> headerMap;
			{
				header.clear();

				while (true)
				{
					std::array<char, 1> buffer;

					size_t piece_size = socket.read_some(boost::asio::buffer(buffer));
					header += buffer[0];

					if (header.size() > 4 && header.substr(header.size() - 4) == "\r\n\r\n")
						break;
				}

				WeakString wstr = header;
				std::vector<WeakString> wstrArray = wstr.split("\r\n");

				for (size_t i = 0; i < wstrArray.size(); i++)
				{
					WeakString wstr = wstrArray[i];
					size_t index = wstr.find(":");

					if (index == std::string::npos)
						continue;

					headerMap.set(wstr.substr(0, index), wstr.substr(index + 1).trim());
				}
			}

			// REGISTER COOKIE
			if (headerMap.has("Set-Cookie") == true)
			{
				std::string &cookie = headerMap.get("Set-Cookie");

				((HashMap<std::string, std::string>*)&cookie_map)->set(host, cookie);
			}

			// CONTENT-LENGTH
			bool reserved = headerMap.has("Content-Length");
			bool chunked = headerMap.has("Transfer-Encoding") && headerMap.get("Transfer-Encoding") == "chunked";

			//////////////////////////////////////////////////
			//	GET DATA
			//////////////////////////////////////////////////
			ByteArray data;

			if (reserved == true)
			{
				// CONTENT-LENGTH
				data.reserve((size_t)stoull(headerMap.get("Content-Length")));

				while (true)
				{
					std::array<unsigned char, 1000> piece;
					boost::system::error_code error;

					size_t size = socket.read_some(boost::asio::buffer(piece), error);
					if (size == 0 || error)
						break;

					data.insert
					(
						data.end(),
						piece.begin(), piece.begin() + size
					);

					if (data.size() == data.capacity())
						break;
				}
			}
			else if (chunked == true)
			{
				std::vector<unsigned char> prevData;

				while (true)
				{
					std::array<char, 1000> piece;
					boost::system::error_code error;

					size_t size = socket.read_some(boost::asio::buffer(piece), error);
					if (size == 0 || error)
						break;

					prevData.insert(prevData.end(), piece.begin(), piece.begin() + size);

					// HANDLING LAST
					WeakString wstr((const char*)&prevData[0], (const char*)&prevData[0] + prevData.size());

					if (wstr.substring(wstr.size() - 7, wstr.size()) == "\r\n0\r\n\r\n")
					{
						size_t startIndex = 0;
						size_t endIndex;

						while (true)
						{
							size_t pos = wstr.find("\r\n", startIndex);
							WeakString piece = wstr.substr(startIndex, pos);

							size_t size = stoull(piece.str(), 0, 16);
							if (size == 0)
								break;

							startIndex = pos + 2;
							endIndex = std::min(startIndex + size, prevData.size());

							data.insert(data.end(), prevData.begin() + startIndex, prevData.begin() + endIndex);
							startIndex = endIndex + 2;
						}

						break;
					}
				}
			}
			else
			{
				while (true)
				{
					std::array<unsigned char, 1000> piece;
					boost::system::error_code error;

					size_t size = socket.read_some(boost::asio::buffer(piece), error);
					if (size == 0 || error)
						break;

					data.insert
					(
						data.end(),
						piece.begin(), piece.begin() + size
					);
				}
			}

			// RETURN
			return data;
		};
	};
};
};