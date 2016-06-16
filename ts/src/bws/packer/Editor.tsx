// A '.tsx' file enables JSX support in the TypeScript compiler, 
// for more information see the following page on the TypeScript wiki:
// https://github.com/Microsoft/TypeScript/wiki/JSX

namespace bws.packer
{
	export abstract class Editor<T extends samchon.protocol.IEntity>
		extends React.Component<{ dataProvider: samchon.protocol.EntityArrayCollection<T> }, {}>
	{
		private columns: react_data_grid.Column[];

		private selected_index: number;

		/* ------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------ */
		/**
		 * Default Constructor.
		 */
		public constructor()
		{
			super();

			this.columns = this.createColumns();
			this.selected_index = 0;
		}

		protected abstract createColumns(): react_data_grid.Column[];

		/* ------------------------------------------------------------
			ACCESSORSS
		------------------------------------------------------------ */
		private get_row(index: number): T
		{
			return this.props.dataProvider.at(index);
		}

		private insert_instance(event: React.MouseEvent): void
		{
			let child: T = this.props.dataProvider.createChild(null);
			this.props.dataProvider.push_back(child);
		}
		private erase_instances(event: React.MouseEvent): void
		{
			try
			{
				this.props.dataProvider.erase(this.props.dataProvider.begin().advance(this.selected_index));
			}
			catch (exception) 
			{
				// OUT OF RANGE 
			}
		}

		/* ------------------------------------------------------------
			EVENT HANDLERS
		------------------------------------------------------------ */
		private handle_data_change(event: samchon.collection.CollectionEvent<any>): void
		{
			setTimeout(this.setState.bind(this, {}), 0);
		}
		private handle_row_change(event: react_data_grid.RowUpdateEvent): void
		{
			(Object as any).assign(this.props.dataProvider.at(event.rowIdx), event.updated);
		}

		private handle_select(event: react_data_grid.CellSelectEvent): void
		{
			this.selected_index = event.rowIdx;
		}

		/* ------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------ */
		public render(): JSX.Element
		{
			this.props.dataProvider.addEventListener("insert", this.handle_data_change, this);
			this.props.dataProvider.addEventListener("erase", this.handle_data_change, this);

			let ret: JSX.Element =
				<div>
					<h3> Type of wrappers to pack </h3>
					<ReactDataGrid rowGetter={this.get_row.bind(this)}
								   rowsCount={this.props.dataProvider.size()}
								   columns={this.columns} 

								   onRowUpdated={this.handle_row_change.bind(this)}
								   onCellSelected={this.handle_select.bind(this)}
								   enableCellSelect={true}
						
								   minHeight={Math.min(document.body.offsetHeight * .3, 40 + this.props.dataProvider.size() * 35)} />
					<p style={{ textAlign: "right" }}>
						<button onClick={this.insert_instance.bind(this) }>Insert</button>
						<button onClick={this.erase_instances.bind(this) }>Erase</button>
					</p>
				</div>;
			return ret;
		}
	}
}