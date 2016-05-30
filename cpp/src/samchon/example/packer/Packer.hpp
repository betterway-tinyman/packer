#pragma once
#include <samchon/protocol/SharedEntityArray.hpp>
#include <samchon/example/packer/WrapperArray.hpp>

#include <samchon/library/CombinedPermutationGenerator.hpp>

#include <iostream>

namespace samchon
{
namespace example
{
namespace packer
{
	using namespace std;

	using namespace library;
	using namespace protocol;

	/**
	 * @brief A packer planning the best packaging.
	 *
	 * <p> @image html cpp/example_packer.png
	 * @image latex cpp/example_packer.png </p>
	 *
	 * @details
	 * <p> Retrieves the solution of packaging by combination permuation and factorial case. </p>
	 *
	 * @warning 
	 * <p> Be careful about number of products and wrappers. </p> 
	 * <p> The time complexity of Packer overs O(m^n). Elapsed time of calculation increases enourmously. 
	 * Do not use Packer if the digits of number of products or wrappers overs 2. </p>
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Packer
		: public SharedEntityArray<WrapperArray>
	{
	private:
		typedef SharedEntityArray<WrapperArray> super;

	protected:
		/**
		 * @brief Prodcut(s) to package in some Wrapper(s)
		 */
		shared_ptr<ProductArray> productArray;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 */
		Packer()
			: super()
		{
			this->productArray = make_shared<ProductArray>();
		};

		/**
		 * @brief Construct from products and wrapper
		 *
		 * @param productArray Product(s) to input some Wrapper
		 * @param wrapperArray Type of Wrapper(s) to be used
		 */
		Packer(shared_ptr<ProductArray> productArray)
			: super()
		{
			this->productArray = productArray;
		};

		/**
		 * @brief Copy Constructor
		 *
		 * @details 
		 * <p> Copy constructor of Packer does not copy children items. </p> 
		 * <p> Only copies member variables(productArray, wrapperArray) of Packer's. </p>
		 */
		Packer(const Packer &packer)
			: Packer(packer.productArray)
		{
			this->reserve(packer.size());

			for (size_t i = 0; i < packer.size(); i++)
				this->emplace_back(new WrapperArray(*packer[i]));
		};

		virtual void construct(shared_ptr<XML> xml) override
		{
			super::construct(xml);

			productArray->construct(xml->get(productArray->TAG())->at(0));
		};

	protected:
		virtual auto createChild(shared_ptr<XML>) -> WrapperArray* override
		{
			return new WrapperArray();
		};

	public:
		/* ---------------------------------------------------------
			GETTERS
		--------------------------------------------------------- */
		/**
		 * @brief Get number of products to package.
		 */
		auto productSize() const -> size_t
		{
			return productArray->size();
		};

		/**
		 * @brief Calculate price of the wrappers.
		 */
		auto calcPrice() const -> int
		{
			int price = 0;
			for (size_t i = 0; i < size(); i++)
				price += at(i)->calcPrice();

			return price;
		};

		/* ---------------------------------------------------------
			OPERATORS
		--------------------------------------------------------- */
		/**
		 * @brief Find the best packaging method.
		 */
		void optimize(size_t start = 0, size_t size = -1)
		{
			if(empty() == true || productArray->empty() == true)
				return;

			CombinedPermutationGenerator caseGenerator(this->size(), productArray->size());
			shared_ptr<Packer> minPacker = nullptr;

			if (size == -1 || start + size > caseGenerator.size())
				size = caseGenerator.size() - start;

			for (size_t i = start; i < start + size; i++)
			{
				vector<size_t> &row = caseGenerator[i];
				shared_ptr<Packer> packer(new Packer(*this));

				bool validity = true;

				for (size_t j = 0; j < row.size(); j++)
				{
					shared_ptr<Product> &product = productArray->at(j);
					shared_ptr<WrapperArray> &wrapperArray = packer->at( row[j] );

					if (wrapperArray->tryInsert(product) == false)
					{
						validity = false;
						break;
					}
				}

				if(validity == false)
					continue;

				//OPTIMIZE ALL WRAPPERS IN A PACKER
				for (size_t j = 0; j < packer->size(); j++)
					packer->at(j)->optimize();

				if (minPacker == nullptr ||
					packer->calcPrice() < minPacker->calcPrice())
				{
					minPacker = packer;
				}
			}

			//COPY
			assign(minPacker->begin(), minPacker->end());
		};

		/* ---------------------------------------------------------
			EXPORT
		--------------------------------------------------------- */
		virtual auto TAG() const -> string override
		{
			return "packer";
		};
		virtual auto CHILD_TAG() const -> string override
		{
			return "wrapperArray";
		};

		virtual auto toXML() const -> shared_ptr<XML> override
		{
			shared_ptr<XML> &xml = super::toXML();
			xml->push_back(productArray->toXML());

			return xml;
		};

		/**
		 * @brief Return a string represents an packaging method.
		 */
		auto toString() const -> string
		{
			string str = "$" + std::to_string(calcPrice()) + "\n";
			for (size_t i = 0; i < size(); i++)
				str += at(i)->toString() + "\n";

			return str;
		};

		/* ---------------------------------------------------------
			MAIN
		--------------------------------------------------------- */
		static void main()
		{
			// CONSTRUCT PRODUCTS
			shared_ptr<ProductArray> productArray(new ProductArray());
			productArray->emplace_back(new Product("Eraser", 500, 10, 70));
			productArray->emplace_back(new Product("Pencil", 400, 30, 35));
			/*productArray->emplace_back(new Product("Pencil", 400, 30, 35));
			productArray->emplace_back(new Product("Pencil", 400, 30, 35));
			productArray->emplace_back(new Product("Book", 8000, 150, 300));*/
			productArray->emplace_back(new Product("Book", 8000, 150, 300));
			productArray->emplace_back(new Product("Drink", 1000, 75, 250));
			productArray->emplace_back(new Product("Umbrella", 4000, 200, 1000));
			productArray->emplace_back(new Product("Notebook-PC", 800000, 150, 850));
			productArray->emplace_back(new Product("Tablet-PC", 600000, 120, 450));

			// CONSTRUCT PACKER
			Packer packer(productArray);
			packer.emplace_back(new WrapperArray("Large", 100, 200, 1000));
			packer.emplace_back(new WrapperArray("Medium", 70, 150, 500));
			packer.emplace_back(new WrapperArray("Small", 50, 100, 250));

			// OPTIMIZE
			packer.optimize();

			// PRINT
			cout << packer.toString();
		};
	};			
};
};
};