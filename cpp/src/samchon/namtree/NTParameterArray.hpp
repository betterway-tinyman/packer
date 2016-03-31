#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#include <samchon/namtree/NTParameter.hpp>

namespace samchon
{
namespace namtree
{
	/**
	 * @brief An Array of NTParameter
	 *
	 * @details
	 * <p> NTParameterArray is an array class containing NTParameter objects. </p>
	 * <p> The NTParameter is belonged to a NTFile class to realize associate relationship between 
	 * NTFile and NTParameter(s). </p>
	 *
	 * <p> @image html  cpp/namtree_file.png
	 *	   @image latex cpp/namtree_file.png </p>
	 *
	 * @see samchon::namtree
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API NTParameterArray
		: public virtual protocol::SharedEntityArray<NTParameter>
	{
	private:
		typedef protocol::SharedEntityArray<NTParameter> super;

	public:
		virtual auto TAG() const->std::string override;
		virtual auto CHILD_TAG() const->std::string override;

		/**
		 * @brief Default Constructor.
		 */
		NTParameterArray();
		virtual ~NTParameterArray() = default;

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>)->NTParameter* override;
	};
};
};