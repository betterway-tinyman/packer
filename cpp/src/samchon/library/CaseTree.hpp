pragma once
#include <samchon/API.hpp>

#include <vector>

namespace samchon
{
	namespace library
	{
		class CaseTree
			: public std::vector<CaseTree*>
		{
		protected:
			CaseTree *topTree;
			size_t index;
			size_t level;

		public:
			CaseTree(size_t indexSize, size_t levelSize);
			virtual ~CaseTree();

			auto toMatrix() const -> std::vector<std::vector<size_t>>;

		protected:
			CaseTree(CaseTree*, size_t, size_t);

			virtual auto MATRIX_SIZE() const -> size_t = 0;
			auto indexSize() const -> size_t;
			auto levelSize() const -> size_t;

			void fetchMatrix(std::vector<std::vector<size_t>>&, std::vector<size_t>) const;
		};
	};
};