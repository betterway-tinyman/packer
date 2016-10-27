#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Communicator.hpp>

#include <exception>
#include <queue>
#include <samchon/protocol/WebSocketUtil.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * A communicator following Web-socket protocol.
	 * 
	 * {@link WebCommunicator} is an abstract class who takes full charge of network communication with remote system, 
	 * following Web-socket protocol. Type of the {@link WebCommunicator} is specified to {@link WebServerConnector} and 
	 * {@link WebClientDriver} whether the remote system is a server (that my system is connecting to) or a client (a client 
	 * conneting to to my server).
	 *
	 * Whenever a replied message comes from the remote system, the message will be converted to an {@link Invoke} class
	 * and the {@link Invoke} object will be shifted to the {@link IProtocol listener}'s
	 * {@link IProtocol.replyData IProtocol.replyData()} method.
	 * 
	 * Note that, one of this or remote system is web-browser based, then there's not any alternative choice. Web browser
	 * supports only Web-socket protocol. In that case, you've use a type of this {@link WebCommunicator} class.
	 * 
	 * ![Basic Components](http://samchon.github.io/framework/images/design/cpp_class_diagram/protocol_basic_components.png)
	 *
	 * @see {@link WebClientDriver}, {@link WebServerConnector}, {@link IProtocol}
	 * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/CPP-Protocol-Basic_Components#icommunicator)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class WebCommunicator 
		: public virtual Communicator
	{
	private:
		bool is_server;

	public:
		WebCommunicator(bool is_server)
			: Communicator()
		{
			this->is_server = is_server;
		};
		virtual ~WebCommunicator() = default;

		virtual void sendData(std::shared_ptr<Invoke> invoke) override
		{
			std::unique_lock<std::mutex> uk(send_mtx);

			// SEND INVOKE
			const std::string &str = invoke->toXML()->toString();
			if (is_server == true)
				send_data(str);
			else // CLIENT MASKS ON SENDING DATA
				send_masked_data(str);

			// SEND BINARY
			for (size_t i = 0; i < invoke->size(); i++)
				if (invoke->at(i)->getType() == "ByteArray")
					if (is_server == true)
						send_data(invoke->at(i)->referValue<ByteArray>());
					else // CLIENT MASKS ON SENDING DATA
						send_masked_data(invoke->at(i)->referValue<ByteArray>());
		};

	protected:
		/* =========================================================
			SOCKET I/O
				- READ
				- WRITE
		============================================================
			READ
		--------------------------------------------------------- */
		virtual void listen_message() override
		{
			std::shared_ptr<Invoke> binary_invoke = nullptr;
			std::queue<std::shared_ptr<InvokeParameter>> binary_parameters;

			while (true)
			{
				try
				{
					const std::pair<unsigned char, size_t> &header = listen_header();

					// EXIT CODE
					if (header.first == WebSocketUtil::DISCONNECT)
						break;

					// READ DATA
					if (header.first == WebSocketUtil::TEXT)
					{
						std::shared_ptr<Invoke> invoke = listen_string(header.second);

						for (size_t i = 0; i < invoke->size(); i++)
						{
							std::shared_ptr<InvokeParameter> &parameter = invoke->at(i);
							if (parameter->getType() != "ByteArray")
								continue;

							if (binary_invoke == nullptr)
								binary_invoke = invoke;
							binary_parameters.push(parameter);
						}

						// NO BINARY, THEN REPLY DIRECTLY
						if (binary_invoke == nullptr)
							this->replyData(invoke);
					}
					else if (header.first == WebSocketUtil::BINARY)
					{
						std::shared_ptr<InvokeParameter> parameter = binary_parameters.front();
						listen_binary(header.second, parameter);
						binary_parameters.pop();

						if (binary_parameters.empty() == true)
						{
							// NO BINARY PARAMETER LEFT,
							std::shared_ptr<Invoke> invoke = binary_invoke;
							binary_invoke = nullptr;

							// THEN REPLY
							this->replyData(invoke);
						}
					}
				}
				catch (...)
				{
					break;
				}
			}
		};

	private:
		auto listen_header() -> std::pair<unsigned char, size_t>
		{
			unsigned char mask = is_server ? WebSocketUtil::MASK : 0;

			std::array<unsigned char, 2> header_bytes;
			unsigned char op_code; // false then binary
			unsigned char size_header;

			size_t content_size = 0;

			// READ HEADER BYTES
			listen_data(header_bytes);
			op_code = header_bytes[0];
			size_header = header_bytes[1];

			// INSPECT MASK VALIDATION
			if (is_server == true)
			{
				if (size_header < WebSocketUtil::MASK)
					throw std::domain_error("masked message from server has delivered.");
			}
			else
			{
				if (size_header >= WebSocketUtil::MASK)
					throw std::domain_error("masked message from server has delivered.");
			}

			// EXIT CODE
			if (op_code == WebSocketUtil::DISCONNECT)
				return{ op_code, 0 }; // DISCONNECTION SIGNAL HAS ARRIVED

									  // READ CONTENT SIZE
			if (is_server)
				size_header -= WebSocketUtil::MASK; // CLIENT SENDS MASKED DATA, DETACH THE MASK

			if (size_header == (unsigned char)WebSocketUtil::TWO_BYTES)
			{
				// SIZE HEADER IS 2 BYTES
				std::array<unsigned char, 2> size_bytes;
				listen_data(size_bytes);

				for (size_t c = 0; c < size_bytes.size(); c++)
					content_size += size_bytes[c] << (8 * (size_bytes.size() - 1 - c));
			}
			else if (size_header == (unsigned char)WebSocketUtil::EIGHT_BYTES)
			{
				// SIZE HEADER IS 8 BYTES
				std::array<unsigned char, 8> size_bytes;
				listen_data(size_bytes);

				for (size_t c = 0; c < size_bytes.size(); c++)
					content_size += size_bytes[c] << (8 * (size_bytes.size() - 1 - c));
			}
			else
				content_size = (size_t)size_header;

			return{ op_code, content_size };
		};

		auto listen_string(size_t size) -> std::shared_ptr<Invoke>
		{
			// READ CONTENT
			std::string data(size, (char)NULL);
			if (is_server) // CLIENT SENDS MASKED DATA
				listen_masked_data(data);
			else
				listen_data(data);

			// CONSTRUCT INVOKE OBJECT
			std::shared_ptr<Invoke> invoke(new Invoke());
			invoke->construct(std::make_shared<library::XML>(data));

			return invoke;
		};

		void listen_binary(size_t size, std::shared_ptr<InvokeParameter> parameter)
		{
			// FETCH BYTE_ARRAY
			ByteArray &data = (ByteArray&)parameter->referValue<ByteArray>();

			// READ CONTENT
			if (is_server == true) // CLIENT SENDS MASKED DATA
				listen_masked_data(data);
			else
				listen_data(data);
		};

		template <class Container>
		void listen_data(Container &data)
		{
			size_t completed = 0;

			if (completed < data.size())
				completed += socket->read_some(boost::asio::buffer((unsigned char*)data.data() + completed, data.size() - completed));
		};

		template <class Container>
		void listen_masked_data(Container &data)
		{
			// READ MASK
			std::array<unsigned char, 4> mask;
			socket->read_some(boost::asio::buffer(mask));

			// READ DATA
			listen_data(data);

			// UNMASK
			for (size_t i = 0; i < data.size(); i++)
				data[i] = data[i] ^ mask[i % 4];
		};

		/* ---------------------------------------------------------
			WRITE
		--------------------------------------------------------- */
		template <class Container>
		void send_data(const Container &data)
		{
			unsigned char op_code = std::is_same<std::string, Container>()
				? WebSocketUtil::TEXT
				: WebSocketUtil::BINARY;
			size_t size = data.size();

			///////
			// SEND HEADER
			///////
			ByteArray header;
			header.write(op_code);

			if (size < 126)
				header.write((unsigned char)size);
			else if (size < 0xFFFF)
			{
				header.write((unsigned char)(WebSocketUtil::TWO_BYTES));
				header.writeReversely((unsigned short)size);
			}
			else
			{
				header.write((unsigned char)(WebSocketUtil::EIGHT_BYTES));
				header.writeReversely((unsigned long long)size);
			}

			socket->write_some(boost::asio::buffer(header)); // SEND HEADER
			socket->write_some(boost::asio::buffer(data)); // SEND DATA
		};

		template <class Container>
		void send_masked_data(const Container &data)
		{
			unsigned char op_code = std::is_same<std::string, Container>()
				? WebSocketUtil::TEXT
				: WebSocketUtil::BINARY;
			size_t size = data.size();

			///////
			// SEND HEADER
			///////
			ByteArray header;
			header.write(op_code);

			if (size < 126)
				header.write((unsigned char)(size + WebSocketUtil::MASK));
			else if (size < 0xFFFF)
			{
				header.write((unsigned char)(WebSocketUtil::TWO_BYTES + WebSocketUtil::MASK));
				header.writeReversely((unsigned short)size);
			}
			else
			{
				header.write((unsigned char)(WebSocketUtil::EIGHT_BYTES + WebSocketUtil::MASK));
				header.writeReversely((unsigned long long)size);
			}

			///////
			// SEND DATA
			///////
			static std::uniform_int_distribution<unsigned short> distribution(0, 255);
			static std::random_device device;

			// CONSTRUCT MASK
			std::array<unsigned char, 4> mask;
			for (size_t i = 0; i < mask.size(); i++)
				mask[i] = (unsigned char)distribution(device);

			// TO BE MASKED
			std::vector<unsigned char> masked_data(data.size());
			for (size_t i = 0; i < masked_data.size(); i++)
				masked_data[i] = data[i] ^ mask[i % 4];

			// SEND
			socket->write_some(boost::asio::buffer(header)); // SEND HEADER
			socket->write_some(boost::asio::buffer(mask)); // SEND MASK
			socket->write_some(boost::asio::buffer(masked_data)); // SEND MASKED DATA
		};
	};
};
};