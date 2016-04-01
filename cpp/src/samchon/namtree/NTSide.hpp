#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Entity.hpp>

#include <vector>

namespace samchon
{
namespace namtree
{
	class NTFactory;
	class NTFile;
	class NTIterator;

	/**
	 * @brief A side of a conditional expresson
	 *
	 * @details
	 * <p> @image html  cpp/result/namtree_nt_side.png
	 *	   @image latex cpp/result/namtree_nt_side.png </p>
	 *
	 * <p> Two NTSide classes are belonged to a NTCriteria as left and right side. The NTSide class has
	 * a NTFile representing a function and parameters to adjust to the function. </p>
	 *
	 * <p> @image html  cpp/namtree_criteria.png
	 *	   @image latex cpp/namtree_criteria.png </p>
	 *
	 * @see samchon::namtree
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API NTSide
		: public virtual protocol::Entity
	{
	private:
		typedef protocol::Entity super;

	protected:
		virtual auto TAG() const->std::string override;

		NTFactory *factory;
		NTFile *file;
		std::vector<double> parameters;

	public:
		NTSide(NTFactory*);
		virtual ~NTSide() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		virtual void initRetrieve();
		auto calcRetrieved(NTIterator&) const -> double;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};