package bws.packer.component.grid
{
	import mx.controls.Label;
	
	import spark.formatters.NumberFormatter;
	
	public class SpaceUtilizationColumn extends Label
	{
		private var percentFormatter:NumberFormatter;
		
		public function SpaceUtilizationColumn()
		{
			super();
			
			percentFormatter = new NumberFormatter();
			percentFormatter.fractionalDigits = 2;
		}
		
		override public function set text(value:String):void
		{
			var num:Number = Number(value);
			if (isNaN(num) == true)
				super.text = value;
			
			super.text = percentFormatter.format(num * 100.0) + " %";
		}
	}
}