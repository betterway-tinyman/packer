package bws.packer.socket
{
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IOErrorEvent;
	import flash.events.SecurityErrorEvent;
	
	import org.samchon.protocol.invoke.IProtocol;
	import org.samchon.protocol.invoke.Invoke;
	import org.samchon.protocol.socket.ServerConnector;
	
	/**
	 * A client socket connecting to CPP server.
	 * 
	 * @author Jeongho Nam
	 */
	public class Client 
		extends ServerConnector
		implements IProtocol
	{
		protected var application:index;
		//protected var socket:ServerConnector;
		
		/**
		 * Construct from the Application.
		 * 
		 * @param application The main application, OrderCrawler
		 */
		public function Client(application:index)
		{
			super(this);
			
			this.application = application;
		}
		
		override public function replyData(invoke:Invoke):void
		{
			application.replyData(invoke);
		}
	}
}