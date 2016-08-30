#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IProtocol.hpp>
#include <samchon/protocol/Socket.hpp>

#include <mutex>

namespace samchon
{
namespace protocol
{
	class SAMCHON_FRAMEWORK_API Communicator 
		: public virtual IProtocol
	{
	protected:
		std::shared_ptr<Socket> socket;
		IProtocol *listener;

		std::mutex send_mtx;

	public:
		Communicator();
		virtual ~Communicator();

	protected:
		virtual void listen_message();

	private:
		auto listen_size() const -> size_t;
		auto listen_string(size_t) -> std::shared_ptr<Invoke>;
		void listen_binary(size_t, std::shared_ptr<InvokeParameter>);

	public:
		virtual void replyData(std::shared_ptr<Invoke> invoke);

		virtual void sendData(std::shared_ptr<Invoke> invoke);
	};
};
};