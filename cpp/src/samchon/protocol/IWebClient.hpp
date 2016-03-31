#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IWebClientBase.hpp>

namespace samchon
{
namespace protocol
{
	template<ENUM_DIRECTION _Direction>
	class IWebClient
		: public IWebClientBase
	{
	private:
		typedef IWebClientBase super;

	public:
		IWebClient()
			: super()
		{
		};
		virtual ~IWebClient() = default;

	protected:
		virtual auto DIRECTION() const -> ENUM_DIRECTION final
		{
			return _Direction;
		};
	};

	SAMCHON_FRAMEWORK_EXTERN template class SAMCHON_FRAMEWORK_API IWebClient<SERVER>;
	SAMCHON_FRAMEWORK_EXTERN template class SAMCHON_FRAMEWORK_API IWebClient<CLIENT>;
};
};