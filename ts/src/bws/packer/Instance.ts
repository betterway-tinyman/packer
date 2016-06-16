namespace bws.packer
{
	/**
	 * An interface of physical 3D-instances.
	 * 
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export interface Instance
		extends samchon.protocol.IEntity
	{
		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * Get name.
		 */
		getName(): string;

		/**
		 * Get width, length on the X-axis in 3D.
		 */
		getWidth(): number;

		/**
		 * Get height, length on the Y-axis in 3D.
		 */
		getHeight(): number;

		/**
		 * Get length, length on the Z-axis in 3D.
		 */
		getLength(): number;

		/**
		 * Get (calculate) volume.
		 *
		 * @return width x height x length
		 */
		getVolume(): number;

		/* -----------------------------------------------------------
			SETTERS
		----------------------------------------------------------- */
		/**
		 * Set name.
		 */
		setName(val: string): void;

		/**
		 * Set width, length on the X-axis in 3D.
		 */
		setWidth(val: number): void;

		/**
		 * Set height, length on the Y-axis in 3D.
		 */
		setHeight(val: number): void;

		/**
		 * Set length, length on the Z-axis in 3D.
		 */
		setLength(val: number): void;

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		/**
		 * <p> A type, identifier of derived class. </p>
		 *
		 * <h4> Derived types </h4>
		 * <ul>
		 *	<li> {@link Product product} </li>
		 *	<li> {@link Wrapper wrapper} </li>
		 * <ul>
		 */
		TYPE(): string;
	}
}