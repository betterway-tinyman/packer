package bws.packer.entity
{
	import org.samchon.protocol.entity.IEntity;
	
	public interface Instance extends IEntity
	{
		function get TYPE():String;
		
		function getName():String;
		function getWidth():Number;
		function getHeight():Number;
		function getLength():Number;
		
		function get volume():Number;
	}
}