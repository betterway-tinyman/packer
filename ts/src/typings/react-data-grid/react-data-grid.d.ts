// Type definitions for react-data-grid 0.14.44
// Project: https://github.com/adazzle/react-data-grid
// Definitions by: Jeongho Nam <https://github.com/samchon>
// Definitions: https://github.com/DefinitelyTyped/DefinitelyTyped

/// <reference path="../react/react.d.ts" />

declare module "react-data-grid"
{
	import ReactDataGrid = react_data_grid.ReactDataGrid;
	export = ReactDataGrid;
}

declare var ReactDataGrid: typeof react_data_grid.ReactDataGrid;

declare namespace react_data_grid
{
	class ReactDataGrid extends __React.Component<ReactDataGridProps, {}>
	{
		static Editors: typeof react_data_grid.Editors;
	}

	interface ReactDataGridProps extends __React.Props<ReactDataGrid>
	{
		columns: Column[];
		rowGetter: (index: number) => Object;
		rowsCount: number;

		toolbar?: JSX.Element;

		onRowUpdated?: (event: RowUpdateEvent) => void;
		onCellDragged?: (event: CellDragEvent) => void;
		onCellCopyPaste?: (event: CellCopyPasteEvent) => void;
		onRowSelect?: (rows: Array<Object>) => void;
		onCellSelected?: (event: CellSelectEvent) => void;
		onAddFilter?: (event: FilterEvent) => void;
		onGridSort?: (column: string, direction: "ASC" | "DESC") => void;
		
		enableRowSelect?: boolean | "single" | "multi";
		enableCellSelect?: boolean;
		minHeight?: number;
	}

	interface Column
	{
		key: string;
		name: string;

		editor?: JSX.Element;
		formatter?: JSX.Element;
		headerRenderer?: JSX.Element;

		editable?: boolean;
		filterable?: boolean;
		sortable?: boolean;

		width?: number;
		resizable?: boolean;
		locked?: boolean;
	}

	interface RowUpdateEvent
	{
		rowIdx: number;
		updated: Object;
		cellKey: string;
		keyCode: string
	}

	interface CellDragEvent
	{
		rowIdx: number;
		fromRow: number;
		toRow: number;
		value: any;
	}

	interface CellSelectEvent
	{
		rowIdx: number;
		idx: number;
	}

	interface CellCopyPasteEvent
	{
		rowIdx: number;
		value: any;
		fromRow: number;
		toRow: number;
		cellKey: string
	}

	interface FilterEvent
	{
		columnKey: string;
		filterTerm: any;
	}
}

declare namespace react_data_grid.Editors
{
	class DropDownEditor extends __React.Component<DropDownEditorProps, {}>
	{
	}
	interface DropDownEditorProps extends __React.Props<DropDownEditor>
	{
		options: string[];
	}

	class AutoCompleteEditor extends __React.Component<AutoCompleteEditorProps, {}>
	{
	}
	interface AutoCompleteEditorProps extends __React.Props<AutoCompleteEditor>
	{
		options: { title: string }[];
	}
}