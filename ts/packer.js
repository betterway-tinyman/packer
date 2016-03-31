var __extends = (this && this.__extends) || function (d, b) {
    for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
    function __() { this.constructor = d; }
    d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
};
var boxologic;
(function (boxologic) {
    /**
     * <p> An abstract instance of boxologic. </p>
     *
     * <p> {@link st_Instance} represents a physical, tangible instance of 3-dimension. </p>
     *
     * @author Jeongho Nam <http://samchon.org>
     */
    var Instance = (function () {
        /* -----------------------------------------------------------
            CONSTRUCTORS
        ----------------------------------------------------------- */
        /**
         * Construct from size members.
         *
         * @param width Width, length on the X-axis in 3D.
         * @param height Height, length on the Y-axis in 3D.
         * @param length Length, length on the Z-axis in 3D.
         */
        function Instance(width, height, length) {
            // INIT MEMBERS
            this.width = width;
            this.height = height;
            this.length = length;
            // INIT LAYOUTS
            this.layout_width = width;
            this.layout_height = height;
            this.layout_length = length;
            // DERIVED PROPERTY; VOLUME
            this.volume = width * height * length;
        }
        return Instance;
    }());
    boxologic.Instance = Instance;
})(boxologic || (boxologic = {}));
/// <reference path="Instance.ts" />
var boxologic;
(function (boxologic) {
    /**
     * A box, trying to pack into a {@link Pallet}.
     *
     * @author Bill Knechtel, <br>
     *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
     */
    var Box = (function (_super) {
        __extends(Box, _super);
        /**
         * Construct from an instance.
         *
         * @param instance An instance adapts with.
         */
        function Box(instance) {
            _super.call(this, instance.getWidth(), instance.getHeight(), instance.getLength());
            this.cox = 0;
            this.coy = 0;
            this.coz = 0;
            this.is_packed = false;
        }
        return Box;
    }(boxologic.Instance));
    boxologic.Box = Box;
})(boxologic || (boxologic = {}));
/**
* <p> A set of programs that calculate the best fit for boxes on a pallet migrated from language C. </p>
*
* <ul>
*	<li> Original Boxologic: https://github.com/exad/boxologic </li>
* </ul>
*
* @author Bill Knechtel, <br>
*		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
*/
var boxologic;
(function (boxologic) {
    /**
     * <p> A facade class of boxologic. </p>
     *
     * <p> The Boxologic class dudcts the best solution of packing boxes to a pallet. </p>
     *
     * <ul>
     *	<li> Reference: https://github.com/exad/boxologic </li>
     * </ul>
     *
     * @author Bill Knechtel, <br>
     *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
     */
    var Boxologic = (function () {
        /* ===========================================================
            CONSTRUCTORS
                - CONSTRUCTOR
                - ENCODER & DECODER
        ==============================================================
            CONSTRUCTOR
        ----------------------------------------------------------- */
        /**
         * Construct from a wrapper and instances.
         *
         * @param wrapper A Wrapper to pack instances.
         * @param instanceArray Instances trying to put into the wrapper.
         */
        function Boxologic(wrapper, instanceArray) {
            this.wrapper = wrapper;
            this.instanceArray = instanceArray;
            this.leftInstances = new bws.packer.InstanceArray();
        }
        /* -----------------------------------------------------------
            ENCODER & DECODER
        ----------------------------------------------------------- */
        /**
         * <p> Encode data </p>
         *
         * <p> Encodes {@link bws.packer Packer}'s data to be suitable for the
         * {@link boxologic Boxologic}'s parametric data. </p>
         */
        Boxologic.prototype.encode = function () {
            /////////////////////////////////////
            // STRUCTURES
            /////////////////////////////////////
            this.pallet = new boxologic.Pallet(this.wrapper);
            this.box_array = new std.Vector();
            this.total_box_volume = 0.0;
            this.layer_map = new std.HashMap();
            this.scrap_list = new std.Vector();
            // CHILDREN ELEMENTS - BOX
            this.box_array.assign(this.instanceArray.size(), null);
            for (var i = 0; i < this.instanceArray.size(); i++) {
                var box = new boxologic.Box(this.instanceArray.at(i));
                this.total_box_volume += box.volume;
                this.box_array[i] = box;
            }
            // SCRAP_LIST
            this.scrap_list.pushBack(new boxologic.Scrap());
            /////////////////////////////////////
            // BEST VARIABLES
            /////////////////////////////////////
            this.best_solution_volume = 0.0;
            this.packing_best = false;
            this.hundred_percent = false;
        };
        /**
         * <p> Decode data </p>
         *
         * <p> Decodes the Boxologic's optimization result data to be suitable for the Packer's own. </p>
         */
        Boxologic.prototype.decode = function () {
            this.wrapper.clear();
            this.leftInstances.clear();
            for (var i = 0; i < this.box_array.size(); i++) {
                var instance = this.instanceArray.at(i);
                var box = this.box_array.at(i);
                if (box.is_packed == true) {
                    var wrap = new bws.packer.Wrap(this.wrapper, instance, box.cox, box.coy, box.coz);
                    wrap.estimateOrientation(box.layout_width, box.layout_height, box.layout_length);
                    if (this.wrapper.getThickness() != 0)
                        wrap.setPosition(wrap.getX() + this.wrapper.getThickness(), wrap.getY() + this.wrapper.getThickness(), wrap.getZ() + this.wrapper.getThickness());
                    this.wrapper.pushBack(wrap);
                }
                else {
                    // NOT WRAPED INSTANCES BY LACK OF VOLUME
                    this.leftInstances.pushBack(instance);
                }
            }
        };
        /* ===========================================================
            MAIN PROCEDURES
                - OPERATORS
                - CHECKERS
                - GETTERS
                - REPORTERS
        ==============================================================
            OPERATORS
        ------------------------------------------------------------ */
        /**
         * <p> Pack instances to the {@link wrapper}. </p>
         *
         * <p> The {@link Boxologic.pack} is an adaptor method between {@link bws.packer Packer} and
         * {@link boxologic}. It encodes data from {@link bws.packer Packer}, deducts the best packing
         * solution decodes the optimization result and returns it. </p>
         *
         * <p> The optimization result is returned as a {@link Pair} like below: </p>
         * <ul>
         *	<li> first: The {@link wrapper} with packed instances. </li>
         *	<li> second: {@link leftInstances Left instances failed to pack} by overloading. </li>
         * </ul>
         *
         * @return A pair of {@link wrapper} with packed instances and
         *		   {@link leftInstances instances failed to pack} by overloading.
         */
        Boxologic.prototype.pack = function () {
            this.encode();
            this.iterate_orientations();
            this.report_results();
            this.decode();
            return new std.Pair(this.wrapper, this.leftInstances);
        };
        /**
         * <p> Execute iterations by calling proper functions. </p>
         *
         * <p> Iterations are done and parameters of the best solution are found. </p>
         */
        Boxologic.prototype.iterate_orientations = function () {
            for (var orientation_1 = 1; orientation_1 <= 6; orientation_1++) {
                this.pallet.set_orientation(orientation_1);
                // CONSTRUCT LAYERS
                this.construct_layers();
                // ITERATION IN LAYERS
                for (var it = this.layer_map.begin(); !it.equals(this.layer_map.end()); it = it.next()) {
                    // BEGINS PACKING
                    this.iterate_layer(it.first);
                    if (this.packed_volume > this.best_solution_volume) {
                        // NEW VOLUME IS THE BEST
                        this.best_solution_volume = this.packed_volume;
                        this.best_orientation = orientation_1;
                        this.best_layer = it.first;
                    }
                    if (this.hundred_percent)
                        break; // SUCCESS TO UTILIZE ALL
                }
                if (this.hundred_percent)
                    break; // SUCCESS TO UTILIZE ALL
                // IF THE PALLET IS REGULAR CUBE,
                if (this.pallet.width == this.pallet.height && this.pallet.height == this.pallet.length)
                    orientation_1 = 6; // DON'T ITERATE ALL ORIENTATIONS
            }
        };
        /**
         * Iterate a layer.
         *
         * @param thickness Thickness of the iterating layer.
         */
        Boxologic.prototype.iterate_layer = function (thickness) {
            // INIT PACKED
            this.packing = true;
            this.packed_volume = 0.0;
            this.packed_layout_height = 0;
            this.layer_thickness = thickness;
            // SET REMAINS FROM PALLET'S DIMENSIONS
            this.remain_layout_height = this.pallet.layout_height;
            this.remain_layout_length = this.pallet.layout_length;
            // UNPACK ALL BOXES
            for (var i = 0; i < this.box_array.size(); i++)
                this.box_array[i].is_packed = false;
            do {
                // INIT VARS OF LAYER ITERATION
                this.layer_in_layer = 0;
                this.layer_done = false;
                // PACK_LAYER AND POST-PROCESS
                this.pack_layer();
                this.packed_layout_height += this.layer_thickness;
                this.remain_layout_height = this.pallet.layout_height - this.packed_layout_height;
                if (this.layer_in_layer != 0) {
                    // STORE ORDINARY PACKING VARS
                    var pre_packed_y = this.packed_layout_height;
                    var pre_remain_py = this.remain_layout_height;
                    // STORE CAUCLATED RESULTS
                    this.remain_layout_height = this.layer_thickness - this.pre_layer;
                    this.packed_layout_height -= this.layer_thickness + this.pre_layer;
                    this.remain_layout_length = this.lilz;
                    this.layer_thickness = this.layer_in_layer;
                    // ITERATION IS NOT FINISHED YET
                    this.layer_done = false;
                    // RE-CALL PACK_LAYER
                    this.pack_layer();
                    // REVERT TO THE STORED ORDINARIES
                    this.packed_layout_height = pre_packed_y;
                    this.remain_layout_height = pre_remain_py;
                    this.remain_layout_length = this.pallet.layout_length;
                }
                // CALL FIND_LAYER
                this.find_layer(this.remain_layout_height);
            } while (this.packing);
        };
        /**
         * <p> Construct layers. </p>
         *
         * <p> Creates all possible layer heights by giving a weight value to each of them. </p>
         */
        Boxologic.prototype.construct_layers = function () {
            this.layer_map.clear();
            for (var i = 0; i < this.box_array.size(); i++) {
                var box = this.box_array[i];
                for (var j = 1; j <= 3; j++) {
                    var ex_dim = void 0; // STANDARD LENGTH ON THE DIMENSION
                    var dimen2 = void 0; // THE SECOND, LENGTH ON A RESIDUAL DIMENSION
                    var dimen3 = void 0; // THE THIRD, LENGTH ON A RESIDUAL DIMENSION
                    var layer_eval = 0; // SUM OF LAYERS (height)
                    // FETCH STANDARD DIMENSIONS FROM EACH AXIS
                    switch (j) {
                        case 1:
                            ex_dim = box.width;
                            dimen2 = box.height;
                            dimen3 = box.length;
                            break;
                        case 2:
                            ex_dim = box.height;
                            dimen2 = box.width;
                            dimen3 = box.length;
                            break;
                        case 3:
                            ex_dim = box.length;
                            dimen2 = box.width;
                            dimen3 = box.height;
                            break;
                    }
                    // A DIMENSIONAL LENGTH IS GREATER THAN THE PALLET ?
                    if (ex_dim > this.pallet.layout_height ||
                        ((dimen2 > this.pallet.layout_width || dimen3 > this.pallet.layout_length) &&
                            (dimen3 > this.pallet.layout_width || dimen2 > this.pallet.layout_length))) {
                        // A DIMENSIONAL LENGTH IS GREATER THAN THE PALLET
                        continue;
                    }
                    // WHEN A DUPLICATED LAYER EXISTS, SKIPS
                    if (this.layer_map.has(ex_dim) == true)
                        continue;
                    // ABOUT ALL BOXES, FIND THE MINIMUM LENGTH OF GAP ~,
                    // STACK ON THE CURRENT LAYER (ADD ON TO THE LAYER_EVAL)
                    for (var k = 0; k < this.box_array.size(); k++) {
                        // SAME INSTANCE WITH THE SAME INDEX
                        if (i == k)
                            continue;
                        var my_box = this.box_array[k];
                        var dim_diff = Math.min(Math.abs(ex_dim - my_box.width), Math.abs(ex_dim - my_box.height), Math.abs(ex_dim - my_box.length));
                        layer_eval += dim_diff;
                    }
                    // RECORD THE SUM
                    this.layer_map.set(ex_dim, layer_eval);
                }
            }
        };
        /**
         * <p> Packs the boxes found and arranges all variables and records properly. </p>
         *
         * <p> Update the linked list and the Boxlist[] array as a box is packed. </p>
         */
        Boxologic.prototype.pack_layer = function () {
            if (this.layer_thickness == 0) {
                this.packing = false;
                return;
            }
            else if (this.scrap_list.empty() == true)
                return;
            var lenx;
            var lenz;
            var lpz;
            this.scrap_list.begin().value.cumx = this.pallet.layout_width;
            this.scrap_list.begin().value.cumz = 0;
            while (true) {
                // INIT SCRAP_MIN_Z
                this.find_smallest_z();
                // FETCH LEFT AND RIGHT OF SCRAP_MIN_Z
                var prev = this.scrap_min_z.prev();
                var next = this.scrap_min_z.next();
                if (this.scrap_min_z.equals(this.scrap_list.end())) {
                    break;
                }
                if (prev.equals(this.scrap_list.end()) && next.equals(this.scrap_list.end())) {
                    /////////////////////////////////////////////////////////
                    // NO LEFT AND RIGHT
                    /////////////////////////////////////////////////////////
                    //*** SITUATION-1: NO BOXES ON THE RIGHT AND LEFT SIDES ***
                    lenx = this.scrap_min_z.value.cumx;
                    lpz = this.remain_layout_length - this.scrap_min_z.value.cumz;
                    // CALL FIND_BOX AND CHECK_FOUND
                    this.find_box(lenx, this.layer_thickness, this.remain_layout_height, lpz, lpz);
                    this.check_found();
                    // BREAK ?
                    if (this.layer_done)
                        break;
                    if (this.evened)
                        continue;
                    // UPDATE CURRENT BOX
                    var box = this.box_array[this.cboxi];
                    box.cox = 0;
                    box.coy = this.packed_layout_height;
                    box.coz = this.scrap_min_z.value.cumz;
                    if (this.cbox_layout_width == this.scrap_min_z.value.cumx) {
                        // CUMULATE
                        this.scrap_min_z.value.cumz += this.cbox_layout_length;
                    }
                    else {
                        // CREATE A NEW NODE AND IT'S THE NEW MIN_Z
                        // ORDINARY MIN_Z WILL BE SHIFTED TO THE RIGHT
                        var scrap = new boxologic.Scrap(this.cbox_layout_width, this.scrap_min_z.value.cumz + this.cbox_layout_length);
                        // SHIFTS ORDINARY MIN_Z TO RIGHT
                        // AND THE NEW NODE'S ITERATOR IS THE NEW MIN_Z FROM NOW ON
                        this.scrap_min_z = this.scrap_list.insert(this.scrap_min_z, scrap);
                    }
                }
                else if (prev.equals(this.scrap_list.end())) {
                    /////////////////////////////////////////////////////////
                    // NO LEFT, BUT RIGHT
                    /////////////////////////////////////////////////////////
                    //*** SITUATION-2: NO BOXES ON THE LEFT SIDE ***
                    lenx = this.scrap_min_z.value.cumx;
                    lenz = next.value.cumz - this.scrap_min_z.value.cumz;
                    lpz = this.remain_layout_length - this.scrap_min_z.value.cumz;
                    // CALL FIND_BOX AND CHECK_FOUND
                    this.find_box(lenx, this.layer_thickness, this.remain_layout_height, lenz, lpz);
                    this.check_found();
                    // BREAK ?
                    if (this.layer_done)
                        break;
                    if (this.evened)
                        continue;
                    // RE-FETCH LEFT AND RIGHT
                    next = this.scrap_min_z.next();
                    // UPDATE CURRENT BOX
                    var box = this.box_array[this.cboxi];
                    box.coy = this.packed_layout_height;
                    box.coz = this.scrap_min_z.value.cumz;
                    if (this.cbox_layout_width == this.scrap_min_z.value.cumx) {
                        box.cox = 0;
                        if (this.scrap_min_z.value.cumz + this.cbox_layout_length == next.value.cumz) {
                            // RIGHT IS THE NEW MIN_Z
                            // ORDINARY MIN_Z WILL BE ERASED
                            this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z);
                        }
                        else {
                            // CUMULATE
                            this.scrap_min_z.value.cumz += this.cbox_layout_length;
                        }
                    }
                    else {
                        box.cox = this.scrap_min_z.value.cumx - this.cbox_layout_width;
                        if (this.scrap_min_z.value.cumz + this.cbox_layout_length == next.value.cumz) {
                            // DE-CUMULATE
                            this.scrap_min_z.value.cumx -= this.cbox_layout_width;
                        }
                        else {
                            // UPDATE MIN_Z
                            this.scrap_min_z.value.cumx -= this.cbox_layout_width;
                            // CREATE A NEW NODE BETWEEN MIN_Z AND RIGHT
                            var scrap = new boxologic.Scrap(this.scrap_min_z.value.cumx, this.scrap_min_z.value.cumz + this.cbox_layout_length);
                            this.scrap_list.insert(next, scrap);
                        }
                    }
                }
                else if (next.equals(this.scrap_list.end())) {
                    ////////////////////////////////////////////////////////
                    // NO RIGHT BUT LEFT
                    /////////////////////////////////////////////////////////
                    //*** SITUATION-3: NO BOXES ON THE RIGHT SIDE ***
                    lenx = this.scrap_min_z.value.cumx - prev.value.cumx;
                    lenz = prev.value.cumz - this.scrap_min_z.value.cumz;
                    lpz = this.remain_layout_length - this.scrap_min_z.value.cumz;
                    // CALL FIND_BOX AND CHECK_FOUND
                    this.find_box(lenx, this.layer_thickness, this.remain_layout_height, lenz, lpz);
                    this.check_found();
                    // BREAK ?
                    if (this.layer_done)
                        break;
                    if (this.evened)
                        continue;
                    // RE-FETCH LEFT AND RIGHT
                    prev = this.scrap_min_z.prev();
                    // UPDATE CURRENT BOX
                    var box = this.box_array[this.cboxi];
                    box.coy = this.packed_layout_height;
                    box.coz = this.scrap_min_z.value.cumz;
                    box.cox = prev.value.cumx;
                    if (this.cbox_layout_width == this.scrap_min_z.value.cumx - prev.value.cumx) {
                        if (this.scrap_min_z.value.cumz + this.cbox_layout_length == prev.value.cumz) {
                            // LEFT FETCHES MIN_Z'S CUM_X
                            prev.value.cumx = this.scrap_min_z.value.cumx;
                            // ERASE FROM MIN_Z TO END
                            this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z, this.scrap_list.end());
                        }
                        else {
                            // CUMULATE
                            this.scrap_min_z.value.cumz += this.cbox_layout_length;
                        }
                    }
                    else {
                        if (this.scrap_min_z.value.cumz + this.cbox_layout_length == prev.value.cumz) {
                            // CUMULATE
                            prev.value.cumx += this.cbox_layout_width;
                        }
                        else {
                            // CREATE A NEW NODE BETWEEN LEFT AND MIN_Z
                            var scrap = new boxologic.Scrap(prev.value.cumx + this.cbox_layout_width, this.scrap_min_z.value.cumz + this.cbox_layout_length);
                            this.scrap_list.insert(this.scrap_min_z, scrap);
                        }
                    }
                }
                else if (prev.value.cumz == next.value.cumz) {
                    ////////////////////////////////////////////////////////
                    // LEFT AND RIGHT ARE ALL EXIST .value. SAME CUMZ
                    /////////////////////////////////////////////////////////
                    //*** SITUATION-4: THERE ARE BOXES ON BOTH OF THE SIDES ***
                    //*** SUBSITUATION-4A: SIDES ARE EQUAL TO EACH OTHER ***
                    lenx = this.scrap_min_z.value.cumx - prev.value.cumx;
                    lenz = prev.value.cumz - this.scrap_min_z.value.cumz;
                    lpz = this.remain_layout_length - this.scrap_min_z.value.cumz;
                    // CALL FIND_BOX AND CHECK_FOUND
                    this.find_box(lenx, this.layer_thickness, this.remain_layout_height, lenz, lpz);
                    this.check_found();
                    // BREAK ?
                    if (this.layer_done)
                        break;
                    if (this.evened)
                        continue;
                    // RE-FETCH LEFT AND RIGHT
                    prev = this.scrap_min_z.prev();
                    next = this.scrap_min_z.next();
                    // UPDATE CURRENT BOX
                    var box = this.box_array[this.cboxi];
                    box.coy = this.packed_layout_height;
                    box.coz = this.scrap_min_z.value.cumz;
                    if (this.cbox_layout_width == this.scrap_min_z.value.cumx - prev.value.cumx) {
                        box.cox = prev.value.cumx;
                        if (this.scrap_min_z.value.cumz + this.cbox_layout_length == next.value.cumz) {
                            // LEFT FETCHES RIGHT'S CUM_X
                            prev.value.cumx = next.value.cumx;
                            // ERASE MIN_Z AND RIGHT
                            this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z, next.next());
                        }
                        else {
                            // CUMULATE
                            this.scrap_min_z.value.cumz += this.cbox_layout_length;
                        }
                    }
                    else if (prev.value.cumx < this.pallet.layout_width - this.scrap_min_z.value.cumx) {
                        if (this.scrap_min_z.value.cumz + this.cbox_layout_length == prev.value.cumz) {
                            // DE-CUMULATE
                            this.scrap_min_z.value.cumx -= this.cbox_layout_width;
                            box.cox = this.scrap_min_z.value.cumx;
                        }
                        else {
                            box.cox = prev.value.cumx;
                            // CREATE A NODE BETWEEN LEFT AND MIN_Z
                            var scrap = new boxologic.Scrap(prev.value.cumx + this.cbox_layout_width, this.scrap_min_z.value.cumz + this.cbox_layout_length);
                            this.scrap_list.insert(this.scrap_min_z, scrap);
                        }
                    }
                    else {
                        if (this.scrap_min_z.value.cumz + this.cbox_layout_length == prev.value.cumz) {
                            // CUMULATE
                            prev.value.cumx += this.cbox_layout_width;
                            box.cox = prev.value.cumx;
                        }
                        else {
                            box.cox = this.scrap_min_z.value.cumx - this.cbox_layout_width;
                            // CREATE A NODE BETWEEN MIN_Z AND RIGHT
                            var scrap = new boxologic.Scrap(this.scrap_min_z.value.cumx, this.scrap_min_z.value.cumz + this.cbox_layout_length);
                            this.scrap_list.insert(next, scrap);
                            // UPDATE MIN_Z
                            this.scrap_min_z.value.cumx -= this.cbox_layout_width;
                        }
                    }
                }
                else {
                    ////////////////////////////////////////////////////////
                    // LEFT AND RIGHT ARE ALL EXIST
                    ////////////////////////////////////////////////////////
                    //*** SUBSITUATION-4B: SIDES ARE NOT EQUAL TO EACH OTHER ***
                    lenx = this.scrap_min_z.value.cumx - prev.value.cumx;
                    lenz = prev.value.cumz - this.scrap_min_z.value.cumz;
                    lpz = this.remain_layout_length - this.scrap_min_z.value.cumz;
                    // CALL FIND_BOX AND CHECK_FOUND
                    this.find_box(lenx, this.layer_thickness, this.remain_layout_height, lenz, lpz);
                    this.check_found();
                    // BREAK ?
                    if (this.layer_done)
                        break;
                    if (this.evened)
                        continue;
                    // RE-FETCH LEFT AND RIGHT
                    prev = this.scrap_min_z.prev();
                    next = this.scrap_min_z.next();
                    // UPDATE CURRENT BOX
                    var box = this.box_array[this.cboxi];
                    box.coy = this.packed_layout_height;
                    box.coz = this.scrap_min_z.value.cumz;
                    box.cox = prev.value.cumx;
                    if (this.cbox_layout_width == this.scrap_min_z.value.cumx - prev.value.cumx) {
                        if (this.scrap_min_z.value.cumz + this.cbox_layout_length == prev.value.cumz) {
                            // LEFT FETCHES MIN_Z'S
                            prev.value.cumx = this.scrap_min_z.value.cumx;
                            // ERASE MIN_Z
                            this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z);
                        }
                        else {
                            // CUMULATE
                            this.scrap_min_z.value.cumz += this.cbox_layout_length;
                        }
                    }
                    else {
                        if (this.scrap_min_z.value.cumz + this.cbox_layout_length == prev.value.cumz) {
                            // CUMULATE
                            prev.value.cumx += this.cbox_layout_width;
                        }
                        else if (this.scrap_min_z.value.cumz + this.cbox_layout_length == next.value.cumz) {
                            // DE-CUMULATE
                            this.scrap_min_z.value.cumx -= this.cbox_layout_width;
                            box.cox = this.scrap_min_z.value.cumx;
                        }
                        else {
                            // CREATE NODE BETWEEN LEFT AND MIN_Z
                            var scrap = new boxologic.Scrap(prev.value.cumx + this.cbox_layout_width, this.scrap_min_z.value.cumz + this.cbox_layout_length);
                            this.scrap_list.insert(this.scrap_min_z, scrap);
                        }
                    }
                }
                this.volume_check();
            }
        };
        /**
         * Find the most proper layer height by looking at the unpacked boxes and
         * the remaining empty space available.
         */
        Boxologic.prototype.find_layer = function (thickness) {
            // MINIMUM SUM OF LAYERS (height)
            var min_eval = Number.MAX_VALUE;
            this.layer_thickness = 0;
            for (var i = 0; i < this.box_array.size(); i++) {
                var box = this.box_array[i];
                if (box.is_packed)
                    continue;
                for (var j = 1; j <= 3; j++) {
                    var ex_dim = void 0; // STANDARD LENGTH ON THE DIMENSION
                    var dim2 = void 0; // THE SECOND, LENGTH ON A RESIDUAL DIMENSION
                    var dim3 = void 0; // THE THIRD, LENGTH ON A RESIDUAL DIMENSION
                    var my_eval = 0;
                    // FETCH STANDARD DIMENSIONS FROM EACH AXIS
                    switch (j) {
                        case 1:
                            ex_dim = box.width;
                            dim2 = box.height;
                            dim3 = box.length;
                            break;
                        case 2:
                            ex_dim = box.height;
                            dim2 = box.width;
                            dim3 = box.length;
                            break;
                        case 3:
                            ex_dim = box.length;
                            dim2 = box.width;
                            dim3 = box.height;
                            break;
                    }
                    // ABOUT ALL BOXES, FIND THE MINIMUM LENGTH OF GAP ~,
                    // STACK ON THE CURRENT LAYER (ADD ON TO THE LAYER_EVAL)
                    if (ex_dim <= thickness &&
                        ((dim2 <= this.pallet.layout_width && dim3 <= this.pallet.layout_length) ||
                            (dim3 <= this.pallet.layout_width && dim2 <= this.pallet.layout_length))) {
                        for (var k = 0; k < this.box_array.size(); k++) {
                            var my_box = this.box_array[k];
                            // SAME INSTANCE WITH THE SAME INDEX OR ALREADY PACKED
                            if (i == k || my_box.is_packed == true)
                                continue;
                            var dim_diff = Math.min(Math.abs(ex_dim - my_box.width), Math.abs(ex_dim - my_box.height), Math.abs(ex_dim - my_box.length));
                            my_eval += dim_diff;
                        }
                        if (my_eval < min_eval) {
                            min_eval = my_eval;
                            this.layer_thickness = ex_dim;
                        }
                    }
                }
            }
            if (this.layer_thickness == 0 || this.layer_thickness > this.remain_layout_height)
                this.packing = false;
        };
        /**
         * <p> Determine the gap with the samllest z value in the current layer. </p>
         *
         * <p> Find the most proper boxes by looking at all six possible orientations,
         * empty space given, adjacent boxes, and pallet limits. </p>
         *
         * @param hmx Maximum available x-dimension of the current gap to be filled.
         * @param hy Current layer thickness value.
         * @param hmy Current layer thickness value.
         * @param hz Z-dimension of the current gap to be filled.
         * @param hmz Maximum available z-dimension to the current gap to be filled.
         */
        Boxologic.prototype.find_box = function (hmx, hy, hmy, hz, hmz) {
            this.boxi = -1;
            this.bboxi = -1;
            this.bfx = Number.MAX_VALUE;
            this.bfy = Number.MAX_VALUE;
            this.bfz = Number.MAX_VALUE;
            this.bbfx = Number.MAX_VALUE;
            this.bbfy = Number.MAX_VALUE;
            this.bbfz = Number.MAX_VALUE;
            for (var i = 0; i < this.box_array.size(); i++) {
                var box = this.box_array[i];
                if (box.is_packed)
                    continue;
                this.analyze_box(i, hmx, hy, hmy, hz, hmz, box.width, box.height, box.length);
                // WHEN REGULAR CUBE
                if (box.width == box.length && box.length == box.height)
                    continue;
                this.analyze_box(i, hmx, hy, hmy, hz, hmz, box.width, box.length, box.height);
                this.analyze_box(i, hmx, hy, hmy, hz, hmz, box.height, box.width, box.length);
                this.analyze_box(i, hmx, hy, hmy, hz, hmz, box.height, box.length, box.width);
                this.analyze_box(i, hmx, hy, hmy, hz, hmz, box.length, box.width, box.height);
                this.analyze_box(i, hmx, hy, hmy, hz, hmz, box.length, box.height, box.width);
            }
        };
        /**
         * <p> Analyzes each unpacked {@link Box box} to find the best fitting one to the empty space. </p>
         *
         * <p> Used by {@link find_box find_box()} to analyze box dimensions. </p>
         *
         * @param x index of a {@link Box box} in the {@link box_array}.
         *
         * @param hmx Maximum available x-dimension of the current gap to be filled.
         * @param hy Current layer thickness value.
         * @param hmy Current layer thickness value.
         * @param hz Z-dimension of the current gap to be filled.
         * @param hmz Maximum available z-dimension to the current gap to be filled.
         *
         * @param dim1 X-dimension of the orientation of the box being examined.
         * @param dim2 Y-dimension of the orientation of the box being examined.
         * @param dim3 Z-dimension of the orientation of the box being examined.
         */
        Boxologic.prototype.analyze_box = function (index, hmx, hy, hmy, hz, hmz, dim1, dim2, dim3) {
            // OUT OF BOUNDARY RANGE
            if (dim1 > hmx || dim2 > hmy || dim3 > hmz)
                return;
            if (dim2 <= hy &&
                (hy - dim2 < this.bfy ||
                    (hy - dim2 == this.bfy && hmx - dim1 < this.bfx) ||
                    (hy - dim2 == this.bfy && hmx - dim1 == this.bfx && Math.abs(hz - dim3) < this.bfz))) {
                this.boxx = dim1;
                this.boxy = dim2;
                this.boxz = dim3;
                this.bfx = hmx - dim1;
                this.bfy = hy - dim2;
                this.bfz = Math.abs(hz - dim3);
                this.boxi = index;
            }
            else if (dim2 > hy &&
                (dim2 - hy < this.bbfy ||
                    (dim2 - hy == this.bbfy && hmx - dim1 < this.bbfx) ||
                    (dim2 - hy == this.bbfy && hmx - dim1 == this.bbfx && Math.abs(hz - dim3) < this.bbfz))) {
                this.bboxx = dim1;
                this.bboxy = dim2;
                this.bboxz = dim3;
                this.bbfx = hmx - dim1;
                this.bbfy = dim2 - hy;
                this.bbfz = Math.abs(hz - dim3);
                this.bboxi = index;
            }
        };
        /**
         * After finding each box, the candidate boxes and the condition of the layer are examined.
         */
        Boxologic.prototype.check_found = function () {
            this.evened = false;
            if (this.boxi != -1) {
                this.cboxi = this.boxi;
                this.cbox_layout_width = this.boxx;
                this.cbox_layout_height = this.boxy;
                this.cbox_layout_length = this.boxz;
            }
            else {
                var prev = this.scrap_min_z.prev();
                var next = this.scrap_min_z.next();
                if (this.bboxi != -1 &&
                    (this.layer_in_layer != 0 ||
                        (
                        // NO LEFT AND RIGHT EXISTS
                        prev.equals(this.scrap_list.end()) && next.equals(this.scrap_list.end())))) {
                    ////////////////////////////////////////////
                    // ~ OR SCRAP_MIN_Z HAS NO NEIGHBOR
                    ////////////////////////////////////////////
                    if (this.layer_in_layer == 0) {
                        this.pre_layer = this.layer_thickness;
                        this.lilz = this.scrap_min_z.value.cumz;
                    }
                    this.cboxi = this.bboxi;
                    this.cbox_layout_width = this.bboxx;
                    this.cbox_layout_height = this.bboxy;
                    this.cbox_layout_length = this.bboxz;
                    this.layer_in_layer += this.bboxy - this.layer_thickness;
                    this.layer_thickness = this.bboxy;
                }
                else {
                    if (prev.equals(this.scrap_list.end()) && next.equals(this.scrap_list.end())) {
                        ///////////////////////////////////////////
                        // SCRAP_MIN_Z HAS NO NEIGHBOR
                        ///////////////////////////////////////////
                        // IN RANGE & NO NEIGHBOR
                        // LAYER HAS DONE.
                        this.layer_done = true;
                    }
                    else {
                        this.evened = true;
                        if (prev.equals(this.scrap_list.end())) {
                            ///////////////////////////////////////////
                            // NO LEFT, BUT RIGHT
                            ///////////////////////////////////////////
                            // ERASE SCRAP_MIN_Z
                            // RIGHT IS THE NEW SCRAP_MIN_Z
                            this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z);
                        }
                        else if (next.equals(this.scrap_list.end())) {
                            ///////////////////////////////////////////
                            // NO RIGHT, BUT LEFT
                            ///////////////////////////////////////////
                            // ERASE CURRENT SCRAP_MIN_Z
                            // THE LEFT ITEM FETCHES MIN'S CUM_X
                            prev.value.cumx = this.scrap_min_z.value.cumx;
                            // ERASE FROM MIN_Z TO END
                            this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z, this.scrap_list.end());
                        }
                        else {
                            ///////////////////////////////////////////
                            // LEFT & RIGHT ARE ALL EXIST
                            ///////////////////////////////////////////
                            if (prev.value.cumz == next.value.cumz) {
                                // ----------------------------------------
                                // LEFT AND RIGHT'S CUM_Z ARE EQUAL
                                // ----------------------------------------
                                // LEFT FETCHES THE RIGHT'S CUM_X
                                prev.value.cumx = next.value.cumx;
                                // ERASE MIN AND ITS RIGHT
                                this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z, next.next());
                            }
                            else {
                                // ----------------------------------------
                                // LEFT AND RIGHT'S CUM_Z ARE NOT EQUAL
                                // ----------------------------------------
                                if (prev.value.cumz == next.value.cumz)
                                    prev.value.cumx = this.scrap_min_z.value.cumx;
                                // ERASE SCRAP_MIN_Z
                                this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z);
                            }
                        }
                    }
                }
            }
        };
        /**
         * After packing of each box, 100% packing condition is checked.
         */
        Boxologic.prototype.volume_check = function () {
            var box = this.box_array[this.cboxi];
            box.is_packed = true;
            box.layout_width = this.cbox_layout_width;
            box.layout_height = this.cbox_layout_height;
            box.layout_length = this.cbox_layout_length;
            this.packed_volume += box.volume;
            if (this.packing_best) {
                // BOXOLOGIC DOESN'T MEMORIZE OPTIMIZED ORIENTATION
                // THUS IT NEEDS ADDITIONAL PROCEDURES FOR EXPORTING
                this.write_box_file();
            }
            else if (this.packed_volume == this.pallet.volume || this.packed_volume == this.total_box_volume) {
                this.packing = false;
                this.hundred_percent = true;
            }
        };
        /* -----------------------------------------------------------
            GETTERS
        ----------------------------------------------------------- */
        /**
         * <p> Find the first to be packed gap in the layer edge. </p>
         *
         * <p> Determine the gap with the {@link scrap_min_z smallest z} value in the current layer. </p>
         */
        Boxologic.prototype.find_smallest_z = function () {
            this.scrap_min_z = this.scrap_list.begin();
            for (var it = this.scrap_min_z; !it.equals(this.scrap_list.end()); it = it.next())
                if (it.value.cumz < this.scrap_min_z.value.cumz)
                    this.scrap_min_z = it;
        };
        /* -----------------------------------------------------------
            REPORTERS
        ----------------------------------------------------------- */
        /**
         * <p> Determine {@link box_arrray boxes}. </p>
         *
         * <p> Using the parameters found, packs the best solution found and reports. </p>
         */
        Boxologic.prototype.report_results = function () {
            ////////////////////////////////////////////////////
            // BEGINS RE-PACKING FOLLOWING THE BEST VARS
            ////////////////////////////////////////////////////
            this.packing_best = true;
            this.pallet.set_orientation(this.best_orientation);
            this.construct_layers();
            this.iterate_layer(this.best_layer);
        };
        /**
         * <p> Determine a {@link Box}. </p>
         *
         * <p> Transforms the found co-ordinate system to the one entered by the user and write them to the
         * report. </p>
         */
        Boxologic.prototype.write_box_file = function () {
            var box = this.box_array[this.cboxi];
            var cox;
            var coy;
            var coz;
            var layout_width;
            var layout_height;
            var layout_length;
            switch (this.best_orientation) {
                case 1:
                    cox = box.cox;
                    coy = box.coy;
                    coz = box.coz;
                    layout_width = box.layout_width;
                    layout_height = box.layout_height;
                    layout_length = box.layout_length;
                    break;
                case 2:
                    cox = box.coz;
                    coy = box.coy;
                    coz = box.cox;
                    layout_width = box.layout_length;
                    layout_height = box.layout_height;
                    layout_length = box.layout_width;
                    break;
                case 3:
                    cox = box.coy;
                    coy = box.coz;
                    coz = box.cox;
                    layout_width = box.layout_height;
                    layout_height = box.layout_length;
                    layout_length = box.layout_width;
                    break;
                case 4:
                    cox = box.coy;
                    coy = box.cox;
                    coz = box.coz;
                    layout_width = box.layout_height;
                    layout_height = box.layout_width;
                    layout_length = box.layout_length;
                    break;
                case 5:
                    cox = box.cox;
                    coy = box.coz;
                    coz = box.coy;
                    layout_width = box.layout_width;
                    layout_height = box.layout_length;
                    layout_length = box.layout_height;
                    break;
                case 6:
                    cox = box.coz;
                    coy = box.cox;
                    coz = box.coy;
                    layout_width = box.layout_length;
                    layout_height = box.layout_width;
                    layout_length = box.layout_height;
                    break;
            }
            box.cox = cox;
            box.coy = coy;
            box.coz = coz;
            box.layout_width = layout_width;
            box.layout_height = layout_height;
            box.layout_length = layout_length;
        };
        return Boxologic;
    }());
    boxologic.Boxologic = Boxologic;
})(boxologic || (boxologic = {}));
/// <reference path="Instance.ts" />
var boxologic;
(function (boxologic) {
    /**
     * A pallet containing boxes.
     *
     * @author Bill Knechtel, <br>
     *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
     */
    var Pallet = (function (_super) {
        __extends(Pallet, _super);
        /**
         * Construct from a wrapper.
         *
         * @param wrapper A wrapper wrapping instances.
         */
        function Pallet(wrapper) {
            _super.call(this, wrapper.getContainableWidth(), wrapper.getContainableHeight(), wrapper.getContainableLength());
        }
        /**
         * Set placement orientation.
         */
        Pallet.prototype.set_orientation = function (orientation) {
            switch (orientation) {
                case 1:
                    this.layout_width = this.width;
                    this.layout_height = this.height;
                    this.layout_length = this.length;
                    break;
                case 2:
                    this.layout_width = this.length;
                    this.layout_height = this.height;
                    this.layout_length = this.width;
                    break;
                case 3:
                    this.layout_width = this.length;
                    this.layout_height = this.width;
                    this.layout_length = this.height;
                    break;
                case 4:
                    this.layout_width = this.height;
                    this.layout_height = this.width;
                    this.layout_length = this.length;
                    break;
                case 5:
                    this.layout_width = this.width;
                    this.layout_height = this.length;
                    this.layout_length = this.height;
                    break;
                case 6:
                    this.layout_width = this.height;
                    this.layout_height = this.length;
                    this.layout_length = this.width;
                    break;
            }
        };
        return Pallet;
    }(boxologic.Instance));
    boxologic.Pallet = Pallet;
})(boxologic || (boxologic = {}));
var boxologic;
(function (boxologic) {
    /**
     * <p> Cumulated lengths of current layer. </p>
     *
     * <p> {@link Scrapped} represents an edge of the current layer under construction. </p>
     *
     * @author Bill Knechtel, <br>
     *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
     */
    var Scrap = (function () {
        function Scrap(cumx, cumz) {
            if (cumx === void 0) { cumx = 0; }
            if (cumz === void 0) { cumz = 0; }
            this.cumx = cumx;
            this.cumz = cumz;
        }
        return Scrap;
    }());
    boxologic.Scrap = Scrap;
})(boxologic || (boxologic = {}));
var bws;
(function (bws) {
    var packer;
    (function (packer) {
        var WrapperArray = (function (_super) {
            __extends(WrapperArray, _super);
            /* -----------------------------------------------------------
                CONSTRUCTORS
            ----------------------------------------------------------- */
            /**
             * Default Constructor.
             */
            function WrapperArray() {
                _super.call(this);
            }
            /**
             * @inheritdoc
             */
            WrapperArray.prototype.createChild = function (xml) {
                return new packer.Wrapper();
            };
            /* -----------------------------------------------------------
                GETTERS
            ----------------------------------------------------------- */
            /**
             * Get (calculate) price.
             */
            WrapperArray.prototype.getPrice = function () {
                var price = 0.0;
                for (var i = 0; i < this.size(); i++)
                    price += this.at(i).getPrice();
                return price;
            };
            /**
             * Get (calculate) utilization rate.
             */
            WrapperArray.prototype.getUtilization = function () {
                if (this.empty() == true)
                    return -1.0;
                var numerator = 0.0;
                var denominator = 0.0;
                for (var i = 0; i < this.size(); i++) {
                    var wrapper = this.at(i);
                    denominator += wrapper.getContainableVolume();
                    for (var j = 0; j < wrapper.size(); j++)
                        numerator += wrapper.getContainableVolume();
                }
                return numerator / denominator;
            };
            /* -----------------------------------------------------------
                EXPORTERS
            ----------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            WrapperArray.prototype.TAG = function () {
                return "wrapperArray";
            };
            /**
             * @inheritdoc
             */
            WrapperArray.prototype.CHILD_TAG = function () {
                return "instance";
            };
            return WrapperArray;
        }(samchon.protocol.EntityArray));
        packer.WrapperArray = WrapperArray;
    })(packer = bws.packer || (bws.packer = {}));
})(bws || (bws = {}));
/// <reference path="WrapperArray.ts" />
var bws;
(function (bws) {
    var packer;
    (function (packer) {
        var GAWrapperArray = (function (_super) {
            __extends(GAWrapperArray, _super);
            function GAWrapperArray(obj) {
                _super.call(this);
                this.result = new std.HashMap();
                this.price = 0.0;
                if (obj instanceof packer.InstanceArray) {
                    this.instanceArray = obj;
                }
                else {
                    var genes = obj;
                    this.instanceArray = genes.instanceArray;
                    this.assign(genes.begin(), genes.end());
                }
            }
            GAWrapperArray.prototype.constructResult = function () {
                if (this.result.empty() == false)
                    return; // IF RESULT IS ALREADY DEDUCTED
                // INSTANCE AND WRAPPER IS CORRESPOND, 1:1 RELATIONSHIP.
                for (var i = 0; i < this.size(); i++) {
                    var wrapper = this.at(i);
                    if (this.result.has(wrapper.key()) == false) {
                        var wrapperGroup_1 = new packer.WrapperGroup(wrapper);
                        this.result.set(wrapper.key(), wrapperGroup_1);
                    }
                    var wrapperGroup = this.result.get(wrapper.key());
                    var instance = this.instanceArray.at(i);
                    if (wrapperGroup.allocate(instance) == false) {
                        // THE INSTANCE IS GREATER THAN THE WRAPPER
                        // THIS GENE IS NOT VALID SO THAT CANNOT PARTICIPATE IN THE OPTIMIZATION PROCESS
                        this.valid = false;
                        return;
                    }
                }
                // THE GENE IS VALID, THEN CALCULATE THE COST
                this.price = 0.0;
                this.valid = true;
                for (var it = this.result.begin(); !it.equals(this.result.end()); it = it.next()) {
                    it.second.optimize();
                    this.price += it.second.getPrice();
                }
            };
            /* -----------------------------------------------------------
                GETTERS
            ----------------------------------------------------------- */
            /**
             * @brief Get optimization result.
             *
             * @return result map.
             */
            GAWrapperArray.prototype.getResult = function () {
                this.constructResult();
                return this.result;
            };
            GAWrapperArray.prototype.less = function (obj) {
                this.constructResult();
                obj.constructResult();
                if (this.valid == true && obj.valid == true)
                    return this.price < obj.price;
                else if (this.valid == true && obj.valid == false)
                    return true;
                else
                    return false;
            };
            return GAWrapperArray;
        }(packer.WrapperArray));
        packer.GAWrapperArray = GAWrapperArray;
    })(packer = bws.packer || (bws.packer = {}));
})(bws || (bws = {}));
var bws;
(function (bws) {
    var packer;
    (function (packer) {
        /**
         * An array of Instance objects.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var InstanceArray = (function (_super) {
            __extends(InstanceArray, _super);
            /* -----------------------------------------------------------
                CONSTRUCTORS
            ----------------------------------------------------------- */
            /**
             * Default Constructor.
             */
            function InstanceArray() {
                _super.call(this);
            }
            /**
             * @inheritdoc
             */
            InstanceArray.prototype.createChild = function (xml) {
                var type = xml.getProperty("type");
                if (type == "product")
                    return new packer.Product();
                else if (type == "wrapper")
                    return new packer.Wrapper();
                else
                    return null;
            };
            /* -----------------------------------------------------------
                EXPORTERS
            ----------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            InstanceArray.prototype.TAG = function () {
                return "instanceArray";
            };
            /**
             * @inheritdoc
             */
            InstanceArray.prototype.CHILD_TAG = function () {
                return "instance";
            };
            return InstanceArray;
        }(samchon.protocol.EntityArray));
        packer.InstanceArray = InstanceArray;
    })(packer = bws.packer || (bws.packer = {}));
})(bws || (bws = {}));
var bws;
(function (bws) {
    var packer;
    (function (packer_1) {
        var wrapperArray;
        function main(str) {
            ////////////////////////
            // READ DATA
            ////////////////////////
            var packerForm = new packer_1.PackerForm();
            var xml = new samchon.library.XML(str);
            if (xml.getTag() == "invoke") {
                var invoke_1 = new samchon.protocol.Invoke(xml);
                packerForm.construct(invoke_1.at(0).getValue());
            }
            else
                packerForm.construct(xml);
            ////////////////////////
            // CONSTRUCT PACKER AND RESULT
            ////////////////////////
            var packer = packerForm.toPacker();
            wrapperArray = packer.optimize();
            var invoke = new samchon.protocol.Invoke("setWrapperArray", wrapperArray.toXML());
            ////////////////////////
            // NOTIFY TO FLEX
            ////////////////////////
            var flex = document.getElementById("flex");
            flex["sendData"](invoke.toXML().toString());
        }
        packer_1.main = main;
        function setWrapperArray(str) {
            wrapperArray = new packer_1.WrapperArray();
            wrapperArray.construct(new samchon.library.XML(str));
        }
        packer_1.setWrapperArray = setWrapperArray;
        function drawWrapper(wrapperIndex, size) {
            var wrapper = wrapperArray.at(wrapperIndex);
            // CLEAR PREVIOUS CANVAS
            var rightTD = document.getElementById("rightTD");
            var div = document.createElement('div');
            rightTD.removeChild(document.body.getElementsByTagName("div")[0]);
            rightTD.appendChild(div);
            // CREATE AND PRINT THE NEW CANVAS
            div.appendChild(wrapper.toCanvas(size));
        }
        packer_1.drawWrapper = drawWrapper;
    })(packer = bws.packer || (bws.packer = {}));
})(bws || (bws = {}));
var bws;
(function (bws) {
    var packer;
    (function (packer) {
        /**
         * A packer.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var Packer = (function (_super) {
            __extends(Packer, _super);
            function Packer(wrapperArray, instanceArray) {
                if (wrapperArray === void 0) { wrapperArray = null; }
                if (instanceArray === void 0) { instanceArray = null; }
                _super.call(this);
                if (wrapperArray == null && instanceArray == null) {
                    this.wrapperArray = new packer.WrapperArray();
                    this.instanceArray = new packer.InstanceArray();
                }
                else {
                    this.wrapperArray = wrapperArray;
                    this.instanceArray = instanceArray;
                }
            }
            /**
             * @inheritdoc
             */
            Packer.prototype.construct = function (xml) {
                this.wrapperArray.construct(xml.get(this.wrapperArray.TAG()).at(0));
                this.instanceArray.construct(xml.get(this.instanceArray.TAG()).at(0));
            };
            /* -----------------------------------------------------------
                GETTERS
            ----------------------------------------------------------- */
            /**
             * Get wrapperArray.
             */
            Packer.prototype.getWrapperArray = function () {
                return this.wrapperArray;
            };
            /**
             * Get instanceArray.
             */
            Packer.prototype.getInstanceArray = function () {
                return this.instanceArray;
            };
            /* -----------------------------------------------------------
                OPTIMIZERS
            ----------------------------------------------------------- */
            Packer.prototype.optimize = function () {
                var wrappers = new packer.WrapperArray(); // TO BE RETURNED
                if (this.wrapperArray.size() == 1) {
                    // ONLY A TYPE OF WRAPPER EXISTS,
                    // OPTMIZE IN LEVEL OF WRAPPER_GROUP AND TERMINATE THE OPTIMIZATION
                    var wrapperGroup = new packer.WrapperGroup(this.wrapperArray.front());
                    for (var i = 0; i < this.instanceArray.size(); i++)
                        if (wrapperGroup.allocate(this.instanceArray.at(i)) == false)
                            return null;
                    // OPTIMIZE
                    wrapperGroup.optimize();
                    // ASSIGN WRAPPERS
                    wrappers.assign(wrapperGroup.begin(), wrapperGroup.end());
                }
                else {
                    ////////////////////////////////////////
                    // WITH GENETIC_ALGORITHM
                    ////////////////////////////////////////
                    // CONSTRUCT INITIAL SET
                    var geneArray = this.initGenes();
                    // EVOLVE
                    // IN JAVA_SCRIPT VERSION, GENETIC_ALGORITHM IS NOT IMPLEMENTED YET.
                    // HOWEVER, IN C++ VERSION, IT IS FULLY SUPPORTED
                    //	http://samchon.github.io/framework/api/cpp/d5/d28/classsamchon_1_1library_1_1GeneticAlgorithm.html
                    // IT WILL BE SUPPORTED SOON
                    // FETCH RESULT
                    var result = geneArray.getResult();
                    for (var it = result.begin(); !it.equals(result.end()); it = it.next())
                        wrappers.insert(wrappers.end(), it.second.begin(), it.second.end());
                    // TRY TO REPACK
                    wrappers = this.repack(wrappers);
                }
                // SORT THE WRAPPERS BY ITEMS' POSITION
                for (var i = 0; i < wrappers.size(); i++)
                    std.sort(wrappers[i].begin(), wrappers[i].end(), function (left, right) {
                        if (left.getZ() != right.getZ())
                            return left.getZ() < right.getZ();
                        else if (left.getY() != right.getY())
                            return left.getY() < right.getY();
                        else
                            return left.getX() < right.getX();
                    });
                return wrappers;
            };
            /**
             * Initialize sequence list (gene_array).
             *
             * @return Initial sequence list.
             */
            Packer.prototype.initGenes = function () {
                ////////////////////////////////////////////////////
                // LINEAR OPTIMIZATION
                ////////////////////////////////////////////////////
                // CONSTRUCT WRAPPER_GROUPS
                var wrapperGroups = new std.Vector();
                for (var i = 0; i < this.wrapperArray.size(); i++) {
                    var wrapper = this.wrapperArray.at(i);
                    wrapperGroups.pushBack(new packer.WrapperGroup(wrapper));
                }
                // ALLOCATE INSTNACES BY AUTHORITY
                for (var i = 0; i < this.instanceArray.size(); i++) {
                    var instance = this.instanceArray.at(i);
                    var minCost = Number.MAX_VALUE;
                    var minIndex = 0;
                    for (var j = 0; j < this.wrapperArray.size(); j++) {
                        var wrapper = this.wrapperArray.at(j);
                        if (wrapper.containable(instance) == false)
                            continue; // CANNOT CONTAIN BY ITS GREATER SIZE
                        var cost = wrapper.getPrice() / wrapper.getContainableVolume();
                        if (cost < minCost) {
                            // CURRENT WRAPPER'S PRICE PER UNIT VOLUME IS CHEAPER
                            minCost = cost;
                            minIndex = j;
                        }
                    }
                    // ALLOCATE TO A GROUP WHICH HAS THE MOST CHEAPER PRICE PER UNIT VOLUME
                    var wrapperGroup = wrapperGroups.at(minIndex);
                    wrapperGroup.allocate(instance);
                }
                ////////////////////////////////////////////////////
                // ADDICTIONAL OPTIMIZATION BY POST-PROCESS
                ////////////////////////////////////////////////////0
                // OPTIMIZE WRAPPER_GROUP
                var wrappers = new packer.WrapperArray();
                for (var i = 0; i < wrapperGroups.size(); i++) {
                    var wrapperGroup = wrapperGroups.at(i);
                    wrapperGroup.optimize();
                    wrappers.insert(wrappers.end(), wrapperGroup.begin(), wrapperGroup.end());
                }
                // DO EARLY POST-PROCESS
                wrappers = this.repack(wrappers);
                ////////////////////////////////////////////////////
                // CONSTRUCT GENE_ARRAY
                ////////////////////////////////////////////////////
                // INSTANCES AND GENES
                var ga_instances = new packer.InstanceArray();
                var genes = new packer.WrapperArray();
                for (var i = 0; i < wrappers.size(); i++) {
                    var wrapper = wrappers.at(i);
                    for (var j = 0; j < wrapper.size(); j++) {
                        ga_instances.pushBack(wrapper.at(j).getInstance());
                        genes.pushBack(wrapper);
                    }
                }
                // GENE_ARRAY
                var geneArray = new packer.GAWrapperArray(ga_instances);
                geneArray.assign(genes.begin(), genes.end());
                return geneArray;
            };
            /**
             * Try to repack each wrappers to another type.
             *
             * @param $wrappers Wrappers to repack.
             * @return Re-packed wrappers.
             */
            Packer.prototype.repack = function ($wrappers) {
                var result = new packer.WrapperArray();
                for (var i = 0; i < $wrappers.size(); i++) {
                    var wrapper = $wrappers.at(i);
                    var minGroup = new packer.WrapperGroup(wrapper);
                    minGroup.pushBack(wrapper);
                    for (var j = 0; j < this.wrapperArray.size(); j++) {
                        var myWrapper = this.wrapperArray.at(j);
                        if (wrapper.equals(myWrapper))
                            continue;
                        var valid = true;
                        // CONSTRUCT GROUP OF TARGET
                        var myGroup = new packer.WrapperGroup(myWrapper);
                        for (var k = 0; k < wrapper.size(); k++)
                            if (myGroup.allocate(wrapper.at(k).getInstance()) == false) {
                                // IF THERE'S AN INSTANCE CANNOT CONTAIN BY ITS GREATER SIZE
                                valid = false;
                                break;
                            }
                        // SKIP
                        if (valid == false)
                            continue;
                        // OPTIMIZATION IN LEVEL OF GROUP
                        myGroup.optimize();
                        // CURRENT GROUP IS CHEAPER, THEN REPLACE
                        if (myGroup.getPrice() < minGroup.getPrice())
                            minGroup = myGroup;
                    }
                    result.insert(result.end(), minGroup.begin(), minGroup.end());
                }
                return result;
            };
            /* -----------------------------------------------------------
                EXPORTERS
            ----------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            Packer.prototype.TAG = function () {
                return "packer";
            };
            /**
             * @inheritdoc
             */
            Packer.prototype.toXML = function () {
                var xml = _super.prototype.toXML.call(this);
                xml.push(this.wrapperArray.toXML());
                xml.push(this.instanceArray.toXML());
                return xml;
            };
            return Packer;
        }(samchon.protocol.Entity));
        packer.Packer = Packer;
    })(packer = bws.packer || (bws.packer = {}));
})(bws || (bws = {}));
var bws;
(function (bws) {
    var packer;
    (function (packer_2) {
        var PackerForm = (function (_super) {
            __extends(PackerForm, _super);
            function PackerForm() {
                _super.call(this);
                this.instanceFormArray = new InstanceFormArray();
                this.wrapperArray = new packer_2.WrapperArray();
            }
            PackerForm.prototype.construct = function (xml) {
                this.instanceFormArray.construct(xml.get(this.instanceFormArray.TAG()).at(0));
                this.wrapperArray.construct(xml.get(this.wrapperArray.TAG()).at(0));
            };
            PackerForm.prototype.optimize = function () {
                var packer = this.toPacker();
                return packer.optimize();
            };
            PackerForm.prototype.TAG = function () {
                return "packerForm";
            };
            PackerForm.prototype.toXML = function () {
                var xml = _super.prototype.toXML.call(this);
                xml.push(this.instanceFormArray.toXML());
                xml.push(this.wrapperArray.toXML());
                return xml;
            };
            PackerForm.prototype.toPacker = function () {
                var packer = new packer_2.Packer(this.wrapperArray, this.instanceFormArray.toInstanceArray());
                return packer;
            };
            return PackerForm;
        }(samchon.protocol.Entity));
        packer_2.PackerForm = PackerForm;
        var InstanceFormArray = (function (_super) {
            __extends(InstanceFormArray, _super);
            function InstanceFormArray() {
                _super.call(this);
            }
            InstanceFormArray.prototype.createChild = function (xml) {
                return new InstanceForm();
            };
            InstanceFormArray.prototype.TAG = function () {
                return "instanceFormArray";
            };
            InstanceFormArray.prototype.CHILD_TAG = function () {
                return "instanceForm";
            };
            InstanceFormArray.prototype.toInstanceArray = function () {
                var instanceArray = new packer_2.InstanceArray();
                for (var i = 0; i < this.size(); i++) {
                    var myInstances = this.at(i).toInstanceArray();
                    //for (let j: number = 0; j < myInstances.size(); j++)
                    //	instanceArray.pushBack(myInstances.at(j));
                    instanceArray.insert(instanceArray.end(), myInstances.begin(), myInstances.end());
                }
                return instanceArray;
            };
            return InstanceFormArray;
        }(samchon.protocol.EntityArray));
        var InstanceForm = (function (_super) {
            __extends(InstanceForm, _super);
            /* -----------------------------------------------------------
                CONSTRUCTORS
            ----------------------------------------------------------- */
            function InstanceForm() {
                _super.call(this);
                this.instance = null;
                this.count = 1;
            }
            InstanceForm.prototype.construct = function (xml) {
                _super.prototype.construct.call(this, xml);
                if (xml.hasProperty("type")) {
                    this.instance = this.createInstance(xml);
                    this.instance.construct(xml);
                }
                else if (xml.has("instance")) {
                    var instanceXML = xml.get("instance").at(0);
                    this.instance = this.createInstance(instanceXML);
                    this.instance.construct(instanceXML);
                }
            };
            InstanceForm.prototype.createInstance = function (xml) {
                if (xml.getProperty("type") == "product")
                    return new packer_2.Product();
                else
                    return new packer_2.Wrapper();
            };
            /* -----------------------------------------------------------
                EXPORTERS
            ----------------------------------------------------------- */
            InstanceForm.prototype.TAG = function () {
                return "instanceForm";
            };
            InstanceForm.prototype.toXML = function () {
                var xml = _super.prototype.toXML.call(this);
                if (this.instance != null)
                    xml.push(this.instance.toXML());
                return xml;
            };
            InstanceForm.prototype.toInstanceArray = function () {
                var instanceArray = new packer_2.InstanceArray();
                instanceArray.assign(this.count, this.instance);
                return instanceArray;
            };
            return InstanceForm;
        }(samchon.protocol.Entity));
    })(packer = bws.packer || (bws.packer = {}));
})(bws || (bws = {}));
var bws;
(function (bws) {
    var packer;
    (function (packer) {
        /**
         * A product.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var Product = (function (_super) {
            __extends(Product, _super);
            function Product(name, width, height, length) {
                if (name === void 0) { name = ""; }
                if (width === void 0) { width = 0; }
                if (height === void 0) { height = 0; }
                if (length === void 0) { length = 0; }
                _super.call(this);
                /**
                 * <p> Name, key of the Product. </p>
                 *
                 * <p> The name must be unique because a name identifies a {@link Product}. </p>
                 */
                this.name = "";
                /**
                 * Width of the Product, length on the X-axis in 3D.
                 */
                this.width = 0.0;
                /**
                 * Height of the Product, length on the Y-axis in 3D.
                 */
                this.height = 0.0;
                /**
                 * Length of the Product, length on the Z-axis in 3D.
                 */
                this.length = 0.0;
                this.name = name;
                this.width = width;
                this.height = height;
                this.length = length;
            }
            /* -----------------------------------------------------------
                GETTERS
            ----------------------------------------------------------- */
            /**
             * Key of a Product is its name.
             */
            Product.prototype.key = function () {
                return this.name;
            };
            /**
             * @inheritdoc
             */
            Product.prototype.getName = function () {
                return this.name;
            };
            /**
             * @inheritdoc
             */
            Product.prototype.getWidth = function () {
                return this.width;
            };
            /**
             * @inheritdoc
             */
            Product.prototype.getHeight = function () {
                return this.height;
            };
            /**
             * @inheritdoc
             */
            Product.prototype.getLength = function () {
                return this.length;
            };
            /**
             * @inheritdoc
             */
            Product.prototype.getVolume = function () {
                return this.width * this.height * this.length;
            };
            /* -----------------------------------------------------------
                EXPORTERS
            ----------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            Product.prototype.TYPE = function () {
                return "product";
            };
            /**
             * @inheritdoc
             */
            Product.prototype.TAG = function () {
                return "instance";
            };
            Product.prototype.toXML = function () {
                var xml = _super.prototype.toXML.call(this);
                xml.setProperty("type", this.TYPE());
                return xml;
            };
            return Product;
        }(samchon.protocol.Entity));
        packer.Product = Product;
    })(packer = bws.packer || (bws.packer = {}));
})(bws || (bws = {}));
var bws;
(function (bws) {
    var packer;
    (function (packer) {
        /**
         * <p> Wrap represents an act wrap(ping). </p>
         *
         * <p> {@link Wrap} is a class represents an act wrapping an {@link Instance} to an {@link Wrapper}.
         * To represent the relationship, Wrap uses Bridge and Capsular patterns to links and intermediates
         * relationship between Wrapper and Instance. </p>
         *
         * <p> Wrap also helps packing optimization and 3d-visualization with its own members
         * {@link orientation} and position variables {@link x}, {@link y} and {@link z}. </p>
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var Wrap = (function (_super) {
            __extends(Wrap, _super);
            function Wrap(wrapper, instance, x, y, z, orientation) {
                if (instance === void 0) { instance = null; }
                if (x === void 0) { x = 0; }
                if (y === void 0) { y = 0; }
                if (z === void 0) { z = 0; }
                if (orientation === void 0) { orientation = 1; }
                _super.call(this);
                /**
                 *
                 */
                this.color = -1;
                this.wrapper = wrapper;
                this.instance = instance;
                this.x = x;
                this.y = y;
                this.z = z;
                this.orientation = orientation;
            }
            /**
             * @inheritdoc
             */
            Wrap.prototype.construct = function (xml) {
                _super.prototype.construct.call(this, xml);
                var instanceXML = xml.get("instance").at(0);
                this.instance = this.createInstance(instanceXML.getProperty("type"));
                this.instance.construct(instanceXML);
            };
            /**
             * Factory method of wrapped Instance.
             *
             * @param type Type of contained Instance's type.
             */
            Wrap.prototype.createInstance = function (type) {
                if (type == "product")
                    return new packer.Product();
                else if (type == "wrapper")
                    return new packer.Wrapper();
                else
                    return null;
            };
            /* ===========================================================
                SETTERS
                    - MEMBERS
                    - ESTIMATERS
            ==============================================================
                MEMBERS
            ----------------------------------------------------------- */
            /**
             * Set orientation.
             *
             * @param orientation Orientation code (1 to 6).
             */
            Wrap.prototype.setOrientation = function (orientation) {
                this.orientation = orientation;
            };
            /**
             * Set position.
             *
             * @param x Coordinate-X of the instance placement in the wrapper.
             * @param y Coordinate-Y of the instance placement in the wrapper.
             * @param z Coordinate-Z of the instance placement in the wrapper.
             */
            Wrap.prototype.setPosition = function (x, y, z) {
                this.x = x;
                this.y = y;
                this.z = z;
            };
            /* -----------------------------------------------------------
                ESTIMATERS
            ----------------------------------------------------------- */
            /**
             * @brief Estimate orientation by given size.
             *
             * @param width Width by placement.
             * @param height Height by placement.
             * @param length Length by placement.
             */
            Wrap.prototype.estimateOrientation = function (width, height, length) {
                if (this.instance.getWidth() == width && this.instance.getHeight() == height)
                    this.orientation = 1;
                else if (this.instance.getWidth() == length && this.instance.getHeight() == height)
                    this.orientation = 2;
                else if (this.instance.getWidth() == length && this.instance.getHeight() == width)
                    this.orientation = 3;
                else if (this.instance.getWidth() == height && this.instance.getHeight() == width)
                    this.orientation = 4;
                else if (this.instance.getWidth() == width && this.instance.getHeight() == length)
                    this.orientation = 5;
                else
                    this.orientation = 6;
            };
            /**
             * @brief Orientation change is occured in level of the packer.
             *
             * @details orientation Packer's new orientation.
             */
            Wrap.prototype.changeWrapperOrientation = function (orietation) {
                if (orientation == 1)
                    return;
                // DECLARES
                var x;
                var y;
                var z;
                var width;
                var height;
                var length;
                if (orientation == 2) {
                    width = this.instance.getLength();
                    height = this.instance.getHeight();
                    length = this.instance.getWidth();
                    x = this.z;
                    y = this.y;
                    z = this.wrapper.getWidth() - (length + this.x);
                }
                else if (orientation == 3) {
                    width = this.instance.getLength();
                    height = this.instance.getWidth();
                    length = this.instance.getHeight();
                    x = this.z;
                    y = this.wrapper.getWidth() - (height + this.x);
                    z = this.wrapper.getHeight() - (length + this.y);
                }
                else if (orientation == 4) {
                    width = this.instance.getHeight();
                    height = this.instance.getWidth();
                    length = this.instance.getLength();
                    x = this.y;
                    y = this.x;
                    z = this.z;
                }
                else if (orientation == 5) {
                    width = this.instance.getWidth();
                    height = this.instance.getLength();
                    length = this.instance.getHeight();
                    x = this.x;
                    y = this.wrapper.getLength() - (height + this.z);
                    z = this.y;
                }
                else {
                    width = this.instance.getHeight();
                    height = this.instance.getLength();
                    length = this.instance.getWidth();
                    x = this.y;
                    y = this.z;
                    z = this.wrapper.getWidth() - (length - this.x);
                }
                this.estimateOrientation(width, height, length);
                this.x = x;
                this.y = y;
                this.z = z;
            };
            /* ===========================================================
                GETTERS
            =========================================================== */
            /**
             * Get wrapper.
             */
            Wrap.prototype.getWrapper = function () {
                return this.wrapper;
            };
            /**
             * Get instance.
             */
            Wrap.prototype.getInstance = function () {
                return this.instance;
            };
            /**
             * Get x.
             */
            Wrap.prototype.getX = function () {
                return this.x;
            };
            /**
             * Get y.
             */
            Wrap.prototype.getY = function () {
                return this.y;
            };
            /**
             * Get z.
             */
            Wrap.prototype.getZ = function () {
                return this.z;
            };
            /**
             * Get orientation.
             */
            Wrap.prototype.getOrientation = function () {
                return this.orientation;
            };
            /**
             * Get width.
             */
            Wrap.prototype.getWidth = function () {
                switch (this.orientation) {
                    case 1:
                    case 5:
                        return this.instance.getWidth();
                    case 3:
                    case 4:
                        return this.instance.getHeight();
                    default:
                        return this.instance.getLength();
                }
            };
            /**
             * Get height.
             */
            Wrap.prototype.getHeight = function () {
                switch (this.orientation) {
                    case 1:
                    case 2:
                        return this.instance.getHeight();
                    case 4:
                    case 6:
                        return this.instance.getWidth();
                    default:
                        return this.instance.getLength();
                }
            };
            /**
             * Get length.
             */
            Wrap.prototype.getLength = function () {
                switch (this.orientation) {
                    case 1:
                    case 4:
                        return this.instance.getLength();
                    case 2:
                    case 3:
                        return this.instance.getWidth();
                    default:
                        return this.instance.getHeight();
                }
            };
            /**
             * Get volume.
             */
            Wrap.prototype.getVolume = function () {
                return this.instance.getVolume();
            };
            /* -----------------------------------------------------------
                EXPORTERS
            ----------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            Wrap.prototype.TAG = function () {
                return "wrap";
            };
            /**
             * @inheritdoc
             */
            Wrap.prototype.toXML = function () {
                var xml = _super.prototype.toXML.call(this);
                xml.push(this.instance.toXML());
                xml.eraseProperty("color");
                return xml;
            };
            Object.defineProperty(Wrap, "BOUNDARY_THICKNESS", {
                /* -----------------------------------------------------------
                    VISUALIZERS
                ----------------------------------------------------------- */
                /**
                 * Thickness of boundary lines of a shape represents the {@link instance}.
                 */
                get: function () {
                    return 0.1;
                },
                enumerable: true,
                configurable: true
            });
            /**
             *
             *
             * @param geometry
             *
             * @return A shape and its boundary lines as 3D-objects.
             */
            Wrap.prototype.toDisplayObjects = function (geometry) {
                var objects = new std.Vector();
                // ---------------------------------------
                // BOUNDARIES
                // ---------------------------------------
                for (var i = 1; i <= 12; i++) {
                    var boundaryLine = new THREE.Mesh(geometry, new THREE.MeshPhongMaterial({
                        color: 0xFF0000, shading: THREE.FlatShading,
                        vertexColors: THREE.VertexColors, shininess: 0
                    }));
                    var width = void 0, height = void 0, length_1 = void 0;
                    var x = void 0, y = void 0, z = void 0;
                    // SCALE
                    switch (i) {
                        case 1:
                        case 3:
                        case 9:
                        case 12:
                            width = this.getWidth();
                            height = Wrap.BOUNDARY_THICKNESS;
                            length_1 = Wrap.BOUNDARY_THICKNESS;
                            break;
                        case 2:
                        case 4:
                        case 10:
                        case 11:
                        case 10:
                            height = this.getHeight();
                            width = Wrap.BOUNDARY_THICKNESS;
                            length_1 = Wrap.BOUNDARY_THICKNESS;
                            break;
                        default:
                            length_1 = this.getLength();
                            width = Wrap.BOUNDARY_THICKNESS;
                            height = Wrap.BOUNDARY_THICKNESS;
                            break;
                    }
                    // X
                    switch (i) {
                        case 4:
                        case 6:
                        case 8:
                        case 11:
                            x = this.x + this.getWidth() - Wrap.BOUNDARY_THICKNESS;
                            break;
                        default:
                            x = this.x;
                            break;
                    }
                    // Y
                    switch (i) {
                        case 3:
                        case 7:
                        case 8:
                        case 12:
                            y = this.y + this.getHeight() - Wrap.BOUNDARY_THICKNESS;
                            break;
                        default:
                            y = this.y;
                            break;
                    }
                    // Z
                    switch (i) {
                        case 9:
                        case 10:
                        case 11:
                        case 12:
                            z = this.z + this.getLength() - Wrap.BOUNDARY_THICKNESS;
                            break;
                        default:
                            z = this.z;
                            break;
                    }
                    // SET POSITION AND SCALE
                    boundaryLine.scale.set(width, height, length_1);
                    boundaryLine.position.set(x + width / 2, y + height / 2, z + length_1 / 2);
                    objects.pushBack(boundaryLine);
                }
                // ---------------------------------------
                // SHAPE
                // ---------------------------------------
                if (this.color == -1)
                    this.color = Math.random() * 0xFFFFFF;
                var shape = new THREE.Mesh(geometry, new THREE.MeshLambertMaterial({
                    color: this.color,
                    opacity: 0.5,
                    transparent: true
                }));
                shape.scale.set(this.getWidth(), this.getHeight(), this.getLength());
                shape.position.set(this.x + this.getWidth() / 2, this.y + this.getHeight() / 2, this.z + this.getLength() / 2);
                objects.pushBack(shape);
                return objects;
            };
            return Wrap;
        }(samchon.protocol.Entity));
        packer.Wrap = Wrap;
    })(packer = bws.packer || (bws.packer = {}));
})(bws || (bws = {}));
var bws;
(function (bws) {
    var packer;
    (function (packer) {
        /**
         * A wrapper wrapping instances.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var Wrapper = (function (_super) {
            __extends(Wrapper, _super);
            function Wrapper() {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i - 0] = arguments[_i];
                }
                _super.call(this);
                /**
                 * <p> Name, key of the Wrapper. </p>
                 *
                 * <p> The name represents a type of Wrapper and identifies the Wrapper. </p>
                 */
                this.name = "";
                /**
                 * Price, cost of using an Wrapper.
                 */
                this.price = 0.0;
                /**
                 * Width of the Wrapper, length on the X-axis in 3D.
                 */
                this.width_ = 0.0;
                /**
                 * Height of the Wrapper, length on the Y-axis in 3D.
                 */
                this.height_ = 0.0;
                /**
                 * Length of the Wrapper, length on the Z-axis in 3D.
                 */
                this.length_ = 0.0;
                /**
                 * <p> Thickness, margin of a Wrapper causes shrinkness of containable volume. </p>
                 *
                 * <p> The thickness reduces each dimension's containable size (dimension - 2*thickness),
                 * so finally, it reduces total containable volume (-8 * thickness^3). </p>
                 */
                this.thickness = 0.0;
                if (args.length == 1 && args[0] instanceof Wrapper) {
                    var wrapper = args[0];
                    this.name = wrapper.name;
                    this.price = wrapper.price;
                    this.width_ = wrapper.width_;
                    this.height_ = wrapper.height_;
                    this.length_ = wrapper.length_;
                    this.thickness = wrapper.thickness;
                }
                else if (args.length == 6) {
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
            Wrapper.prototype.construct = function (xml) {
                _super.prototype.construct.call(this, xml);
                this.width_ = parseFloat(xml.getProperty("width"));
                this.height_ = parseFloat(xml.getProperty("height"));
                this.length_ = parseFloat(xml.getProperty("length"));
            };
            /**
             * @inheritdoc
             */
            Wrapper.prototype.createChild = function (xml) {
                return new packer.Wrap(this);
            };
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
            Wrapper.prototype.key = function () {
                return this.name;
            };
            /**
             * Get name.
             */
            Wrapper.prototype.getName = function () {
                return this.name;
            };
            /**
             * Get price.
             */
            Wrapper.prototype.getPrice = function () {
                return this.price;
            };
            /**
             * Get width, length on X-axis in 3D.
             */
            Wrapper.prototype.getWidth = function () {
                return this.width_;
            };
            /**
             * Get height, length on Y-axis in 3D.
             */
            Wrapper.prototype.getHeight = function () {
                return this.height_;
            };
            /**
             * Get length, length on Z-axis in 3D.
             */
            Wrapper.prototype.getLength = function () {
                return this.length_;
            };
            /**
             * Get thickness.
             */
            Wrapper.prototype.getThickness = function () {
                return this.thickness;
            };
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
            Wrapper.prototype.getContainableWidth = function () {
                return this.width_ - (2 * this.thickness);
            };
            /**
             * <p> Get (calculate) containable height, length on the Y-axis in 3D. </p>
             *
             * <p> Calculates containable height considering the {@link thickness}. </p>
             *
             * @return height - (2 x thickness)
             */
            Wrapper.prototype.getContainableHeight = function () {
                return this.height_ - (2 * this.thickness);
            };
            /**
             * <p> Get (calculate) containable length, length on the Z-axis in 3D. </p>
             *
             * <p> Calculates containable length considering the {@link thickness}. </p>
             *
             * @return length - (2 x thickness)
             */
            Wrapper.prototype.getContainableLength = function () {
                return this.length_ - (2 * this.thickness);
            };
            /**
             * <p> Get (calculate) volume. </p>
             *
             * <h4> Notice </h4>
             * <p> If {@link thickness} of the Wrapper is not 0, the volume does not mean containable volume.
             * In that case, use {@link containableVolume} instead. </p>
             *
             * @return width x height x length
             */
            Wrapper.prototype.getVolume = function () {
                return this.width_ * this.height_ * this.length_;
            };
            /**
             * <p> Get (calculate) containable volume. </p>
             *
             * <p> Calculates containable volume considering the {@link thickness}. </p>
             *
             * @return volume - {(2 x thickness) ^ 3}
             */
            Wrapper.prototype.getContainableVolume = function () {
                return this.getContainableWidth() * this.getContainableHeight() * this.getContainableLength();
            };
            /**
             * Get utilization ratio of containable volume.
             *
             * @return utilization ratio.
             */
            Wrapper.prototype.getUtilization = function () {
                var volume = 0.0;
                for (var i = 0; i < this.size(); i++)
                    volume += this.at(i).getVolume();
                return volume / this.getContainableVolume();
            };
            /* -----------------------------------------------------------
                COMPARISON
            ----------------------------------------------------------- */
            Wrapper.prototype.equals = function (obj) {
                return this.price == obj.price
                    && this.width_ == obj.width_ && this.height_ == obj.height_ && this.length_ == obj.length_
                    && this.thickness == obj.thickness;
            };
            /**
             * <p> Wrapper is enough greater? </p>
             *
             * <p> Test whether the Wrapper is enough greater than an Instance to contain. </p>
             *
             * @param instance An Instance to test.
             * @return Enough greater or not.
             */
            Wrapper.prototype.containable = function (instance) {
                // TEST WHETHER AN INSTANCE IS GREATER THAN WRAPPER
                var myDims = new std.Vector([this.getContainableWidth(), this.getContainableHeight(), this.getContainableLength()]);
                var instanceDims = new std.Vector([instance.getWidth(), instance.getHeight(), instance.getLength()]);
                std.sort(myDims.begin(), myDims.end());
                std.sort(instanceDims.begin(), instanceDims.end());
                for (var i = 0; i < myDims.size(); i++)
                    if (myDims.at(i) < instanceDims.at(i))
                        return false;
                return true;
            };
            /* ===========================================================
                EXPORTERS
            =========================================================== */
            /**
             * @inheritdoc
             */
            Wrapper.prototype.TYPE = function () {
                return "wrapper";
            };
            /**
             * @inheritdoc
             */
            Wrapper.prototype.TAG = function () {
                return "instance";
            };
            /**
             * @inheritdoc
             */
            Wrapper.prototype.CHILD_TAG = function () {
                return "wrap";
            };
            /**
             * @inheritdoc
             */
            Wrapper.prototype.toXML = function () {
                var xml = _super.prototype.toXML.call(this);
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
            };
            Object.defineProperty(Wrapper, "BOUNDARY_THICKNESS", {
                get: function () { return 0.5; },
                enumerable: true,
                configurable: true
            });
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
            Wrapper.prototype.toCanvas = function (endIndex) {
                if (endIndex === void 0) { endIndex = Number.MAX_VALUE; }
                // ---------------------------------------
                // CONSTRUCTS
                // ---------------------------------------
                // SCENE AND GEOMETRY
                Wrapper.scene = new THREE.Scene();
                var geometry = new THREE.BoxGeometry(1, 1, 1);
                // BOUNDARY LINES
                for (var i = 1; i <= 12; i++) {
                    var boundaryLine = new THREE.Mesh(geometry, new THREE.MeshPhongMaterial({
                        color: 0xFFFFFF, shading: THREE.FlatShading,
                        vertexColors: THREE.VertexColors, shininess: 0
                    }));
                    var width = void 0, height = void 0, length_2 = void 0;
                    var x = void 0, y = void 0, z = void 0;
                    // SCALE
                    switch (i) {
                        case 1:
                        case 3:
                        case 9:
                        case 12:
                            width = this.width_ + 2 * Wrapper.BOUNDARY_THICKNESS;
                            height = Wrapper.BOUNDARY_THICKNESS;
                            length_2 = Wrapper.BOUNDARY_THICKNESS;
                            break;
                        case 2:
                        case 4:
                        case 10:
                        case 11:
                        case 10:
                            height = this.height_ + 2 * Wrapper.BOUNDARY_THICKNESS;
                            width = Wrapper.BOUNDARY_THICKNESS;
                            length_2 = Wrapper.BOUNDARY_THICKNESS;
                            break;
                        default:
                            length_2 = this.length_ + 2 * Wrapper.BOUNDARY_THICKNESS;
                            width = Wrapper.BOUNDARY_THICKNESS;
                            height = Wrapper.BOUNDARY_THICKNESS;
                            break;
                    }
                    // X
                    switch (i) {
                        case 4:
                        case 6:
                        case 8:
                        case 11:
                            x = this.width_ + Wrapper.BOUNDARY_THICKNESS;
                            break;
                        default:
                            x = -Wrapper.BOUNDARY_THICKNESS;
                            break;
                    }
                    // Y
                    switch (i) {
                        case 3:
                        case 7:
                        case 8:
                        case 12:
                            y = this.height_;
                            break;
                        default:
                            y = -Wrapper.BOUNDARY_THICKNESS;
                            break;
                    }
                    // Z
                    switch (i) {
                        case 9:
                        case 10:
                        case 11:
                        case 12:
                            z = this.length_ + Wrapper.BOUNDARY_THICKNESS;
                            break;
                        default:
                            z = -Wrapper.BOUNDARY_THICKNESS;
                            break;
                    }
                    // SET POSITION AND SCALE
                    boundaryLine.scale.set(width, height, length_2);
                    boundaryLine.position.set(x + width / 2, y + height / 2, z + length_2 / 2);
                    Wrapper.scene.add(boundaryLine);
                }
                // CHILDREN (PACKED) INSTANCES
                for (var i = 0; i < Math.min(endIndex, this.size()); i++) {
                    // 1st to 11th: boundaries, 12th: shape
                    var objects = this.at(i).toDisplayObjects(geometry);
                    for (var j = 0; j < objects.size(); j++)
                        Wrapper.scene.add(objects.at(j));
                }
                // LIGHTS
                var ambientLight = new THREE.AmbientLight(0x555555);
                //let spotLight: THREE.SpotLight = new THREE.SpotLight(0xFFFFFF, 1.5);
                //spotLight.position.set(0, 500, 2000);
                Wrapper.scene.add(ambientLight);
                //Wrapper.scene.add(spotLight);
                // ---------------------------------------
                // CAMERA, TRACKBALL AND MOUSE
                // ---------------------------------------
                if (Wrapper.camera == null) {
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
            };
            /* -----------------------------------------------------------
                EVENTS
            ----------------------------------------------------------- */
            Wrapper.handleMouseMove = function (event) {
                Wrapper.mouse.x = event.clientX;
                Wrapper.mouse.y = event.clientY;
            };
            Wrapper.animate = function () {
                requestAnimationFrame(Wrapper.animate);
                Wrapper.render();
            };
            Wrapper.render = function () {
                Wrapper.trackball.update();
                Wrapper.renderer.render(Wrapper.scene, Wrapper.camera);
            };
            /* ===========================================================
                VISUALIZERS
                    - MEMBERS
                    - FACTORY METHOD
                    - EVENTS
            ==============================================================
                MEMBERS
            ----------------------------------------------------------- */
            Wrapper.scene = null;
            Wrapper.renderer = null;
            Wrapper.camera = null;
            Wrapper.trackball = null;
            Wrapper.mouse = null;
            return Wrapper;
        }(samchon.protocol.EntityArray));
        packer.Wrapper = Wrapper;
    })(packer = bws.packer || (bws.packer = {}));
})(bws || (bws = {}));
var bws;
(function (bws) {
    var packer;
    (function (packer) {
        /**
         * A group of same type of {@link Wrapper Wrappers}.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var WrapperGroup = (function (_super) {
            __extends(WrapperGroup, _super);
            function WrapperGroup() {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i - 0] = arguments[_i];
                }
                _super.call(this);
                if (args.length == 0) {
                    this.sample = new packer.Wrapper();
                }
                else if (args.length == 1 && args[0] instanceof packer.Wrapper) {
                    this.sample = args[0];
                }
                else if (args.length == 6) {
                    this.sample = new packer.Wrapper(args[0], args[1], args[2], args[3], args[4], args[5]);
                }
                this.allocatedInstanceArray = new packer.InstanceArray();
            }
            /* -----------------------------------------------------------
                GETTERS
            ----------------------------------------------------------- */
            /**
             * Key of a WrapperGroup is dependent on its sample.
             */
            WrapperGroup.prototype.key = function () {
                return this.sample.key();
            };
            /**
             * Get sample.
             */
            WrapperGroup.prototype.getSample = function () {
                return this.sample;
            };
            /**
             * Get allocated instances.
             */
            WrapperGroup.prototype.getAllocatedInstanceArray = function () {
                return this.allocatedInstanceArray;
            };
            /**
             * Get (calculate) price.
             *
             * @return (Price of the sample) x (numbers of children Wrappers)
             */
            WrapperGroup.prototype.getPrice = function () {
                return this.sample.getPrice() * this.size();
            };
            /**
             * @inheritdoc
             */
            WrapperGroup.prototype.getUtilization = function () {
                var utilization = 0.0;
                for (var i = 0; i < this.size(); i++)
                    utilization += this.at(i).getUtilization();
                return utilization / this.size();
            };
            /* -----------------------------------------------------------
                OPERATORS
            ----------------------------------------------------------- */
            /**
             * <p> Allocate instance(s) to the WrapperGroup. </p>
             *
             * <p> Inspect the instance is enough small to be wrapped into an empty wrapper. If the instance
             * is enough small, registers the instance (or repeated instances) to the {@link reserveds} and
             * returns <code>true</code>. If the instance is too large to be capsuled, returns <code>false</code>. </p>
             *
             * <h4>Note</h4>
             * <p> The word <u>the instance is enough small to be wrapped into the empty wrapper</u> means
             * the instance can be contained into an empty, a new wrapper contaning nothing literally. </p>
             *
             * <p> In the method allocate(), it doesn't consider how many instances are wrapped into ordinary
             * wrapper and how much volumes are consumed.  </p>
             *
             * @param instance An Instance to allocate.
             * @param n Repeating number of the <i>instance</i>.
             *
             * @return Whether the instance is enough small to be wrapped into a (new) wrapper
             *		   of same type with the sample.
             */
            WrapperGroup.prototype.allocate = function (instance, n) {
                if (n === void 0) { n = 1; }
                // TEST WHETHER A PRODUCT IS NOT LARGER THAN BOX
                if (this.sample.containable(instance) == false)
                    return false;
                // INSERTS TO THE RESERVED ITEMS
                this.allocatedInstanceArray.insert(this.allocatedInstanceArray.end(), n, instance);
                return true;
            };
            /**
             * <p> Run optimization in level of the group. </p>
             *
             * <p> The optimization routine begins by creating a {@link Wrapper} like the {@link sample}. Then
             * try to pack {@link allocatedInstanceArray allocated instances} to the {@link Wrapper} as a lot as
             * possible. If there're some {@link Wrappers} can't be packed by overloading, then create a new
             * {@link Wrapper} again and try to pack {@link allocatedInstanceArray instances} again, too. </p>
             *
             * <p> Repeats those steps until all {@link alloctedInstanceArray instances} are {@link Wrap packed}
             * so that there's not any {@link Instance instance} left. </p>
             *
             * <h4> Warning </h4>
             * <p> When call this {@link optimize optimize()} method, ordinary children {@link Wrapper} objects
             * in the {@link WrapperGroup} will be substituted with the newly optimized {@link Wrapper} objects. </p>
             */
            WrapperGroup.prototype.optimize = function () {
                this.clear();
                var instanceArray = new packer.InstanceArray();
                instanceArray.assign(this.allocatedInstanceArray.begin(), this.allocatedInstanceArray.end());
                while (instanceArray.empty() == false)
                    instanceArray = this.pack(instanceArray);
            };
            /**
             * <p> Wrap allocated instances into <b>a new</b> {@link Wrapper}. </p>
             *
             * <p> {@link Wrap Wraps} instances to a new Wrapper which is copied from the sample. </p>
             * <p> After the wrapping is done, the new {@link Wrapper} is registered to the {@link WrapperGroup}
             * as a child and instances failed to wrap by overloading is returned. </p>
             *
             * @param instanceArray instances to {@link Wrap wrap} into <b>a new</b> {@link Wrapper}.
             *
             * @return Instances failed to {@link Wrap wrap} by overloading.
             * @see boxologic
             */
            WrapperGroup.prototype.pack = function (instanceArray) {
                var boxo = new boxologic.Boxologic(new packer.Wrapper(this.sample), instanceArray);
                var resultPair = boxo.pack();
                this.pushBack(resultPair.first);
                return resultPair.second;
            };
            /* -----------------------------------------------------------
                EXPORTERS
            ----------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            WrapperGroup.prototype.TAG = function () {
                return "wrapperGroup";
            };
            return WrapperGroup;
        }(packer.WrapperArray));
        packer.WrapperGroup = WrapperGroup;
    })(packer = bws.packer || (bws.packer = {}));
})(bws || (bws = {}));
//# sourceMappingURL=packer.js.map