#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Entity.hpp>

namespace samchon
{
namespace namtree
{
	/**
	 * @brief A pre-determined value of a parameter
	 *
	 * @details
	 * <p> @image html  cpp/result/namtree_nt_parameter_determined.png
	 *	   @image latex cpp/result/namtree_nt_parameter_determined.png </p>
	 *
	 * <p> NTParameterDetermined is a class belonged to a NTParameter object presents a pre-defined value.
	 * In UI module of Nam-Tree, the NTParameterDetermined is expressed by an item of a ComboBox. </p>
	 *
	 * <p> When optimization of Nam-Tree, in a part of NTParameter, if the pre-determined value is defined
	 * optimizer does not explore with grid method from referencing INTExplore. Just selects a value between 
	 * the pre-defineds. (One of NTParameterDetermined is selected in NTParameter). </p>
	 *
	 * <p> @image html  cpp/namtree_file.png
	 *	   @image latex cpp/namtree_file.png </p>
	 *
	 * @see samchon::namtree
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API NTParameterDetermined
		: public virtual protocol::Entity
	{
	private:
		typedef protocol::Entity super;

	protected:
		/**
		 * @brief A label will be printed in a combo-box.
		 * @details The string label is identifier of a NTParameterDetermined. It must be unique.
		 */
		std::string label;

		/**
		 * @brief A value, pre-defined
		 */
		double value;

	public:
		/* -------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		NTParameterDetermined();
		virtual ~NTParameterDetermined() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		/* -------------------------------------------------------------------
			GETTERS
		------------------------------------------------------------------- */
		virtual auto key() const->std::string override;

		/**
		 * @brief Get label
		 */
		auto getLabel() const->std::string;

		/**
		 * @brief Get value
		 */
		auto getValue() const -> double;

		/* -------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------- */
		virtual auto TAG() const->std::string override;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};