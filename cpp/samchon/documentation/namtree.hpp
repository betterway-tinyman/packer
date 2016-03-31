#pragma once

namespace samchon
{
	/**
	 * @brief Package of Nam-Tree
	 *
	 * @details
	 * <p> Nam-Tree is a module of tree-strucrued complicate logical condition model for realizing A.N.N.
	 * (Artificial Neural Network). You can make the ANN model with weight and bias and express not only 
	 * logical condition by your hand, but also construct the logical condition by automaticaly with 
	 * optimization of genetic algorithm and grid method. </p>
	 *
	 * <p> @image html  cpp/result/namtree.png
	 *	   @image latex cpp/result/namtree.png </p>
	 *
	 * <p> @image html  cpp/result/namtree_back_testing.png
	 *	   @image latex cpp/result/namtree_back_testing.png </p>
	 *
	 * <p> The basic pricinple of Nam-Tree are follow: </p>
	 *
	 * <ol>
	 *	<li> A conditional expression with weight </li>
	 *		<p> F(x) = (NTSide < NTSide ? 1: 0) X weight </p>
	 *		<p> NTCrietria is made up for conditional expression. </p>
	 *		<ul>
	 *			<li> When the expression is true, returns 1 * weight </li>
	 *			<li> When the expression is false, returns 0 </li>
	 *		</ul>
	 *	<li> Hierarchical relationship </li>
	 *		<ul>
	 *			<li> In vertical relationship: multiply (X) </li>
	 *			<li> In horizontal relationship: plus (+) </li>
	 *		</ul>
	 *
	 *		<p> With that rule, you can make enormous conditions. I can sure 
	 *		there's not any condition can'be be expressed by the model. </p>
	 *
	 *	<li> Making bias </li>
	 *		<p> Just make a NTCriteria returns only true. </p>
	 *		<p> It's the bias returns only weight. </p>
	 *
	 *	<li> Explore </li>
	 *	<ol>
	 *		<li> Exporing in a NTCriteria (optimize a side) </li>
	 *			<p> Make a NTSide to be nullptr, then NTCriteria will explore the best value.</p>
	 *
	 *			<p> Nam-Tree will calculate the condtions from minimum to maximum in INTExplore
	 *			reach to the precision, by the method of multi-demensional grid. </p>
	 *
	 *		<li> Exploring parameter in NTSide </li>
	 *			<p> If you set the parameterMNap to be empty, Nam-Tree will explore the best parameter 
	 *			until reach to the precision in INTExploreParameter from minimum to maximum. </p>
	 *
	 *			<p> @image html  cpp/result/namtree_grid_method.png
	 *			    @image latex cpp/result/namtree_grid_method.png </p>
	 *
	 *		<li> Exploring by creating NTCriteria </li>
	 *			<p> (Creates lots of hierarchical conditions and tests them) </p>
	 *
	 *			<p> If all the side (left and right) in a NTCriteria are nullptr, the NTCriteria
	 *			will make a lot of children NTCriteria(s) to test lots of cases, so that derives
	 *			the best condition set will be made up. </p>
	 *
	 *			<p> This process will ride on same routines of 4-1 and 4-2, for each created
	 *			cases. Of course, this process needs too much time, so that you may need to be
	 *			patient. </p>
	 *	</ol>
	 * </ol>
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
	 * @author Jeongho Nam <http://samchon.org>
	 */
	namespace namtree
	{
		//THIS HEADER FILE IS ONLY FOR DOCUMENTATION
	};
};