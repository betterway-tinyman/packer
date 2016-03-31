package bws.packer.entity
{
	import org.samchon.protocol.entity.Entity;

	public class Product 
		extends Entity
		implements Instance
	{
		public var name:String;
		public var width:Number;
		public var height:Number;
		public var length:Number;
		
		/* ----------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------- */
		public function Product()
		{
			super();
		}
		override public function construct(xml:XML):void
		{
			this.name = xml.@name;
			this.width = xml.@width;
			this.height = xml.@height;
			this.length = xml.@length;
		}
		
		/* ----------------------------------------------------
			GETTERS
		---------------------------------------------------- */
		override public function get key():*
		{
			return this.name;
		}
		
		public function get TYPE():String
		{
			return "product";
		}
		
		public function get volume():Number
		{
			return width * height * length;
		}
		
		public function getName():String
		{
			return name;
		}
		public function getWidth():Number
		{
			return width;
		}
		public function getHeight():Number
		{
			return height;
		}
		public function getLength():Number
		{
			return length;	
		}
		
		/* ----------------------------------------------------
			EXPORTERS
		---------------------------------------------------- */
		override public function get TAG():String
		{
			return "instance";
		}		
		override public function toXML():XML
		{
			var xml:XML = super.toXML();
			xml.@name = name;
			xml.@type = TYPE;
			xml.@width = width;
			xml.@height = height;
			xml.@length = length;
			
			return xml;
		}
	}
}