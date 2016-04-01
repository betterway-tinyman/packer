#pragma once

namespace samchon
{
	/**
	 * @brief Package of network protocol and libraries.
	 * 
	 * <ul>
	 *	<li><h3> Entity - data classes with utility methods </h3></li>
	 *		<p> In Samchon Framework, with entity, boundary and control, entity is the main. 
	 *		The entity package provides I/O interface to/from XML, Invoke and DBMS. If you
	 *		need some additional function (role of boundary or control) for an entity, use
	 *		chain of responsibility pattern starting from the entity. </p>
	 *
	 *		<p> When data-set has a "hierarchical relationship", do not have the children objects
	 *		by having a container member variable. Directly inherits container set. Composes the data 
	 *		class (entity) having children by inheriting EntityGroup and terminate the leaf node by 
	 *		inheriting Entity (single object).
	 *
	 *		@image html cpp/protocol_entity.png
	 *		@image latex cpp/protocol_entity.png
	 *
	 *	<li><h3> Invoke - standard network I/O message </h3></li>
	 *		<p> Invoke is a standard message structure using network I/O in Samchon Framework. </p>
	 *
	 *		<p> The Invoke message has an XML structure like the piacture in below. 
	 *		We can enjoy lots of benefits by the normalized and standardized message structure used in
	 *		network I/O.\n
	 *
	 *		<p> The greatest advantage is that we can make any type of network system, even how the system 
	 *		is enourmously complicated. As network communication message is standardized, we only need to
	 *		concentrate on logical relationships between network systems. We can handle each network system 
	 *		like a object (class) in OOD. And those relationships can be easily designed by using design
	 *		pattern. </p>
	 * 
	 *		<p> Furthermore, The module invoke not only provides parsing and converting from/to Invoke 
	 *		message, but also history classes can be used to archiving log or estimating performance of 
	 *		a system. The purpose of estimating performance a system, especially, used in module of 
	 *		distributed processing and parallel processing systems. </p>
	 *
	 *		@image html cpp/protocol_invoke.png
	 *		@image latex cpp/protocol_invoke.png
	 *
	 *	<li><h3> Intefaces </h3></li>
	 *		<p> You can make any type of network system with only three + one interfaces;
	 *		IProtocol, IServer and IClient + ServerConnector which are called basic 3 + 1 components.
	 *		As I've mentiond in previous invoke module, because of standardization of message of network 
	 *		I/O, we only need to concentrate on logical relationship like handling OOD objects </p>
	 *
	 *		<p> The basic 3 + 1 components are top level abstract interfaces for designing network system
	 *		like handling OOD objects. When you see the protocol module from a different view, out of framework, 
	 *		you can find that all the other classes in protocol are another examples combinationing and utilizing 
	 *		those basic 3 + 1 components (interaces). </p>
	 *
	 *		<p> @image html cpp/protocol_interface.png
	 *		@image latex cpp/protocol_interface.png </p>
	 *
	 *		<p> @image html sequence/send_invoke.png
	 *		@image latex sequence/send_invoke.png </p>
	 *
	 *	<li><h3> External System </h3></li>
	 *		<p> Module <i>external_system</i> provides interfaces for interaction with external network system. 
	 *		Although, the module <i>external_system</i> acts boundary as main role, what you've to concentrate
	 *		on is the entity. Samchon Framework takes responsibility of network communication and you only
	 *		consider about relationship and role of each <i>external network systems</i>. </p>
	 *		
	 *		<p> ExternalSystem objects are managed by ExternalSystemArray and the ExternalSystemArray can
	 *		access to an ExternalSystemRole belongs to an ExternalSystem directly. When you send an Invoke 
	 *		message to ExternalSystemArray, the ExternalSystemArray finds matched ExternalSystemRole and the 
	 *		ExternalSystemRole shifts the network I/O responsibility to belonged ExternalSystem. </p>
	 *
	 *		<p> The relationship called as "Proxy Pattern". With the pattern, "Proxy", you can concentrate 
	 *		on roles irrespective of where each role is belonged to (you can only concentrate on ExternalSystemRole 
	 *		itself, what to do with Invoke message, irrespective of the ExternalSystemRole is belonged to which 
	 *		ExternalSystem). </p>
	 *
	 *		<ul>
	 *			<li> ExternalSystemArray::sendData() -> ExternalSystemRole(Proxy)::sendData() -> ExternalSystem::sendData() </li>
	 *			<li> ExternalSystem::replyData() -> ExternalSystemRole(Proxy)::replyData() </li>
	 *		</ul>
	 *
	 *		<p> Whether using the "Proxy pattern" is on your mind in <i>external_system module</i> level.
	 *		"Proxy pattern" is recommend to use in <i>external_system</i> module, but not forced. 
	 *		However, since <i>parallel_processing_system</i> module, you've to follow the pattern.
	 *
	 *		@image html  cpp/protocol_external_system.png
	 *		@image latex cpp/protocol_external_system.png
	 *
	 *	<li><h3> Packages in protocol </h3></li>
	 *	<ul>
	 *		<li><h4> Cloud service </h4></li>
	 *			<p> The "service" package is for realizing cloud service. </p>
	 *
	 *			<p> In Samchon Framework, realizing cloud service, there're two classes representing physical
	 *			server and client as service::Server and service::Client. Between the two classes representing 
	 *			boundaries, there is a class representing logical conception user as service::User.
	 *			In the terminal node under the service::Client, service::Service class represents a control. </p>
	 *
	 *			<p> The default protocol of the service package is not 'web socket'. The default only has
	 *			message protocol of Invoke, does not have handshake or another pre-defined protocol. By
	 *			the default protocol, you can connect to the cloud server (built from c++, by service package)
	 *			as a client by a program or plug-in like Flex or C#. However you can't connect by web-browser
	 *			only allowing socket protocol as web-socket. </p>
	 *
	 *			<p> To build a cloud service to follow web-socket protocol, inherits derviced classes 
	 *			not from service::Server, service::Client, but from service::WebServer, service::WebClient or 
	 *			implements IWebServer, IWebClient which are derived from service::Server and service::Client. </p>
	 *
	 *			<h5> Class Diagram </h5>
	 *			<p> @image html cpp/protocol_service.png
	 *			@image latex cpp/protocol_service.png </p>
	 *
	 *			<h5> Sequence Diagram - A client's connection </h5>
	 *			<p> @image html sequence/service_connection.png
	 *			@image latex sequence/service_connection.png </p>
	 *
	 *			<h5> Sequence Diagram - Network communication </h5>
	 *			<p> @image html sequence/service_communication.png
	 *			@image latex sequence/service_communication.png </p>
	 *
	 *		<li><h4> Master system </h4></li>
	 *
	 *			<p> @image html conception/distributed_and_parallel_processing_system.png
	 *			@image latex conception/distributed_and_parallel_processing_system.png </p>
	 *
	 *			<p> In master package, provides distributed and parallel processing system modules
	 *			which are in framework of master. With classes in master package, you can realize
	 *			any type of distributed or parallel processing system in master side, even how enormouse 
	 *			scale those have. </p>
	 *
	 *			<h5> Distributed Processing System </h5>
	 *			<p> A distributed processing system of master, a master (master::DistributedSystemArray) 
	 *			allocates roles (master::DistributedSystemRole) to systems (master::DistributedSystem). 
	 *			User does not consider about which roles are allocated to which systems. User just accesses 
	 *			to a <i>role</i> and send or reply message from those roles.
	 *			
	 *			<ul>
	 *				<li> It's the proxy pattern have mentioned in external system</li>
	 *				<p> ExternalSystem objects are managed by ExternalSystemArray and the ExternalSystemArray 
	 *				can access to a role belongs to a system directly. When you send an Invoke message to 
	 *				ExternalSystemArray, the ExternalSystemArray finds matched ExternalSystemRole and the 
	 *				ExternalSystemRole shifts the network I/O responsibility to belonged ExternalSystem. 
	 *				This relationship called "Proxy Pattern". By the pattern, "Proxy", you can concentrate 
	 *				on roles irrespective of where each role is belonged to. </p>
	 *				
	 *				<ul>
	 *					<li> ExternalSystemArray::sendData() -> ExternalSystemRole(Proxy)::sendData() -> ExternalSystem::sendData() </li>
	 *					<li> ExternalSystem::replyData() -> ExternalSystemRole(Proxy)::replyData() </li>
	 *				</ul>
	 *			</ul>
	 *
	 *			<p> However, unlike the case of managing roles from ExternalSystemArray, in DistributedSystemArray,
	 *			roles can be allocated in multiple systems duplicately. And the roles can be moved to another systems.
	 *			Those management and allocation of roles are determined by estimation of performance of each system, and
	 *			required performance of each role. </p>
	 *
	 *			<p> @image html cpp/protocol_master_distributed_system.png
	 *			@image latex cpp/protocol_master_distributed_system.png </p>
	 *
	 *			<h5> Parallel Processing System </h5>
	 *			<p> Unlike distributed processing system, the parallel processing system is not complicate. It has very
	 *			simple logic. About requested processes, master(master::ParallelSystemArray) allocates each process to
	 *			each slave system by their own performance index have estimated. </p>
	 *
	 *			<p> @image html cpp/protocol_master_parallel_system.png
	 *			@image latex cpp/protocol_master_parallel_system.png </p>
	 *
	 *		<li><h4> Slave system </h4></li>
	 *			<p> A package for composing distributed or parallel processing system in slave side. </p>
	 *			<p> Slave system get orders from master, processes the orders and report the result with
	 *			its elapsed time for estimating performance of the slave system. </p>
	 *
	 *			@image html cpp/protocol_slave.png
	 *			@image latex cpp/protocol_slave.png
	 *
	 *	</ul>
	 * </ul>
	 *
	 * @note
	 *	\li Boost Libraries: http://www.boost.org/
	 *	\li Boot.Asio: http://www.boost.org/doc/libs/1_59_0/doc/html/boost_asio.html
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	namespace protocol
	{
		//THIS HEADER FILE IS ONLY FOR DOCUMENTATION
	};
};