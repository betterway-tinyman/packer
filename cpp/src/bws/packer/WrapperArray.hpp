#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#	include <bws/packer/Wrapper.hpp>

namespace bws
{
namespace packer
{
	/**
	 * @brief An array of Wrapper objects.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class WrapperArray
		: public protocol::SharedEntityArray<Wrapper>
	{
	private:
		typedef protocol::SharedEntityArray<Wrapper> super;
	
	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		using super::super;
		virtual ~WrapperArray() = default;

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>) -> Wrapper* override;

	public:
		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * @brief Get (calculate) price.
		 */
		virtual auto getPrice() const -> double;

		/**
		 * @brief Get (calculate) utilization rate.
		 */
		virtual auto getUtilization() const -> double;

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string override;
		virtual auto CHILD_TAG() const -> std::string override;
	};
};
};