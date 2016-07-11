package bws.packer.entity
{
	import org.samchon.protocol.entity.EntityArray;
	import org.samchon.protocol.entity.IEntity;
	
	public class Wrapper 
		extends EntityArray
		implements Instance
	{
		public var name:String;
		public var price:Number;
		public var width_:Number;
		public var height_:Number;
		public var length_:Number;
		public var thickness:Number;
		
		/* ----------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------- */
		public function Wrapper()
		{
			super();
			
			this.name = "";
			this.width_ = 0;
			this.price = 0;
			this.height_ = 0;
			this.length_ = 0;
			this.thickness = 0;
		}
		override public function construct(xml:XML):void
		{
			this.name = xml.@name;
			this.price = xml.@price;
			this.width_ = xml.@width;
			this.height_ = xml.@height;
			this.length_ = xml.@length;
			this.thickness = xml.@thickness;
			
			super.construct(xml);
		}
		
		override public function createChild(xml:XML):IEntity
		{
			return new Wrap(this);
		}
		
		/* ----------------------------------------------------
			GETTERS
		---------------------------------------------------- */
		override public function get key():*
		{
			return this.name;
		}
		public function getName():String
		{
			return name;
		}
		public function getWidth():Number
		{
			return width_;
		}
		public function getHeight():Number
		{
			return height_;
		}
		public function getLength():Number
		{
			return length_;	
		}
		
		public function at(index:int):Wrap
		{
			return super._at(index) as Wrap;
		}
		public function get(key:*):Wrap
		{
			return super._get(key) as Wrap;
		}
		
		public function get TYPE():String
		{
			return "wrapper";
		}
		
		public function get volume():Number
		{
			return width_ * height_ * length_;
		}
		
		public function get containableWidth():Number
		{
			return width_ - 2*thickness;
		}
		public function get containableHeight():Number
		{
			return height_ - 2*thickness;
		}
		public function get containableLength():Number
		{
			return length_ - 2*thickness;
		}
		public function get containableVolume():Number
		{
			return containableWidth * containableHeight * containableLength;
		}
		
		// FIELDS OF DATA-GRID
		public function get scale():Array
		{
			return [width_, height_, length_];
		}
		public function get utilization():Number
		{
			var totalVolume:Number = 0.0;
			for (var i:int = 0; i < this.length; i++)
				totalVolume += this.at(i).instance.volume;
			
			return totalVolume / containableVolume;
		}
		
		/* ----------------------------------------------------
			EXPORTERS
		---------------------------------------------------- */
		override public function get TAG():String
		{
			return "instance";
		}
		override public function get CHILD_TAG():String
		{
			return "wrap";
		}
		
		override public function toXML():XML
		{
			var xml:XML = super.toXML();
			xml.@name = name;
			xml.@type = TYPE;
			xml.@price = price;
			xml.@width = width_;
			xml.@height = height_;
			xml.@length = length_;
			xml.@thickness = thickness;
			
			return xml;
		}
	}
}