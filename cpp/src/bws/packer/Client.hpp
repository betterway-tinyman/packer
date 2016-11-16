#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/IProtocol.hpp>
#include <samchon/protocol/ClientDriver.hpp>

#include <bws/packer/PackerForm.hpp>
#include <bws/packer/Packer.hpp>
#include <bws/packer/WrapperArray.hpp>
#include <samchon/library/GAParameters.hpp>

#include <iostream>
#include <samchon/library/Date.hpp>
#include <samchon/library/ProgressEvent.hpp>

namespace bws
{
namespace packer
{
	/**
	 * @brief Driver for a connected client.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Client 
		: public protocol::IProtocol
	{
	private:
		std::shared_ptr<protocol::ClientDriver> driver;

	public:
		/**
		 * Construct from a socket.
		 *
		 * @param socket A socket connected with the client.
		 */
		Client(std::shared_ptr<protocol::ClientDriver> driver)
		{
			this->driver = driver;
			driver->listen(this);
		};
		virtual ~Client() = default;

		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			driver->sendData(invoke);
		};

		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			library::Date date;
			std::cout << invoke->getListener() << " - " << date.toString() << std::endl;

			if (invoke->getListener() == "pack")
				pack(invoke->at(0)->getValueAsXML());
		};

	private:
		/**
		 * @brief Pack instances to wrappers.
		 *
		 * @details 
		 * <p> Deducts the best optimization solution of packing and send the optimization 
		 * result data to the client. #pack needs lots of execution time and during the optimization
		 * process, progress data of the optimization will bed sent to the client. </p>
		 * 
		 * @param xml XML represents instances and wrappers to pack.
		 */
		void pack(std::shared_ptr<library::XML> xml)
		{
			std::unique_ptr<PackerForm> packerForm(new PackerForm());
			packerForm->construct(xml);

			auto packer = packerForm->toPacker();
			auto gaParams = packerForm->getGAParameters();

			packer->addEventListener(library::ProgressEvent::PROGRESS, handleProgress, this);

			std::shared_ptr<WrapperArray> wrapperArray = packer->optimize(*gaParams);
			sendData(std::make_shared<protocol::Invoke>("setWrapperArray", wrapperArray->toXML()));
		};

		static void handleProgress(std::shared_ptr<library::Event> evt, void *lpVoid)
		{
			Client *client = (Client*)lpVoid;
			library::ProgressEvent *event = (library::ProgressEvent*)evt.get();

			client->sendData(std::make_shared<protocol::Invoke>("setProgress", event->getNumerator(), event->getDenominator()));
		};
	};
};
};