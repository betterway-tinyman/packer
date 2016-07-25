/// <reference path="API.ts" />

// A '.tsx' file enables JSX support in the TypeScript compiler, 
// for more information see the following page on the TypeScript wiki:
// https://github.com/Microsoft/TypeScript/wiki/JSX

namespace bws.packer
{
	export class PackerApplication extends React.Component<{}, {}>
	{
		private instances: InstanceFormArray;
		private wrappers: WrapperArray;

		private result: WrapperArray;
		
		/**
		 * Default Constructor.
		 */
		public constructor()
		{
			super();

			this.instances = new InstanceFormArray();
			this.wrappers = new WrapperArray();
			this.result = new WrapperArray();

			// INITIAL, EXMAPLE DATA
			this.wrappers.push
			(
				new Wrapper("Large", 1000, 40, 40, 15, 0),
				new Wrapper("Medium", 700, 20, 20, 10, 0),
				new Wrapper("Small", 500, 15, 15, 8, 0)
			);
			this.instances.push
			(
				new InstanceForm(new Product("Eraser", 1, 2, 5), 15),
				new InstanceForm(new Product("Book", 15, 30, 3), 15),
				new InstanceForm(new Product("Drink", 3, 3, 10), 15),
				new InstanceForm(new Product("Umbrella", 5, 5, 20), 15),
				new InstanceForm(new Product("Notebook-Box", 30, 40, 4), 15),
				new InstanceForm(new Product("Tablet-Box", 20, 28, 2), 15)
			);
		}

		public pack(): void
		{
			/////
			// FIND THE OPTIMIZED SOLUTION
			/////
			let packer = new PackerForm(this.instances, this.wrappers).toPacker();
			let result: WrapperArray;

			try
			{
				result = packer.optimize();
			}
			catch (exception)
			{
				alert(exception.what());
				return;
			}

			this.result.assign(result.begin(), result.end());

			/////
			// DRAW THE 1ST WRAPPER
			/////
			if (this.result.empty() == true)
				return;

			this.drawWrapper(this.result.front());
			(this.refs["tabNavigator"] as flex.TabNavigator).setState({ selectedIndex: 1 });
		}

		public drawWrapper(wrapper: Wrapper, index: number = wrapper.size()): void
		{
			// INITIALIZE
			let div = document.getElementById("wrapper_viewer") as HTMLDivElement;
			let canvas = wrapper.toCanvas(index); // DRAW

			// PRINT
			if (div.hasChildNodes() == true)
				div.removeChild(div.childNodes[0]);
			div.appendChild(canvas);
		}

		public render(): JSX.Element
		{
			let ret: JSX.Element =
				<div>
					<div style={{width: "100%", height: "100%", fontSize: 12}}>
						<flex.TabNavigator ref="tabNavigator" 
										   style={{ width: 400, height: "100%", float: "left" }}>
							<flex.NavigatorContent label="First Tab">
								<ItemEditor application={this}
											instances={this.instances} wrappers={this.wrappers} />
							</flex.NavigatorContent>
							<flex.NavigatorContent label="Second Tab">
								<ResultViewer application={this} 
											  wrappers={this.result} />
							</flex.NavigatorContent>
						</flex.TabNavigator>
						<div id="wrapper_viewer" style={{height: "100%", overflow: "hidden"}}>
						</div>
					</div>
					<div style={{position: "absolute", right: 10, bottom: 10}}>
						<a href="http://redprinting.co.kr/" target="_blank">
							<img src="images/redprinting_logo.png"
								 width="250" />
						</a>
					</div>
				</div>;
			
			return ret;
		}

		public static main(): void
		{
			ReactDOM.render(<PackerApplication />, document.body);
		}
	}
}