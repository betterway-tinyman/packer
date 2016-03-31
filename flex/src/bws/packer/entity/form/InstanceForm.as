package bws.packer.entity.form
{
	import bws.packer.entity.Instance;
	import bws.packer.entity.InstanceArray;
	import bws.packer.entity.Product;
	import bws.packer.entity.Wrapper;
	
	import mx.collections.ArrayList;
	
	import org.samchon.protocol.entity.Entity;

	public class InstanceForm extends Product
	{
		public var count:int;
		
		/* ----------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------- */
		public function InstanceForm()
		{
			super();
			
			this.count = 1;
			
			this.name = "unnamed";
			this.width = 0;
			this.height = 0;
			this.length =0;
			
		}
		
		override public function construct(xml:XML):void
		{
			this.count = xml.@count;
			
			if (xml.hasOwnProperty("@type"))
			{
				super.construct(xml);
			}
			else if (xml.hasOwnProperty("instance"))
			{
				var instanceXML: XML = xml.instance[0];
				
				super.construct(instanceXML);
			}
		}
		
		private function createInstance(xml: XML): Instance
		{
			if (xml.@type == "product")
				return new Product();
			else
				return new Wrapper();
		}
		
		/* ----------------------------------------------------
			EXPORTERS
		---------------------------------------------------- */
		override public function get TAG():String
		{
			return "instanceForm";
		}
		override public function toXML():XML
		{
			var xml:XML = super.toXML();
			xml.@count = count;
			
			return xml;
		}
		
		public function toInstanceArray():InstanceArray
		{
			var instanceArray:InstanceArray = new InstanceArray();
			var instance:Product = new Product();
			
			instance.name = this.name;
			instance.width = this.width;
			instance.height = this.height;
			instance.length = this.length;
			
			for (var i:int = 0; i < count; i++)
				instanceArray.addItem(instance);
			
			return instanceArray;
		}
	}
}