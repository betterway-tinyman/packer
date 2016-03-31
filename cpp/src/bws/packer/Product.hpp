#pragma once
#include <bws/packer/API.hpp>

#include <bws/packer/Instance.hpp>

namespace bws
{
namespace packer
{
	/**
	 * @brief A product.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Product
		: public virtual Instance
	{
	private:
		typedef Instance super;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		using super::super;
		virtual ~Product() = default;

	protected:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TYPE() const -> std::string override;
	};
};
};