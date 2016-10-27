#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IProtocol.hpp>

#include <array>
#include <exception>
#include <mutex>
#include <boost/asio.hpp>
#include <samchon/ByteArray.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * A communicator.
	 * 
	 * The {@link Communicator} is an abstract class who take full charge of network communication with remote system, without 
	 * reference to whether the remote system is a server or a client. Type of the {@link Communicator} is specified to 
	 * {@link ServerConnector} and {@link ClientDriver} whether the remote system is a server (that I've to connect) or a 
	 * client (a client connected to my server).
	 *
	 * Whenever a replied message comes from the remote system, the message will be converted to an {@link Invoke} class
	 * and the {@link Invoke} object will be shifted to the {@link IProtocol listener}'s
	 * {@link IProtocol.replyData IProtocol.replyData()} method.
	 *
	 * Note that, if one of this or remote system is web-browser based, then you don't have to use this {@link Communicator} 
	 * class who follows the Samchon Framework's own protocol. Web-browser supports only Web-socket protocol. Thus in that 
	 * case, you have to use {@link WebCommunicator} instead.
	 *
	 * ![Basic Components](http://samchon.github.io/framework/images/design/cpp_class_diagram/protocol_basic_components.png)
	 *
	 * @see {@link ClientDriver}, {@link ServerConnector}, {@link IProtocol}
	 * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/CPP-Protocol-Basic_Components#communicator)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Communicator 
		: public virtual IProtocol
	{
	protected:
		std::shared_ptr<boost::asio::ip::tcp::socket> socket;
		IProtocol *listener;

		std::mutex send_mtx;

	public:
		Communicator()
		{
			listener = nullptr;
		};
		virtual ~Communicator()
		{
			close();
		};

		/**
		 * Close connection.
		 */
		virtual void close()
		{
			if (socket != nullptr && socket->is_open())
				socket->close();
		};

		/**
		* Handle replied message.
		*
		* Handles replied {@link Invoke} message recived from remove system. The {@link Invoke} message will be shifted to
		* the {@link IProtocol listener}'s {@link IProtocol.replyData IProtocol.replyData()} by this method.
		*
		* @param invoke An {@link Invoke} message received from remote system.
		*/
		virtual void replyData(std::shared_ptr<Invoke> invoke)
		{
			listener->replyData(invoke);
		};

		/**
		* Send message.
		*
		* Send the {@link Invoke} message to remote system.
		*
		* @param invoke An {@link Invoke} message to send.
		*/
		virtual void sendData(std::shared_ptr<Invoke> invoke)
		{
			std::unique_lock<std::mutex> uk(send_mtx);

			// SEND INVOKE
			send_data(invoke->toXML()->toString());

			// SEND BINARY
			for (size_t i = 0; i < invoke->size(); i++)
				if (invoke->at(i)->getType() == "ByteArray")
					send_data(invoke->at(i)->referValue<ByteArray>());
		};

	protected:
		/* =========================================================
			SOCKET I/O
				- READ
				- WRITE
		============================================================
			READ
		--------------------------------------------------------- */
		virtual void listen_message()
		{
			std::shared_ptr<Invoke> binary_invoke = nullptr;
			std::queue<std::shared_ptr<InvokeParameter>> binary_parameters;

			while (true)
			{
				try
				{
					// READ CONTENT SIZE
					size_t content_size = listen_size();

					// READ CONTENT
					if (binary_invoke == nullptr)
					{
						std::shared_ptr<Invoke> invoke = listen_string(content_size);

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
					else
					{
						std::shared_ptr<InvokeParameter> parameter = binary_parameters.front();
						listen_binary(content_size, parameter);
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
		auto listen_size() -> size_t
		{
			std::array<unsigned char, 8> size_header;
			listen_data(size_header);

			size_t size = 0;
			for (size_t c = 0; c < size_header.size(); c++)
				size += size_header[c] << (8 * (size_header.size() - 1 - c));

			return size;
		};

		auto listen_string(size_t size) -> std::shared_ptr<Invoke>
		{
			// READ CONTENT
			std::string data(size, (char)NULL);
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
			data.assign(size, NULL);

			// READ CONTENT
			listen_data(data);
		};

		template <class Container>
		void listen_data(Container &data)
		{
			size_t completed = 0;

			if (completed < data.size())
				completed += socket->read_some(boost::asio::buffer((unsigned char*)data.data() + completed, data.size() - completed));
		};

		/* ---------------------------------------------------------
			SEND
		--------------------------------------------------------- */
		template <class Container>
		void send_data(const Container &data)
		{
			ByteArray header;
			header.writeReversely((unsigned long long)data.size());

			socket->write_some(boost::asio::buffer(header));
			socket->write_some(boost::asio::buffer(data));
		};
	};
};
};