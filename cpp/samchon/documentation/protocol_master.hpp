#pragma once

namespace samchon
{
	namespace protocol
	{
		/**
		 * @brief Package for external system, within the framework of master
		 *
		 * @details
		 * <p> @image html conception/distributed_and_parallel_processing_system.png
		 * @image latex conception/distributed_and_parallel_processing_system.png </p>
		 *
		 * <p> In master package, provides distributed and parallel processing system modules
		 * which are in framework of master. With classes in master package, you can realize
		 * any type of distributed or parallel processing system in master side, even how enormouse 
		 * scale those have. </p>
		 *
		 * <h5> Distributed Processing System </h5>
		 * <p> A distributed processing system of master, a master (master::DistributedSystemArray) 
		 * allocates roles (master::DistributedSystemRole) to systems (master::DistributedSystem). 
		 * User does not consider about which roles are allocated to which systems. User just accesses 
		 * to a <i>role</i> and send or reply message from those roles.
		 *			
		 * <ul>
		 *	<li> It's the proxy pattern have mentioned in external system</li>
		 *		<p> ExternalSystem objects are managed by ExternalSystemArray and the ExternalSystemArray 
		 *			can access to a role belongs to a system directly. When you send an Invoke message to 
		 *			ExternalSystemArray, the ExternalSystemArray finds matched ExternalSystemRole and the 
		 *			ExternalSystemRole shifts the network I/O responsibility to belonged ExternalSystem. 
		 *			This relationship called "Proxy Pattern". By the pattern, "Proxy", you can concentrate 
		 *			on roles irrespective of where each role is belonged to. </p>
		 *				
		 *		<ul>
		 *			<li> ExternalSystemArray::sendData() -> ExternalSystemRole(Proxy)::sendData() -> ExternalSystem::sendData() </li>
		 *			<li> ExternalSystem::replyData() -> ExternalSystemRole(Proxy)::replyData() </li>
		 *		</ul>
		 *	</ul>
		 *
		 *	<p> However, unlike the case of managing roles from ExternalSystemArray, in DistributedSystemArray,
		 *	roles can be allocated in multiple systems duplicately. And the roles can be moved to another systems.
		 *	Those management and allocation of roles are determined by estimation of performance of each system, and
		 *	required performance of each role. </p>
		 *
		 *	<p> @image html cpp/protocol_master_distributed_system.png
		 *	@image latex cpp/protocol_master_distributed_system.png </p>
		 *
		 *	<h5> Parallel Processing System </h5>
		 *	<p> Unlike distributed processing system, the parallel processing system is not complicate. It has very
		 *	simple logic. About requested processes, master(master::ParallelSystemArray) allocates each process to
		 *	each slave system by their own performance index have estimated. </p>
		 *
		 *	<p> @image html cpp/protocol_master_parallel_system.png
		 *	@image latex cpp/protocol_master_parallel_system.png </p>
		 *
		 * @author Jeongho Nam <http://samchon.org>
		 */
		namespace master
		{
			//THIS HEADER FILE IS ONLY FOR DOCUMENTATION
		};
	};
};