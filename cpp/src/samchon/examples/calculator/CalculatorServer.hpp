#pragma once
#include <samchon/protocol/Server.hpp>

#include <unordered_set>
#include <samchon/library/RWMutex.hpp>

namespace samchon
{
namespace examples
{
namespace calculator
{
	class CalculatorServer;
	class CalculatorClient;

	class CalculatorClient 
		: public protocol::IProtocol
	{
	private:
		CalculatorServer *server;
		std::shared_ptr<protocol::ClientDriver> driver;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		CalculatorClient(CalculatorServer *server, std::shared_ptr<protocol::ClientDriver> driver)
		{
			this->server = server;
			this->driver = driver;
		};
		virtual ~CalculatorClient() = default;

		/* ------------------------------------------------------------------
			INVOKE MESSAGE CHAIN
		------------------------------------------------------------------ */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			driver->sendData(invoke);
		};
		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			if (invoke->getListener() == "computePlus")
			{
				double x = invoke->front()->getValue<double>();
				double y = invoke->front()->getValue<double>();

				computePlus(x, y);
			}
			else if (invoke->getListener() == "computeMinus")
			{
				double x = invoke->front()->getValue<double>();
				double y = invoke->front()->getValue<double>();

				computeMinus(x, y);
			}
			else
				((protocol::IProtocol*)server)->replyData(invoke);
		};

	private:
		/* ------------------------------------------------------------------
			PROCEDURES
		------------------------------------------------------------------ */
		void computePlus(double x, double y)
		{
			sendData(std::make_shared<protocol::Invoke>("printPlus", x, y, x + y));
		};
		void computeMinus(double x, double y)
		{
			sendData(std::make_shared<protocol::Invoke>("printPlus", x, y, x - y));
		};
	};

	class CalculatorServer 
		: public protocol::Server,
		public protocol::IProtocol
	{
	private:
		std::unordered_set<std::shared_ptr<CalculatorClient>> clients;
		library::RWMutex mtx;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		CalculatorServer() : protocol::Server()
		{
		};
		virtual ~CalculatorServer() = default;

	protected: 
		void addClient(std::shared_ptr<protocol::ClientDriver> driver) override
		{
			// CREATE CLIENT OBJECT AND ENROLL TO SET CONTAINER
			std::shared_ptr<CalculatorClient> client(new CalculatorClient(this, driver));
			library::UniqueWriteLock uk(mtx);
			{
				clients.insert(client);
			}
			uk.unlock();
			
			// LISTEN FROM THE CLIENT
			driver->listen(client.get());
			
			// WHEN DISCONNECTED, ERASE CLIENT OBJECT FROM THE SET CONTAINER
			uk.lock();
			clients.erase(client);
		};

	public:
		/* ------------------------------------------------------------------
			INVOKE MESSAGE CHAIN
		------------------------------------------------------------------ */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke)
		{
			std::vector<std::thread> threads;
			library::UniqueReadLock uk(mtx);
			{
				threads.reserve(clients.size());
				for (auto it = clients.begin(); it != clients.end(); it++)
					threads.emplace_back(&CalculatorClient::sendData, it->get());
			}
			uk.unlock();

			for (auto it = threads.begin(); it != threads.end(); it++)
				it->join();
		};
		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke)
		{
			if (invoke->getListener() == "computeMultiply")
			{
				double x = invoke->front()->getValue<double>();
				double y = invoke->front()->getValue<double>();

				computeMultiply(x, y);
			}
			else if (invoke->getListener() == "computeDivide")
			{
				double x = invoke->front()->getValue<double>();
				double y = invoke->front()->getValue<double>();

				computeDivide(x, y);
			}
		};

	private:
		/* ------------------------------------------------------------------
			PROCEDURES
		------------------------------------------------------------------ */
		void computeMultiply(double x, double y)
		{
			sendData(std::make_shared<protocol::Invoke>("printMultiply", x, y, x * y));
		};
		void computeDivide(double x, double y)
		{
			sendData(std::make_shared<protocol::Invoke>("printDivide", x, y, x / y));
		};

	public:
		/* ------------------------------------------------------------------
			MAIN
		------------------------------------------------------------------ */
		static void main()
		{
			CalculatorServer server;
			server.open(17823);
		};
	};
};
};
};