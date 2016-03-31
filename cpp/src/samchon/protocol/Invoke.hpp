#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#include <samchon/protocol/InvokeParameter.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief Standard message of network I/O
	 *
	 * @details
	 * <p> Invoke is a class used in network I/O in protocol package of Samchon Framework.  </p>
	 *
	 * <p> The Invoke message has an XML structure like the result screen of provided example in below. 
	 * We can enjoy lots of benefits by the normalized and standardized message structure used in
	 * network I/O. </p>
	 *
	 * <p> The greatest advantage is that we can make any type of network system, even how the system 
	 * is enourmously complicated. As network communication message is standardized, we only need to
	 * concentrate on logical relationships between network systems. We can handle each network system 
	 * like a object (class) in OOD. And those relationships can be easily designed by using design
	 * pattern. </p>
	 *
	 * <p> In Samchon Framework, you can make any type of network system with basic 3 + 1 componenets
	 * (IProtocol, IServer and IClient + ServerConnector), by implemens or inherits them, like designing
	 * classes of S/W architecture. </p>
	 *
	 * <p> @image html  cpp/protocol_invoke.png
	 *	   @image latex cpp/protocol_invoke.png </p>
	 * 
	 * \par Example Sources
	 *	\li example::invoke
	 *	\li example::interaction
	 *
	 * \par [Inherited]
	 *		@copydetails protocol::EntityGroup
	 * 
	 * @see protocol::IProtocol
	 * @see protocol::InvokeParameter
	 * @see samchon::protocol
	 * 		
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API Invoke
		: public SharedEntityArray<InvokeParameter>
	{
	private:
		typedef SharedEntityArray<InvokeParameter> super;

	protected:
		/**
		 * @brief Represent who listens, often be a function name
		 */
		std::string listener;

	public:
		/* --------------------------------------------------------------------
			CONSTRUCTORS
		-------------------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 */
		Invoke();
		virtual ~Invoke() = default;

		/**
		 * @brief Construct from a listener
		 *
		 * @param listener Represents who listens the Invoke message. Almost same with Function name
		 */
		Invoke(const std::string &listener);

		virtual void construct(std::shared_ptr<library::XML>) override;

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>)->InvokeParameter* override;

		/* --------------------------------------------------------------------
			VARIADIC CONSTRUCTORS
		-------------------------------------------------------------------- */
	public:
		/**
		 * @brief Construct from arguments
		 *
		 * @tparam _Ty Type of an argument which represents a parameter
		 * @tparam _Args Left varadic template arguments' types
		 *
		 * @details
		 * <p> Creates Invoke and InvokeParameter(s) at the same time by varadic template method. </p>
		 * <p> By the varadic template constructor, you can't specify name of each InvokeParameter, but
		 * specify type and value of each InvokeParameter. If you try to record the Invoke to Database,
		 * the name of InvokeParameter will be <i>NULL</i>.</p>
		 *
		 * @note
		 * <p> By the varadic template constructor, name of InovkeParameter(s) will be omitted. Because
		 * of name, an identifier of an InvokeParameter, is omitted, you can't access to InvokeParameter
		 * by Invoke::has() or Invoke::get(). </p>
		 *
		 * @param listener A string represents who listens the Invoke message. Almost same with name of a function.
		 * @param val A value to be a parameter of Invoke
		 * @param args Left arguments to be parameters of Invoke
		 *
		 */
		template <typename _Ty, typename ... _Args>
		Invoke(const std::string &listener, const _Ty &val, const _Args& ... args)
			: Invoke(listener)
		{
			construct_by_vardic_template(val);
			construct_by_vardic_template(args...);
		};

		template <typename _Ty>
		Invoke(const std::string &listener, const _Ty &val)
			: Invoke(listener)
		{
			construct_by_vardic_template(val);
		};

	private:
		template <typename _Ty, typename ... _Args>
		void construct_by_vardic_template(const _Ty &val, const _Args& ... args)
		{
			construct_by_vardic_template(val);
			construct_by_vardic_template(args...);
		};

		template <typename _Ty>
		void construct_by_vardic_template(const _Ty &val)
		{
			emplace_back(new InvokeParameter("", val));
		};

	public:
		/* -----------------------------------------------------------------------
			GETTERS
		----------------------------------------------------------------------- */
		/**
		 * @brief Get listener
		 */
		auto getListener() const->std::string;

		/**
		 * @brief Set listener
		 */
		void setListener(const std::string &);

		/* -----------------------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------------------- */
		virtual auto TAG() const->std::string override;
		virtual auto CHILD_TAG() const->std::string override;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;

		/**
		 * @brief Get a string of sql statement used to archive history log
		 */
		auto toSQL() const->std::string;
	};
};
};