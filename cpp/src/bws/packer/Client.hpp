#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/IClient.hpp>

namespace samchon
{
	namespace library
	{
		class XML;
	};
};

namespace bws
{
namespace packer
{
	class Client
		: public protocol::IClient
	{
	private:
		typedef protocol::IClient super;

	public:
		Client(protocol::Socket*);
		virtual ~Client() = default;

		virtual void replyData(std::shared_ptr<protocol::Invoke>) override;

	protected:
		virtual void _replyData(std::shared_ptr<protocol::Invoke>) override;

		void pack(std::shared_ptr<library::XML>);
	};
};
};