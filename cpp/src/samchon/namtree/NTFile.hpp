#pragma once
#include <samchon/API.hpp>

#include <samchon/library/FTFile.hpp>
#include <samchon/namtree/INTExplore.hpp>

#include <functional>
#include <vector>

namespace samchon
{
namespace namtree
{
	class NTFactory;
	class NTParameterArray;
	class NTIterator;

		/**
		 * @brief A file archiving metadata of a function.
		 *
		 * @details
		 * <p> @image html  cpp/result/namtree_nt_file.png
		 *	   @image latex cpp/result/namtree_nt_file.png </p>
		 *
		 * <p> NTFile class is a FTFile archiving meta of a function which is used by a NTSide class. It may
		 * better to understand what NTFile is, if you consider the NTFile as a header of a function. Name, 
		 * metadata of parameters and function pointer is archived on the NTFile. </p>
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
		 * @see namtree::NTSide
		 * @author Jeongho Nam <http://samchon.org>
		 */
	class SAMCHON_FRAMEWORK_API NTFile
		: public library::FTFile,
		public INTExplore
	{
	private:
		typedef library::FTFile super;

	protected:
		typedef std::function<double(const NTIterator&, const std::vector<double> &)> SideFunction;

		/**
		 * @brief A factory and manager class for nam-tree.
		 *
		 * @details NTFile references NTFactory to find otherside file and function pointer.
		 */
		NTFactory *factory; //TO FIND OTHERSIDE AND FUNCTION POINTER

		/**
		 * @brief Metadata of parameters
		 */
		NTParameterArray *parameterArray;

		/**
		 * @brief A file of otherside
		 *
		 * @details
		 * <p> The variable otherside is a pair of file which is inserted to a criteria's opposite side. </p>
		 *
		 * <p> When the otherside is specified and you determine the NTFile to be contained in a NTSide,
		 * the NTCriteria owning the NTSide will determine opposite side's NTSide to have the otherside
		 * NTFile forcibly. </p>
		 *
		 * @note
		 * <p> The determination will be automatic and unchangeble. Consider hardly when specifying an
		 * otherside. </p>
		 */
		NTFile *otherside;

		/**
		 * @brief A function pointer
		 */
		SideFunction function;

	public:
		/* -------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------- */
		/**
		 * @brief Construct from factory and parent folder.
		 *
		 * @param factory A factory class creating nam-tree objects.
		 * @param folder A parent folder of the NTFile.
		 */
		NTFile(NTFactory*, library::FTFolder*);
		virtual ~NTFile() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		/* -------------------------------------------------------------------
			GETTERS
		------------------------------------------------------------------- */
		/**
		 * @brief Get metadata of parameters
		 */
		auto getParameterArray() const->NTParameterArray*;

		/**
		 * @brief Get otherside file
		 */
		auto getOtherside() const->NTFile*;

		/**
		 * @brief Get function pointer
		 */
		auto getFunction() const->SideFunction;

		/* -------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------- */
		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};