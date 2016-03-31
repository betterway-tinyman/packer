#pragma once
#include <samchon/API.hpp>

#include <samchon/library/FTFactory.hpp>

#include <functional>
#include <samchon/Dictionary.hpp>

namespace samchon
{
namespace namtree
{
	class NTCriteria;
	class NTSide;

	class NTEntityGroup;
	class NTIterator;

	/** 
	 * @brief A factory for Nam-Tree objects.
	 *
	 * @details
	 * <p> NTFactory class is a FTFactory and an abstract class creating and managing Nam-Tree objects. 
	 * When overriding nam-tree module, do not override methods <i>createChild()</i> of file or criteria
	 * objects like NTFile, NTCriteria and NTSide. Only overrides method of creation in the NTFactory. </p>
	 *
	 *	\li Creator method of history data is not in NTFactory. It must be done by hand of user.
	 *
	 * <p> @image html  cpp/namtree_criteria.png
	 *	   @image latex cpp/namtree_criteria.png </p>
	 *
	 * <p> @image html  cpp/namtree_file.png
	 *	   @image latex cpp/namtree_file.png </p>
	 *
	 * @note
	 * <p> Previous version of the Samchon Framework, NTFile class had a function script on the NTFile
	 * and Nam-Tree module compiled the script like Nam-Tree moudle of Flex standalone that is keeping 
	 * the compiling method. </p>
	 *
	 * <p> However, modern Samchon Framework's C++ NTFile class doesn't have a function script and does 
	 * not compile. I don't know a way to compile a script in lots of operating systems, so I dropped 
	 * the compiling method. Until update, NTFactory has function pointers in a Dictionary and NTFile 
	 * reference them. Users must put own methods into the Dictionary by their hands. </p>
	 * 
	 * <p> I will solve the problem sooon. Until next generation of Samchon Framework, put your own 
	 * function pointers to a Dictionary in NTFactory by your hand please. Sorry for my unripe skill on
	 * programming. </p>
	 *
	 * @see samchon::namtree
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API NTFactory
		: public library::FTFactory
	{
	private:
		typedef library::FTFactory super;

	protected:
		typedef std::function<double(const NTIterator&, const std::vector<double> &)> SideFunction;

		/**
		 * @brief A entity gorup containing historical data.
		 */
		NTEntityGroup *data;

		/**
		 * @brief A dictionary of function pointers.
		 */
		Dictionary<SideFunction> sideFunctionMap;

	public:
		/**
		 * @brief Construct from historical(studying) data.
		 *
		 * @param entity A group of history data.
		 */
		NTFactory(NTEntityGroup*);
		virtual ~NTFactory() = default;

		virtual auto createFile(library::FTFolder*, std::shared_ptr<library::XML>)->library::FTFile* override;

		/**
		 * @brief Factory method of a NTCriteria
		 *
		 * @param parent A parent NTCriteria object
		 * @oaram xml An xml object for discriminating type.
		 * @return A NTCrteria object representing a logical conditional.
		 */
		virtual auto createCriteria(NTCriteria*, std::shared_ptr<library::XML>)->NTCriteria*;

		/**
		 * @brief Factory method of a NTSide
		 *
		 * @param xml An xml oject for distriminating type.
		 * @return A NTSide object which means a side of a logical condition.
		 */
		virtual auto createSide(std::shared_ptr<library::XML>)->NTSide*;
	};
};
};