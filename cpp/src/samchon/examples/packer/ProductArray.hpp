#pragma once
#include <samchon/examples/packer/Product.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>

namespace samchon
{
namespace examples
{
namespace packer
{
	using namespace std;

	using namespace library;
	using namespace protocol;

	/**
	 * @brief An array of Product objects.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class ProductArray
		: public SharedEntityArray<Product>
	{
	private:
		typedef SharedEntityArray<Product> super;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		using super::super;
		virtual ~ProductArray() = default;

	protected:
		virtual auto createChild(shared_ptr<XML>) -> Product* override
		{
			return new Product();
		};

	public:
		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto TAG() const -> string override
		{
			return "productArray";
		};
		virtual auto CHILD_TAG() const -> string override
		{
			return "product";
		};
	};			
};
};
};