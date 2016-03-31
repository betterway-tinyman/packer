namespace bws.packer
{
	var wrapperArray: WrapperArray;
	
	export function main(str: string): void
	{
		////////////////////////
		// READ DATA
		////////////////////////
		let packerForm: PackerForm = new PackerForm();
		
		let xml: samchon.library.XML = new samchon.library.XML(str);
		if (xml.getTag() == "invoke")
		{
			let invoke: samchon.protocol.Invoke = new samchon.protocol.Invoke(xml);	
			packerForm.construct(invoke.at(0).getValue());
		}
		else
			packerForm.construct(xml);
		
		////////////////////////
		// CONSTRUCT PACKER AND RESULT
		////////////////////////
		let packer: Packer = packerForm.toPacker();
		wrapperArray = packer.optimize();

		let invoke = new samchon.protocol.Invoke("setWrapperArray", wrapperArray.toXML());
		
		////////////////////////
		// NOTIFY TO FLEX
		////////////////////////
		let flex = document.getElementById("flex");
		flex["sendData"](invoke.toXML().toString());
	}

	export function setWrapperArray(str: string): void
	{
		wrapperArray = new WrapperArray();
		wrapperArray.construct(new samchon.library.XML(str));
	}

	export function drawWrapper(wrapperIndex: number, size: number): void
	{
		let wrapper: Wrapper = wrapperArray.at(wrapperIndex);
		
		// CLEAR PREVIOUS CANVAS
		let rightTD: HTMLTableColElement = <HTMLTableColElement>document.getElementById("rightTD");
		let div = document.createElement('div');

		rightTD.removeChild(document.body.getElementsByTagName("div")[0]);
		rightTD.appendChild(div);

		// CREATE AND PRINT THE NEW CANVAS
		div.appendChild(wrapper.toCanvas(size));
	}
}


function main(): void
{
	///////////////////////////
	// CONSTRUCT OBJECTS
	///////////////////////////
	let wrapperArray: bws.packer.WrapperArray = new bws.packer.WrapperArray();
	let instanceArray: bws.packer.InstanceArray = new bws.packer.InstanceArray();

	// Wrappers
	wrapperArray.push
		(
		new bws.packer.Wrapper("Large", 1000, 40, 40, 15, 0),
		new bws.packer.Wrapper("Medium", 700, 20, 20, 10, 0),
		new bws.packer.Wrapper("Small", 500, 15, 15, 8, 0)
		);

	///////
	// Each Instance is repeated #15
	///////
	instanceArray.insert(instanceArray.end(), 15, new bws.packer.Product("Eraser", 1, 2, 5));
	instanceArray.insert(instanceArray.end(), 15, new bws.packer.Product("Book", 15, 30, 3));
	instanceArray.insert(instanceArray.end(), 15, new bws.packer.Product("Drink", 3, 3, 10));
	instanceArray.insert(instanceArray.end(), 15, new bws.packer.Product("Umbrella", 5, 5, 20));

	// Wrappers also can be packed into another Wrapper.
	instanceArray.insert(instanceArray.end(), 15, new bws.packer.Wrapper("Notebook-Box", 2000, 30, 40, 4, 2));
	instanceArray.insert(instanceArray.end(), 15, new bws.packer.Wrapper("Tablet-Box", 2500, 20, 28, 2, 0));

	///////////////////////////
	// BEGINS PACKING
	///////////////////////////
	// CONSTRUCT PACKER
	let packer: bws.packer.Packer = new bws.packer.Packer(wrapperArray, instanceArray);

	///////
	// PACK (OPTIMIZE)
	let result: bws.packer.WrapperArray = packer.optimize();
	///////

	///////////////////////////
	// TRACE PACKING RESULT
	///////////////////////////
	let xml: samchon.library.XML = result.toXML();
	samchon.trace(xml);
}