package bws.packer.entity
{
	import org.samchon.protocol.entity.Entity;
	import org.samchon.protocol.invoke.Invoke;
	
	public class Packer extends Entity
	{
		public var instanceArray:InstanceArray;
		public var wrapperArray:WrapperArray;
		
		/* ----------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------- */
		public function Packer(instanceArray:InstanceArray, wrapperArray:WrapperArray)
		{
			super();
			
			if (instanceArray == null)
				this.instanceArray = new InstanceArray();
			else
				this.instanceArray = instanceArray;
			
			if (wrapperArray == null)
				this.wrapperArray = new WrapperArray();
			else
				this.wrapperArray = wrapperArray;
		}
		
		override public function construct(xml:XML):void
		{
			instanceArray.construct(xml[instanceArray.TAG][0]);
			wrapperArray.construct(xml[wrapperArray.TAG][0]);
		}
		
		/* ----------------------------------------------------
			EXPORTERS
		---------------------------------------------------- */
		override public function get TAG():String
		{
			return "packer";
		}
		
		override public function toXML():XML
		{
			var xml:XML = super.toXML();
			xml[instanceArray.TAG] = instanceArray.toXML();
			xml[wrapperArray.TAG] = wrapperArray.toXML();
			
			return xml;
		}
	}
}