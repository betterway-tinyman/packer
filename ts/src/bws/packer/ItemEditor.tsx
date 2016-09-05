/// <reference path="API.ts" />

// A '.tsx' file enables JSX support in the TypeScript compiler, 
// for more information see the following page on the TypeScript wiki:
// https://github.com/Microsoft/TypeScript/wiki/JSX

namespace bws.packer
{
	export interface ItemEditorProps extends React.Props<ItemEditor>
	{
		application: PackerApplication;
		instances: InstanceFormArray;
		wrappers: WrapperArray;
	}

	export class ItemEditor
		extends React.Component<ItemEditorProps, { }>
	{
		private clear(event: React.MouseEvent): void
		{
			this.props.instances.clear();
			this.props.wrappers.clear();
		}
		private open(event: React.MouseEvent): void
		{
			let this_ = this;
			let handle_select = function (event: library.BasicEvent): void
			{
				file_ref.load();
			}
			let handle_complete = function (event: library.BasicEvent): void
			{
				let packer_form = new PackerForm();
				packer_form.construct(new library.XML(file_ref.data));

				this_.props.instances.assign(packer_form.getInstanceFormArray().begin(), packer_form.getInstanceFormArray().end());
				this_.props.wrappers.assign(packer_form.getWrapperArray().begin(), packer_form.getWrapperArray().end());
			}

			let file_ref = new library.FileReference();
			file_ref.addEventListener("select", handle_select);
			file_ref.addEventListener("complete", handle_complete);

			file_ref.browse();
		}
		private save(event: React.MouseEvent): void
		{
			let packer_form = new PackerForm(this.props.instances, this.props.wrappers);

			let file_ref = new library.FileReference();
			file_ref.save(packer_form.toXML().toString(), "packing_items.xml");
		}

		private pack(event: React.MouseEvent): void
		{
			this.props.application.pack();
		}

		public render(): JSX.Element
		{
			return <div>
					<table style={{ textAlign: "center" }}>
					<tbody>
						<tr>
							<td> <img src="images/newFile.png" onClick={this.clear.bind(this)} /> </td>
							<td> <img src="images/openFile.png" onClick={this.open.bind(this)} /> </td>
							<td> <img src="images/saveFile.png" onClick={this.save.bind(this)} /> </td>
							<td> <img src="images/document.png" onClick={this.pack.bind(this)} /> </td>
						</tr>
						<tr>
							<td> New File </td>
							<td> Open File </td>
							<td> Save File </td>
							<td> Pack </td>
						</tr>
					</tbody>
					</table>
					<hr />
					<p> <InstanceEditor dataProvider={this.props.instances} /> </p>
					<hr />
					<p> <WrapperEditor dataProvider={this.props.wrappers} /> </p>
				</div>;
		}
	}

	export class InstanceEditor extends Editor<InstanceForm>
	{
		protected createColumns(): AdazzleReactDataGrid.Column[]
		{
			let columns: AdazzleReactDataGrid.Column[] = 
				[
					{ key: "$name", name: "Name", editable: true, width: 100 },
					{ key: "$width", name: "Width", editable: true, width: 60 },
					{ key: "$height", name: "Height", editable: true, width: 60 },
					{ key: "$length", name: "Length", editable: true, width: 60 },
					{ key: "$count", name: "Count", editable: true, width: 60 }
				];
			return columns;
		}
	}

	export class WrapperEditor extends Editor<Wrapper>
	{
		protected createColumns(): AdazzleReactDataGrid.Column[]
		{
			let columns: AdazzleReactDataGrid.Column[] =
				[
					{ key: "$name", name: "Name", editable: true, width: 80 },
					{ key: "$price", name: "Price", editable: true, width: 70 },
					{ key: "$width", name: "Width", editable: true, width: 45 },
					{ key: "$height", name: "Height", editable: true, width: 45 },
					{ key: "$length", name: "Length", editable: true, width: 45 },
					{ key: "$thickness", name: "Thickness", editable: true, width: 45 }
				];
			return columns;
		}
	}
}