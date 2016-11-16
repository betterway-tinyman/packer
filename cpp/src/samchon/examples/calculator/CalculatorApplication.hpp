#pragma once
#include <samchon/protocol/IProtocol.hpp>

#include <iostream>
#include <functional>
#include <memory>

#include <samchon/library/StringUtil.hpp>
#include <samchon/protocol/ServerConnector.hpp>

namespace samchon
{
namespace examples
{
namespace calculator
{
	class CalculatorApplication
		: public virtual protocol::IProtocol
	{
	private:
		std::shared_ptr<protocol::ServerConnector> connector;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		CalculatorApplication()
		{
			connector.reset(new protocol::ServerConnector(this));
			connector->connect("127.0.0.1", 17823);
		};
		virtual ~CalculatorApplication() = default;

		/* ------------------------------------------------------------------
			INVOKE MESSAGE CHAIN
		------------------------------------------------------------------ */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			connector->sendData(invoke);
		};

		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			std::function<void(double, double, double)> listener;
			double x = invoke->at(0)->getValue<double>();
			double y = invoke->at(1)->getValue<double>();;
			double ret = invoke->at(2)->getValue<double>();

			if (invoke->getListener() == "printPlus")
				listener = std::bind(&CalculatorApplication::printPlus, this);
			else if (invoke->getListener() == "printMinus")
				listener = std::bind(&CalculatorApplication::printMinus, this);
			else if (invoke->getListener() == "printMultiply")
				listener = std::bind(&CalculatorApplication::printMultiply, this);
			else if (invoke->getListener() == "printDivide")
				listener = std::bind(&CalculatorApplication::printDivide, this);

			listener(x, y, ret);
		};

	private:
		/* ------------------------------------------------------------------
			PROCEDURES
		------------------------------------------------------------------ */
		void printPlus(double x, double y, double ret)
		{
			std::cout << 
				library::StringUtil::substitute("{1} + {2} = {3}", x, y, ret) 
				<< std::endl;
		};
		void printMinus(double x, double y, double ret)
		{
			std::cout << 
				library::StringUtil::substitute("{1} - {2} = {3}", x, y, ret) 
				<< std::endl;
		};
		void printMultiply(double x, double y, double ret)
		{
			std::cout <<
				library::StringUtil::substitute("{1} * {2} = {3}", x, y, ret)
				<< std::endl;
		};
		void printDivide(double x, double y, double ret)
		{
			std::cout << 
				library::StringUtil::substitute("{1} / {2} = {3}", x, y, ret)
				<< std::endl;
		};

	public:
		/* ------------------------------------------------------------------
			STATIC MAIN
		------------------------------------------------------------------ */
		static void main()
		{
			CalculatorApplication();
		};
	};
};
};
};