#pragma once
#include <samchon/example/interaction/Master.hpp>

#include <samchon/example/interaction/ChiefDriver.hpp>
#include <samchon/example/interaction/SlaveDriver.hpp>
#include <samchon/protocol/Invoke.hpp>

#include <samchon/example/packer/Packer.hpp>
#include <samchon/library/CombinedPermutationGenerator.hpp>

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
	 * @brief A master of parallel system solving packaging problem.
	 *
	 * @details
	 * \par [Inherited]
	 *		@copydetails interaction::Master
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class PackerMaster
		: public Master
	{
	private:
		typedef Master super;

	protected:
		/**
		 * @brief A packer solver.
		 */
		shared_ptr<packer::Packer> packer;

	public:
		/* ---------------------------------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		PackerMaster()
			: super(37300)
		{
			packer = make_shared<packer::Packer>();
		};
		virtual ~PackerMaster() = default;

	protected:
		/* ---------------------------------------------------------------------------------
			INVOKE MESSATE CHAIN
		--------------------------------------------------------------------------------- */
		virtual void optimize(shared_ptr<XML> xml) override
		{
			super::optimize(nullptr);

			packer->construct(xml);

			CombinedPermutationGenerator caseGen(packer->size(), packer->productSize());

			sendSegmentData
			(
				make_shared<Invoke>
				(
					"optimize", 
					packer->toXML()
				), 
				caseGen.size()
			);
		};
		virtual void replyOptimization(shared_ptr<XML> xml) override
		{
			unique_lock<mutex> uk(mtx);

			shared_ptr<packer::Packer> packer(new packer::Packer());
			packer->construct(xml);

			cout << "An optimization process from a slave system has completed" << endl;
			cout << "\tOrdinary minimum price: " << this->packer->calcPrice()
				<< ", New Price from the slave: " << packer->calcPrice() << endl;

			if (this->packer->calcPrice() == 0 || packer->calcPrice() < this->packer->calcPrice())
				this->packer = packer;

			if (++optimized < this->size())
				return;

			cout << "Parallel optimization has completed." << endl;
			cout << packer->toString() << endl << endl;

			chiefDriver->sendData( make_shared<Invoke>("replyOptimization", packer->toXML()) );
		};

		/* ---------------------------------------------------------------------------------
			MAIN
		--------------------------------------------------------------------------------- */
	public:
		/**
		 * @brief Main functino.
		 */
		static void main()
		{
			cout << "----------------------------------------------------------------------------" << endl;
			cout << "	PACKER MASTER" << endl;
			cout << "----------------------------------------------------------------------------" << endl;

			PackerMaster master;
			master.start();
		};
	};			
};
};
};