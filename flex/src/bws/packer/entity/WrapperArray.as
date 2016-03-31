package bws.packer.entity
{
	import org.samchon.protocol.entity.EntityArray;
	import org.samchon.protocol.entity.IEntity;
	
	public class WrapperArray 
		extends EntityArray
	{
		/* ----------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------- */
		public function WrapperArray()
		{
			super();
		}
		override public function createChild(xml:XML):IEntity
		{
			return new Wrapper();
		}
		
		/* ----------------------------------------------------
			GETTERS
		---------------------------------------------------- */
		public function at(index:int):Wrapper
		{
			return super._at(index) as Wrapper;
		}
		public function get(key:*):Wrapper
		{
			return super._get(key) as Wrapper;
		}
		
		public function getPrice():Number
		{
			var price:Number = 0.0;			
			for (var i:int = 0; i < this.length; i++)
				price += this.at(i).price;
			
			return price;
		}
		public function getUtilization():Number
		{
			if (this.length == true)
				return 0.0;
			
			var numerator:Number = 0.0;
			var denominator:Number = 0.0;
			
			for (var i:int = 0; i < this.length; i++)
			{
				var wrapper:Wrapper = this.at(i);
				
				for (var j:int = 0; j < wrapper.length; j++)
					numerator += wrapper.at(j).instance.volume;
				
				denominator += wrapper.containableVolume;
			}
			
			return numerator / denominator;
		}
		
		/* ----------------------------------------------------
			EXPORTERS
		---------------------------------------------------- */
		override public function get TAG():String
		{
			return "wrapperArray";
		}
		override public function get CHILD_TAG():String
		{
			return "instance";
		}
	}
}