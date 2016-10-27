/// <reference types="react" />
/// <reference types="react-dom" />
/// <reference types="react-data-grid" />
/// <reference types="three" />

/// <reference types="typescript-stl" />
/// <reference types="samchon-framework" />

try
{
	eval("var React = require('react');");
	eval("var ReactDOM = require('react-dom');");
	eval("var ReactDataGrid = require('react-data-grid');");
	eval("var THREE = require('three');");

	eval("var std = require('typescript-stl');");
	eval("var samchon = require('samchon-framework');");
}
catch (exception) { }

declare var ReactDataGrid: typeof AdazzleReactDataGrid.ReactDataGrid;

namespace bws.packer
{
	export import library = samchon.library;
	export import protocol = samchon.protocol;

	//export const SERVER_IP: string = "172.16.0.209";
	export const SERVER_IP: string = "127.0.0.1";
	export const SERVER_PORT: number = 37896;
}