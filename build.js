const fs = require("fs");
const process = require('child_process');

compile();
attach_header();
minify();
distribute();

function compile()
{
	process.execSync("tsc -p ./ts/tsconfig.json");
}

function attach_header()
{
	const TITLE_FILE = "./ts/src/typings/3d-bin-packing/3d-bin-packing.d.ts";
	const HEADER_FILE = "./lib/3d-bin-packing.d.ts";

	var text = fs.readFileSync(TITLE_FILE, "utf8");
	text += fs.readFileSync(HEADER_FILE, "utf8");

	fs.writeFileSync(HEADER_FILE, text, "utf8");
}

function minify()
{
	//minifier.minify("./lib/3d-bin-packing.js");
}

function distribute()
{
	// COPY HEADER
	fs.writeFileSync
	(
		"./demo/node_modules/@types/3d-bin-packing/index.d.ts",
		fs.readFileSync("./lib/3d-bin-packing.d.ts", "utf8"), 
		"utf8"
	);

	// COPY BODY
	fs.writeFileSync
	(
		"./demo/include/3d-bin-packing.js",
		fs.readFileSync("./lib/3d-bin-packing.js", "utf8"), 
		"utf8"
	);
}