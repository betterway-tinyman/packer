try
{
	eval("var std = require('typescript-stl')");
	eval("var samchon = require('samchon-framework')");
	eval("var React = require('react')");
	eval("var ReactDataGrid = require('react-data-grid')");
	eval("var THREE = require('three')");
}
catch (exception) { }

declare var ReactDataGrid: typeof AdazzleReactDataGrid.ReactDataGrid;

namespace bws.packer
{
	export import library = samchon.library;
	export import protocol = samchon.protocol;

	export const SERVER_IP: string = "172.16.0.209";
	//export const SERVER_IP: string = "127.0.0.1";
	export const SERVER_PORT: number = 37896;
}