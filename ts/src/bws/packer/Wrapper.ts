namespace bws.packer
{
	/**
	 * A wrapper wrapping instances.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export class Wrapper
		extends samchon.protocol.EntityArray<Wrap>
		implements Instance
	{
		/**
		 * <p> Name, key of the Wrapper. </p>
		 *
		 * <p> The name represents a type of Wrapper and identifies the Wrapper. </p>
		 */
		protected name: string = "";

		/**
		 * Price, cost of using an Wrapper.
		 */
		protected price: number = 0.0;

		/**
		 * Width of the Wrapper, length on the X-axis in 3D.
		 */
		protected width_: number = 0.0;

		/**
		 * Height of the Wrapper, length on the Y-axis in 3D.
		 */
		protected height_: number = 0.0;

		/**
		 * Length of the Wrapper, length on the Z-axis in 3D.
		 */
		protected length_: number = 0.0;

		/**
		 * <p> Thickness, margin of a Wrapper causes shrinkness of containable volume. </p>
		 *
		 * <p> The thickness reduces each dimension's containable size (dimension - 2*thickness),
		 * so finally, it reduces total containable volume (-8 * thickness^3). </p>
		 */
		protected thickness: number = 0.0;

		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		public constructor();

		/**
		 * Copy Constructor.
		 */
		public constructor(wrapper: Wrapper);

		/**
		 * Construct from members. 
		 *
		 * @param name Name, identifier of a Wrapper.
		 * @param price Price, issued cost for a type of the Wrapper.
		 * @param width Width, dimensional length on the X-axis in 3D.
		 * @param height Height, dimensional length on the Y-axis in 3D.
		 * @param length Length, dimensional length on the Z-axis in 3D.
		 * @param thickness A thickness causes shrinkness on containable volume.
		 */
		public constructor
			(
				name: string, price: number,
				width: number, height: number, length: number, thickness: number
			);

		public constructor(...args: any[])
		{
			super();

			if (args.length == 1 && args[0] instanceof Wrapper)
			{
				let wrapper: Wrapper = args[0];

				this.name = wrapper.name;
				this.price = wrapper.price;

				this.width_ = wrapper.width_;
				this.height_ = wrapper.height_;
				this.length_ = wrapper.length_;
				this.thickness = wrapper.thickness;
			}
			else if (args.length == 6)
			{
				this.name = args[0];
				this.price = args[1];

				this.width_ = args[2];
				this.height_ = args[3];
				this.length_ = args[4];
				this.thickness = args[5];
			}
		}

		/**
		 * @inheritdoc
		 */
		public construct(xml: samchon.library.XML): void
		{
			super.construct(xml);

			this.width_ = parseFloat(xml.getProperty("width"));
			this.height_ = parseFloat(xml.getProperty("height"));
			this.length_ = parseFloat(xml.getProperty("length"));
		}

		/**
		 * @inheritdoc
		 */
		protected createChild(xml: samchon.library.XML): Wrap
		{
			return new Wrap(this);
		}

		/* ===========================================================
			GETTERS
				- MEMBERS
				- DERIVED PROPERTIES
				- COMPARISON
		==============================================================
			MEMBERS
		----------------------------------------------------------- */
		/**
		 * Key of a Wrapper is its name.
		 */
		public key(): any
		{
			return this.name;
		}

		/**
		 * Get name.
		 */
		public getName(): string
		{
			return this.name;
		}

		/**
		 * Get price.
		 */
		public getPrice(): number
		{
			return this.price;
		}

		/**
		 * Get width, length on X-axis in 3D.
		 */
		public getWidth(): number
		{
			return this.width_;
		}

		/**
		 * Get height, length on Y-axis in 3D.
		 */
		public getHeight(): number
		{
			return this.height_;
		}

		/**
		 * Get length, length on Z-axis in 3D.
		 */
		public getLength(): number
		{
			return this.length_;
		}
		
		/**
		 * Get thickness.
		 */
		public getThickness(): number
		{
			return this.thickness;
		}

		/* -----------------------------------------------------------
			DERIVED PROPERTIES
		----------------------------------------------------------- */
		/**
		 * <p> Get (calculate) containable width, length on the X-axis in 3D. </p>
		 *
		 * <p> Calculates containable width considering the {@link thickness}. </p>
		 *
		 * @return width - (2 x thickness)
		 */
		public getContainableWidth(): number
		{
			return this.width_ - (2 * this.thickness);
		}

		/**
		 * <p> Get (calculate) containable height, length on the Y-axis in 3D. </p>
		 *
		 * <p> Calculates containable height considering the {@link thickness}. </p>
		 *
		 * @return height - (2 x thickness)
		 */
		public getContainableHeight(): number
		{
			return this.height_ - (2 * this.thickness);
		}
		
		/**
		 * <p> Get (calculate) containable length, length on the Z-axis in 3D. </p>
		 *
		 * <p> Calculates containable length considering the {@link thickness}. </p>
		 *
		 * @return length - (2 x thickness)
		 */
		public getContainableLength(): number
		{
			return this.length_ - (2 * this.thickness);
		}
		
		/**
		 * <p> Get (calculate) volume. </p>
		 *
		 * <h4> Notice </h4>
		 * <p> If {@link thickness} of the Wrapper is not 0, the volume does not mean containable volume. 
		 * In that case, use {@link containableVolume} instead. </p>
		 *
		 * @return width x height x length
		 */
		public getVolume(): number
		{
			return this.width_ * this.height_ * this.length_;
		}

		/**
		 * <p> Get (calculate) containable volume. </p>
		 *
		 * <p> Calculates containable volume considering the {@link thickness}. </p>
		 *
		 * @return volume - {(2 x thickness) ^ 3}
		 */
		public getContainableVolume(): number
		{
			return this.getContainableWidth() * this.getContainableHeight() * this.getContainableLength();
		}

		/**
		 * Get utilization ratio of containable volume.
		 *
		 * @return utilization ratio.
		 */
		public getUtilization(): number
		{
			let volume: number = 0.0;
			for (let i = 0; i < this.size(); i++)
				volume += this.at(i).getVolume();

			return volume / this.getContainableVolume();
		}

		/* -----------------------------------------------------------
			COMPARISON
		----------------------------------------------------------- */
		public equal_to(obj: Wrapper): boolean
		{
			return this.price == obj.price
				&& this.width_ == obj.width_ && this.height_ == obj.height_ && this.length_ == obj.length_ 
				&& this.thickness == obj.thickness;
		}

		/**
		 * <p> Wrapper is enough greater? </p>
		 * 
		 * <p> Test whether the Wrapper is enough greater than an Instance to contain. </p>
		 *
		 * @param instance An Instance to test.
		 * @return Enough greater or not.
		 */
		public containable(instance: Instance): boolean
		{
			// TEST WHETHER AN INSTANCE IS GREATER THAN WRAPPER
			let myDims = new std.Vector<number>([this.getContainableWidth(), this.getContainableHeight(), this.getContainableLength()]);
			let instanceDims = new std.Vector<number>([instance.getWidth(), instance.getHeight(), instance.getLength()]);

			std.sort(myDims.begin(), myDims.end());
			std.sort(instanceDims.begin(), instanceDims.end());

			for (let i: number = 0; i < myDims.size(); i++)
				if (myDims.at(i) < instanceDims.at(i))
					return false;

			return true;
		}

		/* ===========================================================
			EXPORTERS
		=========================================================== */
		/**
		 * @inheritdoc
		 */
		public TYPE(): string
		{
			return "wrapper";
		}

		/**
		 * @inheritdoc
		 */
		public TAG(): string
		{
			return "instance";
		}

		/**
		 * @inheritdoc
		 */
		public CHILD_TAG(): string
		{
			return "wrap";
		}

		/**
		 * @inheritdoc
		 */
		public toXML(): samchon.library.XML
		{
			let xml: samchon.library.XML = super.toXML();

			// WIDTH, HEIGHT AND LENGTH
			xml.setProperty("width", this.width_);
			xml.setProperty("height", this.height_);
			xml.setProperty("length", this.length_);
			xml.eraseProperty("width_");
			xml.eraseProperty("height_");
			xml.eraseProperty("length_");

			// TYPE
			xml.setProperty("type", this.TYPE());

			return xml;
		}

		/* ===========================================================
			VISUALIZERS
				- MEMBERS
				- FACTORY METHOD
				- EVENTS
		==============================================================
			MEMBERS
		----------------------------------------------------------- */
		private static scene: THREE.Scene = null;
		private static renderer: THREE.WebGLRenderer = null;

		private static camera: THREE.PerspectiveCamera = null;
		private static trackball: THREE.TrackballControls = null;
		private static mouse: THREE.Vector2 = null;

		private static get BOUNDARY_THICKNESS(): number { return 0.5; }

		/* -----------------------------------------------------------
			FACTORY METHOD
		----------------------------------------------------------- */
		/**
		 * <p> Convert to a canvas containing 3D elements. </p>
		 * 
		 * @param endIndex
		 *
		 * @return A 3D-canvans printing the Wrapper and its children {@link Wrap wrapped} 
		 *						{@link Instance instances} with those boundary lines.
		 */
		public toCanvas(endIndex: number = Number.MAX_VALUE): HTMLCanvasElement
		{
			// ---------------------------------------
			// CONSTRUCTS
			// ---------------------------------------
			// SCENE AND GEOMETRY
			Wrapper.scene = new THREE.Scene();
			let geometry: THREE.BoxGeometry = new THREE.BoxGeometry(1, 1, 1);

			// BOUNDARY LINES
			for (let i: number = 1; i <= 12; i++)
			{
				let boundaryLine: THREE.Mesh = 
					new THREE.Mesh
					(
						geometry,
						new THREE.MeshPhongMaterial
						({
							color: 0xFFFFFF, shading: THREE.FlatShading, 
							vertexColors: THREE.VertexColors, shininess: 0 
						})
					);
				let width: number, height: number, length: number;
				let x: number, y: number, z: number;
				
				// SCALE
				switch (i)
				{
					case 1: case 3: case 9: case 12:
						width = this.width_ + 2 * Wrapper.BOUNDARY_THICKNESS;
						height = Wrapper.BOUNDARY_THICKNESS;
						length = Wrapper.BOUNDARY_THICKNESS;
						break;
					case 2: case 4: case 10: case 11: case 10:
						height = this.height_ + 2 * Wrapper.BOUNDARY_THICKNESS;
						width = Wrapper.BOUNDARY_THICKNESS;
						length = Wrapper.BOUNDARY_THICKNESS;
						break;
					default: // 5, 6, 7, 8
						length = this.length_ + 2 * Wrapper.BOUNDARY_THICKNESS;
						width = Wrapper.BOUNDARY_THICKNESS;
						height = Wrapper.BOUNDARY_THICKNESS;
						break;
				}

				// X
				switch (i)
				{
					case 4: case 6: case 8: case 11:
						x = this.width_ + Wrapper.BOUNDARY_THICKNESS;
						break;
					default:
						x = -Wrapper.BOUNDARY_THICKNESS;
						break;
				}

				// Y
				switch (i)
				{
					case 3: case 7: case 8: case 12:
						y = this.height_;
						break;
					default:
						y = -Wrapper.BOUNDARY_THICKNESS;
						break;
				}

				// Z
				switch (i)
				{
					case 9: case 10: case 11: case 12:
						z = this.length_ + Wrapper.BOUNDARY_THICKNESS;
						break;
					default:
						z = -Wrapper.BOUNDARY_THICKNESS;
						break;
				}

				// SET POSITION AND SCALE
				boundaryLine.scale.set(width, height, length);
				boundaryLine.position.set(x + width/2, y + height/2, z + length/2);
				
				Wrapper.scene.add(boundaryLine);
			}

			// CHILDREN (PACKED) INSTANCES
			for (let i: number = 0; i < Math.min(endIndex, this.size()); i++)
			{
				// 1st to 11th: boundaries, 12th: shape
				let objects = this.at(i).toDisplayObjects(geometry);
				
				for (let j: number = 0; j < objects.size(); j++)
					Wrapper.scene.add(objects.at(j));
			}

			// LIGHTS
			let ambientLight: THREE.AmbientLight = new THREE.AmbientLight(0x555555);
			//let spotLight: THREE.SpotLight = new THREE.SpotLight(0xFFFFFF, 1.5);

			//spotLight.position.set(0, 500, 2000);
			
			Wrapper.scene.add(ambientLight);
			//Wrapper.scene.add(spotLight);

			// ---------------------------------------
			// CAMERA, TRACKBALL AND MOUSE
			// ---------------------------------------
			if (Wrapper.camera == null) // LAZY CREATION
			{
				Wrapper.camera = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 1, 10000);
				Wrapper.camera.position.z = this.length_ * 5;

				Wrapper.trackball = new THREE.TrackballControls(Wrapper.camera);
				Wrapper.trackball.rotateSpeed = 10;
				Wrapper.trackball.zoomSpeed = 1.2;
				Wrapper.trackball.panSpeed = 0.8;
				Wrapper.trackball.noZoom = false;
				Wrapper.trackball.noPan = false;
				Wrapper.trackball.staticMoving = true;
				Wrapper.trackball.dynamicDampingFactor = 0.3;
				
				Wrapper.mouse = new THREE.Vector2();

				// RENDERER
				Wrapper.renderer = new THREE.WebGLRenderer({ antialias: true });
				Wrapper.renderer.setClearColor(0xFFFFFF);
				Wrapper.renderer.setPixelRatio(window.devicePixelRatio);
				Wrapper.renderer.setSize(window.innerWidth * .75, window.innerHeight);
				Wrapper.renderer.sortObjects = false;

				Wrapper.renderer.domElement.addEventListener("mousemove", Wrapper.handleMouseMove);
				Wrapper.animate();
			}

			// ---------------------------------------
			// RETURNS AN HTML_ELEMENT OF THE RENDERER
			// ---------------------------------------
			return Wrapper.renderer.domElement;
		}

		/* -----------------------------------------------------------
			EVENTS
		----------------------------------------------------------- */
		private static handleMouseMove(event: MouseEvent): void
		{
			Wrapper.mouse.x = event.clientX;
			Wrapper.mouse.y = event.clientY;
		}
		private static animate(): void
		{
			requestAnimationFrame(Wrapper.animate);
			Wrapper.render();
		}
		private static render(): void
		{
			Wrapper.trackball.update();
			Wrapper.renderer.render(Wrapper.scene, Wrapper.camera);
		}
	}
}