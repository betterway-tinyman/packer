package bws.packer.entity
{
	import org.samchon.protocol.entity.EntityArray;
	import org.samchon.protocol.entity.IEntity;
	
	public class InstanceArray extends EntityArray
	{
		/* ----------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------- */
		public function InstanceArray()
		{
			super();
		}
		override public function createChild(xml:XML):IEntity
		{
			if (xml.@type == "product")
				return new Product();
			else if (xml.@type == "wrapper")
				return new Wrapper();
			else
				return null;
		}
		
		/* ----------------------------------------------------
			GETTERS
		---------------------------------------------------- */
		public function at(index:int):Instance
		{
			return super._at(index) as Instance;
		}
		public function get(key:*):Instance
		{
			return super._get(key) as Instance;
		}
	
		/* ----------------------------------------------------
			EXPORTERS
		---------------------------------------------------- */
		override public function get TAG():String
		{
			return "instanceArray";
		}
		override public function get CHILD_TAG():String
		{
			return "instance";
		}
	}
}