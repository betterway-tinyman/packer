/// <reference path="API.ts" />

namespace bws.packer
{
	/**
	 * An array of Instance objects.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export class InstanceArray
		extends samchon.protocol.EntityArray<Instance>
	{
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		public constructor()
		{
			super();
		}

		/**
		 * @inheritdoc
		 */
		public createChild(xml: samchon.library.XML): Instance
		{
			let type = xml.getProperty("type");
			
			if (type == "product")
				return new Product();
			else if (type == "wrapper")
				return new Wrapper();
			else
				return null;
		}

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		/**
		 * @inheritdoc
		 */
		public TAG(): string
		{
			return "instanceArray";
		}

		/**
		 * @inheritdoc
		 */
		public CHILD_TAG(): string
		{
			return "instance";
		}
	}
}