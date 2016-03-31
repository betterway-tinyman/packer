#pragma once

namespace samchon
{
	/**
	 * @brief Package of libraries
	 *
	 * @details
	 * <p> The library package supports some classes for package of protocol and nam-tree. </p>
	 *
	 * <p> Furthermore, library package provides some utility libraries, which are not being supported
	 * by standard library (STL). Those can be helpful for users and those are like STL, independent 
	 * from operating systems, so that can compile in any operating system. </p>
	 *
	 * <p> The library package can be divided into some sectinos. </p>
	 * <ul>
	 * <li><h3> Common libraries. </h3></li>
	 *		<p> Common containers with utility methods. </p>
	 *
	 *		@image html cpp/library_common.png 
	 *		@image latex cpp/library_common.png
	 *	
	 *	<li><h3> Mathmatical libaries. </h3></li>
	 *		<p> Utilty classes for mathmatical methods, or libraries of case generator 
	 *		and genetic algorithm. In those mathmatical part, lots of examples are supported. </p>
	 *
	 *		@image html cpp/library_math.png 
	 *		@image latex cpp/library_math.png
	 *
	 *	<li><h3> Critical section libraries. </h3></li>
	 *		<p> Classes handling critical section, like RWMutex and Semaphore which are not supported 
	 *		in STL yet. </p>
	 *
	 *		<p> Of course, rw_mutex is already defined in linux C, semaphore is already defined in
	 *		MFC. But they are dependent on their own operating system, so that cannot compile in
	 *		multiple operating systems. It's the reason why Samchon Framework provides those classes. </p>
	 *
	 *		<p> Furthermore, unique and shared lock of those critical section classes, which are managing
	 *		locking and unlocking, and critical allocator are provided. </p>
	 *
	 *		@image html cpp/library_critical_section.png 
	 *		@image latex cpp/library_critical_section.png
	 *	
	 *	<li><h3> Data I/O libraries. </h3></li>
	 *		<p> Provides libraries about data I/O.
	 *
	 *		<p> SQLi and SQLStatement are the ODBC drivier designed to follow principles of OOP 
	 *		by adapter pattern. XML class is designed to follow composite relationship. </p>
	 * 
	 *		@image html cpp/library_data.png 
	 *		@image latex cpp/library_data.png
	 *	
	 *	<li><h3> Event libraries. </h3></li>
	 *		<p> Libraries representing events and dispatching those events, which are running on background, 
	 *		with own exclusive thread. </p>
	 *
	 *		<p> But there's something to notice. Since C++11, calling member method of a class by new thread 
	 *		passing by static method and void pointer is recommended to avoid. By guidance of the STL, using 
	 *		std::thread and std::bind will be better. As that reason, Event and StaticEventDispatcher can be 
	 *		deprecated in next generation of Samchon Framework. </p>
	 *
	 *		@image html cpp/library_event.png 
	 *		@image latex cpp/library_event.png
	 *
	 *	<li><h3> File-tree libraries. </h3></li>
	 *		<p> A module for expressing folder and file instances. The objects in file-tree module are 
	 *		realized by protocol::Entity. 
	 *		
	 *		<p> They have recursive and hierarchical relationship and created by Factory class (FTFactory, 
	 *		factory pattern). Those objects can be archived in and loaded from Database. Using the 
	 *		pre-defined methods interaction with Databse, you can not only express real file and folder 
	 *		instances but also realize virtual file system. </p>
	 *
	 *		<p> Classes in the Module of file-tree are all abstract, but package nam-tree can be an 
	 *		example inheriting and having real model from those abstract classes. </p>
	 *
	 *		@image html cpp/library_file_tree.png 
	 *		@image latex cpp/library_file_tree.png
	 *
	 * </ul>
	 *
	 * @author Jeongho Nam
	 */
	namespace library
	{
		//THIS HEADER FILE IS ONLY FOR DOCUMENTATION
	};
};