#pragma once
#include <samchon/example/packer/Instance.hpp>

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
	* @brief A product, merchandise
	*
	* @details 
	* <p> A merchandise with its price, volume and weight. </p>
	*
	* <p> @image html cpp/example_packer.png
	* @image latex cpp/example_packer.png </p>
	*
	* @author Jeongho Nam <http://samchon.org>
	*/
	class Product
		: public Instance
	{
	private:
		typedef Instance super;

	public:
		/* ---------------------------------------------------------
		CONSTRUCTOR
		--------------------------------------------------------- */
		/**
		* @brief Defualt Constructor
		*/
		Product()
			: super()
		{
		};

		/**
		* @brief Construct from arguments
		*
		* @param name Name of a product can be an identifier
		* @param price Price of the product
		* @param volume Volume of the product
		* @param weight Weight of the product
		*/
		Product(const string &name, int price, int volume, int weight)
			: super(name, price, volume, weight)
		{
		};

		virtual ~Product() = default;

		/* ---------------------------------------------------------
		EXPORT
		--------------------------------------------------------- */
		virtual auto TAG() const -> string override
		{
			return "product";
		};

		/**
		* @brief Return a string represents the Instance
		*/
		virtual auto toString() const -> string override
		{
			return "Product " + super::toString();
		};
	};			
};
};
};