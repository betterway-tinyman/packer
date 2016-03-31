#pragma once
#include <samchon/protocol/IServer.hpp>
#include <samchon/protocol/IClient.hpp>

#include <samchon/protocol/Invoke.hpp>

#include <mutex>

namespace samchon
{
namespace example
{
namespace interaction
{
	using namespace std;

	using namespace library;
	using namespace protocol;

	/**
	 * @brief A boundary class interacting with a Chief system.
	 * 
	 * @details
	 * <p> ChiefDriver is a boundary class interacting with a chief system as a server. However, the
	 * ChiefDriver represents a weird server that accepts only a client, the chief system. </p>
	 * 
	 * <p> The ChiefDriver is built for providing a guidance for designing a boundary class which is
	 * representing an unusual system within framework of OOD, handling a network system like a
	 * software class of Object-Oriented Design. </p>
	 *
	 * <p> @image html  cpp/example_interaction.png
	 *	   @image latex cpp/example_interaction.png </p>
	 * 
	 * <p> @image html  conception/example_interaction_network_diagram.png
	 *	   @image latex conception/example_interaction_network_diagram.png </p>
	 *
	 * \par [Inherited] IServer
	 *		@copydetails IServer
	 *					 
	 * \par [Inherited] IClient
	 *		@copydetails IClient
	 * 
	 * @see protocol
	 * @author Jeongho Nam
	 */
	class ChiefDriver
		: public IServer,
		public IClient
	{
	private:
		typedef IServer super;

	protected:
		/**
		 * @brief A Master object containing the ChiefDriver.
		 */
		IProtocol *master;

		/**
		 * @brief A port number to open for the Chief system. 
		 */
		int port;

		/**
		 * @brief A mutex for realizing 1:1 server.
		 */
		mutex mtx;

	public:
		/**
		 * @brief Construct from master and port number.
		 * 
		 * @param master A master object associated with the chief system.
		 * @param port A port number to open for chief system.
		 */
		ChiefDriver(IProtocol *master, int port)
			: super(),
			IClient()
		{
			this->master = master;
			this->port = port;
		};
		virtual ~ChiefDriver() = default;

		virtual void addClient(Socket *socket) override
		{
			unique_lock<mutex> uk(mtx);

			this->socket = socket;
			listen();
		};

		virtual void replyData(shared_ptr<Invoke> invoke) override
		{
			master->replyData(invoke);
		};

	protected:
		virtual auto PORT() const -> int override
		{
			return port;
		};
	};			
};
};
};