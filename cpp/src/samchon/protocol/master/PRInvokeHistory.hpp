#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/InvokeHistory.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class ParallelSystem;

	class PRInvokeHistory
		: public InvokeHistory
	{
		friend class ParallelSystem;

	private:
		typedef InvokeHistory super;

		size_t index;
		size_t size;

	public:
		PRInvokeHistory();
		PRInvokeHistory(std::shared_ptr<Invoke>);

		virtual ~PRInvokeHistory();

		auto getIndex() const -> size_t
		{
			return index;
		};

		auto getSize() const -> size_t
		{
			return size;
		};
	};
};
};
};