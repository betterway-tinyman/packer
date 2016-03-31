#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/ExternalSystemArray.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An array of ExternalServer(s)
	 *
	 * @details
	 * <p> ExternalServerArray is an ExternalSystemArray specialized in servers' driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails protocol::ExternalSystemArray
	 */
	class SAMCHON_FRAMEWORK_API ExternalServerArray
		: public virtual ExternalSystemArray
	{
	protected:
		typedef ExternalSystemArray super;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		ExternalServerArray();
		virtual ~ExternalServerArray() = default;

		virtual void start() override;
	};
};
};