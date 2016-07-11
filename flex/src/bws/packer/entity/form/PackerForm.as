package bws.packer.entity.form
{
	import bws.packer.entity.InstanceArray;
	import bws.packer.entity.Packer;
	import bws.packer.entity.WrapperArray;
	
	import org.samchon.library.utils.GAParameters;
	import org.samchon.protocol.entity.Entity;
	import org.samchon.protocol.invoke.Invoke;
	
	public class PackerForm extends Entity
	{
		public var instanceFormArray:InstanceFormArray;
		public var wrapperArray:WrapperArray;
		public var gaParameters:GAParameters;
		
		/* ----------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		public function PackerForm()
		{
			super();
			
			instanceFormArray = new InstanceFormArray();
			wrapperArray = new WrapperArray();
			gaParameters = new GAParameters();
		}
		
		override public function construct(xml:XML):void
		{
			instanceFormArray.construct(xml[instanceFormArray.TAG][0]);
			wrapperArray.construct(xml[wrapperArray.TAG][0]);
			
			if (xml.hasOwnProperty(gaParameters.TAG))
				gaParameters.construct(xml[gaParameters.TAG][0]);
		}
		
		/* ----------------------------------------------------
			EXPORTERS
		---------------------------------------------------- */
		override public function get TAG():String
		{
			return "packerForm";
		}
		
		override public function toXML():XML
		{
			var xml:XML = super.toXML();
			xml[instanceFormArray.TAG] = instanceFormArray.toXML();
			xml[wrapperArray.TAG] = wrapperArray.toXML();
			xml[gaParameters.TAG] = gaParameters.toXML();
			
			return xml;
		}
		
		public function toPacker():Packer
		{
			var instanceArray:InstanceArray = new InstanceArray();
			for (var i:int = 0; i < instanceFormArray.length; i++)
				instanceArray.addAll((instanceFormArray.getItemAt(i) as InstanceForm).toInstanceArray());
			
			return new Packer(instanceArray, wrapperArray);
		}
		
		public function toInvoke():Invoke
		{
			var invoke:Invoke = 
				new Invoke
				(
					"pack", 
					this.toXML()
				);
			return invoke;
		}
	}
}