package bws.packer.entity.form
{
	import org.samchon.protocol.entity.EntityArray;
	import org.samchon.protocol.entity.IEntity;
	
	public class InstanceFormArray extends EntityArray
	{
		/* ----------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------- */
		public function InstanceFormArray()
		{
			super();
		}
		override public function createChild(xml:XML):IEntity
		{
			return new InstanceForm();
		}
		
		/* ----------------------------------------------------
			GETTERS
		---------------------------------------------------- */
		public function at(index:int):InstanceForm
		{
			return super._at(index) as InstanceForm;
		}
		public function get(key:*):InstanceForm
		{
			return super._get(key) as InstanceForm;
		}
		
		/* ----------------------------------------------------
			EXPORTERS
		---------------------------------------------------- */
		override public function get TAG():String
		{
			return "instanceFormArray";
		}
		override public function get CHILD_TAG():String
		{
			return "instanceForm";
		}
	}
}