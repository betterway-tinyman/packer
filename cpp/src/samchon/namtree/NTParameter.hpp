#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#include <samchon/namtree/NTParameterDetermined.hpp>
#include <samchon/namtree/INTExplore.hpp>

namespace samchon
{
namespace namtree
{
	/**
	 * @brief A metadata of a parameter in a function
	 * 
	 * @details
	 * <p> NTParameter is a class containing metadata of a parameter which is belonged to a class, metadata 
	 * of a function, NTFile. Between the NTFile and NTParameter(s), there's a associate relationship. </p>
	 *
	 * <p> @image html  cpp/namtree_file.png
	 *	   @image latex cpp/namtree_file.png </p>
	 *
	 * @see samchon::namtree
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API NTParameter
		: public virtual protocol::SharedEntityArray<NTParameterDetermined>,
		public INTExplore
	{
	private:
		typedef protocol::SharedEntityArray<NTParameterDetermined> super;

	protected:
		/**
		 * @brief Name of a paremeter.
		 * @details The string label is identifier of a NTParameter. It must be unique.
		 */
		std::string name;

		/**
		 * @brief An initial, default value
		 */
		double initialValue;

	public:
		/* -------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		NTParameter();
		virtual ~NTParameter() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>)->NTParameterDetermined* override;

	public:
		/* -------------------------------------------------------------------
			GETTERS
		------------------------------------------------------------------- */
		virtual auto key() const->std::string override;

		/**
		 * @brief Get name
		 */
		auto getName() const->std::string;

		/**
		 * @brief Get initial, default vlaue
		 */
		auto getInitialValue() const -> double;

		/* -------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------- */
		virtual auto TAG() const->std::string override;
		virtual auto CHILD_TAG() const->std::string override;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};