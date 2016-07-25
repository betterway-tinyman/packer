/// <reference path="../bws/packer/API.ts" />

// A '.tsx' file enables JSX support in the TypeScript compiler, 
// for more information see the following page on the TypeScript wiki:
// https://github.com/Microsoft/TypeScript/wiki/JSX

namespace flex
{
	export class TabNavigator extends React.Component<TabNavigatorProps, TabNavigatorProps>
	{
		public render(): JSX.Element
		{
			if (this.state == null)
				this.state = { selectedIndex: this.props.selectedIndex };
			if (this.state.selectedIndex == undefined)
				this.state = { selectedIndex: 0 };
			
			let children: NavigatorContent[] = this.props.children as NavigatorContent[];
			let selected: NavigatorContent = children[this.state.selectedIndex];

			let tabs: JSX.Element[] = [];

			for (let i = 0; i < children.length; i++)
			{
				let child: NavigatorContent = children[i];
				let className: string = (i == this.state.selectedIndex) ? "active" : "";

				let label: JSX.Element = 
					<li key={i} className="tabNavigator_label">
						<a href="#" className={className} 
						   onClick={this.handle_change.bind(this, i) }>{child.props.label}</a>
					</li>;

				tabs.push(label);
			}
			
			let ret: JSX.Element =
				<div className="tabNavigator" style={this.props.style}>
					<ul className="tabNavigator_label">
						{tabs}
					</ul>
					{selected}
				</div>;

			return ret;
		}

		private handle_change(index: number, event: React.MouseEvent): void
		{
			this.setState({ selectedIndex: index });
		}
	}
	
	export class NavigatorContent extends React.Component<NavigatorContentProps, NavigatorContentProps>
	{
		public render(): JSX.Element
		{
			return <div className="tabNavigator_content">{this.props.children}</div>;
		}
	}

	export interface TabNavigatorProps extends React.Props<TabNavigator>
	{
		selectedIndex?: number;
		style?: React.CSSProperties;
	}
	export interface NavigatorContentProps extends React.Props<NavigatorContent>
	{
		label: string;
	}
}