package bws.packer.entity
{
	import org.samchon.protocol.entity.Entity;
	
	public class Wrap extends Entity
	{
		public var wrapper:Wrapper;
		public var instance:Instance;
		
		public var x:Number;
		public var y:Number;
		public var z:Number;
		public var orientation:int;
		
		/* ----------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------- */
		public function Wrap
			(
				wrapper:Wrapper, 
				instance:Instance = null, x:Number = 0, y:Number = 0, z:Number = 0, orientation:int = 1)
		{
			super();
			
			this.wrapper = wrapper;
			this.instance = instance;
			this.x = x;
			this.y = y;
			this.z = z;
			this.orientation = orientation;
		}
		override public function construct(xml:XML):void
		{
			var instanceXML:XML = xml["instance"][0];
			this.instance = createInstance(instanceXML.@type);
			this.instance.construct(instanceXML);
			
			this.x = xml.@x;
			this.y = xml.@y;
			this.z = xml.@z;
			this.orientation = xml.@orientation;
		}
		
		protected function createInstance(type:String):Instance
		{
			if (type == "product")
				return new Product();
			else if (type == "wrapper")
				return new Wrapper();
			else
				return null;
		}
		
		/* ----------------------------------------------------
			GETTERS
		---------------------------------------------------- */
		public function get name():String
		{
			return instance.getName();
		}
		public function get width():Number
		{
			switch (this.orientation)
			{
				case 1:	case 5:
					return this.instance.getWidth();
				case 3:	case 4:
					return this.instance.getHeight();
				default: // 2, 5
					return this.instance.getLength();
			}
		}
		public function get height():Number
		{
			switch (this.orientation)
			{
				case 1:	case 2:
					return this.instance.getHeight();
				case 4:	case 6:
					return this.instance.getWidth();
				default: // 3, 5
					return this.instance.getLength();
			}
		}
		public function get length():Number
		{
			switch (this.orientation)
			{
				case 1: case 4:
					return this.instance.getLength();
				case 2:	case 3:
					return this.instance.getWidth();
				default: // 5, 6
					return this.instance.getHeight();
			}
		}
		
		public function get scale():Array
		{
			return [width, height, length];
		}
		public function get position():Array
		{
			return [x, y, z];
		}
		
		/* ----------------------------------------------------
			EXPORTERS
		---------------------------------------------------- */
		override public function get TAG():String
		{
			return "wrap";
		}
		override public function toXML():XML
		{
			var xml:XML = super.toXML();
			xml[instance.TAG] = instance.toXML();
			
			xml.@x = x;
			xml.@y = y;
			xml.@z = z;
			xml.@orientation = orientation;
			
			return xml;
		}
	}
}