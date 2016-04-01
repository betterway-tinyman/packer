#pragma once
#include <samchon/example/interaction/Slave.hpp>

#include <samchon/example/packer/Packer.hpp>
#include <samchon/protocol/Invoke.hpp>

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
	 * @brief A slave system for solving Packer.
	 * 
	 * @details
	 * \par [Inherited]
	 *		@copydetails slave::ParallelClient 
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class PackerSlave
		: public Slave
	{
	private:
		typedef Slave super;

	public:
		/* ---------------------------------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------------------------------- */
		/**
		 * @brief Construct from ip and port of the master.
		 *
		 * @param ip IP address of the master.
		 * @param port Port number of the master.
		 */
		PackerSlave(const string &ip, int port)
			: super(ip, port)
		{
		};
		virtual ~PackerSlave() = default;

	protected:
		/* ---------------------------------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------------------------------- */
		/**
		 * @brief Optimize TSP and report the result.
		 * 
		 * @param xml XML object representing a Travel.
		 * @param index Starting index of a segmentation allocated to the Slave.
		 * @param size Size of the segmentation.
		 */
		virtual void optimize(shared_ptr<XML> xml, size_t index, size_t size) override
		{
			super::optimize(xml, index, size);

			packer::Packer packer;
			packer.construct(xml);
			packer.optimize(index, size);

			cout << packer.toString() << endl << endl;
			sendOptimization(packer.toXML());
		};

	public:
		/* ---------------------------------------------------------------------------------
			MAIN
		--------------------------------------------------------------------------------- */
		/**
		 * @brief Main function.
		 */
		static void main()
		{
			string ip;
			int port;

			cout << "----------------------------------------------------------------------------" << endl;
			cout << "	PACKER SLAVE" << endl;
			cout << "----------------------------------------------------------------------------" << endl;
			cout << "	ip: ";		cin >> ip;
			cout << "	port: ";	cin >> port;

			PackerSlave slave(ip, port);
			slave.start();
		};
	};			
};
};
};