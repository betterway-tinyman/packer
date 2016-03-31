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