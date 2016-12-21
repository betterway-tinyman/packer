var __extends = (this && this.__extends) || function (d, b) {
    for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
    function __() { this.constructor = d; }
    d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
};
/**
 * # Samchon-Framework
 *
 * <a href="https://nodei.co/npm/samchon-framework">
 *	<img src="https://nodei.co/npm/samchon-framework.png?downloads=true&downloadRank=true&stars=true"> </a>
 *
 * Samchon, a OON (Object-Oriented Network) framework.
 *
 * With Samchon Framework, you can implement distributed processing system within framework of OOD like handling S/W
 * objects (classes). You can realize cloud and distributed system very easily with provided system templates and even
 * integration with C++ is possible.
 *
 * The goal, ultimate utilization model of Samchon Framework is, building cloud system with NodeJS and taking heavy works
 * to C++ distributed systems with provided modules (those are system templates).
 *
 * @git https://github.com/samchon/framework
 * @author Jeongho Nam <http://samchon.org>
 */
var samchon;
(function (samchon) {
    /**
     * Running on Node.
     *
     * Test whether the JavaScript is running on Node.
     *
     * @references http://stackoverflow.com/questions/17575790/environment-detection-node-js-or-browser
     */
    function is_node() {
        if (typeof process === "object")
            if (typeof process.versions === "object")
                if (typeof process.versions.node !== "undefined")
                    return true;
        return false;
    }
    samchon.is_node = is_node;
})(samchon || (samchon = {}));
try {
    // IF THE CONDITION BE IS_NODE(), THEN CANNOT BE USED IN BROWSERIFY
    eval("var std = require('tstl')");
    eval("var http = require('http')");
    eval("var websocket = require('websocket')");
    eval("var net = require('net')");
}
catch (exception) { }
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * A {@link Vector} who can detect element I/O events.
         *
         * Below is the list of methods who are dispatching {@link CollectionEvent}:
         * - *insert* typed events:
         *   - {@link assign}
         *   - {@link insert}
         *   - {@link push}
         *   - {@link push_back}
         * - *erase* typed events:
         *   - {@link assign}
         *   - {@link clear}
         *   - {@link erase}
         *   - {@link pop_back}
         * - *refresh* typed events:
         *   - {@link refresh}
         *
         * @copydoc std.Vector
         *
         * @reference http://www.cplusplus.com/reference/vector/vector
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var ArrayCollection = (function (_super) {
            __extends(ArrayCollection, _super);
            function ArrayCollection() {
                var _this = _super.apply(this, arguments) || this;
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                _this.event_dispatcher_ = new samchon.library.EventDispatcher(_this);
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor		
            /* =========================================================
                ELEMENTS I/O
                    - INSERT
                    - ERASE
                    - NOTIFIER
            ============================================================
                INSERT
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            ArrayCollection.prototype.push = function () {
                var items = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    items[_i] = arguments[_i];
                }
                var ret = _super.prototype.push.apply(this, items);
                this._Notify_insert(this.end().advance(-items.length), this.end());
                return ret;
            };
            /**
             * @inheritdoc
             */
            ArrayCollection.prototype.push_back = function (val) {
                _super.prototype.push_back.call(this, val);
                this._Notify_insert(this.end().prev(), this.end());
            };
            /**
             * @hidden
             */
            ArrayCollection.prototype._Insert_by_repeating_val = function (position, n, val) {
                var ret = _super.prototype._Insert_by_repeating_val.call(this, position, n, val);
                this._Notify_insert(ret, ret.advance(n));
                return ret;
            };
            /**
             * @hidden
             */
            ArrayCollection.prototype._Insert_by_range = function (position, begin, end) {
                var n = this.size();
                var ret = _super.prototype._Insert_by_range.call(this, position, begin, end);
                n = this.size() - n;
                this._Notify_insert(ret, ret.advance(n));
                return ret;
            };
            /* ---------------------------------------------------------
                ERASE
            --------------------------------------------------------- */
            /**
             * @hidden
             */
            ArrayCollection.prototype._Erase_by_range = function (first, last) {
                this._Notify_erase(first, last);
                return _super.prototype._Erase_by_range.call(this, first, last);
            };
            /* ---------------------------------------------------------
                NOTIFIER
            --------------------------------------------------------- */
            /**
             * @hidden
             */
            ArrayCollection.prototype._Notify_insert = function (first, last) {
                collections.ICollection._Dispatch_CollectionEvent(this, "insert", first, last);
            };
            /**
             * @hidden
             */
            ArrayCollection.prototype._Notify_erase = function (first, last) {
                collections.ICollection._Dispatch_CollectionEvent(this, "erase", first, last);
            };
            /* =========================================================
                EVENT_DISPATCHER
                    - ACCESSORS
                    - ADD
                    - REMOVE
            ============================================================
                ACCESSORS
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            ArrayCollection.prototype.hasEventListener = function (type) {
                return this.event_dispatcher_.hasEventListener(type);
            };
            /**
             * @inheritdoc
             */
            ArrayCollection.prototype.dispatchEvent = function (event) {
                return this.event_dispatcher_.dispatchEvent(event);
            };
            ArrayCollection.prototype.refresh = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var first;
                var last;
                if (args.length == 0) {
                    first = this.begin();
                    last = this.end();
                }
                else if (args.length == 1) {
                    first = args[0];
                    last = first.next();
                }
                else {
                    first = args[0];
                    last = args[1];
                }
                this.dispatchEvent(new collections.CollectionEvent("refresh", first, last));
            };
            ArrayCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            ArrayCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return ArrayCollection;
        }(std.Vector));
        collections.ArrayCollection = ArrayCollection;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var library;
    (function (library) {
        /**
         * A basic event class of Samchon Framework.
         *
         * @reference https://developer.mozilla.org/en-US/docs/Web/API/Event
         * @handbook https://github.com/samchon/framework/wiki/TypeScript-Library-EventDispatcher
         * @author Jeongho Nam <http://samchon.org>
         */
        var BasicEvent = (function () {
            /* -------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------- */
            function BasicEvent(type, bubbles, cancelable) {
                if (bubbles === void 0) { bubbles = false; }
                if (cancelable === void 0) { cancelable = false; }
                this.type_ = type.toLowerCase();
                this.target_ = null;
                this.currentTarget_ = null;
                this.trusted_ = false;
                this.bubbles_ = bubbles;
                this.cancelable_ = cancelable;
                this.defaultPrevented_ = false;
                this.cancelBubble_ = false;
                this.timeStamp_ = new Date();
            }
            /**
             * @inheritdoc
             */
            BasicEvent.prototype.initEvent = function (type, bubbles, cancelable) {
                this.type_ = type.toLowerCase();
                this.bubbles_ = bubbles;
                this.cancelable_ = cancelable;
            };
            /* -------------------------------------------------------------------
                ACTIONS ON PROGRESS
            ------------------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            //public abstract preventDefault(): void;
            /**
             * @inheritdoc
             */
            BasicEvent.prototype.stopImmediatePropagation = function () {
            };
            /**
             * @inheritdoc
             */
            BasicEvent.prototype.stopPropagation = function () {
            };
            Object.defineProperty(BasicEvent.prototype, "type", {
                /* -------------------------------------------------------------------
                    GETTERS
                ------------------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return this.type_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "target", {
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return this.target_;
                },
                set: function (obj) {
                    this.target_ = obj;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "currentTarget", {
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return this.currentTarget_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "bubbles", {
                ///**
                // * @inheritdoc
                // */
                //public get isTrusted(): boolean
                //{
                //	return this.isTrusted;
                //}
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return this.bubbles_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "cancelable", {
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return this.cancelable_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "eventPhase", {
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return 0;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "defaultPrevented", {
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return this.defaultPrevented_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "srcElement", {
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return null;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "cancelBubble", {
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return this.cancelBubble_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "timeStamp", {
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return this.timeStamp_.getTime();
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "returnValue", {
                /**
                 * Don't know what it is.
                 */
                get: function () {
                    return false;
                },
                enumerable: true,
                configurable: true
            });
            return BasicEvent;
        }());
        library.BasicEvent = BasicEvent;
        //export class ProgressEvent
        //	extends library.BasicEvent
        //{
        //	public static get PROGRESS(): string { return "progress"; }
        //	protected numerator_: number;
        //	protected denominator_: number;
        //	public constructor(type: string, numerator: number, denominator: number)
        //	{
        //		super(type);
        //		this.numerator_ = numerator;
        //		this.denominator_ = denominator;
        //	}
        //	public get numerator(): number
        //	{
        //		return this.numerator_;
        //	}
        //	public get denominator(): number
        //	{
        //		return this.denominator_;
        //	}
        //}
    })(library = samchon.library || (samchon.library = {}));
})(samchon || (samchon = {}));
;
/// <reference path="../API.ts" />
/// <reference path="../library/BasicEvent.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * An event occured in a {@link ICollection collection} object.
         *
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var CollectionEvent = (function (_super) {
            __extends(CollectionEvent, _super);
            function CollectionEvent(type, first, last) {
                var _this = _super.call(this, type, false, (type == "insert" || type == "erase")) || this;
                if (type == "erase" && (first instanceof std.VectorIterator || first instanceof std.DequeIterator)) {
                    _this.temporary_container_ = new std.Vector(first, last);
                    _this.origin_first_ = first;
                    _this.first_ = _this.temporary_container_.begin();
                    _this.last_ = _this.temporary_container_.end();
                }
                else {
                    _this.temporary_container_ = null;
                    _this.origin_first_ = null;
                    _this.first_ = first;
                    _this.last_ = last;
                }
                return _this;
            }
            Object.defineProperty(CollectionEvent.prototype, "target", {
                /**
                 * Associative target, the {@link ICollection collection}.
                 */
                get: function () {
                    return this.target_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(CollectionEvent.prototype, "first", {
                /**
                 * An {@link Iterator} to the initial position in this {@link CollectionEvent}.
                 */
                get: function () {
                    return this.first_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(CollectionEvent.prototype, "last", {
                /**
                 * An {@link Iterator} to the final position in this {@link CollectionEvent}.
                 */
                get: function () {
                    return this.last_;
                },
                enumerable: true,
                configurable: true
            });
            /**
             * @inheritdoc
             */
            CollectionEvent.prototype.preventDefault = function () {
                if (this.cancelable == false)
                    return;
                this.defaultPrevented_ = true;
                if (this.type == "insert") {
                    this.target.erase(this.first_, this.last_);
                }
                else if (this.type == "erase") {
                    var container = this.target;
                    var it = void 0;
                    if (this.temporary_container_ == null)
                        it = this.first_.prev().next();
                    else
                        it = this.origin_first_.prev().next();
                    container.insert(it, this.first_, this.last_);
                }
                this.defaultPrevented_ = false;
            };
            return CollectionEvent;
        }(samchon.library.BasicEvent));
        collections.CollectionEvent = CollectionEvent;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var collections;
    (function (collections) {
        var CollectionEvent;
        (function (CollectionEvent) {
            CollectionEvent.INSERT = "insert";
            CollectionEvent.ERASE = "erase";
            CollectionEvent.REFRESH = "refresh";
        })(CollectionEvent = collections.CollectionEvent || (collections.CollectionEvent = {}));
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * A {@link Deque} who can detect element I/O events.
         *
         * Below is the list of methods who are dispatching {@link CollectionEvent}:
         * - *insert* typed events:
         *   - {@link assign}
         *   - {@link insert}
         *   - {@link push}
         *   - {@link push_front}
         *   - {@link push_back}
         * - *erase* typed events:
         *   - {@link assign}
         *   - {@link clear}
         *   - {@link erase}
         *   - {@link pop_front}
         *   - {@link pop_back}
         * - *refresh* typed events:
         *   - {@link refresh}
         *
         * #### [Inherited] {@link Deque}
         * {@link Deque} (usually pronounced like "*deck*") is an irregular acronym of **d**ouble-**e**nded **q**ueue.
         * Double-ended queues are sequence containers with dynamic sizes that can be expanded or contracted on both ends
         * (either its front or its back).
         *
         * Specific libraries may implement deques in different ways, generally as some form of dynamic array. But in any
         * case, they allow for the individual elements to be accessed directly through random access iterators, with
         * storage handled automatically by expanding and contracting the container as needed.
         *
         * Therefore, they provide a functionality similar to vectors, but with efficient insertion and deletion of
         * elements also at the beginning of the sequence, and not only at its end. But, unlike {@link Vector Vectors},
         * {@link Deque Deques} are not guaranteed to store all its elements in contiguous storage locations: accessing
         * elements in a <u>deque</u> by offsetting a pointer to another element causes undefined behavior.
         *
         * Both {@link Vector}s and {@link Deque}s provide a very similar interface and can be used for similar purposes,
         * but internally both work in quite different ways: While {@link Vector}s use a single array that needs to be
         * occasionally reallocated for growth, the elements of a {@link Deque} can be scattered in different chunks of
         * storage, with the container keeping the necessary information internally to provide direct access to any of its
         * elements in constant time and with a uniform sequential interface (through iterators). Therefore,
         * {@link Deque Deques} are a little more complex internally than {@link Vector}s, but this allows them to grow
         * more efficiently under certain circumstances, especially with very long sequences, where reallocations become
         * more expensive.
         *
         * For operations that involve frequent insertion or removals of elements at positions other than the beginning or
         * the end, {@link Deque Deques} perform worse and have less consistent iterators and references than
         * {@link List Lists}.
         *
         * <a href="http://samchon.github.io/typescript-stl/images/design/class_diagram/linear_containers.png" target="_blank">
         * <img src="http://samchon.github.io/typescript-stl/images/design/class_diagram/linear_containers.png" style="max-width: 100%" />
         * </a>
         *
         * <h3> Container properties </h3>
         * <dl>
         *	<dt> Sequence </dt>
         *	<dd> Elements in sequence containers are ordered in a strict linear sequence. Individual elements
         *		 are accessed by their position in this sequence. </dd>
         *
         *	<dt> Dynamic array </dt>
         *	<dd> Generally implemented as a dynamic array, it allows direct access to any element in the
         *		 sequence and provides relatively fast addition/removal of elements at the beginning or the end
         *		 of the sequence. </dd>
         * </dl>
         *
         * @param <T> Type of the elements.
         *
         * @reference http://www.cplusplus.com/reference/deque/deque/
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var DequeCollection = (function (_super) {
            __extends(DequeCollection, _super);
            function DequeCollection() {
                var _this = _super.apply(this, arguments) || this;
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                _this.event_dispatcher_ = new samchon.library.EventDispatcher(_this);
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /* =========================================================
                ELEMENTS I/O
                    - INSERT
                    - ERASE
                    - NOTIFIER
            ============================================================
                INSERT
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            DequeCollection.prototype.push = function () {
                var items = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    items[_i] = arguments[_i];
                }
                var ret = _super.prototype.push.apply(this, items);
                this._Notify_insert(this.end().advance(-items.length), this.end());
                return ret;
            };
            /**
             * @inheritdoc
             */
            DequeCollection.prototype.push_back = function (val) {
                _super.prototype.push.call(this, val);
                this._Notify_insert(this.end().prev(), this.end());
            };
            /**
             * @hidden
             */
            DequeCollection.prototype._Insert_by_repeating_val = function (position, n, val) {
                var ret = _super.prototype._Insert_by_repeating_val.call(this, position, n, val);
                this._Notify_insert(ret, ret.advance(n));
                return ret;
            };
            /**
             * @hidden
             */
            DequeCollection.prototype._Insert_by_range = function (position, begin, end) {
                var n = this.size();
                var ret = _super.prototype._Insert_by_range.call(this, position, begin, end);
                n = this.size() - n;
                this._Notify_insert(ret, ret.advance(n));
                return ret;
            };
            /* ---------------------------------------------------------
                ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            DequeCollection.prototype.pop_front = function () {
                this._Notify_erase(this.begin(), this.begin().next());
                _super.prototype.pop_front.call(this);
            };
            /**
             * @inheritdoc
             */
            DequeCollection.prototype.pop_back = function () {
                this._Notify_erase(this.end().prev(), this.end());
                _super.prototype.pop_back.call(this);
            };
            /**
             * @hidden
             */
            DequeCollection.prototype._Erase_by_range = function (first, last) {
                this._Notify_erase(first, last);
                return _super.prototype._Erase_by_range.call(this, first, last);
            };
            /* ---------------------------------------------------------
                NOTIFIER
            --------------------------------------------------------- */
            /**
             * @hidden
             */
            DequeCollection.prototype._Notify_insert = function (first, last) {
                collections.ICollection._Dispatch_CollectionEvent(this, "insert", first, last);
            };
            /**
             * @hidden
             */
            DequeCollection.prototype._Notify_erase = function (first, last) {
                collections.ICollection._Dispatch_CollectionEvent(this, "erase", first, last);
            };
            /* =========================================================
                EVENT_DISPATCHER
                    - ACCESSORS
                    - ADD
                    - REMOVE
            ============================================================
                ACCESSORS
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            DequeCollection.prototype.hasEventListener = function (type) {
                return this.event_dispatcher_.hasEventListener(type);
            };
            /**
             * @inheritdoc
             */
            DequeCollection.prototype.dispatchEvent = function (event) {
                return this.event_dispatcher_.dispatchEvent(event);
            };
            DequeCollection.prototype.refresh = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var first;
                var last;
                if (args.length == 0) {
                    first = this.begin();
                    last = this.end();
                }
                else if (args.length == 1) {
                    first = args[0];
                    last = first.next();
                }
                else {
                    first = args[0];
                    last = args[1];
                }
                this.dispatchEvent(new collections.CollectionEvent("refresh", first, last));
            };
            DequeCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            DequeCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return DequeCollection;
        }(std.Deque));
        collections.DequeCollection = DequeCollection;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * A {@link HashMap} who can detect element I/O events.
         *
         * Below is the list of methods who are dispatching {@link MapCollectionEvent}:
         * - *insert* typed events:
         *   - {@link assign}
         *   - {@link insert}
         *   - {@link insert_or_assign}
         *   - {@link emplace}
         *   - {@link set}
         *   - {@link push}
         * - *erase* typed events:
         *   - {@link assign}
         *   - {@link clear}
         *   - {@link erase}
         *   - {@link extract}
         * - *refresh* typed events:
         *   - {@link refresh}
         *
         * #### [Inherited] {@link HashMap}
         * {@link HashMap HashMaps} are associative containers that store elements formed by the combination of a
         * *key value* and a *mapped value*, and which allows for fast retrieval of individual elements based on their
         * *keys*.
         *
         * In an {@link HashMap}, the *key value* is generally used to uniquely identify the element, while the
         * *mapped value* is an object with the content associated to this *key*. Types of *key* and *mapped value* may
         * differ.
         *
         * Internally, the elements in the {@link HashMap} are not sorted in any particular order with respect to either
         * their *key* or *mapped values*, but organized into *buckets* depending on their hash values to allow for fast
         * access to individual elements directly by their *key values* (with a constant average time complexity on
         * average).
         *
         * {@link HashMap} containers are faster than {@link TreeMap} containers to access individual elements by their
         * *key*, although they are generally less efficient for range iteration through a subset of their elements.
         *
         *  <a href="http://samchon.github.io/typescript-stl/images/design/class_diagram/map_containers.png" target="_blank">
         * <img src="http://samchon.github.io/typescript-stl/images/design/class_diagram/map_containers.png" style="max-width: 100%" />
         * </a>
         *
         * <h3> Container properties </h3>
         * <dl>
         * 	<dt> Associative </dt>
         * 	<dd> Elements in associative containers are referenced by their *key* and not by their absolute
         *		 position in the container. </dd>
         *
         * 	<dt> Hashed </dt>
         * 	<dd> Hashed containers organize their elements using hash tables that allow for fast access to elements
         *		 by their *key*. </dd>
         *
         * 	<dt> Map </dt>
         * 	<dd> Each element associates a *key* to a *mapped value*:
         *		 *Keys* are meant to identify the elements whose main content is the *mapped value*. </dd>
         *
         * 	<dt> Unique keys </dt>
         * 	<dd> No two elements in the container can have equivalent keys. </dd>
         * </dl>
         *
         * @param <Key> Type of the key values.
         *				Each element in an {@link HashMap} is uniquely identified by its key value.
         * @param <T> Type of the mapped value.
         *			  Each element in an {@link HashMap} is used to store some data as its mapped value.
         *
         * @reference http://www.cplusplus.com/reference/unordered_map/unordered_map
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var HashMapCollection = (function (_super) {
            __extends(HashMapCollection, _super);
            function HashMapCollection() {
                var _this = _super.apply(this, arguments) || this;
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                _this.event_dispatcher_ = new samchon.library.EventDispatcher(_this);
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /* =========================================================
                ELEMENTS I/O
                    - HANDLE_INSERT & HANDLE_ERASE
            ============================================================
                HANDLE_INSERT & HANDLE_ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            HashMapCollection.prototype._Handle_insert = function (first, last) {
                _super.prototype._Handle_insert.call(this, first, last);
                collections.ICollection._Dispatch_MapCollectionEvent(this, "insert", first, last);
            };
            /**
             * @inheritdoc
             */
            HashMapCollection.prototype._Handle_erase = function (first, last) {
                _super.prototype._Handle_erase.call(this, first, last);
                collections.ICollection._Dispatch_MapCollectionEvent(this, "erase", first, last);
            };
            /* =========================================================
                EVENT_DISPATCHER
                    - ACCESSORS
                    - ADD
                    - REMOVE
            ============================================================
                ACCESSORS
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            HashMapCollection.prototype.hasEventListener = function (type) {
                return this.event_dispatcher_.hasEventListener(type);
            };
            /**
             * @inheritdoc
             */
            HashMapCollection.prototype.dispatchEvent = function (event) {
                return this.event_dispatcher_.dispatchEvent(event);
            };
            HashMapCollection.prototype.refresh = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var first;
                var last;
                if (args.length == 0) {
                    first = this.begin();
                    last = this.end();
                }
                else if (args.length == 1) {
                    first = args[0];
                    last = first.next();
                }
                else {
                    first = args[0];
                    last = args[1];
                }
                this.dispatchEvent(new collections.MapCollectionEvent("refresh", first, last));
            };
            HashMapCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            HashMapCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return HashMapCollection;
        }(std.HashMap));
        collections.HashMapCollection = HashMapCollection;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * A {@link HashMultiMap} who can detect element I/O events.
         *
         * Below is the list of methods who are dispatching {@link MapCollectionEvent}:
         * - *insert* typed events:
         *   - {@link assign}
         *   - {@link insert}
         *   - {@link emplace}
         *   - {@link push}
         * - *erase* typed events:
         *   - {@link assign}
         *   - {@link clear}
         *   - {@link erase}
         * - *refresh* typed events:
         *   - {@link refresh}
         *
         * #### [Inherited] {@link HashMultiMap}
         * {@link HashMultiMap HashMultiMap}s are associative containers that store elements formed by the combination of
         * a *key value* and a *mapped value*, much like {@link HashMultiMap} containers, but allowing different elements
         * to have equivalent *keys*.
         *
         * In an {@link HashMultiMap}, the *key value* is generally used to uniquely identify the element, while the
         * *mapped value* is an object with the content associated to this *key*. Types of *key* and *mapped value* may
         * differ.
         *
         * Internally, the elements in the {@link HashMultiMap} are not sorted in any particular order with respect to
         * either their *key* or *mapped values*, but organized into *buckets* depending on their hash values to allow for
         * fast access to individual elements directly by their *key values* (with a constant average time complexity on
         * average).
         *
         * Elements with equivalent *keys* are grouped together in the same bucket and in such a way that an iterator can
         * iterate through all of them. Iterators in the container are doubly linked iterators.
         *
         * <a href="http://samchon.github.io/typescript-stl/images/design/class_diagram/map_containers.png" target="_blank">
         * <img src="http://samchon.github.io/typescript-stl/images/design/class_diagram/map_containers.png" style="max-width: 100%" />
         * </a>
         *
         * <h3> Container properties </h3>
         * <dl>
         *	<dt> Associative </dt>
         *	<dd> Elements in associative containers are referenced by their *key* and not by their absolute
         *		 position in the container. </dd>
         *
         *	<dt> Hashed </dt>
         *	<dd> Hashed containers organize their elements using hash tables that allow for fast access to elements
         *		 by their *key*. </dd>
         *
         *	<dt> Map </dt>
         *	<dd> Each element associates a *key* to a *mapped value*:
         *		 *Keys* are meant to identify the elements whose main content is the *mapped value*. </dd>
         *
         *	<dt> Multiple equivalent keys </dt>
         *	<dd> The container can hold multiple elements with equivalent *keys*. </dd>
         * </dl>
         *
         * @param <Key> Type of the key values.
         *				Each element in an {@link HashMultiMap} is identified by a key value.
         * @param <T> Type of the mapped value.
         *			  Each element in an {@link HashMultiMap} is used to store some data as its mapped value.
         *
         * @reference http://www.cplusplus.com/reference/unordered_map/unordered_multimap
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var HashMultiMapCollection = (function (_super) {
            __extends(HashMultiMapCollection, _super);
            function HashMultiMapCollection() {
                var _this = _super.apply(this, arguments) || this;
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                _this.event_dispatcher_ = new samchon.library.EventDispatcher(_this);
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /* =========================================================
                ELEMENTS I/O
                    - HANDLE_INSERT & HANDLE_ERASE
            ============================================================
                HANDLE_INSERT & HANDLE_ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            HashMultiMapCollection.prototype._Handle_insert = function (first, last) {
                _super.prototype._Handle_insert.call(this, first, last);
                collections.ICollection._Dispatch_MapCollectionEvent(this, "insert", first, last);
            };
            /**
             * @inheritdoc
             */
            HashMultiMapCollection.prototype._Handle_erase = function (first, last) {
                _super.prototype._Handle_erase.call(this, first, last);
                collections.ICollection._Dispatch_MapCollectionEvent(this, "erase", first, last);
            };
            /* =========================================================
                EVENT_DISPATCHER
                    - ACCESSORS
                    - ADD
                    - REMOVE
            ============================================================
                ACCESSORS
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            HashMultiMapCollection.prototype.hasEventListener = function (type) {
                return this.event_dispatcher_.hasEventListener(type);
            };
            /**
             * @inheritdoc
             */
            HashMultiMapCollection.prototype.dispatchEvent = function (event) {
                return this.event_dispatcher_.dispatchEvent(event);
            };
            HashMultiMapCollection.prototype.refresh = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var first;
                var last;
                if (args.length == 0) {
                    first = this.begin();
                    last = this.end();
                }
                else if (args.length == 1) {
                    first = args[0];
                    last = first.next();
                }
                else {
                    first = args[0];
                    last = args[1];
                }
                this.dispatchEvent(new collections.MapCollectionEvent("refresh", first, last));
            };
            HashMultiMapCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            HashMultiMapCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return HashMultiMapCollection;
        }(std.HashMap));
        collections.HashMultiMapCollection = HashMultiMapCollection;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * A {@link HashMultiSet} who can detect element I/O events.
         *
         * Below is the list of methods who are dispatching {@link CollectionEvent}:
         * - *insert* typed events:
         *   - {@link assign}
         *   - {@link insert}
         *   - {@link push}
         * - *erase* typed events:
         *   - {@link assign}
         *   - {@link clear}
         *   - {@link erase}
         * - *refresh* typed events:
         *   - {@link refresh}
         *
         * #### [Inherited] {@link HashMultiSet}
         * {@link HashMultiSet HashMultiSets} are containers that store elements in no particular order, allowing fast
         * retrieval of individual elements based on their value, much like {@link HashMultiSet} containers, but allowing
         * different elements to have equivalent values.
         *
         * In an {@link HashMultiSet}, the value of an element is at the same time its *key*, used to identify it. *Keys*
         * are immutable, therefore, the elements in an {@link HashMultiSet} cannot be modified once in the container -
         * they can be inserted and removed, though.
         *
         * Internally, the elements in the {@link HashMultiSet} are not sorted in any particular, but organized into
         * *buckets* depending on their hash values to allow for fast access to individual elements directly by their
         * *values* (with a constant average time complexity on average).
         *
         * Elements with equivalent values are grouped together in the same bucket and in such a way that an iterator can
         * iterate through all of them. Iterators in the container are doubly linked iterators.
         *
         * <a href="http://samchon.github.io/typescript-stl/images/design/class_diagram/set_containers.png" target="_blank">
         * <img src="http://samchon.github.io/typescript-stl/images/design/class_diagram/set_containers.png" style="max-width: 100%" />
         * </a>
         *
         * <h3> Container properties </h3>
         * <dl>
         *	<dt> Associative </dt>
         *	<dd> Elements in associative containers are referenced by their *key* and not by their absolute
         *		 position in the container. </dd>
         *
         *	<dt> Hashed </dt>
         *	<dd> Hashed containers organize their elements using hash tables that allow for fast access to elements
         *		 by their *key*. </dd>
         *
         *	<dt> Set </dt>
         *	<dd> The value of an element is also the *key* used to identify it. </dd>
         *
         *	<dt> Multiple equivalent keys </dt>
         *	<dd> The container can hold multiple elements with equivalent *keys*. </dd>
         * </dl>
         *
         * @param <T> Type of the elements.
         *		   Each element in an {@link UnorderedMultiSet} is also identified by this value..
         *
         * @reference http://www.cplusplus.com/reference/unordered_set/unordered_multiset
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var HashMultiSetCollection = (function (_super) {
            __extends(HashMultiSetCollection, _super);
            function HashMultiSetCollection() {
                var _this = _super.apply(this, arguments) || this;
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                _this.event_dispatcher_ = new samchon.library.EventDispatcher(_this);
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /* =========================================================
                ELEMENTS I/O
                    - HANDLE_INSERT & HANDLE_ERASE
            ============================================================
                HANDLE_INSERT & HANDLE_ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            HashMultiSetCollection.prototype._Handle_insert = function (first, last) {
                _super.prototype._Handle_insert.call(this, first, last);
                collections.ICollection._Dispatch_CollectionEvent(this, "insert", first, last);
            };
            /**
             * @inheritdoc
             */
            HashMultiSetCollection.prototype._Handle_erase = function (first, last) {
                _super.prototype._Handle_erase.call(this, first, last);
                collections.ICollection._Dispatch_CollectionEvent(this, "erase", first, last);
            };
            /* =========================================================
                EVENT_DISPATCHER
                    - ACCESSORS
                    - ADD
                    - REMOVE
            ============================================================
                ACCESSORS
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            HashMultiSetCollection.prototype.hasEventListener = function (type) {
                return this.event_dispatcher_.hasEventListener(type);
            };
            /**
             * @inheritdoc
             */
            HashMultiSetCollection.prototype.dispatchEvent = function (event) {
                return this.event_dispatcher_.dispatchEvent(event);
            };
            HashMultiSetCollection.prototype.refresh = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var first;
                var last;
                if (args.length == 0) {
                    first = this.begin();
                    last = this.end();
                }
                else if (args.length == 1) {
                    first = args[0];
                    last = first.next();
                }
                else {
                    first = args[0];
                    last = args[1];
                }
                this.dispatchEvent(new collections.CollectionEvent("refresh", first, last));
            };
            HashMultiSetCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            HashMultiSetCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return HashMultiSetCollection;
        }(std.HashMultiSet));
        collections.HashMultiSetCollection = HashMultiSetCollection;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * A {@link HashSet} who can detect element I/O events.
         *
         * Below is the list of methods who are dispatching {@link CollectionEvent}:
         * - *insert* typed events:
         *   - {@link assign}
         *   - {@link insert}
         *   - {@link push}
         *   - {@link insert_or_assign}
         * - *erase* typed events:
         *   - {@link assign}
         *   - {@link clear}
         *   - {@link erase}
         *   - {@link extract}
         * - *refresh* typed events:
         *   - {@link refresh}
         *
         * #### [Inherited] {@link HashSet}
         * {@link HashSet HashSets} are containers that store unique elements in no particular order, and which allow for
         * fast retrieval of individual elements based on their value.
         *
         * In an {@link HashSet}, the value of an element is at the same time its *key*, that identifies it uniquely.
         * Keys are immutable, therefore, the elements in an {@link HashSet} cannot be modified once in the container -
         * they can be inserted and removed, though.
         *
         * Internally, the elements in the {@link HashSet} are not sorted in any particular order, but organized into
         * buckets depending on their hash values to allow for fast access to individual elements directly by their
         * *values* (with a constant average time complexity on average).
         *
         * {@link HashSet} containers are faster than {@link TreeSet} containers to access individual elements by their
         * *key*, although they are generally less efficient for range iteration through a subset of their elements.
         *
         * <a href="http://samchon.github.io/typescript-stl/images/design/class_diagram/set_containers.png" target="_blank">
         * <img src="http://samchon.github.io/typescript-stl/images/design/class_diagram/set_containers.png" style="max-width: 100%" />
         * </a>
         *
         * <h3> Container properties </h3>
         * <dl>
         *	<dt> Associative </dt>
         *	<dd> Elements in associative containers are referenced by their *key* and not by their absolute
         *		 position in the container. </dd>
         *
         *	<dt> Hashed </dt>
         *	<dd> Hashed containers organize their elements using hash tables that allow for fast access to elements
         *		 by their *key*. </dd>
         *
         *	<dt> Set </dt>
         *	<dd> The value of an element is also the *key* used to identify it. </dd>
         *
         *	<dt> Unique keys </dt>
         *	<dd> No two elements in the container can have equivalent *keys*. </dd>
         * </dl>
         *
         * @param <T> Type of the elements.
         *			  Each element in an {@link HashSet} is also uniquely identified by this value.
         *
         * @reference http://www.cplusplus.com/reference/unordered_set/unordered_set
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var HashSetCollection = (function (_super) {
            __extends(HashSetCollection, _super);
            function HashSetCollection() {
                var _this = _super.apply(this, arguments) || this;
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                _this.event_dispatcher_ = new samchon.library.EventDispatcher(_this);
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /* =========================================================
                ELEMENTS I/O
                    - HANDLE_INSERT & HANDLE_ERASE
            ============================================================
                HANDLE_INSERT & HANDLE_ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            HashSetCollection.prototype._Handle_insert = function (first, last) {
                _super.prototype._Handle_insert.call(this, first, last);
                collections.ICollection._Dispatch_CollectionEvent(this, "insert", first, last);
            };
            /**
             * @inheritdoc
             */
            HashSetCollection.prototype._Handle_erase = function (first, last) {
                _super.prototype._Handle_erase.call(this, first, last);
                collections.ICollection._Dispatch_CollectionEvent(this, "erase", first, last);
            };
            /* =========================================================
                EVENT_DISPATCHER
                    - ACCESSORS
                    - ADD
                    - REMOVE
            ============================================================
                ACCESSORS
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            HashSetCollection.prototype.hasEventListener = function (type) {
                return this.event_dispatcher_.hasEventListener(type);
            };
            /**
             * @inheritdoc
             */
            HashSetCollection.prototype.dispatchEvent = function (event) {
                return this.event_dispatcher_.dispatchEvent(event);
            };
            HashSetCollection.prototype.refresh = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var first;
                var last;
                if (args.length == 0) {
                    first = this.begin();
                    last = this.end();
                }
                else if (args.length == 1) {
                    first = args[0];
                    last = first.next();
                }
                else {
                    first = args[0];
                    last = args[1];
                }
                this.dispatchEvent(new collections.CollectionEvent("refresh", first, last));
            };
            HashSetCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            HashSetCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return HashSetCollection;
        }(std.HashSet));
        collections.HashSetCollection = HashSetCollection;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/**
 * Collections, elements I/O detectable STL containers.
 *
 * STL Containers       | Collections
 * ---------------------|-------------------
 * {@link Vector}       | {@link ArrayCollection}
 * {@link List}         | {@link ListCollection}
 * {@link Deque}        | {@link DequeCollection}
 *                      |
 * {@link TreeSet}      | {@link TreeSetCollection}
 * {@link HashSet}      | {@link HashSetCollection}
 * {@link TreeMultiSet} | {@link TreeMultiSetCollection}
 * {@link HashMultiSet} | {@link HashMultiSetCollection}
 *                      |
 * {@link TreeMap}      | {@link TreeMapCollection}
 * {@link HashMap}      | {@link HashMapCollection}
 * {@link TreeMultiMap} | {@link TreeMultiMapCollection}
 * {@link HashMultiMap} | {@link HashMultiMapCollection}
 *
 * @author Jeongho Nam <http://samchon.org>
 */
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * @hidden
         */
        var ICollection;
        (function (ICollection) {
            /**
             * @hidden
             */
            function _Dispatch_CollectionEvent(collection, type, first, last) {
                if (collection.hasEventListener(type) == false)
                    return;
                var event = new collections.CollectionEvent(type, first, last);
                setTimeout(function () {
                    collection.dispatchEvent(event);
                });
            }
            ICollection._Dispatch_CollectionEvent = _Dispatch_CollectionEvent;
            /**
             * @hidden
             */
            function _Dispatch_MapCollectionEvent(collection, type, first, last) {
                if (collection.hasEventListener(type) == false)
                    return;
                var event = new collections.MapCollectionEvent(type, first, last);
                setTimeout(function () {
                    collection.dispatchEvent(event);
                });
            }
            ICollection._Dispatch_MapCollectionEvent = _Dispatch_MapCollectionEvent;
        })(ICollection = collections.ICollection || (collections.ICollection = {}));
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * A {@link List} who can detect element I/O events.
         *
         * Below is the list of methods who are dispatching {@link CollectionEvent}:
         *	- *insert* typed events:
         *   - {@link assign}
         *   - {@link insert}
         *   - {@link push}
         *   - {@link push_front}
         *   - {@link push_back}
         *   - {@link merge}
         * - *erase* typed events:
         *   - {@link assign}
         *   - {@link clear}
         *   - {@link erase}
         *   - {@link pop_front}
         *   - {@link pop_back}
         *   - {@link unique}
         *   - {@link remove}
         *   - {@link remove_if}
         *   - {@link splice}
         * - *refresh* typed events:
         *   - {@link refresh}
         *   - {@link sort}
         *
         * #### [Inherited] {@link List}
         * {@link List Lists} are sequence containers that allow constant time insert and erase operations anywhere within
         * the sequence, and iteration in both directions.
         *
         * List containers are implemented as doubly-linked lists; Doubly linked lists can store each of the elements they
         * contain in different and unrelated storage locations. The ordering is kept internally by the association to
         * each element of a link to the element preceding it and a link to the element following it.
         *
         * They are very similar to forward_list: The main difference being that forward_list objects are single-linked
         * lists, and thus they can only be iterated forwards, in exchange for being somewhat smaller and more efficient.
         *
         * Compared to other base standard sequence containers (array, vector and deque), lists perform generally better
         * in inserting, extracting and moving elements in any position within the container for which an iterator has
         * already been obtained, and therefore also in algorithms that make intensive use of these, like sorting
         * algorithms.
         *
         * The main drawback of lists and forward_lists compared to these other sequence containers is that they lack
         * direct access to the elements by their position; For example, to access the sixth element in a list, one has to
         * iterate from a known position (like the beginning or the end) to that position, which takes linear time in the
         * distance between these. They also consume some extra memory to keep the linking information associated to each
         * element (which may be an important factor for large lists of small-sized elements).
         *
         * <a href="http://samchon.github.io/typescript-stl/images/design/class_diagram/linear_containers.png" target="_blank">
         * <img src="http://samchon.github.io/typescript-stl/images/design/class_diagram/linear_containers.png" style="max-width: 100%" />
         * </a>
         *
         * <h3> Container properties </h3>
         * <dl>
         * 	<dt> Sequence </dt>
         * 	<dd> Elements in sequence containers are ordered in a strict linear sequence. Individual elements are accessed by
         *		 their position in this sequence. </dd>
         *
         * 	<dt> Doubly-linked list </dt>
         *	<dd> Each element keeps information on how to locate the next and the previous elements, allowing constant time
         *		 insert and erase operations before or after a specific element (even of entire ranges), but no direct random
         *		 access. </dd>
         * </dl>
         *
         * @param <T> Type of the elements.
         *
         * @reference http://www.cplusplus.com/reference/list/list/
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var ListCollection = (function (_super) {
            __extends(ListCollection, _super);
            function ListCollection() {
                var _this = _super.apply(this, arguments) || this;
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                _this.event_dispatcher_ = new samchon.library.EventDispatcher(_this);
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /* =========================================================
                ELEMENTS I/O
                    - INSERT
                    - ERASE
                    - NOTIFIER
            ============================================================
                INSERT
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            ListCollection.prototype.push = function () {
                var items = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    items[_i] = arguments[_i];
                }
                var ret = _super.prototype.push.apply(this, items);
                this._Notify_insert(this.end().advance(-items.length), this.end());
                return ret;
            };
            /**
             * @hidden
             */
            ListCollection.prototype._Insert_by_repeating_val = function (position, n, val) {
                var ret = _super.prototype._Insert_by_repeating_val.call(this, position, n, val);
                this._Notify_insert(ret, ret.advance(n));
                return ret;
            };
            /**
             * @hidden
             */
            ListCollection.prototype._Insert_by_range = function (position, begin, end) {
                var n = this.size();
                var ret = _super.prototype._Insert_by_range.call(this, position, begin, end);
                n = this.size() - n;
                this._Notify_insert(ret, ret.advance(n));
                return ret;
            };
            /* ---------------------------------------------------------
                ERASE
            --------------------------------------------------------- */
            /**
             * @hidden
             */
            ListCollection.prototype._Erase_by_range = function (first, last) {
                var ret = _super.prototype._Erase_by_range.call(this, first, last);
                this._Notify_erase(first, last);
                return ret;
            };
            /* ---------------------------------------------------------
                NOTIFIER
            --------------------------------------------------------- */
            /**
             * @hidden
             */
            ListCollection.prototype._Notify_insert = function (first, last) {
                collections.ICollection._Dispatch_CollectionEvent(this, "insert", first, last);
            };
            /**
             * @hidden
             */
            ListCollection.prototype._Notify_erase = function (first, last) {
                collections.ICollection._Dispatch_CollectionEvent(this, "erase", first, last);
            };
            /* =========================================================
                EVENT_DISPATCHER
                    - ACCESSORS
                    - ADD
                    - REMOVE
            ============================================================
                ACCESSORS
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            ListCollection.prototype.hasEventListener = function (type) {
                return this.event_dispatcher_.hasEventListener(type);
            };
            /**
             * @inheritdoc
             */
            ListCollection.prototype.dispatchEvent = function (event) {
                return this.event_dispatcher_.dispatchEvent(event);
            };
            ListCollection.prototype.refresh = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var first;
                var last;
                if (args.length == 0) {
                    first = this.begin();
                    last = this.end();
                }
                else if (args.length == 1) {
                    first = args[0];
                    last = first.next();
                }
                else {
                    first = args[0];
                    last = args[1];
                }
                this.dispatchEvent(new collections.CollectionEvent("refresh", first, last));
            };
            ListCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            ListCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return ListCollection;
        }(std.List));
        collections.ListCollection = ListCollection;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="CollectionEvent.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * An event occured in a {@link MapContainer map container} object.
         *
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var MapCollectionEvent = (function (_super) {
            __extends(MapCollectionEvent, _super);
            function MapCollectionEvent() {
                return _super.apply(this, arguments) || this;
            }
            Object.defineProperty(MapCollectionEvent.prototype, "first", {
                // using super::constructor
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return this.first_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(MapCollectionEvent.prototype, "last", {
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return this.last_;
                },
                enumerable: true,
                configurable: true
            });
            return MapCollectionEvent;
        }(collections.CollectionEvent));
        collections.MapCollectionEvent = MapCollectionEvent;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * A {@link TreeMap} who can detect element I/O events.
         *
         * Below is the list of methods who are dispatching {@link MapCollectionEvent}:
         * - *insert* typed events:
         *   - {@link assign}
         *   - {@link insert}
         *   - {@link insert_or_assign}
         *   - {@link emplace}
         *   - {@link set}
         *   - {@link push}
         * - *erase* typed events:
         *   - {@link assign}
         *   - {@link clear}
         *   - {@link erase}
         *   - {@link extract}
         * - *refresh* typed events:
         *   - {@link refresh}
         *
         * #### [Inherited] {@link TreeMap}
         * {@link TreeMap TreeMaps} are associative containers that store elements formed by a combination of a
         * *key value* (*Key*) and a *mapped value* (*T*), following order.
         *
         * In a {@link TreeMap}, the *key values* are generally used to sort and uniquely identify the elements, while the
         * *mapped values* store the content associated to this key. The types of *key* and *mapped value* may differ, and
         * are grouped together in member type *value_type*, which is a {@link Pair} type combining both:
         *
         * ```typedef Pair<Key, T> value_type;```
         *
         * Internally, the elements in a {@link TreeMap} are always sorted by its *key* following a *strict weak ordering*
         * criterion indicated by its internal comparison method {@link less}.
         *
         * {@link TreeMap} containers are generally slower than {@link HashMap HashMap} containers to access individual
         * elements by their *key*, but they allow the direct iteration on subsets based on their order.
         *
         * {@link TreeMap}s are typically implemented as binary search trees.
         *
         * <a href="http://samchon.github.io/typescript-stl/images/design/class_diagram/map_containers.png" target="_blank">
         * <img src="http://samchon.github.io/typescript-stl/images/design/class_diagram/map_containers.png" style="max-width: 100%" />
         * </a>
         *
         * <h3> Container properties </h3>
         * <dl>
         *	<dt> Associative </dt>
         *	<dd> Elements in associative containers are referenced by their *key* and not by their absolute
         *		 position in the container. </dd>
         *
         *	<dt> Ordered </dt>
         *	<dd> The elements in the container follow a strict order at all times. All inserted elements are
         *		 given a position in this order. </dd>
         *
         *	<dt> Map </dt>
         *	<dd> Each element associates a *key* to a *mapped value*:
         *		 *Keys* are meant to identify the elements whose main content is the *mapped value*. </dd>
         *
         *	<dt> Unique keys </dt>
         *	<dd> No two elements in the container can have equivalent *keys*. </dd>
         * </dl>
         *
         * @param <Key> Type of the keys. Each element in a map is uniquely identified by its key value.
         * @param <T> Type of the mapped value. Each element in a map stores some data as its mapped value.
         *
         * @reference http://www.cplusplus.com/reference/map/map
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var TreeMapCollection = (function (_super) {
            __extends(TreeMapCollection, _super);
            function TreeMapCollection() {
                var _this = _super.apply(this, arguments) || this;
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                _this.event_dispatcher_ = new samchon.library.EventDispatcher(_this);
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /* =========================================================
                ELEMENTS I/O
                    - HANDLE_INSERT & HANDLE_ERASE
            ============================================================
                HANDLE_INSERT & HANDLE_ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            TreeMapCollection.prototype._Handle_insert = function (first, last) {
                _super.prototype._Handle_insert.call(this, first, last);
                collections.ICollection._Dispatch_MapCollectionEvent(this, "insert", first, last);
            };
            /**
             * @inheritdoc
             */
            TreeMapCollection.prototype._Handle_erase = function (first, last) {
                _super.prototype._Handle_erase.call(this, first, last);
                collections.ICollection._Dispatch_MapCollectionEvent(this, "erase", first, last);
            };
            /* =========================================================
                EVENT_DISPATCHER
                    - ACCESSORS
                    - ADD
                    - REMOVE
            ============================================================
                ACCESSORS
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            TreeMapCollection.prototype.hasEventListener = function (type) {
                return this.event_dispatcher_.hasEventListener(type);
            };
            /**
             * @inheritdoc
             */
            TreeMapCollection.prototype.dispatchEvent = function (event) {
                return this.event_dispatcher_.dispatchEvent(event);
            };
            TreeMapCollection.prototype.refresh = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var first;
                var last;
                if (args.length == 0) {
                    first = this.begin();
                    last = this.end();
                }
                else if (args.length == 1) {
                    first = args[0];
                    last = first.next();
                }
                else {
                    first = args[0];
                    last = args[1];
                }
                this.dispatchEvent(new collections.MapCollectionEvent("refresh", first, last));
            };
            TreeMapCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            TreeMapCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return TreeMapCollection;
        }(std.TreeMap));
        collections.TreeMapCollection = TreeMapCollection;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * A {@link TreeMultiMap} who can detect element I/O events.
         *
         * Below is the list of methods who are dispatching {@link MapCollectionEvent}:
         * - *insert* typed events:
         *   - {@link assign}
         *   - {@link insert}
         *   - {@link emplace}
         *   - {@link push}
         * - *erase* typed events:
         *   - {@link assign}
         *   - {@link clear}
         *   - {@link erase}
         * - *refresh* typed events:
         *   - {@link refresh}
         *
         * #### [Inherited] {@link TreeMultiMap}
         * {@link TreeMultiMap TreeMultiMaps} are associative containers that store elements formed by a combination of a
         * *key value* and a *mapped value*, following a specific order, and where multiple elements can have equivalent
         * keys.
         *
         * In a {@link TreeMultiMap}, the *key values* are generally used to sort and uniquely identify the elements,
         * while the *mapped values* store the content associated to this *key*. The types of *key* and *mapped value* may
         * differ, and are grouped together in member type ```value_type```, which is a {@link Pair} type combining both:
         *
         * ```typedef Pair<const Key, T> value_type;```
         *
         * Internally, the elements in a {@link TreeMultiMap}are always sorted by its key following a strict weak ordering
         * criterion indicated by its internal comparison method (of {@link less}).
         *
         * {@link TreeMultiMap}containers are generally slower than {@link HashMap} containers to access individual
         * elements by their *key*, but they allow the direct iteration on subsets based on their order.
         *
         * {@link TreeMultiMap TreeMultiMaps} are typically implemented as binary search trees.
         *
         * <a href="http://samchon.github.io/typescript-stl/images/design/class_diagram/map_containers.png" target="_blank"> <
         * img src="http://samchon.github.io/typescript-stl/images/design/class_diagram/map_containers.png" style="max-width: 100%" />
         * </a>
         *
         * <h3> Container properties </h3>
         * <dl>
         *	<dt> Associative </dt>
         *	<dd>
         *		Elements in associative containers are referenced by their *key* and not by their absolute
         *		position in the container.
         *	</dd>
         *
         *	<dt> Ordered </dt>
         *	<dd>
         *		The elements in the container follow a strict order at all times. All inserted elements are
         *		given a position in this order.
         *	</dd>
         *
         *	<dt> Map </dt>
         *	<dd>
         *		Each element associates a *key* to a *mapped value*:
         *		*Keys* are meant to identify the elements whose main content is the *mapped value*.
         *	</dd>
         *
         *	<dt> Multiple equivalent keys </dt>
         *	<dd> Multiple elements in the container can have equivalent *keys*. </dd>
         * </dl>
         *
         * @param <Key> Type of the keys. Each element in a map is uniquely identified by its key value.
         * @param <T> Type of the mapped value. Each element in a map stores some data as its mapped value.
         *
         * @reference http://www.cplusplus.com/reference/map/multimap
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var TreeMultiMapCollection = (function (_super) {
            __extends(TreeMultiMapCollection, _super);
            function TreeMultiMapCollection() {
                var _this = _super.apply(this, arguments) || this;
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                _this.event_dispatcher_ = new samchon.library.EventDispatcher(_this);
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /* =========================================================
                ELEMENTS I/O
                    - HANDLE_INSERT & HANDLE_ERASE
            ============================================================
                HANDLE_INSERT & HANDLE_ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            TreeMultiMapCollection.prototype._Handle_insert = function (first, last) {
                _super.prototype._Handle_insert.call(this, first, last);
                collections.ICollection._Dispatch_MapCollectionEvent(this, "insert", first, last);
            };
            /**
             * @inheritdoc
             */
            TreeMultiMapCollection.prototype._Handle_erase = function (first, last) {
                _super.prototype._Handle_erase.call(this, first, last);
                collections.ICollection._Dispatch_MapCollectionEvent(this, "erase", first, last);
            };
            /* =========================================================
                EVENT_DISPATCHER
                    - ACCESSORS
                    - ADD
                    - REMOVE
            ============================================================
                ACCESSORS
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            TreeMultiMapCollection.prototype.hasEventListener = function (type) {
                return this.event_dispatcher_.hasEventListener(type);
            };
            /**
             * @inheritdoc
             */
            TreeMultiMapCollection.prototype.dispatchEvent = function (event) {
                return this.event_dispatcher_.dispatchEvent(event);
            };
            TreeMultiMapCollection.prototype.refresh = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var first;
                var last;
                if (args.length == 0) {
                    first = this.begin();
                    last = this.end();
                }
                else if (args.length == 1) {
                    first = args[0];
                    last = first.next();
                }
                else {
                    first = args[0];
                    last = args[1];
                }
                this.dispatchEvent(new collections.MapCollectionEvent("refresh", first, last));
            };
            TreeMultiMapCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            TreeMultiMapCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return TreeMultiMapCollection;
        }(std.TreeMultiMap));
        collections.TreeMultiMapCollection = TreeMultiMapCollection;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * A {@link TreeMultiSet} who can detect element I/O events.
         *
         * Below is the list of methods who are dispatching {@link CollectionEvent}:
         * - *insert* typed events:
         *   - {@link assign}
         *   - {@link insert}
         *   - {@link push}
         * - *erase* typed events:
         *   - {@link assign}
         *   - {@link clear}
         *   - {@link erase}
         * - *refresh* typed events:
         *   - {@link refresh}
         *
         * #### [Inherited] {@link TreeMultiSet}
         * {@link TreeMultiSet TreeMultiSets} are containers that store elements following a specific order, and where
         * multiple elements can have equivalent values.
         *
         * In a {@link TreeMultiSet}, the value of an element also identifies it (the value is itself the *key*, of type
         * *T*). The value of the elements in a {@link TreeMultiSet} cannot be modified once in the container (the
         * elements are always const), but they can be inserted or removed from the container.
         *
         * Internally, the elements in a {@link TreeMultiSet TreeMultiSets} are always sorted following a strict weak
         * ordering criterion indicated by its internal comparison method (of {@link IComparable.less less}).
         *
         * {@link TreeMultiSet} containers are generally slower than {@link HashMultiSet} containers to access individual
         * elements by their *key*, but they allow the direct iteration on subsets based on their order.
         *
         * <p> {@link TreeMultiSet TreeMultiSets} are typically implemented as binary search trees. </p>
         *
         * <p> <a href="http://samchon.github.io/typescript-stl/images/design/class_diagram/set_containers.png" target="_blank">
         * <img src="http://samchon.github.io/typescript-stl/images/design/class_diagram/set_containers.png" style="max-width: 100%" /> </a></p>
         *
         * <h3> Container properties </h3>
         * <dl>
         *	<dt> Associative </dt>
         *	<dd>
         *		Elements in associative containers are referenced by their *key* and not by their absolute
         *		position in the container.
         *	</dd>
         *
         *	<dt> Ordered </dt>
         *	<dd>
         *		The elements in the container follow a strict order at all times. All inserted elements are
         *		given a position in this order.
         *	</dd>
         *
         *	<dt> Set </dt>
         *	<dd> The value of an element is also the *key* used to identify it. </dd>
         *
         *	<dt> Multiple equivalent keys </dt>
         *	<dd> Multiple elements in the container can have equivalent *keys*. </dd>
         * </dl>
         *
         * @param <T> Type of the elements. Each element in a {@link TreeMultiSet} container is also identified
         *			  by this value (each value is itself also the element's *key*).
         *
         * @reference http://www.cplusplus.com/reference/set/multiset
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var TreeMultiSetCollection = (function (_super) {
            __extends(TreeMultiSetCollection, _super);
            function TreeMultiSetCollection() {
                var _this = _super.apply(this, arguments) || this;
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                _this.event_dispatcher_ = new samchon.library.EventDispatcher(_this);
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /* =========================================================
                ELEMENTS I/O
                    - HANDLE_INSERT & HANDLE_ERASE
            ============================================================
                HANDLE_INSERT & HANDLE_ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            TreeMultiSetCollection.prototype._Handle_insert = function (first, last) {
                _super.prototype._Handle_insert.call(this, first, last);
                collections.ICollection._Dispatch_CollectionEvent(this, "insert", first, last);
            };
            /**
             * @inheritdoc
             */
            TreeMultiSetCollection.prototype._Handle_erase = function (first, last) {
                _super.prototype._Handle_erase.call(this, first, last);
                collections.ICollection._Dispatch_CollectionEvent(this, "erase", first, last);
            };
            /* =========================================================
                EVENT_DISPATCHER
                    - ACCESSORS
                    - ADD
                    - REMOVE
            ============================================================
                ACCESSORS
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            TreeMultiSetCollection.prototype.hasEventListener = function (type) {
                return this.event_dispatcher_.hasEventListener(type);
            };
            /**
             * @inheritdoc
             */
            TreeMultiSetCollection.prototype.dispatchEvent = function (event) {
                return this.event_dispatcher_.dispatchEvent(event);
            };
            TreeMultiSetCollection.prototype.refresh = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var first;
                var last;
                if (args.length == 0) {
                    first = this.begin();
                    last = this.end();
                }
                else if (args.length == 1) {
                    first = args[0];
                    last = first.next();
                }
                else {
                    first = args[0];
                    last = args[1];
                }
                this.dispatchEvent(new collections.CollectionEvent("refresh", first, last));
            };
            TreeMultiSetCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            TreeMultiSetCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return TreeMultiSetCollection;
        }(std.TreeMultiSet));
        collections.TreeMultiSetCollection = TreeMultiSetCollection;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collections;
    (function (collections) {
        /**
         * A {@link TreeMap} who can detect element I/O events.
         *
         * Below is the list of methods who are dispatching {@link CollectionEvent}:
         * - *insert* typed events:
         *   - {@link assign}
         *   - {@link insert}
         *   - {@link insert_or_assign}
         *   - {@link push}
         * - *erase* typed events:
         *   - {@link assign}
         *   - {@link clear}
         *   - {@link erase}
         *   - {@link extract}
         * - *refresh* typed events:
         *   - {@link refresh}
         *
         * #### [Inherited] {@link TreeSet}
         * {@link TreeSet TreeSets} are containers that store unique elements following a specific order.
         *
         * In a {@link TreeSet}, the value of an element also identifies it (the value is itself the *key*, of type *T*),
         * and each value must be unique. The value of the elements in a {@link TreeSet} cannot be modified once in the
         * container (the elements are always const), but they can be inserted or removed from the container.
         *
         * Internally, the elements in a {@link TreeSet} are always sorted following a specific strict weak ordering
         * criterion indicated by its internal comparison method (of {@link less}).
         *
         * {@link TreeSet} containers are generally slower than {@link HashSet} containers to access individual elements
         * by their *key*, but they allow the direct iteration on subsets based on their order.
         *
         * {@link TreeSet}s are typically implemented as binary search trees.
         *
         * <a href="http://samchon.github.io/typescript-stl/images/design/class_diagram/set_containers.png" target="_blank">
         * <img src="http://samchon.github.io/typescript-stl/images/design/class_diagram/set_containers.png" style="max-width: 100%" />
         * </a>
         *
         * <h3> Container properties </h3>
         * <dl>
         *	<dt> Associative </dt>
         *	<dd>
         *		Elements in associative containers are referenced by their *key* and not by their absolute
         *		position in the container.
         *	</dd>
         *
         *	<dt> Ordered </dt>
         *	<dd>
         *		The elements in the container follow a strict order at all times. All inserted elements are
         *		given a position in this order.
         *	</dd>
         *
         *	<dt> Set </dt>
         *	<dd> The value of an element is also the *key* used to identify it. </dd>
         *
         *	<dt> Unique keys </dt>
         *	<dd> No two elements in the container can have equivalent *keys*. </dd>
         * </dl>
         *
         * @param <T> Type of the elements.
         *			  Each element in an {@link TreeSet} is also uniquely identified by this value.
         *
         * @reference http://www.cplusplus.com/reference/set/set
         * @handbook [Collections](https://github.com/samchon/framework/wiki/TypeScript-STL#collections)
         * @author Jeongho Nam <http://samchon.org>
         */
        var TreeSetCollection = (function (_super) {
            __extends(TreeSetCollection, _super);
            function TreeSetCollection() {
                var _this = _super.apply(this, arguments) || this;
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                _this.event_dispatcher_ = new samchon.library.EventDispatcher(_this);
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /* =========================================================
                ELEMENTS I/O
                    - HANDLE_INSERT & HANDLE_ERASE
            ============================================================
                HANDLE_INSERT & HANDLE_ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            TreeSetCollection.prototype._Handle_insert = function (first, last) {
                _super.prototype._Handle_insert.call(this, first, last);
                collections.ICollection._Dispatch_CollectionEvent(this, "insert", first, last);
            };
            /**
             * @inheritdoc
             */
            TreeSetCollection.prototype._Handle_erase = function (first, last) {
                _super.prototype._Handle_erase.call(this, first, last);
                collections.ICollection._Dispatch_CollectionEvent(this, "erase", first, last);
            };
            /* =========================================================
                EVENT_DISPATCHER
                    - ACCESSORS
                    - ADD
                    - REMOVE
            ============================================================
                ACCESSORS
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            TreeSetCollection.prototype.hasEventListener = function (type) {
                return this.event_dispatcher_.hasEventListener(type);
            };
            /**
             * @inheritdoc
             */
            TreeSetCollection.prototype.dispatchEvent = function (event) {
                return this.event_dispatcher_.dispatchEvent(event);
            };
            TreeSetCollection.prototype.refresh = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var first;
                var last;
                if (args.length == 0) {
                    first = this.begin();
                    last = this.end();
                }
                else if (args.length == 1) {
                    first = args[0];
                    last = first.next();
                }
                else {
                    first = args[0];
                    last = args[1];
                }
                this.dispatchEvent(new collections.CollectionEvent("refresh", first, last));
            };
            TreeSetCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            TreeSetCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return TreeSetCollection;
        }(std.TreeSet));
        collections.TreeSetCollection = TreeSetCollection;
    })(collections = samchon.collections || (samchon.collections = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var library;
    (function (library) {
        /**
         * Case generator.
         *
         * {@link CaseGenerator} is an abstract case generator being used like a matrix.
         * <ul>
         *  <li> n��r(n^r) -> {@link CombinedPermutationGenerator} </li>
         *  <li> nPr -> {@link PermutationGenerator} </li>
         *  <li> n! -> {@link FactorialGenerator} </li>
         * </ul>
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var CaseGenerator = (function () {
            /* ---------------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------------- */
            /**
             * Construct from size of N and R.
             *
             * @param n Size of candidates.
             * @param r Size of elements of each case.
             */
            function CaseGenerator(n, r) {
                this.n_ = n;
                this.r_ = r;
            }
            /* ---------------------------------------------------------------
                ACCESSORS
            --------------------------------------------------------------- */
            /**
             * Get size of all cases.
             *
             * @return Get a number of the all cases.
             */
            CaseGenerator.prototype.size = function () {
                return this.size_;
            };
            /**
             * Get size of the N.
             */
            CaseGenerator.prototype.n = function () {
                return this.n_;
            };
            /**
             * Get size of the R.
             */
            CaseGenerator.prototype.r = function () {
                return this.r_;
            };
            return CaseGenerator;
        }());
        library.CaseGenerator = CaseGenerator;
        /**
         * A combined-permutation case generator.
         *
         * <sub>n</sub>��<sub>r</sub>
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var CombinedPermutationGenerator = (function (_super) {
            __extends(CombinedPermutationGenerator, _super);
            /* ---------------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------------- */
            /**
             * Construct from size of N and R.
             *
             * @param n Size of candidates.
             * @param r Size of elements of each case.
             */
            function CombinedPermutationGenerator(n, r) {
                var _this = _super.call(this, n, r) || this;
                _this.size_ = Math.pow(n, r);
                _this.divider_array = new Array();
                for (var i = 0; i < r; i++) {
                    var x = r - (i + 1);
                    var val = Math.pow(n, x);
                    _this.divider_array.push(val);
                }
                return _this;
            }
            CombinedPermutationGenerator.prototype.at = function (index) {
                var row = [];
                for (var i = 0; i < this.r_; i++) {
                    var val = Math.floor(index / this.divider_array[i]) % this.n_;
                    row.push(val);
                }
                return row;
            };
            return CombinedPermutationGenerator;
        }(CaseGenerator));
        library.CombinedPermutationGenerator = CombinedPermutationGenerator;
        /**
         * A permutation case generator.
         *
         * <sub>n</sub>P<sub>r</sub>
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var PermuationGenerator = (function (_super) {
            __extends(PermuationGenerator, _super);
            /* ---------------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------------- */
            /**
             * Construct from size of N and R.
             *
             * @param n Size of candidates.
             * @param r Size of elements of each case.
             */
            function PermuationGenerator(n, r) {
                var _this = _super.call(this, n, r) || this;
                _this.size_ = n;
                for (var i = n - 1; i > n - r; i--)
                    _this.size_ *= i;
                return _this;
            }
            /**
             * @inheritdoc
             */
            PermuationGenerator.prototype.at = function (index) {
                var atoms = [];
                for (var i = 0; i < this.n_; i++)
                    atoms.push(i);
                var row = [];
                for (var i = 0; i < this.r_; i++) {
                    var item = index % atoms.length;
                    index = Math.floor(index / atoms.length);
                    row.push(atoms[item]);
                    atoms.splice(item, 1);
                }
                return row;
            };
            return PermuationGenerator;
        }(CaseGenerator));
        library.PermuationGenerator = PermuationGenerator;
        /**
         * Factorial case generator.
         *
         * n! = <sub>n</sub>P<sub>n</sub>
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var FactorialGenerator = (function (_super) {
            __extends(FactorialGenerator, _super);
            /**
             * Construct from factorial size N.
             *
             * @param n Factoria size N.
             */
            function FactorialGenerator(n) {
                return _super.call(this, n, n) || this;
            }
            return FactorialGenerator;
        }(PermuationGenerator));
        library.FactorialGenerator = FactorialGenerator;
    })(library = samchon.library || (samchon.library = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var library;
    (function (library) {
        /**
         * The {@link EventDispatcher} class is the base class for all classes that dispatch events. The
         * {@link EventDispatcher} class implements the {@link IEventDispatcher} interface and is the base class for the
         * {@link DisplayObject} class. The {@link EventDispatcher} class allows any object on the display list to be an
         * event target and as such, to use the methods of the {@link IEventDispatcher} interface.
         *
         * The event target serves as the local point for how events flow through the display list hierarchy. When an
         * event such as a mouse click or a key press occurs, an event object is dispatched into the event flow from the
         * root of the display list. The event object makes a round-trip journey to the event target, which is
         * conceptually divided into three phases: the capture phase includes the journey from the root to the last node
         * before the event target's node; the target phase includes only the event target node; and the bubbling phase
         * includes any subsequent nodes encountered on the return trip to the root of the display list.
         *
         * In general, the easiest way for a user-defined class to gain event dispatching capabilities is to extend
         * {@link EventDispatcher}. If this is impossible (that is, if the class is already extending another class), you
         * can instead implement the {@link IEventDispatcher} interface, create an {@link EventDispatcher} member, and
         * write simple hooks to route calls into the aggregated {@link EventDispatcher}.
         *
         * @reference http://help.adobe.com/en_US/FlashPlatform/reference/actionscript/3/flash/events/EventDispatcher.html
         * @author Migrated by Jeongho Nam <http://samchon.org>
         */
        var EventDispatcher = (function () {
            function EventDispatcher(dispatcher) {
                if (dispatcher === void 0) { dispatcher = null; }
                if (dispatcher == null)
                    this.event_dispatcher_ = this;
                else
                    this.event_dispatcher_ = dispatcher;
                this.event_listeners_ = new std.HashMap();
            }
            /**
             * @inheritdoc
             */
            EventDispatcher.prototype.hasEventListener = function (type) {
                type = type.toLowerCase();
                return this.event_listeners_.has(type);
            };
            /**
             * @inheritdoc
             */
            EventDispatcher.prototype.dispatchEvent = function (event) {
                event.target = this.event_dispatcher_;
                if (this.event_listeners_.has(event.type) == false)
                    return false;
                var listenerSet = this.event_listeners_.get(event.type);
                for (var it = listenerSet.begin(); it.equals(listenerSet.end()) == false; it = it.next()) {
                    if (event.defaultPrevented == true)
                        continue;
                    it.value.first.apply(it.value.second, [event]);
                }
                return true;
            };
            EventDispatcher.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                type = type.toLowerCase();
                var listenerSet;
                if (this.event_listeners_.has(type) == false) {
                    listenerSet = new std.HashSet();
                    this.event_listeners_.set(type, listenerSet);
                }
                else
                    listenerSet = this.event_listeners_.get(type);
                listenerSet.insert(new std.Pair(listener, thisArg));
            };
            EventDispatcher.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                type = type.toLowerCase();
                if (this.event_listeners_.has(type) == false)
                    return;
                var listenerSet = this.event_listeners_.get(type);
                var bind = new std.Pair(listener, thisArg);
                if (listenerSet.has(bind) == false)
                    return;
                listenerSet.erase(bind);
                if (listenerSet.empty() == true)
                    this.event_listeners_.erase(type);
            };
            return EventDispatcher;
        }());
        library.EventDispatcher = EventDispatcher;
    })(library = samchon.library || (samchon.library = {}));
})(samchon || (samchon = {}));
var samchon;
(function (samchon) {
    var library;
    (function (library) {
        /**
         * The {@link FileReference} class provides a means to load and save files in browser level.
         *
         * The {@link FileReference} class provides a means to {@link load} and {@link save} files in browser level. A
         * browser-system dialog box prompts the user to select a file to {@link load} or a location for {@link svae}. Each
         * {@link FileReference} object refers to a single file on the user's disk and has properties that contain
         * information about the file's size, type, name, creation date, modification date, and creator type (Macintosh only).
         *
         *
         * FileReference instances are created in the following ways:
         * <ul>
         *	<li>
         *		When you use the new operator with the {@link FileReference} constructor:
         *		<code>let myFileReference: FileReference = new FileReference();</code>
         *	</li>
         *	<li>
         *		When you call the {@link FileReferenceList.browse} method, which creates an array of {@link FileReference}
         *		objects.
         *	</li>
         * </ul>
         *
         * During a load operation, all the properties of a {@link FileReference} object are populated by calls to the
         * {@link FileReference.browse} or {@link FileReferenceList.browse} methods. During a save operation, the name
         * property is populated when the select event is dispatched; all other properties are populated when the complete
         * event is dispatched.
         *
         * The {@link browse browse()} method opens an browser-system dialog box that prompts the user to select a file
         * for {@link load}. The {@link FileReference.browse} method lets the user select a single file; the
         * {@link FileReferenceList.browse} method lets the user select multiple files. After a successful call to the
         * {@link browse browse()} method, call the {@link FileReference.load} method to load one file at a time. The
         * {@link FileReference.save} method prompts the user for a location to save the file and initiates downloading from
         * a binary or string data.
         *
         * The {@link FileReference} and {@link FileReferenceList} classes do not let you set the default file location
         * for the dialog box that the {@link browse} or {@link save} methods generate. The default location shown in the
         * dialog box is the most recently browsed folder, if that location can be determined, or the desktop. The classes do
         * not allow you to read from or write to the transferred file. They do not allow the browser that initiated the
         * {@link load} or {@link save} to access the loaded or saved file or the file's location on the user's disk.
         *
         * @references http://help.adobe.com/en_US/FlashPlatform/reference/actionscript/3/flash/net/FileReference.html
         * @author Jeongho Nam <http://samchon.org>
         */
        var FileReference = (function (_super) {
            __extends(FileReference, _super);
            /**
             * Default Constructor.
             */
            function FileReference() {
                var _this = _super.call(this) || this;
                _this.file_ = null;
                _this.data_ = null;
                return _this;
            }
            Object.defineProperty(FileReference.prototype, "data", {
                /* ---------------------------------------------------------
                    ACCESSORS
                --------------------------------------------------------- */
                /**
                 * The data from the loaded file after a successful call to the {@link load load()} method.
                 *
                 * If the {@link FileReference} object was not populated (by a valid call to {@link FileReference.browse}),
                 * an {@link LogicError exception} will be thrown when you try to get the value of this property.
                 *
                 * All the properties of a {@link FileReference} object are populated by calling the {@link browse browse()}.
                 *
                 */
                get: function () {
                    return this.data_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(FileReference.prototype, "name", {
                /**
                 * The name of the file on the local disk.
                 *
                 * If the {@link FileReference} object was not populated (by a valid call to {@link FileReference.browse}),
                 * an {@link LogicError exception} will be thrown when you try to get the value of this property.
                 *
                 * All the properties of a {@link FileReference} object are populated by calling the {@link browse browse()}.
                 *
                 */
                get: function () {
                    var index = this.file_.name.lastIndexOf(".");
                    if (index == -1)
                        return this.file_.name;
                    else
                        return this.file_.name.substr(0, index);
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(FileReference.prototype, "extension", {
                /**
                 * The filename extension.
                 *
                 * A file's extension is the part of the name following (and not including) the final dot (&quot;.&quot;). If
                 * there is no dot in the filename, the extension is <code>null</code>.
                 *
                 * If the {@link FileReference} object was not populated (by a valid call to {@link FileReference.browse}),
                 * an {@link LogicError exception} will be thrown when you try to get the value of this property.
                 *
                 * All the properties of a {@link FileReference} object are populated by calling the {@link browse browse()}.
                 *
                 */
                get: function () {
                    var index = this.file_.name.lastIndexOf(".");
                    if (index == -1)
                        return null;
                    else
                        return this.file_.name.substr(index + 1);
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(FileReference.prototype, "type", {
                /**
                 * The file type, metadata of the {@link extension}.
                 *
                 * If the {@link FileReference} object was not populated (by a valid call to {@link FileReference.browse}),
                 * an {@link LogicError exception} will be thrown when you try to get the value of this property.
                 *
                 * All the properties of a {@link FileReference} object are populated by calling the {@link browse browse()}.
                 *
                 */
                get: function () {
                    return this.file_.type;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(FileReference.prototype, "size", {
                /**
                 * The size of the file on the local disk in bytes.
                 *
                 * If the {@link FileReference} object was not populated (by a valid call to {@link FileReference.browse}),
                 * an {@link LogicError exception} will be thrown when you try to get the value of this property.
                 *
                 * All the properties of a {@link FileReference} object are populated by calling the {@link browse browse()}.
                 *
                 */
                get: function () {
                    return this.file_.size;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(FileReference.prototype, "modificationDate", {
                /**
                 * The date that the file on the local disk was last modified.
                 *
                 * If the {@link FileReference} object was not populated (by a valid call to {@link FileReference.browse}),
                 * an {@link LogicError exception} will be thrown when you try to get the value of this property.
                 *
                 * All the properties of a {@link FileReference} object are populated by calling the {@link browse browse()}.
                 *
                 */
                get: function () {
                    return this.file_.lastModifiedDate;
                },
                enumerable: true,
                configurable: true
            });
            /* =========================================================
                PROCEDURES
                    - OPEN FILE
                    - SAVE FILE
            ============================================================
                OPEN FILE
            --------------------------------------------------------- */
            /**
             * Displays a file-browsing dialog box that lets the user select a file to upload. The dialog box is native
             * to the user's browser system. The user can select a file on the local computer or from other systems, for
             * example, through a UNC path on Windows.
             *
             * When you call this method and the user successfully selects a file, the properties of this
             * {@link FileReference} object are populated with the properties of that file. Each subsequent time that the
             * {@link FileReference.browse} method is called, the {@link FileReference} object's properties are reset to
             * the file that the user selects in the dialog box. Only one {@link browse browse()} can be performed at a time
             * (because only one dialog box can be invoked at a time).
             *
             * Using the *typeFilter parameter*, you can determine which files the dialog box displays.
             *
             * @param typeFilter An array of filter strings used to filter the files that are displayed in the dialog box.
             *					 If you omit this parameter, all files are displayed.
             */
            FileReference.prototype.browse = function () {
                var typeFilter = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    typeFilter[_i] = arguments[_i];
                }
                var this_ = this;
                // CREATE A FILE SELECTOR
                var input = document.createElement("input");
                input.type = "file";
                if (typeFilter.length > 0)
                    input.accept = typeFilter.toString();
                // EVENT HANDLER
                input.onchange = function (event) {
                    this_.file_ = input.files[0];
                    this_.dispatchEvent(new library.BasicEvent("select"));
                };
                // APEND TO BODY TEMPORARILY
                input.onclick = function (event) {
                    document.body.removeChild(event.target);
                };
                input.style.display = "none";
                document.body.appendChild(input);
                // CLICK DIRECTLY
                input.click();
            };
            /**
             * Starts the load of a local file selected by a user.
             *
             * You must call the {@link FileReference.browse} or {@link FileReferenceList.browse} method before you call
             * the {@link load load()} method.
             *
             * Listeners receive events to indicate the progress, success, or failure of the load. Although you can use
             * the {@link FileReferenceList} object to let users select multiple files to load, you must {@link load} the
             * {@link FileReferenceList files} one by one. To {@link load} the files one by one, iterate through the
             * {@link FileReferenceList.fileList} array of {@link FileReference} objects.
             *
             * If the file finishes loading successfully, its contents are stored in the {@link data} property.
             */
            FileReference.prototype.load = function () {
                var this_ = this;
                var reader = new FileReader();
                reader.onload = function (event) {
                    this_.data_ = reader.result;
                    this_.dispatchEvent(new library.BasicEvent("complete"));
                };
                reader.readAsText(this.file_);
            };
            /* ---------------------------------------------------------
                SAVE FILE
            --------------------------------------------------------- */
            /**
             * Save a file to local filesystem.
             *
             * {@link FileReference.save} implemented the save function by downloading a file from a hidden anchor tag.
             * However, the plan, future's {@link FileReference} will follow such rule:
             *
             * Opens a dialog box that lets the user save a file to the local filesystem.
             *
             * The {@link save save()} method first opens an browser-system dialog box that asks the user to enter a
             * filename and select a location on the local computer to save the file. When the user selects a location and
             * confirms the save operation (for example, by clicking Save), the save process begins. Listeners receive events
             * to indicate the progress, success, or failure of the save operation. To ascertain the status of the dialog box
             * and the save operation after calling {@link save save()}, your code must listen for events such as cancel,
             * open, progress, and complete.
             *
             * When the file is saved successfully, the properties of the {@link FileReference} object are populated with
             * the properties of the local file. The complete event is dispatched if the save is successful.
             *
             * Only one {@link browse browse()} or {@link save()} session can be performed at a time (because only one
             * dialog box can be invoked at a time).
             *
             * @param data The data to be saved. The data can be in one of several formats, and will be treated appropriately.
             * @param fileName File name to be saved.
             */
            FileReference.prototype.save = function (data, fileName) {
                FileReference.save(data, fileName);
            };
            /**
             * Save a file to local filesystem.
             *
             * {@link FileReference.save} implemented the save function by downloading a file from a hidden anchor tag.
             * However, the plan, future's {@link FileReference} will follow such rule:
             *
             * Opens a dialog box that lets the user save a file to the local filesystem.
             *
             * The {@link save save()} method first opens an browser-system dialog box that asks the user to enter a
             * filename and select a location on the local computer to save the file. When the user selects a location and
             * confirms the save operation (for example, by clicking Save), the save process begins. Listeners receive events
             * to indicate the progress, success, or failure of the save operation. To ascertain the status of the dialog box
             * and the save operation after calling {@link save save()}, your code must listen for events such as cancel,
             * open, progress, and complete.
             *
             * When the file is saved successfully, the properties of the {@link FileReference} object are populated with
             * the properties of the local file. The complete event is dispatched if the save is successful.
             *
             * Only one {@link browse browse()} or {@link save()} session can be performed at a time (because only one
             * dialog box can be invoked at a time).
             *
             * @param data The data to be saved. The data can be in one of several formats, and will be treated appropriately.
             * @param fileName File name to be saved.
             */
            FileReference.save = function (data, fileName) {
                var blob = new Blob([data], { type: "text/plain" });
                if (window.navigator.msSaveBlob != undefined) {
                    // IE ONLY
                    window.navigator.msSaveBlob(blob, fileName);
                }
                else {
                    // CREATE AN ANCHOR
                    var anchor = document.createElement("a");
                    anchor.download = fileName;
                    anchor.innerHTML = "";
                    // LINK TO THE BLOB
                    anchor.href = window.URL.createObjectURL(blob);
                    // APEND TO BODY TEMPORARILY
                    anchor.onclick = function (event) {
                        // CLICKS AND REMOVES IT DIRECTLY
                        document.body.removeChild(event.target);
                    };
                    anchor.style.display = "none";
                    document.body.appendChild(anchor);
                    // CLICK DIRECTLY
                    anchor.click();
                }
            };
            return FileReference;
        }(library.EventDispatcher));
        library.FileReference = FileReference;
        /**
         * The {@link FileReferenceList} class provides a means to let users select one or more files for
         * {@link FileReference.load loading}. A {@link FileReferenceList} object represents a group of one or more local
         * files on the user's disk as an array of {@link FileReference} objects. For detailed information and important
         * considerations about {@link FileReference} objects and the FileReference class, which you use with
         * {@link FileReferenceList}, see the {@link FileReference} class.
         *
         * To work with the {@link FileReferenceList} class:
         * <ul>
         *	<li> Instantiate the class: <code>var myFileRef = new FileReferenceList();</code> </li>
         *	<li>
         *		Call the {@link FileReferenceList.browse} method, which opens a dialog box that lets the user select one or
         *		more files for upload: <code>myFileRef.browse();</code>
         *	</li>
         *	<li>
         *		After the {@link browse browse()} method is called successfully, the {@link fileList} property of the
         *		{@link FileReferenceList} object is populated with an array of {@link FileReference} objects.
         *	</li>
         *	<li> Call {@link FileReference.load} on each element in the {@link fileList} array. </li>
         * </ul>
         *
         * The {@link FileReferenceList} class includes a {@link browse browse()} method and a {@link fileList} property
         * for working with multiple files.
         *
         * @reference http://help.adobe.com/en_US/FlashPlatform/reference/actionscript/3/flash/net/FileReferenceList.html
         * @author Jeongho Nam <http://samchon.org>
         */
        var FileReferenceList = (function (_super) {
            __extends(FileReferenceList, _super);
            /**
             * Default Constructor.
             */
            function FileReferenceList() {
                var _this = _super.call(this) || this;
                _this.file_list = new std.Vector();
                return _this;
            }
            Object.defineProperty(FileReferenceList.prototype, "fileList", {
                /**
                 * An array of {@link FileReference} objects.
                 *
                 * When the {@link FileReferenceList.browse} method is called and the user has selected one or more files
                 * from the dialog box that the {@link browse browse()} method opens, this property is populated with an array of
                 * {@link FileReference} objects, each of which represents the files the user selected.
                 *
                 * The {@link fileList} property is populated anew each time {@link browse browse()} is called on that
                 * {@link FileReferenceList} object.
                 */
                get: function () {
                    return this.file_list;
                },
                enumerable: true,
                configurable: true
            });
            /**
             * Displays a file-browsing dialog box that lets the user select one or more local files to upload. The
             * dialog box is native to the user's browser system.
             *
             * When you call this method and the user successfully selects files, the {@link fileList} property of this
             * {@link FileReferenceList} object is populated with an array of {@link FileReference} objects, one for each
             * file that the user selects. Each subsequent time that the {@link FileReferenceList.browse} method is called,
             * the {@link FileReferenceList.fileList} property is reset to the file(s) that the user selects in the dialog
             * box.
             *
             * Using the *typeFilter* parameter, you can determine which files the dialog box displays.
             *
             * Only one {@link FileReference.browse}, {@link FileReference.load}, or {@link FileReferenceList.browse}
             * session can be performed at a time on a {@link FileReferenceList} object (because only one dialog box can be
             * opened at a time).
             *
             * @param typeFilter An array of filter strings used to filter the files that are displayed in the dialog box.
             *					 If you omit this parameter, all files are displayed.
             */
            FileReferenceList.prototype.browse = function () {
                var typeFilter = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    typeFilter[_i] = arguments[_i];
                }
                var this_ = this;
                // CREATE A FILE SELECTOR
                var input = document.createElement("input");
                input.type = "file";
                if (typeFilter.length > 0)
                    input.accept = typeFilter.toString();
                // EVENT HANDLER
                input.onchange = function (event) {
                    var fileList = input.files;
                    this_.file_list.clear();
                    for (var i = 0; i < fileList.length; i++) {
                        var reference = new FileReference();
                        reference.file_ = fileList[i];
                        this_.file_list.push(reference);
                    }
                    this_.dispatchEvent(new library.BasicEvent("select"));
                };
                // APEND TO BODY TEMPORARILY
                input.onclick = function (event) {
                    document.body.removeChild(event.target);
                };
                input.style.display = "none";
                document.body.appendChild(input);
                // CLICK DIRECTLY
                input.click();
            };
            return FileReferenceList;
        }(library.EventDispatcher));
        library.FileReferenceList = FileReferenceList;
    })(library = samchon.library || (samchon.library = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var library;
    (function (library) {
        /**
         * A genetic algorithm class.
         *
         * In the field of artificial intelligence, a genetic algorithm (GA) is a search heuristic that mimics the
         * process of natural selection. This heuristic (also sometimes called a metaheuristic) is routinely used to generate
         * useful solutions to optimization and search problems.
         *
         * Genetic algorithms belong to the larger class of evolutionary algorithms (EA), which generate solutions to
         * optimization problems using techniques inspired by natural evolution, such as inheritance, {@link mutate mutation},
         * {@link selection}, and {@link crossover}.
         *
         * @reference https://en.wikipedia.org/wiki/Genetic_algorithm
         * @author Jeongho Nam <http://samchon.org>
         */
        var GeneticAlgorithm = (function () {
            /* ---------------------------------------------------------
                CONSTRUCTORS AND MAIN PROCEDURES
            --------------------------------------------------------- */
            /**
             * Initialization Constructor.
             *
             * @param unique Whether each Gene is unique in their GeneArray.
             * @param mutation_rate Rate of mutation.
             * @param tournament Number of tournaments in selection.
             */
            function GeneticAlgorithm(unique, mutation_rate, tournament) {
                if (unique === void 0) { unique = true; }
                if (mutation_rate === void 0) { mutation_rate = .015; }
                if (tournament === void 0) { tournament = 10; }
                this.unique_ = unique;
                this.mutation_rate_ = mutation_rate;
                this.tournament_ = tournament;
            }
            /**
             * Evolove *GeneArray*.
             *
             * Convenient method accessing to {@link evolvePopulation evolvePopulation()}.
             *
             * @param individual An initial set of genes; sequence listing.
             * @param population Size of population in a generation.
             * @param generation Size of generation in evolution.
             * @param compare A comparison function returns whether left gene is more optimal.
             *
             * @return An evolved *GeneArray*, optimally.
             *
             * @see {@link GAPopulation.compare}
             */
            GeneticAlgorithm.prototype.evolveGeneArray = function (individual, population, generation, compare) {
                if (compare === void 0) { compare = std.greater; }
                var ga_population = new GAPopulation(individual, population);
                for (var i = 0; i < generation; i++)
                    ga_population = this.evolvePopulation(ga_population);
                return ga_population.fitTest();
            };
            /**
             * Evolve *population*, a mass of *GeneArraies*.
             *
             * @param population An initial population.
             * @param compare A comparison function returns whether left gene is more optimal.
             *
             * @return An evolved population.
             *
             * @see {@link GAPopulation.compare}
             */
            GeneticAlgorithm.prototype.evolvePopulation = function (population, compare) {
                if (compare === void 0) { compare = std.greater; }
                var size = population.children().size();
                var evolved = new GAPopulation(size);
                // ELITICISM
                evolved.children().set(0, population.fitTest());
                for (var i = 1; i < size; i++) {
                    var gene1 = this.selection(population);
                    var gene2 = this.selection(population);
                    var child = this.crossover(gene1, gene2);
                    this.mutate(child);
                    evolved.children().set(i, child);
                }
                return evolved;
            };
            /* ---------------------------------------------------------
                MAIN LOGICS
            --------------------------------------------------------- */
            /**
             * Select the best GeneArray in *population* from tournament.
             *
             * {@link selection Selection} is the stage of a genetic algorithm in which individual genomes are chosen
             * from a population for later breeding (using {@linlk crossover} operator). A generic {@link selection}
             * procedure may be implemented as follows:
             *
             * <ol>
             *	<li>
             *		The fitness function is evaluated for each individual, providing fitness values, which are then
             *		normalized. ization means dividing the fitness value of each individual by the sum of all fitness
             *		values, so that the sum of all resulting fitness values equals 1.
             *	</li>
             *	<li> The population is sorted by descending fitness values. </li>
             *	<li>
             *		Accumulated normalized fitness values are computed (the accumulated fitness value of an individual is the
             *		sum of its own fitness value plus the fitness values of all the previous individuals). The accumulated
             *		fitness of the last individual should be 1 (otherwise something went wrong in the normalization step).
             *	</li>
             *	<li> A random number R between 0 and 1 is chosen. </li>
             *	<li> The selected individual is the first one whose accumulated normalized value is greater than R. </li>
             * </ol>
             *
             * @param population The target of tournament.
             * @return The best genes derived by the tournament.
             *
             * @reference https://en.wikipedia.org/wiki/Selection_(genetic_algorithm)
             */
            GeneticAlgorithm.prototype.selection = function (population) {
                var size = population.children().size();
                var tournament = new GAPopulation(size);
                for (var i = 0; i < size; i++) {
                    var random_index = Math.floor(Math.random() * size);
                    if (random_index == size)
                        random_index--;
                    tournament.children().set(i, population.children().at(random_index));
                }
                return tournament.fitTest();
            };
            /**
             * Create a new GeneArray by crossing over two *GeneArray*(s).
             *
             * {@link crossover} is a genetic operator used to vary the programming of a chromosome or chromosomes from
             * one generation to the next. It is analogous to reproduction and biological crossover, upon which genetic
             * algorithms are based.
             *
             * {@link crossover Cross over} is a process of taking more than one parent solutions and producing a child
             * solution from them. There are methods for selection of the chromosomes.
             *
             * @param parent1 A parent sequence listing
             * @param parent2 A parent sequence listing
             *
             * @reference https://en.wikipedia.org/wiki/Crossover_(genetic_algorithm)
             */
            GeneticAlgorithm.prototype.crossover = function (parent1, parent2) {
                var individual = parent1.constructor(parent1);
                var size = parent1.size();
                if (this.unique_ == false) {
                    for (var i = 0; i < size; i++)
                        if (Math.random() > .5)
                            individual.set(i, parent1.at(i));
                }
                else {
                    var ptr_set = new std.HashSet();
                    var index_set = new std.HashSet();
                    // RANGES
                    var first = Math.random() * size;
                    var last = Math.random() * size;
                    if (first > last)
                        _a = [last, first], first = _a[0], last = _a[1];
                    // INDEXING
                    for (var i = 0; i < size; i++)
                        if (first <= i && i < last)
                            ptr_set.insert(parent1.at(i));
                        else
                            index_set.insert(i);
                    // INSERT PARENT_2
                    for (var i = 0; i < size; i++) {
                        var ptr = parent2.at(i);
                        if (ptr_set.find(ptr).equals(ptr_set.end()) == false)
                            continue;
                        individual.set(index_set.begin().value, ptr);
                        index_set.erase(index_set.begin());
                    }
                }
                return individual;
                var _a;
            };
            /**
             * Cause a mutation on the *GeneArray*.
             *
             * {@link mutate Mutation} is a genetic operator used to maintain genetic diversity from one generation of a
             * population of genetic algorithm chromosomes to the next. It is analogous to biological mutation.
             *
             * {@link mutate Mutation} alters one or more gene values in a chromosome from its initial state. In
             * {@link mutate mutation}, the solution may change entirely from the previous solution. Hence GA can come to
             * better solution by using {@link mutate mutation}.
             *
             * {@link mutate Mutation} occurs during evolution according to a user-definable mutation probability. This
             * probability should be set low. If it is set too high, the search will turn into a primitive random search.
             *
             * <h4> Note </h4>
             * Muttion is pursuing diversity. Mutation is useful for avoiding the following problem.
             *
             * When initial set of genes(GeneArray) is far away from optimail, without mutation (only with selection and
             * crossover), the genetic algorithm has a tend to wandering outside of the optimal.
             *
             * Genes in the GeneArray will be swapped following percentage of the {@link mutation_rate}.
             *
             * @param individual A container of genes to mutate
             *
             * @reference https://en.wikipedia.org/wiki/Mutation_(genetic_algorithm)
             * @see {@link mutation_rate}
             */
            GeneticAlgorithm.prototype.mutate = function (individual) {
                for (var it = individual.begin(); !it.equals(individual.end()); it = it.next()) {
                    if (Math.random() > this.mutation_rate_)
                        continue;
                    // JUST SHUFFLE SEQUENCE OF GENES
                    var j = Math.floor(Math.random() * individual.size());
                    it.swap(individual.begin().advance(j));
                }
            };
            return GeneticAlgorithm;
        }());
        library.GeneticAlgorithm = GeneticAlgorithm;
        /**
         * A population in a generation.
         *
         * {@link GAPopulation} is a class representing population of candidate genes (sequence listing) having an array
         * of GeneArray as a member. {@link GAPopulation} also manages initial set of genes and handles fitting test direclty
         * by the method {@link fitTest fitTest()}.
         *
         * The success of evolution of genetic algorithm is depend on the {@link GAPopulation}'s initial set and fitting
         * test. (*GeneArray* and {@link compare}.)
         *
         * <h4> Warning </h4>
         * Be careful for the mistakes of direction or position of the {@link compare}.
         * Most of logical errors failed to access optimal solution are occured from those mistakes.
         *
         * @param <T> Type of gene elements.
         * @param <GeneArray> An array containing genes as elments; sequnce listing.
         *
         * @author Jeongho Nam <http://samcho.org>
         */
        var GAPopulation = (function () {
            function GAPopulation() {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                if (args.length == 1) {
                    this.children_ = new std.Vector();
                }
                else {
                    var geneArray = args[0];
                    var size = args[1];
                    var compare = (args.length == 2) ? std.greater : args[2];
                    this.children_ = new std.Vector();
                    this.compare_ = compare;
                    for (var i = 0; i < size; i++) {
                        var child = this.clone(geneArray);
                        if (i > 0)
                            std.random_shuffle(child.begin(), child.end());
                        this.children_.push_back(child);
                    }
                }
            }
            GAPopulation.prototype.children = function () {
                return this.children_;
            };
            /**
             * Test fitness of each *GeneArray* in the {@link population}.
             *
             * @return The best *GeneArray* in the {@link population}.
             */
            GAPopulation.prototype.fitTest = function () {
                var best = this.children_.front();
                for (var i = 1; i < this.children_.size(); i++)
                    if (this.compare_(this.children_.at(i), best) == true)
                        best = this.children_.at(i);
                return best;
            };
            /**
             * @hidden
             */
            GAPopulation.prototype.clone = function (obj) {
                var ret = eval("new obj.constructor()");
                for (var key in obj)
                    if (obj.hasOwnProperty(key) == true)
                        ret[key] = obj[key];
                return ret;
            };
            return GAPopulation;
        }());
        library.GAPopulation = GAPopulation;
    })(library = samchon.library || (samchon.library = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var library;
    (function (library) {
        /**
         * A utility class supporting static methods of string.
         *
         * The {@link StringUtil} utility class is an all-static class with methods for working with string objects.
         * You do not create instances of {@link StringUtil}; instead you call methods such as the
         * ```StringUtil.substitute()``` method.
         *
         * @reference http://help.adobe.com/en_US/FlashPlatform/reference/actionscript/3/mx/utils/StringUtil.html
         * @author Jeongho Nam <http://samchon.org>
         */
        var StringUtil = (function () {
            function StringUtil() {
            }
            /* ==================================================================
                SUBSTRING
                    - BETWEEN(s)
                    - TRIM
            =====================================================================
                BETWEEN(s)
            ------------------------------------------------------------------ */
            /**
             * Generate a substring.
             *
             * Extracts a substring consisting of the characters from specified start to end.
             * It's same with str.substring( ? = (str.find(start) + start.size()), str.find(end, ?) )
             *
             * ```typescript
             * let str: string = StringUtil.between("ABCD(EFGH)IJK", "(", ")");
             * console.log(str); // PRINTS "EFGH"
             * ```
             *
             * - If start is not specified, extracts from begin of the string to end. </li>
             * - If end is not specified, extracts from start to end of the string. </li>
             * - If start and end are all omitted, returns str, itself. </li>
             *
             * @param str Target string to be applied between.
             * @param start A string for separating substring at the front.
             * @param end A string for separating substring at the end.
             *
             * @return substring by specified terms.
             */
            StringUtil.between = function (str, start, end) {
                if (start === void 0) { start = ""; }
                if (end === void 0) { end = ""; }
                if (start == "" && end == "")
                    return str;
                else if (start == "")
                    return str.substr(0, str.indexOf(end));
                else if (end == "")
                    return str.substr(str.indexOf(start) + start.length);
                else {
                    var startIndex = str.indexOf(start);
                    if (startIndex == -1)
                        return "";
                    return str.substring(startIndex + start.length, str.indexOf(end, startIndex + start.length));
                }
            };
            /**
             * Fetch substrings.
             *
             * Splits a string into an array of substrings dividing by specified delimeters of start and end.
             * It's the array of substrings adjusted the between.
             *
             * <ul>
             *	<li> If startStr is omitted, it's same with the split by endStr not having last item. </li>
             *	<li> If endStr is omitted, it's same with the split by startStr not having first item. </li>
             *	<li> If startStr and endStar are all omitted, returns *str*. </li>
             * </ul>
             *
             * @param str Target string to split by between.
             * @param start A string for separating substring at the front.
             *				If omitted, it's same with split(end) not having last item.
             * @param end A string for separating substring at the end.
             *			  If omitted, it's same with split(start) not having first item.
             * @return An array of substrings.
             */
            StringUtil.betweens = function (str, start, end) {
                if (start === void 0) { start = ""; }
                if (end === void 0) { end = ""; }
                var substrings = [];
                if (start == "" && end == "") {
                    // PARAMETER IS NOT SPECIFIED
                    // DO NOTHING
                    return [str];
                }
                else if (start == end) {
                    // SPLITTERS START AND END ARE EQUAL
                    var prevIndex = -1;
                    var endIndex = void 0;
                    var n = 0;
                    while ((endIndex = str.indexOf(start, prevIndex + 1)) != -1) {
                        if (++n % 2 == 0) {
                            substrings.push(str.substring(prevIndex, endIndex));
                        }
                        endIndex = prevIndex;
                    }
                }
                else {
                    substrings = str.split(start).splice(1);
                    if (end != "")
                        for (var i = substrings.length - 1; i >= 0; i--)
                            if (substrings[i].indexOf(end) == -1)
                                substrings.splice(i, 1);
                            else
                                substrings[i] = StringUtil.between(substrings[i], "", end);
                }
                return substrings;
            };
            /**
             * Remove all designated characters from the beginning and end of the specified string.
             *
             * @param str The string whose designated characters should be trimmed.
             * @param args Designated character(s).
             *
             * @return Updated string where designated characters was removed from the beginning and end.
             */
            StringUtil.trim = function (str) {
                var args = [];
                for (var _i = 1; _i < arguments.length; _i++) {
                    args[_i - 1] = arguments[_i];
                }
                if (args.length == 0)
                    args = StringUtil.SPACE_ARRAY;
                return StringUtil.ltrim.apply(StringUtil, [StringUtil.rtrim.apply(StringUtil, [str].concat(args))].concat(args));
            };
            /**
             * Remove all designated characters from the beginning of the specified string.
             *
             * @param str The string should be trimmed.
             * @param delims Designated character(s).
             *
             * @return Updated string where designated characters was removed from the beginning
             */
            StringUtil.ltrim = function (str) {
                var args = [];
                for (var _i = 1; _i < arguments.length; _i++) {
                    args[_i - 1] = arguments[_i];
                }
                if (args.length == 0)
                    args = StringUtil.SPACE_ARRAY;
                var index = 0;
                while (index < str.length) {
                    var maxIndex = index;
                    for (var i = 0; i < args.length; i++) {
                        var myIndex = str.indexOf(args[i], maxIndex) // START FROM PREVIOUS MAX_INDEX
                            + args[i].length; // ADD ITS LENGTH
                        maxIndex = Math.max(maxIndex, myIndex);
                    }
                    if (maxIndex <= index)
                        break; // CAN BE -1
                    else
                        index = maxIndex;
                }
                if (index == str.length)
                    return "";
                else
                    return str.substr(index);
            };
            /**
             * Remove all designated characters from the end of the specified string.
             *
             * @param str The string should be trimmed.
             * @param delims Designated character(s).
             *
             * @return Updated string where designated characters was removed from the end.
             */
            StringUtil.rtrim = function (str) {
                var args = [];
                for (var _i = 1; _i < arguments.length; _i++) {
                    args[_i - 1] = arguments[_i];
                }
                if (args.length == 0)
                    args = StringUtil.SPACE_ARRAY;
                var index = str.length;
                while (index != 0) {
                    var minIndex = index;
                    for (var i = 0; i < args.length; i++) {
                        var myIndex = str.lastIndexOf(args[i], minIndex - 1);
                        if (myIndex == -1)
                            continue;
                        minIndex = Math.min(minIndex, myIndex);
                    }
                    if (minIndex == -1 || minIndex >= index)
                        break;
                    else
                        index = minIndex;
                }
                return str.substr(0, index);
            };
            /* ==================================================================
                REPLACERS
                    - SUBSTITUTE
                    - REPLACE_ALL
                    - MISCELLANEOUS
            =====================================================================
                SUBSTITUTE
            ------------------------------------------------------------------ */
            /**
             * Substitute <code>{n}</code> tokens within the specified string.
             *
             * @param format The string to make substitutions in. This string can contain special tokens of the form
             *				 <code>{n}</code>, where <code>n</code> is a zero based index, that will be replaced with the
             *				 additional parameters found at that index if specified.
             * @param args Additional parameters that can be substituted in the *format* parameter at each
             *			   <code>{n}</code> location, where <code>n</code> is an integer (zero based) index value into
             *			   the array of values specified.
             *
             * @return New string with all of the <code>{n}</code> tokens replaced with the respective arguments specified.
             */
            StringUtil.substitute = function (format) {
                var args = [];
                for (var _i = 1; _i < arguments.length; _i++) {
                    args[_i - 1] = arguments[_i];
                }
                while (true) {
                    if (args.length == 0)
                        break;
                    var parenthesisArray = StringUtil.betweens(format, "{", "}");
                    var minIndex = Number.MAX_VALUE;
                    for (var i = 0; i < parenthesisArray.length; i++) {
                        var index = Number(parenthesisArray[i]);
                        if (isNaN(index) == true)
                            continue;
                        minIndex = Math.min(minIndex, index);
                    }
                    if (minIndex == Number.MAX_VALUE)
                        break;
                    format = StringUtil.replaceAll(format, "{" + minIndex + "}", args[0]);
                    args.shift();
                }
                return format;
            };
            StringUtil.replaceAll = function (str) {
                var args = [];
                for (var _i = 1; _i < arguments.length; _i++) {
                    args[_i - 1] = arguments[_i];
                }
                if (args.length == 2 && typeof args[0] == "string") {
                    var before = args[0];
                    var after = args[1];
                    return str.split(before).join(after);
                }
                else {
                    if (args.length == 0)
                        return str;
                    for (var i = 0; i < args.length; i++) {
                        var pair = args[i];
                        str = str.split(pair.first).join(pair.second);
                    }
                    return str;
                }
            };
            /* ------------------------------------------------------------------
                MISCELLANEOUS
            ------------------------------------------------------------------ */
            /**
             * Replace all HTML spaces to a literal space.
             *
             * @param str Target string to replace.
             */
            StringUtil.removeHTMLSpaces = function (str) {
                return StringUtil.replaceAll(str, new std.Pair("&nbsp;", " "), new std.Pair("\t", " "), new std.Pair("  ", " "));
            };
            /**
             * Repeat a string.
             *
             * Returns a string consisting of a specified string concatenated with itself a specified number of times.
             *
             * @param str The string to be repeated.
             * @param n The repeat count.
             *
             * @return The repeated string.
             */
            StringUtil.repeat = function (str, n) {
                var ret = "";
                for (var i = 0; i < n; i++)
                    ret += str;
                return ret;
            };
            /* ==================================================================
                NUMBER FORMAT
                    - NUMBER
                    - PERCENT
            =====================================================================
                NUMBER
            ------------------------------------------------------------------ */
            /**
             * Number to formatted string with &quot;,&quot; sign.
             *
             * Returns a string converted from the number rounded off from specified precision with &quot;,&quot; symbols.
             *
             * @param val A number wants to convert to string.
             * @param precision Target precision of round off.
             *
             * @return A string who represents the number with roundoff and &quot;,&quot; symbols.
             */
            StringUtil.numberFormat = function (val, precision) {
                if (precision === void 0) { precision = 2; }
                var str = "";
                // FIRST, DO ROUND-OFF
                val = Math.round(val * Math.pow(10, precision));
                val = val / Math.pow(10, precision);
                // SEPERATE NUMBERS
                var is_negative = (val < 0);
                var natural = Math.floor(Math.abs(val));
                var fraction = Math.abs(val) - Math.floor(Math.abs(val));
                // NATURAL NUMBER
                if (natural == 0)
                    str = "0";
                else {
                    // NOT ZERO
                    var cipher_count = Math.floor(Math.log(natural) / Math.log(10)) + 1;
                    for (var i = 0; i <= cipher_count; i++) {
                        var cipher = Math.floor(natural % Math.pow(10, i + 1));
                        cipher = Math.floor(cipher / Math.pow(10, i));
                        if (i == cipher_count && cipher == 0)
                            continue;
                        // IS MULTIPLIER OF 3
                        if (i > 0 && i % 3 == 0)
                            str = "," + str;
                        // PUSH FRONT TO THE STRING
                        str = cipher + str;
                    }
                }
                // NEGATIVE SIGN
                if (is_negative == true)
                    str = "-" + str;
                // ADD FRACTION
                if (precision > 0 && fraction != 0) {
                    fraction = Math.round(fraction * Math.pow(10, precision));
                    var zeros = precision - Math.floor(Math.log(fraction) / Math.log(10)) - 1;
                    str += "." + StringUtil.repeat("0", zeros) + fraction;
                }
                return str;
            };
            StringUtil.percentFormat = function (val, precision) {
                if (precision === void 0) { precision = 2; }
                return StringUtil.numberFormat(val * 100, precision) + " %";
            };
            return StringUtil;
        }());
        /* ------------------------------------------------------------------
            TRIM
        ------------------------------------------------------------------ */
        /**
         * An array containing whitespaces.
         */
        StringUtil.SPACE_ARRAY = [" ", "\t", "\r", "\n"];
        library.StringUtil = StringUtil;
    })(library = samchon.library || (samchon.library = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var library;
    (function (library) {
        /**
         * URLVariables class is for representing variables of HTTP.
         *
         * {@link URLVariables} class allows you to transfer variables between an application and server.
         *
         * When transfering, {@link URLVariables} will be converted to a *URI* string.
         * - URI: Uniform Resource Identifier
         *
         * @reference http://help.adobe.com/en_US/FlashPlatform/reference/actionscript/3/flash/net/URLVariables.html
         * @author Migrated by Jeongho Nam <http://samchon.org>
         */
        var URLVariables = (function (_super) {
            __extends(URLVariables, _super);
            function URLVariables(str) {
                if (str === void 0) { str = ""; }
                var _this = _super.call(this) || this;
                if (str != "")
                    _this.decode(str);
                return _this;
            }
            /**
             * Converts the variable string to properties of the specified URLVariables object.
             *
             * @param str A URL-encoded query string containing name/value pairs.
             */
            URLVariables.prototype.decode = function (str) {
                this.clear();
                if (str.trim() == "")
                    return;
                if (str.indexOf("?") != -1)
                    str = str.substr(str.indexOf("?") + 1);
                var var_pairs = str.split("&");
                for (var i = 0; i < var_pairs.length; i++) {
                    var equal_index = var_pairs[i].indexOf("=");
                    var key = void 0;
                    var value = void 0;
                    if (equal_index == -1) {
                        key = var_pairs[i];
                        value = "";
                    }
                    else {
                        key = var_pairs[i].substr(0, equal_index);
                        value = decodeURIComponent(var_pairs[i].substr(equal_index + 1));
                    }
                    this.insert([key, value]);
                }
            };
            /**
             * Returns a string containing all enumerable variables, in the MIME content encoding application/x-www-form-urlencoded.
             */
            URLVariables.prototype.toString = function () {
                var str = "";
                for (var it = this.begin(); !it.equals(this.end()); it = it.next()) {
                    if (!it.equals(this.begin()))
                        str += "&";
                    str += it.first + "=" + encodeURIComponent(it.second);
                }
                return str;
            };
            return URLVariables;
        }(std.HashMap));
        library.URLVariables = URLVariables;
    })(library = samchon.library || (samchon.library = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var library;
    (function (library) {
        /**
         * A tree-structured XML object.
         *
         * The {@link XML| class contains methods and properties for working with XML objects. The {@link XML} class (along
         * with the {@link XMLList}) implements the powerful XML-handling standards defined in ECMAScript for XML (E4X)
         * specification (ECMA-357 edition 2).
         *
         * An XML object, it is composed with three members; {@link getTag tag}, {@link getProperty properties} and
         * {@link getValue value}. As you know, XML is a tree structured data expression method. The tree-stucture;
         * {@link XML} class realizes it by extending ```std.HashMap<string, XMLList>```. Child {@link XML} objects are
         * contained in the matched {@link XMLList} object being grouped by their {@link getTag tag name}. The
         * {@link XMLList} objects, they're stored in the {@link std.HashMap} ({@link XML} itself) with its **key**; common
         * {@link getTag tag name} of children {@link XML} objects.
         *
         * ```typescript
         * class XML extends std.HashMap<string, XMLList>
         * {
         *	private tag_: string;
         *	private properties_: std.HashMap<string, string>;
         *	private value_: string;
         * }
         * ```
         *
         * ```xml
         * <?xml version="1.0" ?>
         * <TAG property_name={property_value}>
         *	<!--
         *		The cchild XML objects with "CHILD_TAG", They're contained in an XMLList object.
         *		The XMLList object, it is stored in std.HashMap (XML class itself) with its key "CHILD_TAG"
         *	-->
         *	<CHILD_TAG property_name={property_value}>{value}</CHILD_TAG>
         *  <CHILD_TAG property_name={property_value}>{value}</CHILD_TAG>
         *	<CHILD_TAG property_name={property_value}>{value}</CHILD_TAG>
         *
         *	<!--
         *		The child XML object named "ANOTHER_TAG", it also belonged to an XMLList ojbect.
         *		And the XMLList is also being contained in the std.HashMap with its key "ANOTHER_TAG"
         *	-->
         *	<ANOTHER_TAG />
         * </TAG>
         * ```
         *
         * Use the {@link toString toString()} method to return a string representation of the {@link XML} object regardless
         * of whether the {@link XML} object has simple content or complex content.
         *
         * @reference http://help.adobe.com/en_US/FlashPlatform/reference/actionscript/3/XML.html
         * @handbook https://github.com/samchon/framework/wiki/TypeScript-Library-XML
         * @author Jeongho Nam <http://samchon.org>
         */
        var XML = (function (_super) {
            __extends(XML, _super);
            function XML(str) {
                if (str === void 0) { str = ""; }
                var _this = _super.call(this) || this;
                _this.property_map_ = new std.HashMap();
                _this.value_ = "";
                if (str.indexOf("<") == -1)
                    return _this;
                var start;
                var end;
                //ERASE HEADER OF XML
                if ((start = str.indexOf("<?xml")) != -1) {
                    end = str.indexOf("?>", start);
                    if (end != -1)
                        str = str.substr(end + 2);
                }
                //ERASE COMMENTS
                while ((start = str.indexOf("<!--")) != -1) {
                    end = str.indexOf("-->", start);
                    if (end != -1)
                        break;
                    str = str.substr(0, start) + str.substr(end + 3);
                }
                //BEGIN PARSING
                _this._Parse(str);
                return _this;
            }
            /* -------------------------------------------------------------
                PARSERS
            ------------------------------------------------------------- */
            /**
             * @hidden
             */
            XML.prototype._Parse = function (str) {
                this._Parse_tag(str);
                this._Parse_properties(str);
                var res = this._Parse_value(str);
                if (res.second == true)
                    this._Parse_children(res.first);
            };
            /**
             * @hidden
             */
            XML.prototype._Parse_tag = function (str) {
                var start = str.indexOf("<") + 1;
                var end = this._Compute_min_index(str.indexOf(" ", start), str.indexOf("\r\n", start), str.indexOf("\n", start), str.indexOf("\t", start), str.indexOf(">", start), str.indexOf("/", start));
                if (start == 0 || end == -1)
                    return;
                this.tag_ = str.substring(start, end);
            };
            /**
             * @hidden
             */
            XML.prototype._Parse_properties = function (str) {
                var start = str.indexOf("<" + this.tag_) + this.tag_.length + 1;
                var end = this._Compute_min_index(str.lastIndexOf("/"), str.indexOf(">", start));
                if (start == -1 || end == -1 || start >= end)
                    return;
                //<comp label='ABCD' /> : " label='ABCD' "
                var line = str.substring(start, end);
                if (line.indexOf("=") == -1)
                    return;
                var label;
                var value;
                var helpers = [];
                var inQuote = false;
                var quoteType;
                var equal;
                //INDEXING
                for (var i = 0; i < line.length; i++) {
                    //Start of quote
                    if (inQuote == false && (line.charAt(i) == "'" || line.charAt(i) == "\"")) {
                        inQuote = true;
                        start = i;
                        if (line.charAt(i) == "'")
                            quoteType = 1;
                        else if (line.charAt(i) == "\"")
                            quoteType = 2;
                    }
                    else if (inQuote == true &&
                        ((quoteType == 1 && line.charAt(i) == "'") ||
                            (quoteType == 2 && line.charAt(i) == "\""))) {
                        helpers.push({ type: quoteType, start: start, end: i });
                        inQuote = false;
                    }
                }
                //CONSTRUCTING
                for (var i = 0; i < helpers.length; i++) {
                    var quote = helpers[i];
                    if (i == 0) {
                        equal = line.indexOf("=");
                        label = line.substring(0, equal).trim();
                    }
                    else {
                        equal = line.indexOf("=", helpers[i - 1].end + 1);
                        label = line.substring(helpers[i - 1].end + 1, equal).trim();
                    }
                    value = line.substring(helpers[i].start + 1, helpers[i].end);
                    this.setProperty(label, this._Decode_property(value));
                }
            };
            /**
             * @hidden
             */
            XML.prototype._Parse_value = function (str) {
                var end_slash = str.lastIndexOf("/");
                var end_block = str.indexOf(">");
                if (end_slash < end_block || end_slash + 1 == str.lastIndexOf("<")) {
                    //STATEMENT1: <TAG />
                    //STATEMENT2: <TAG></TAG> -> SAME WITH STATEMENT1: <TAG />
                    this.value_ = "";
                    return new std.Pair(str, false);
                }
                var start = end_block + 1;
                var end = str.lastIndexOf("<");
                str = str.substring(start, end); //REDEFINE WEAK_STRING -> IN TO THE TAG
                if (str.indexOf("<") == -1)
                    this.value_ = this._Decode_value(str.trim());
                else
                    this.value_ = "";
                return new std.Pair(str, true);
            };
            /**
             * @hidden
             */
            XML.prototype._Parse_children = function (str) {
                if (str.indexOf("<") == -1)
                    return;
                var start = str.indexOf("<");
                var end = str.lastIndexOf(">") + 1;
                str = str.substring(start, end);
                var blockStart = 0;
                var blockEnd = 0;
                start = 0;
                for (var i = 0; i < str.length; i++) {
                    if (str.charAt(i) == "<" && str.substr(i, 2) != "</")
                        blockStart++;
                    else if (str.substr(i, 2) == "/>" || str.substr(i, 2) == "</")
                        blockEnd++;
                    if (blockStart >= 1 && blockStart == blockEnd) {
                        end = str.indexOf(">", i);
                        var xmlList = void 0;
                        var xml = new XML();
                        xml._Parse(str.substring(start, end + 1));
                        if (this.has(xml.tag_) == true)
                            xmlList = this.get(xml.tag_);
                        else {
                            xmlList = new library.XMLList();
                            this.set(xml.tag_, xmlList);
                        }
                        xmlList.push(xml);
                        i = end;
                        start = end + 1;
                        blockStart = 0;
                        blockEnd = 0;
                    }
                }
            };
            /* =============================================================
                ACCESSORS
                    - GETTERS
                    - SETTERS
                    - ELEMENTS I/O
            ================================================================
                GETTERS
            ------------------------------------------------------------- */
            /**
             * Get tag.
             *
             * ```xml
             * <TAG property_key={property_value}>{value}</TAG>
             * ```
             *
             * @return tag.
             */
            XML.prototype.getTag = function () {
                return this.tag_;
            };
            /**
             * Get value.
             *
             * ```xml
             * <tag property_key={property_value}>{VALUE}</tag>
             * ```
             *
             * @return value.
             */
            XML.prototype.getValue = function () {
                return this.value_;
            };
            /**
             * Get iterator to property element.
             *
             * Searches the {@link getPropertyMap properties} for an element with a identifier equivalent to <i>key</i>
             * and returns an iterator to it if found, otherwise it returns an iterator to {@link HashMap.end end()}.
             *
             * <p> Two keys are considered equivalent if the properties' comparison object returns false reflexively
             * (i.e., no matter the order in which the elements are passed as arguments). </p>
             *
             * Another member function, {@link hasProperty hasProperty()} can be used to just check whether a particular
             * <i>key</i> exists.
             *
             * ```xml
             * <tag PROPERTY_KEY={property_value}>{value}</tag>
             * ```
             *
             * @param key Key to be searched for
             * @return An iterator to the element, if an element with specified <i>key</i> is found, or
             *		   {@link end HashMap.end()} otherwise.
             */
            XML.prototype.findProperty = function (key) {
                return this.property_map_.find(key);
            };
            /**
             * Test whether a property exists.
             *
             * ```xml
             * <tag PROPERTY_KEY={property_value}>{value}</tag>
             * ```
             *
             * @return Whether a property has the *key* exists or not.
             */
            XML.prototype.hasProperty = function (key) {
                return this.property_map_.has(key);
            };
            /**
             * Get property.
             *
             * Get property by its *key*, property name. If the matched *key* does not exist, then exception
             * {@link std.OutOfRange} is thrown. Thus, it would better to test whether the *key* exits or not by calling the
             * {@link hasProperty hasProperty()} method before calling this {@link getProperty getProperty()}.
             *
             * This method can be substituted by {@link getPropertyMap getPropertyMap()} such below:
             * - ```getPropertyMap().get(key, value);```
             * - ```getPropertyMap().find(key).second;```
             *
             * ```xml
             * <tag PROPERTY_KEY={PROPERTY_VALUE}>{value}</tag>
             * ```
             *
             * @return Value of the matched property.
             */
            XML.prototype.getProperty = function (key) {
                return this.property_map_.get(key);
            };
            /**
             * Get property map.
             *
             * ```xml
             * <tag PROPERTY_KEY1={PROPERTY_VALUE1}
             *		PROPERTY_KEY2={PROPERTY_VALUE2}
             *		PROPERTY_KEY3={PROPERTY_VALUE3}>{value}</tag>
             * ```
             *
             * @return {@link HashMap} containing properties' keys and values.
             */
            XML.prototype.getPropertyMap = function () {
                return this.property_map_;
            };
            /* -------------------------------------------------------------
                SETTERS
            ------------------------------------------------------------- */
            /**
             * Set tag.
             *
             * Set tag name, identifier of this {@link XML} object.
             *
             * If this {@link XML} object is belonged to, a child of, an {@link XMLList} and its related {@link XML} objects,
             * then calling this {@link setTag setTag()} method direclty is not recommended. Erase this {@link XML} object
             * from parent objects and insert this object again.
             *
             * ```xml
             * <TAG property_key={property_value}>{value}</TAG>
             * ```
             *
             * @param val To be new {@link getTag tag}.
             */
            XML.prototype.setTag = function (val) {
                this.tag_ = val;
            };
            /**
             * Set value.
             *
             * ```xml
             * <tag property_key={property_value}>{VALUE}</tag>
             * ```
             *
             * @param val To be new {@link getValue value}.
             */
            XML.prototype.setValue = function (val) {
                this.value_ = val;
            };
            /**
             * Set property.
             *
             * Set a property *value* with its *key*. If the *key* already exists, then the *value* will be overwritten to
             * the property. Otherwise the *key* is not exist yet, then insert the *key* and *value* {@link Pair pair} to
             * {@link getPropertyMao property map}.
             *
             * This method can be substituted by {@link getPropertyMap getPropertyMap()} such below:
             * - ```getPropertyMap().set(key, value);```
             * - ```getPropertyMap().emplace(key, value);```
             * - ```getPropertyMap().insert([key, value]);```
             * - ```getPropertyMap().insert(std.make_pair(key, value));```
             *
             * ```xml
             * <tag PROPERTY_KEY={PROPERTY_VALUE}>{value}</tag>
             * ```
             *
             * @param key Key, identifier of property to be newly inserted.
             * @param value Value of new property to be newly inserted.
             */
            XML.prototype.setProperty = function (key, value) {
                this.property_map_.set(key, value);
            };
            /**
             * Erase property.
             *
             * Erases a property by its *key*, property name. If the matched *key* does not exist, then exception
             * {@link std.OutOfRange} is thrown. Thus, it would better to test whether the *key* exits or not by calling the
             * {@link hasProperty hasProperty()} method before calling this {@link eraseProperty eraseProperty()}.
             *
             * This method can be substituted by ``getPropertyMap().erase(key)````.
             *
             * ```xml
             * <tag PROPERTY_KEY={property_value}>{value}</tag>
             * ```
             *
             * @param key Key of the property to erase
             * @throw {@link std.OutOfRange}
             */
            XML.prototype.eraseProperty = function (key) {
                var it = this.property_map_.find(key);
                if (it.equals(this.property_map_.end()) == true)
                    throw Error("out of range");
                this.property_map_.erase(it);
            };
            XML.prototype.push = function () {
                var items = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    items[_i] = arguments[_i];
                }
                for (var i = 0; i < items.length; i++) {
                    if (items[i] instanceof XML) {
                        var xml = items[i];
                        if (this.has(xml.tag_) == true)
                            this.get(xml.tag_).push(xml);
                        else {
                            var xmlList = new library.XMLList();
                            xmlList.push(xml);
                            this.set(xml.tag_, xmlList);
                        }
                    }
                    else if (items[i] instanceof library.XMLList) {
                        var xmlList = items[i];
                        if (xmlList.empty() == true)
                            continue;
                        if (this.has(xmlList.getTag()) == true) {
                            var myXMLList = this.get(xmlList.getTag());
                            myXMLList.insert(myXMLList.end(), xmlList.begin(), xmlList.end());
                        }
                        else
                            this.set(xmlList.getTag(), xmlList);
                    }
                    else
                        _super.prototype.push.call(this, items[i]);
                }
                return this.size();
            };
            /**
             * Add all properties from other {@link XML} object.
             *
             * All the properties in the *obj* are copied to this {@link XML} object. If this {@link XML} object has same
             * property key in the *obj*, then value of the property will be replaced to *obj*'s own. If you don't want to
             * overwrite properties with same key, then use {@link getPropertyMap getPropertyMap()} method.
             *
             * ```typescript
             * let x: library.XML;
             * let y: library.XML;
             *
             * x.addAllProperties(y); // duplicated key exists, then overwrites
             * x.getPropertyMap().insert(y.getPropertyMap().begin(), y.getPropertyMap().end());
             *	// ducpliated key, then ignores. only non-duplicateds are copied.
             * ```
             *
             * ```xml
             * <tag PROPERTY_KEY1={property_value1}
             *		PROPERTY_KEY2={property_value2}
             *		PROPERTY_KEY3={property_value3}>{value}</tag>
             * ```
             *
             * @param obj Target {@link XML} object to copy properties.
             */
            XML.prototype.insertAllProperties = function (obj) {
                for (var it = obj.property_map_.begin(); it.equals(obj.property_map_.end()) == false; it = it.next())
                    this.setProperty(it.first, it.second);
            };
            /**
             * Clear properties.
             *
             * Remove all properties. It's same with calling ```getPropertyMap().clear()```.
             *
             * ```xml
             * <tag PROPERTY_KEY1={property_value1}
             *		PROPERTY_KEY2={property_value2}
             *		PROPERTY_KEY3={property_value3}>{value}</tag>
             * ```
             */
            XML.prototype.clearProperties = function () {
                this.property_map_.clear();
            };
            /* -------------------------------------------------------------
                FILTERS
            ------------------------------------------------------------- */
            /**
             * @hidden
             */
            XML.prototype._Compute_min_index = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var min = args[0];
                for (var i = 1; i < args.length; i++) {
                    if (args[i] == -1)
                        continue;
                    if (min == -1 || args[i] < min)
                        min = args[i];
                }
                return min;
            };
            /**
             * @hidden
             */
            XML.prototype._Decode_value = function (str) {
                var pairs = [
                    new std.Pair("&amp;", "&"),
                    new std.Pair("&lt;", "<"),
                    new std.Pair("&gt;", ">")
                ];
                return library.StringUtil.replaceAll.apply(library.StringUtil, [str].concat(pairs));
            };
            /**
             * @hidden
             */
            XML.prototype._Encode_value = function (str) {
                var pairs = [
                    new std.Pair("&", "&amp;"),
                    new std.Pair("<", "&lt;"),
                    new std.Pair(">", "&gt;")
                ];
                return library.StringUtil.replaceAll.apply(library.StringUtil, [str].concat(pairs));
            };
            /**
             * @hidden
             */
            XML.prototype._Decode_property = function (str) {
                var pairs = [
                    new std.Pair("&amp;", "&"),
                    new std.Pair("&lt;", "<"),
                    new std.Pair("&gt;", ">"),
                    new std.Pair("&quot;", "\""),
                    new std.Pair("&apos;", "'"),
                    new std.Pair("&#x9;", "\t"),
                    new std.Pair("&#xA;", "\n"),
                    new std.Pair("&#xD;", "\r"),
                ];
                return library.StringUtil.replaceAll.apply(library.StringUtil, [str].concat(pairs));
            };
            /**
             * @hidden
             */
            XML.prototype._Encode_property = function (str) {
                var pairs = [
                    new std.Pair("&", "&amp;"),
                    new std.Pair("<", "&lt;"),
                    new std.Pair(">", "&gt;"),
                    new std.Pair("\"", "&quot;"),
                    new std.Pair("'", "&apos;"),
                    new std.Pair("\t", "&#x9;"),
                    new std.Pair("\n", "&#xA;"),
                    new std.Pair("\r", "&#xD;"),
                ];
                return library.StringUtil.replaceAll.apply(library.StringUtil, [str].concat(pairs));
            };
            /* -------------------------------------------------------------
                EXPORTS
            ------------------------------------------------------------- */
            /**
             * {@link XML} object to xml string.
             *
             * Returns a string representation of the {@link XML} object.
             *
             * @param tab Number of tabs to spacing.
             * @return The string representation of the {@link XML} object.
             */
            XML.prototype.toString = function (tab) {
                if (tab === void 0) { tab = 0; }
                var str = library.StringUtil.repeat("\t", tab) + "<" + this.tag_;
                var children_str = "";
                //PROPERTIES
                for (var p_it = this.property_map_.begin(); p_it.equals(this.property_map_.end()) == false; p_it = p_it.next())
                    str += " " + p_it.first + "=\"" + this._Encode_property(p_it.second) + "\"";
                if (this.size() == 0) {
                    // VALUE
                    if (this.value_ != "")
                        str += ">" + this._Encode_value(this.value_) + "</" + this.tag_ + ">";
                    else
                        str += " />";
                }
                else {
                    // CHILDREN
                    str += ">\n";
                    for (var x_it = this.begin(); x_it.equals(this.end()) == false; x_it = x_it.next())
                        str += x_it.second.toString(tab + 1);
                    str += library.StringUtil.repeat("\t", tab) + "</" + this.tag_ + ">";
                }
                return str;
            };
            return XML;
        }(std.HashMap));
        library.XML = XML;
    })(library = samchon.library || (samchon.library = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var library;
    (function (library) {
        /**
         * List of {@link XML} objects with same tag.
         *
         * @reference http://help.adobe.com/en_US/FlashPlatform/reference/actionscript/3/XMLList.html
         * @handbook https://github.com/samchon/framework/wiki/TypeScript-Library-XML
         * @author Jeongho Nam <http://samchon.org>
         */
        var XMLList = (function (_super) {
            __extends(XMLList, _super);
            function XMLList() {
                return _super.apply(this, arguments) || this;
            }
            // using super::constructor
            /**
             * Get tag.
             */
            XMLList.prototype.getTag = function () {
                return this.front().getTag();
            };
            /**
             * {@link XMLList XML objects} to string.
             *
             * Returns a string representation of the {@link XMLList XML objects}.
             *
             * @param tab Number of tabs to spacing.
             * @return The string representation of the {@link XMLList XML objects}.
             */
            XMLList.prototype.toString = function (level) {
                if (level === void 0) { level = 0; }
                var str = "";
                for (var i = 0; i < this.size(); i++)
                    str += this.at(i).toString(level) + "\n";
                return str;
            };
            return XMLList;
        }(std.Deque));
        library.XMLList = XMLList;
    })(library = samchon.library || (samchon.library = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * @hidden
         */
        var IEntity;
        (function (IEntity) {
            function construct(entity, xml) {
                var prohibited_names = [];
                for (var _i = 2; _i < arguments.length; _i++) {
                    prohibited_names[_i - 2] = arguments[_i];
                }
                // MEMBER VARIABLES
                //  - ATOMIC ONLY; STRING, NUMBER AND BOOLEAN
                var property_map = xml.getPropertyMap();
                for (var it = property_map.begin(); !it.equals(property_map.end()); it = it.next()) {
                    if (entity[it.first] == undefined)
                        continue;
                    var prohibited = false;
                    for (var i = 0; i < prohibited_names.length; i++)
                        if (prohibited_names[i] == it.first) {
                            prohibited = true;
                            break;
                        }
                    if (prohibited == true)
                        continue;
                    if (typeof entity[it.first] == "string")
                        entity[it.first] = it.second;
                    else if (typeof entity[it.first] == "number")
                        entity[it.first] = Number(it.second);
                    else if (typeof entity[it.first] == "boolean")
                        entity[it.first] = (it.second == "true");
                }
            }
            IEntity.construct = construct;
            function toXML(entity) {
                var prohibited_names = [];
                for (var _i = 1; _i < arguments.length; _i++) {
                    prohibited_names[_i - 1] = arguments[_i];
                }
                var xml = new samchon.library.XML();
                xml.setTag(entity.TAG());
                // MEMBER VARIABLES
                //  - ATOMIC ONLY; STRING, NUMBER AND BOOLEAN
                for (var key in entity)
                    if (typeof key == "string"
                        && (typeof entity[key] == "string"
                            || typeof entity[key] == "number"
                            || typeof entity[key] == "boolean")
                        && entity.hasOwnProperty(key)) {
                        if (key == "" || key.charAt(0) == "_" || key.charAt(key.length - 1) == "_")
                            continue;
                        var prohibited = false;
                        for (var i = 0; i < prohibited_names.length; i++)
                            if (prohibited_names[i] == key) {
                                prohibited = true;
                                break;
                            }
                        if (prohibited == true)
                            continue;
                        // ATOMIC
                        xml.setProperty(key, String(entity[key]));
                    }
                return xml;
            }
            IEntity.toXML = toXML;
        })(IEntity = protocol.IEntity || (protocol.IEntity = {}));
        /**
         * An entity, a standard data class.
         *
         * Entity is a class for standardization of expression method using on network I/O by XML. If
         * Invoke is a standard message protocol of Samchon Framework which must be kept, Entity is a
         * recommended semi-protocol of message for expressing a data class. Following the semi-protocol
         * Entity is not imposed but encouraged.
         *
         * As we could get advantages from standardization of message for network I/O with Invoke,
         * we can get additional advantage from standardizing expression method of data class with Entity.
         * We do not need to know a part of network communication. Thus, with the Entity, we can only
         * concentrate on entity's own logics and relationships between another entities. Entity does not
         * need to how network communications are being done.
         *
         * I say repeatedly. Expression method of Entity is recommended, but not imposed. It's a semi
         * protocol for network I/O but not a essential protocol must be kept. The expression method of
         * Entity, using on network I/O, is expressed by XML string.
         *
         * If your own network system has a critical performance issue on communication data class,
         * it would be better to using binary communication (with ByteArray).
         * Don't worry about the problem! Invoke also provides methods for binary data (ByteArray).
         *
         * ![Class Diagram](http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_message_protocol.png)
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var Entity = (function () {
            /**
             * Default Constructor.
             */
            function Entity() {
                //NOTHING
            }
            Entity.prototype.construct = function (xml) {
                IEntity.construct(this, xml);
            };
            /**
             * @inheritdoc
             */
            Entity.prototype.key = function () { return ""; };
            /**
             * @inheritdoc
             */
            Entity.prototype.toXML = function () {
                return IEntity.toXML(this);
            };
            return Entity;
        }());
        protocol.Entity = Entity;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="../collections/ArrayCollection.ts" />
/// <reference path="../collections/ListCollection.ts" />
/// <reference path="../collections/DequeCollection.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * @inheritdoc
         */
        var EntityArrayCollection = (function (_super) {
            __extends(EntityArrayCollection, _super);
            function EntityArrayCollection() {
                return _super.apply(this, arguments) || this;
            }
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            // using super::super;
            /**
             * @inheritdoc
             */
            EntityArrayCollection.prototype.construct = function (xml) {
                protocol.IEntityGroup.construct(this, xml);
            };
            /* ------------------------------------------------------------------
                GETTERS
            ------------------------------------------------------------------ */
            /**
             * @inheritdoc
             */
            EntityArrayCollection.prototype.key = function () {
                return "";
            };
            /**
             * @inheritdoc
             */
            EntityArrayCollection.prototype.has = function (key) {
                return protocol.IEntityGroup.has(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityArrayCollection.prototype.count = function (key) {
                return protocol.IEntityGroup.count(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityArrayCollection.prototype.get = function (key) {
                return protocol.IEntityGroup.get(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityArrayCollection.prototype.toXML = function () {
                return protocol.IEntityGroup.toXML(this, "length");
            };
            return EntityArrayCollection;
        }(samchon.collections.ArrayCollection));
        protocol.EntityArrayCollection = EntityArrayCollection;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * @inheritdoc
         */
        var EntityListCollection = (function (_super) {
            __extends(EntityListCollection, _super);
            function EntityListCollection() {
                return _super.apply(this, arguments) || this;
            }
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            // using super::super;
            /**
             * @inheritdoc
             */
            EntityListCollection.prototype.construct = function (xml) {
                protocol.IEntityGroup.construct(this, xml);
            };
            /* ------------------------------------------------------------------
                GETTERS
            ------------------------------------------------------------------ */
            /**
             * @inheritdoc
             */
            EntityListCollection.prototype.key = function () {
                return "";
            };
            /**
             * @inheritdoc
             */
            EntityListCollection.prototype.has = function (key) {
                return protocol.IEntityGroup.has(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityListCollection.prototype.count = function (key) {
                return protocol.IEntityGroup.count(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityListCollection.prototype.get = function (key) {
                return protocol.IEntityGroup.get(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityListCollection.prototype.toXML = function () {
                return protocol.IEntityGroup.toXML(this);
            };
            return EntityListCollection;
        }(samchon.collections.ListCollection));
        protocol.EntityListCollection = EntityListCollection;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * @inheritdoc
         */
        var EntityDequeCollection = (function (_super) {
            __extends(EntityDequeCollection, _super);
            function EntityDequeCollection() {
                return _super.apply(this, arguments) || this;
            }
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            // using super::super;
            /**
             * @inheritdoc
             */
            EntityDequeCollection.prototype.construct = function (xml) {
                protocol.IEntityGroup.construct(this, xml);
            };
            /* ------------------------------------------------------------------
                GETTERS
            ------------------------------------------------------------------ */
            /**
             * @inheritdoc
             */
            EntityDequeCollection.prototype.key = function () {
                return "";
            };
            /**
             * @inheritdoc
             */
            EntityDequeCollection.prototype.has = function (key) {
                return protocol.IEntityGroup.has(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityDequeCollection.prototype.count = function (key) {
                return protocol.IEntityGroup.count(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityDequeCollection.prototype.get = function (key) {
                return protocol.IEntityGroup.get(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityDequeCollection.prototype.toXML = function () {
                return protocol.IEntityGroup.toXML(this);
            };
            return EntityDequeCollection;
        }(samchon.collections.DequeCollection));
        protocol.EntityDequeCollection = EntityDequeCollection;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../../protocol/EntityCollection.ts" />
/**
 * A template for External Systems Manager.
 *
 * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/TypeScript-Templates-External_System)
 * @author Jeongho Nam <http://samchon.org>
 */
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var external;
        (function (external) {
            /**
             * An array and manager of {@link ExternalSystem external system drivers}.
             *
             * The {@link ExternalSystemArray} is an abstract class containing and managing external system drivers,
             * {@link ExternalSystem} objects. Within framewokr of network, {@link ExternalSystemArray} represents your system
             * and children {@link ExternalSystem} objects represent remote, external systems connected with your system.
             * With this {@link ExternalSystemArray}, you can manage multiple external systems as a group.
             *
             * You can specify this {@link ExternalSystemArray} class to be *a server accepting external clients* or
             * *a client connecting to external servers*. Even both of them is also possible.
             *
             * - {@link ExternalClientArray}: A server accepting {@link ExternalSystem external clients}.
             * - {@link ExternalServerArray}: A client connecting to {@link ExternalServer external servers}.
             * - {@link ExternalServerClientArray}: Both of them. Accepts {@link ExternalSystem external clients} and connects to
             *   {@link ExternalServer external servers} at the same time.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/TypeScript-Templates-External_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ExternalSystemArray = (function (_super) {
                __extends(ExternalSystemArray, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function ExternalSystemArray() {
                    var _this = _super.call(this) || this;
                    _this.addEventListener("erase", _this._Handle_system_erase, _this);
                    return _this;
                }
                /**
                 * @hidden
                 */
                ExternalSystemArray.prototype._Handle_system_erase = function (event) {
                    for (var it = event.first; !it.equals(event.last); it = it.next())
                        it.value.destructor();
                };
                /* ---------------------------------------------------------
                    ACCESSORS
                --------------------------------------------------------- */
                /**
                 * Test whether the role exists.
                 *
                 * @param name Name, identifier of target {@link ExternalSystemRole role}.
                 *
                 * @return Whether the role has or not.
                 */
                ExternalSystemArray.prototype.hasRole = function (name) {
                    for (var i = 0; i < this.size(); i++)
                        for (var j = 0; j < this.at(i).size(); j++)
                            if (this.at(i).at(j).key() == name)
                                return true;
                    return false;
                };
                /**
                 * Get a role.
                 *
                 * @param name Name, identifier of target {@link ExternalSystemRole role}.
                 *
                 * @return The specified role.
                 */
                ExternalSystemArray.prototype.getRole = function (name) {
                    for (var i = 0; i < this.size(); i++)
                        for (var j = 0; j < this.at(i).size(); j++)
                            if (this.at(i).at(j).key() == name)
                                return this.at(i).at(j);
                    throw new std.OutOfRange("No role with such name.");
                };
                /* ---------------------------------------------------------
                    MESSAGE CHAIN
                --------------------------------------------------------- */
                /**
                 * Send an {@link Invoke} message.
                 *
                 * @param invoke An {@link Invoke} message to send.
                 */
                ExternalSystemArray.prototype.sendData = function (invoke) {
                    for (var i = 0; i < this.size(); i++)
                        this.at(i).sendData(invoke);
                };
                /* ---------------------------------------------------------
                    EXPORTERS
                --------------------------------------------------------- */
                /**
                 * Tag name of the {@link ExternalSytemArray} in {@link XML}.
                 *
                 * @return *systemArray*.
                 */
                ExternalSystemArray.prototype.TAG = function () {
                    return "systemArray";
                };
                /**
                 * Tag name of {@link ExternalSystem children elements} belonged to the {@link ExternalSytemArray} in {@link XML}.
                 *
                 * @return *system*.
                 */
                ExternalSystemArray.prototype.CHILD_TAG = function () {
                    return "system";
                };
                return ExternalSystemArray;
            }(samchon.protocol.EntityDequeCollection));
            external.ExternalSystemArray = ExternalSystemArray;
        })(external = templates.external || (templates.external = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../external/ExternalSystemArray.ts" />
/**
 * A template for Parallel Processing System.
 *
 * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System)
 * @author Jeongho Nam <http://samchon.org>
 */
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * Master of Parallel Processing System.
             *
             * The {@link ParallelSystemArray} is an abstract class containing and managing remote parallel **slave** system
             * drivers, {@link ParallelSystem} objects. Within framework of network, {@link ParallelSystemArray} represents your
             * system, a **Master** of *Parallel Processing System* that requesting *parallel process* to **slave** systems and the
             * children {@link ParallelSystem} objects represent the remote **slave** systems, who is being requested the
             * *parallel processes*.
             *
             * You can specify this {@link ParallelSystemArray} class to be *a server accepting parallel clients* or
             * *a client connecting to parallel servers*. Even both of them is possible. Extends one of them below and overrides
             * abstract factory method(s) creating the child {@link ParallelSystem} object.
             *
             * - {@link ParallelClientArray}: A server accepting {@link ParallelSystem parallel clients}.
             * - {@link ParallelServerArray}: A client connecting to {@link ParallelServer parallel servers}.
             * - {@link ParallelServerClientArray}: Both of them. Accepts {@link ParallelSystem parallel clients} and connects to
             *   {@link ParallelServer parallel servers} at the same time.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices
             * will be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * This class {@link ParallelSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link ParallelSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ParallelSystemArray = (function (_super) {
                __extends(ParallelSystemArray, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function ParallelSystemArray() {
                    var _this = _super.call(this) || this;
                    _this.history_sequence_ = 0;
                    return _this;
                }
                /* =========================================================
                    INVOKE MESSAGE CHAIN
                        - SEND DATA
                        - PERFORMANCE ESTIMATION
                ============================================================
                    SEND & REPLY DATA
                --------------------------------------------------------- */
                /**
                 * Send an {@link Invoke} message with segment size.
                 *
                 * Sends an {@link Invoke} message requesting a **parallel process** with its *segment size*. The {@link Invoke}
                 * message will be delivered to children {@link ParallelSystem} objects with the *piece size*, which is divided
                 * from the *segment size*, basis on their {@link ParallelSystem.getPerformance performance indices}.
                 *
                 * - If segment size is 100,
                 * - The segment will be allocated such below:
                 *
                 * Name    | Performance index | Number of pieces to be allocated | Formula
                 * --------|-------------------|----------------------------------|--------------
                 * Snail   |                 1 |                               10 | 100 / 10 * 1
                 * Cheetah |                 4 |                               40 | 100 / 10 * 4
                 * Rabbit  |                 3 |                               30 | 100 / 10 * 3
                 * Turtle  |                 2 |                               20 | 100 / 10 * 2
                 *
                 * When the **parallel process** has completed, then this {@link ParallelSystemArraY} will estimate
                 * {@link ParallelSystem.getPerformance performance indices} of {@link ParallelSystem} objects basis on their
                 * execution time.
                 *
                 * @param invoke An {@link Invoke} message requesting parallel process.
                 * @param size Number of pieces to segment.
                 *
                 * @return Number of {@link ParallelSystem slave systems} participating in the *Parallel Process*.
                 *
                 * @see {@link sendPieceData}, {@link ParallelSystem.getPerformacen}
                 */
                ParallelSystemArray.prototype.sendSegmentData = function (invoke, size) {
                    return this.sendPieceData(invoke, 0, size);
                };
                /**
                 * Send an {@link Invoke} message with range of pieces.
                 *
                 * Sends an {@link Invoke} message requesting a **parallel process** with its *range of pieces [first, last)*.
                 * The {@link Invoke} will be delivered to children {@link ParallelSystem} objects with the newly computed
                 * *range of sub-pieces*, which is divided from the *range of pieces (first to last)*, basis on their
                 * {@link ParallelSystem.getPerformance performance indices}.
                 *
                 * - If indices of pieces are 0 to 50,
                 * - The sub-pieces will be allocated such below:
                 *
                 * Name    | Performance index | Range of sub-pieces to be allocated | Formula
                 * --------|-------------------|-------------------------------------|------------------------
                 * Snail   |                 1 |                            ( 0,  5] | (50 - 0) / 10 * 1
                 * Cheetah |                 4 |                            ( 5, 25] | (50 - 0) / 10 * 4 + 5
                 * Rabbit  |                 3 |                            (25, 40] | (50 - 0) / 10 * 3 + 25
                 * Turtle  |                 2 |                            (40, 50] | (50 - 0) / 10 * 2 + 40
                 *
                 * When the **parallel process** has completed, then this {@link ParallelSystemArraY} will estimate
                 * {@link ParallelSystem.getPerformance performance indices} of {@link ParallelSystem} objects basis on their
                 * execution time.
                 *
                 * @param invoke An {@link Invoke} message requesting parallel process.
                 * @param first Initial piece's index in a section.
                 * @param last Final piece's index in a section. The range used is [*first*, *last*), which contains
                 *			   all the pieces' indices between *first* and *last*, including the piece pointed by index
                 *			   *first*, but not the piece pointed by the index *last*.
                 *
                 * @return Number of {@link ParallelSystem slave systems} participating in the *Parallel Process*.
                 *
                 * @see {@link sendSegmentData}, {@link ParallelSystem.getPerformacen}
                 */
                ParallelSystemArray.prototype.sendPieceData = function (invoke, first, last) {
                    if (invoke.has("_History_uid") == false)
                        invoke.push_back(new samchon.protocol.InvokeParameter("_History_uid", ++this.history_sequence_));
                    else {
                        // INVOKE MESSAGE ALREADY HAS ITS OWN UNIQUE ID
                        //	- THIS IS A TYPE OF ParallelSystemArrayMediator. THE MESSAGE HAS COME FROM ITS MASTER
                        //	- A ParallelSystem HAS DISCONNECTED. THE SYSTEM SHIFTED ITS CHAIN TO OTHER SLAVES.
                        var uid = invoke.get("_History_uid").getValue();
                        // FOR CASE 1. UPDATE HISTORY_SEQUENCE TO MAXIMUM
                        if (uid > this.history_sequence_)
                            this.history_sequence_ = uid;
                    }
                    var segment_size = last - first; // TOTAL NUMBER OF PIECES TO DIVIDE
                    var candidate_systems = new std.Vector(); // SYSTEMS TO BE GET DIVIDED PROCESSES
                    var participants_count = 0;
                    // POP EXCLUDEDS
                    for (var i = 0; i < this.size(); i++)
                        if (this.at(i).exclude_ == false)
                            candidate_systems.push(this.at(i));
                    // ORDERS
                    for (var i = 0; i < candidate_systems.size(); i++) {
                        var system = candidate_systems.at(i);
                        // COMPUTE FIRST AND LAST INDEX TO ALLOCATE
                        var piece_size = (i == candidate_systems.size() - 1)
                            ? segment_size - first
                            : Math.floor(segment_size / candidate_systems.size() * system.getPerformance());
                        if (piece_size == 0)
                            continue;
                        // SEND DATA WITH PIECES' INDEXES
                        system._Send_piece_data(invoke, first, first + piece_size);
                        first += piece_size; // FOR THE NEXT STEP
                        participants_count++;
                    }
                    return participants_count;
                };
                /* ---------------------------------------------------------
                    PERFORMANCE ESTIMATION
                --------------------------------------------------------- */
                /**
                 * @hidden
                 */
                ParallelSystemArray.prototype._Complete_history = function (history) {
                    // WRONG TYPE
                    if ((history instanceof parallel.PRInvokeHistory) == false)
                        return false;
                    var uid = history.getUID();
                    // ALL THE SUB-TASKS ARE DONE?
                    for (var i = 0; i < this.size(); i++)
                        if (this.at(i).progress_list_.has(uid) == true)
                            return false; // IT'S ON A PROCESS IN SOME SYSTEM.
                    //--------
                    // RE-CALCULATE PERFORMANCE INDEX
                    //--------
                    // CONSTRUCT BASIC DATA
                    var system_pairs = new std.Vector();
                    var performance_index_average = 0.0;
                    for (var i = 0; i < this.size(); i++) {
                        var system = this.at(i);
                        if (system.history_list_.has(uid) == false)
                            continue; // NO HISTORY (HAVE NOT PARTICIPATED IN THE PARALLEL PROCESS)
                        // COMPUTE PERFORMANCE INDEX BASIS ON EXECUTION TIME OF THIS PARALLEL PROCESS
                        var my_history = system.history_list_.get(uid);
                        var performance_index = my_history.computeSize() / my_history.computeElapsedTime();
                        // PUSH TO SYSTEM PAIRS AND ADD TO AVERAGE
                        system_pairs.push_back(std.make_pair(system, performance_index));
                        performance_index_average += performance_index;
                    }
                    performance_index_average /= system_pairs.size();
                    // RE-CALCULATE PERFORMANCE INDEX
                    for (var i = 0; i < system_pairs.size(); i++) {
                        // SYSTEM AND NEW PERFORMANCE INDEX BASIS ON THE EXECUTION TIME
                        var system = system_pairs.at(i).first;
                        if (system.enforced_ == true)
                            continue; // PERFORMANCE INDEX IS ENFORCED. DOES NOT PERMIT REVALUATION
                        var new_performance = system_pairs.at(i).second / performance_index_average;
                        // DEDUCT RATIO TO REFLECT THE NEW PERFORMANCE INDEX -> MAXIMUM: 30%
                        var ordinary_ratio = void 0;
                        if (system.history_list_.size() < 2)
                            ordinary_ratio = .3;
                        else
                            ordinary_ratio = Math.min(0.7, 1.0 / (system.history_list_.size() - 1.0));
                        // DEFINE NEW PERFORMANCE
                        system.setPerformance((system.getPerformance() * ordinary_ratio) + (new_performance * (1 - ordinary_ratio)));
                    }
                    // AT LAST, NORMALIZE PERFORMANCE INDEXES OF ALL SYSTEMS
                    this._Normalize_performance();
                    return true;
                };
                /**
                 * @hidden
                 */
                ParallelSystemArray.prototype._Normalize_performance = function () {
                    // COMPUTE AVERAGE
                    var average = 0.0;
                    var denominator = 0;
                    for (var i = 0; i < this.size(); i++) {
                        var system = this.at(i);
                        if (system.enforced_ == true)
                            continue; // PERFORMANCE INDEX IS ENFORCED. DOES NOT PERMIT REVALUATION
                        average += system.getPerformance();
                        denominator++;
                    }
                    average /= denominator;
                    // DIVIDE FROM THE AVERAGE
                    for (var i = 0; i < this.size(); i++) {
                        var system = this.at(i);
                        if (system.enforced_ == true)
                            continue; // PERFORMANCE INDEX IS ENFORCED. DOES NOT PERMIT REVALUATION
                        system.setPerformance(system.getPerformance() / average);
                    }
                };
                return ParallelSystemArray;
            }(templates.external.ExternalSystemArray));
            parallel.ParallelSystemArray = ParallelSystemArray;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../parallel/ParallelSystemArray.ts" />
/**
 * A template for Distributed Processing System.
 *
 * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
 * @author Jeongho Nam <http://samchon.org>
 */
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var distributed;
        (function (distributed) {
            /**
             * Master of Distributed Processing System.
             *
             * The {@link DistributedSystemArray} is an abstract class containing and managing remote distributed **slave** system
             * drivers, {@link DistributedSystem} objects. Within framework of network, {@link DistributedSystemArray} represents
             * your system, a **Master** of *Distributed Processing System* that requesting *distributed process* to **slave**
             * systems and the children {@link DistributedSystem} objects represent the remote **slave** systems, who is being
             * requested the *distributed processes*.
             *
             * You can specify this {@link DistributedSystemArray} class to be *a server accepting distributed clients* or
             * *a client connecting to distributed servers*. Even both of them is possible. Extends one of them below and overrides
             * abstract factory method(s) creating the child {@link DistributedSystem} object.
             *
             * - {@link DistributedClientArray}: A server accepting {@link DistributedSystem distributed clients}.
             * - {@link DistributedServerArray}: A client connecting to {@link DistributedServer distributed servers}.
             * - {@link DistributedServerClientArray}: Both of them. Accepts {@link DistributedSystem distributed clients} and
             *   connects to {@link DistributedServer distributed servers} at the same time.
             *
             * The {@link DistributedSystemArray} contains {@link DistributedProcess} objects directly. You can request a
             * **distributed process** through the {@link DistributedProcess} object. You can access the
             * {@link DistributedProcess} object(s) with those methods:
             *
             * - {@link hasProcess}
             * - {@link getProcess}
             * - {@link insertProcess}
             * - {@link eraseProcess}
             * - {@link getProcessMap}
             *
             * When you need the **distributed process**, call the {@link DistributedProcess.sendData} method. Then the
             * {@link DistributedProcess} will find the most idle {@link DistributedSystem} object who represents a distributed
             * **slave **system. The {@link Invoke} message will be sent to the most idle {@link DistributedSystem} object. When
             * the **distributed process** has completed, then {@link DistributedSystem.getPerformance performance index} and
             * {@link DistributedProcess.getResource resource index} of related objects will be revaluated.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Parallel Process
             * This {@link DistributedSystemArray} class is derived from the {@link ParallelSystemArray} class, so you can request
             * a **parallel process**, too.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices will
             * be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * #### Proxy Pattern
             * This class {@link DistributedSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link DistributedSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var DistributedSystemArray = (function (_super) {
                __extends(DistributedSystemArray, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function DistributedSystemArray() {
                    var _this = _super.call(this) || this;
                    // CREATE ROLE MAP AND ENROLL COLLECTION EVENT LISTENRES
                    _this.process_map_ = new std.HashMap();
                    return _this;
                }
                /**
                 * @inheritdoc
                 */
                DistributedSystemArray.prototype.construct = function (xml) {
                    //--------
                    // CONSTRUCT ROLES
                    //--------
                    // CLEAR ORDINARY ROLES
                    this.process_map_.clear();
                    // CREATE ROLES
                    if (xml.has("processes") == true && xml.get("processes").front().has("process") == true) {
                        var role_xml_list = xml.get("processes").front().get("process");
                        for (var i = 0; i < role_xml_list.size(); i++) {
                            var role_xml = role_xml_list.at(i);
                            // CONSTRUCT ROLE FROM XML
                            var process_1 = this.createProcess(role_xml);
                            process_1.construct(role_xml);
                            // AND INSERT TO ROLE_MAP
                            this.process_map_.insert([process_1.getName(), process_1]);
                        }
                    }
                    //--------
                    // CONSTRUCT SYSTEMS
                    //--------
                    _super.prototype.construct.call(this, xml);
                };
                /* ---------------------------------------------------------
                    ACCESSORS
                --------------------------------------------------------- */
                /**
                 * Get process map.
                 *
                 * Gets an {@link HashMap} containing {@link DistributedProcess} objects with their *key*.
                 *
                 * @return An {@link HasmMap> containing pairs of string and {@link DistributedProcess} object.
                 */
                DistributedSystemArray.prototype.getProcessMap = function () {
                    return this.process_map_;
                };
                /**
                 * Test whether the process exists.
                 *
                 * @param name Name, identifier of target {@link DistributedProcess process}.
                 *
                 * @return Whether the process has or not.
                 */
                DistributedSystemArray.prototype.hasProcess = function (name) {
                    return this.process_map_.has(name);
                };
                /**
                 * Get a process.
                 *
                 * @param name Name, identifier of target {@link DistributedProcess process}.
                 *
                 * @return The specified process.
                 */
                DistributedSystemArray.prototype.getProcess = function (name) {
                    return this.process_map_.get(name);
                };
                /**
                 * Insert a process.
                 *
                 * @param process A process to be inserted.
                 * @return Success flag.
                 */
                DistributedSystemArray.prototype.insertProcess = function (process) {
                    return this.process_map_.insert([process.getName(), process]).second;
                };
                /**
                 * Erase a process.
                 *
                 * @param name Name, identifier of target {@link DistributedProcess process}.
                 */
                DistributedSystemArray.prototype.eraseProcess = function (name) {
                    var prev_size = this.process_map_.size();
                    return (this.process_map_.erase(name) != prev_size);
                };
                /* ---------------------------------------------------------
                    HISTORY HANDLER - PERFORMANCE ESTIMATION
                --------------------------------------------------------- */
                /**
                 * @hidden
                 */
                DistributedSystemArray.prototype._Complete_history = function (history) {
                    if (history instanceof distributed.DSInvokeHistory) {
                        //--------
                        // DistributedProcess's history -> DSInvokeHistory
                        //--------
                        // NO ROLE, THEN FAILED TO COMPLETE
                        if (history.getProcess() == null)
                            return false;
                        // ESTIMATE PERFORMANCE INDEXES
                        this.estimate_system_performance(history); // ESTIMATE SYSTEMS' INDEX
                        this.estimate_process_resource(history); // ESTIMATE PROCESS' RESOURCE
                        // AT LAST, NORMALIZE PERFORMANCE INDEXES OF ALL SYSTEMS AND ROLES
                        this._Normalize_performance();
                        return true;
                    }
                    else {
                        // ParallelSystem's history -> PRInvokeHistory
                        return _super.prototype._Complete_history.call(this, history);
                    }
                };
                /**
                 * @hidden
                 */
                DistributedSystemArray.prototype.estimate_process_resource = function (history) {
                    var process = history.getProcess();
                    if (process.enforced_ == true)
                        return; // THE RESOURCE INDEX IS ENFORCED. DO NOT PERMIT REVALUATION
                    var average_elapsed_time_of_others = 0;
                    var denominator = 0;
                    // COMPUTE AVERAGE ELAPSED TIME
                    for (var it = this.process_map_.begin(); !it.equals(this.process_map_.end()); it = it.next()) {
                        var my_process = it.second;
                        if (my_process == history.getProcess() || my_process.history_list_.empty() == true)
                            continue;
                        average_elapsed_time_of_others += my_process._Compute_average_elapsed_time() * my_process.getResource();
                        denominator++;
                    }
                    // COMPARE WITH THIS HISTORY'S ELAPSED TIME
                    if (denominator != 0) {
                        // DIVE WITH DENOMINATOR
                        average_elapsed_time_of_others /= denominator;
                        // DEDUCT NEW PERFORMANCE INDEX BASED ON THE EXECUTION TIME
                        //	- ROLE'S PERFORMANCE MEANS; HOW MUCH TIME THE ROLE NEEDS
                        //	- ELAPSED TIME IS LONGER, THEN PERFORMANCE IS HIGHER
                        var elapsed_time = history.computeElapsedTime() / history.getWeight(); // CONSIDER WEIGHT
                        var new_resource = elapsed_time / average_elapsed_time_of_others; // NEW PERFORMANCE
                        // DEDUCT RATIO TO REFLECT THE NEW PERFORMANCE INDEX -> MAXIMUM: 15%
                        var ordinary_ratio = void 0;
                        if (process.history_list_.size() < 2)
                            ordinary_ratio = .15;
                        else
                            ordinary_ratio = Math.min(.85, 1.0 / (process.history_list_.size() - 1.0));
                        // DEFINE NEW PERFORMANCE
                        process.setResource((process.getResource() * ordinary_ratio)
                            + (new_resource * (1 - ordinary_ratio)));
                    }
                };
                /**
                 * @hidden
                 */
                DistributedSystemArray.prototype.estimate_system_performance = function (history) {
                    var system = history.getSystem();
                    if (system.enforced_ == true)
                        return; // THE PERFORMANCE INDEX IS ENFORCED. IT DOESN'T PERMIT REVALUATION
                    var average_elapsed_time_of_others = 0;
                    var denominator = 0;
                    // COMPUTE AVERAGE ELAPSED TIME
                    for (var i = 0; i < this.size(); i++) {
                        var system_1 = this.at(i);
                        var avg = system_1._Compute_average_elapsed_time();
                        if (avg == -1)
                            continue;
                        average_elapsed_time_of_others += avg;
                        denominator++;
                    }
                    // COMPARE WITH THIS HISTORY'S ELAPSED TIME
                    if (denominator != 0) {
                        // DIVE WITH DENOMINATOR
                        average_elapsed_time_of_others /= denominator;
                        // DEDUCT NEW PERFORMANCE INDEX BASED ON THE EXECUTION TIME
                        //	- SYSTEM'S PERFORMANCE MEANS; HOW FAST THE SYSTEM IS
                        //	- ELAPSED TIME IS LOWER, THEN PERFORMANCE IS HIGHER
                        var elapsed_time = history.computeElapsedTime() / history.getWeight(); // CONSIDER WEIGHT
                        var new_performance = average_elapsed_time_of_others / elapsed_time; // NEW PERFORMANCE
                        // DEDUCT RATIO TO REFLECT THE NEW PERFORMANCE INDEX -> MAXIMUM: 30%
                        var ordinary_ratio = void 0;
                        if (system.history_list_.size() < 2)
                            ordinary_ratio = .3;
                        else
                            ordinary_ratio = Math.min(0.7, 1.0 / (system.history_list_.size() - 1.0));
                        // DEFINE NEW PERFORMANCE
                        system.setPerformance((system.getPerformance() * ordinary_ratio)
                            + (new_performance * (1 - ordinary_ratio)));
                    }
                };
                /**
                 * @hidden
                 */
                DistributedSystemArray.prototype._Normalize_performance = function () {
                    // NORMALIZE SYSTEMS' PERFORMANCE INDEXES
                    _super.prototype._Normalize_performance.call(this);
                    // COMPUTE AVERAGE
                    var average = 0.0;
                    var denominator = 0;
                    for (var it = this.process_map_.begin(); !it.equals(this.process_map_.end()); it = it.next()) {
                        var process_2 = it.second;
                        if (process_2.enforced_ == true)
                            continue; // THE RESOURCE INDEX IS ENFORCED. DO NOT PERMIT REVALUATION
                        average += process_2.getResource();
                        denominator++;
                    }
                    average /= denominator;
                    // DIVIDE FROM THE AVERAGE
                    for (var it = this.process_map_.begin(); !it.equals(this.process_map_.end()); it = it.next()) {
                        var process_3 = it.second;
                        if (process_3.enforced_ == true)
                            continue; // THE RESOURCE INDEX IS ENFORCED. DO NOT PERMIT REVALUATION
                        process_3.setResource(process_3.getResource() / average);
                    }
                };
                /* ---------------------------------------------------------
                    EXPORTERS
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                DistributedSystemArray.prototype.toXML = function () {
                    var xml = _super.prototype.toXML.call(this);
                    if (this.process_map_.empty() == true)
                        return xml;
                    var processes_xml = new samchon.library.XML();
                    {
                        processes_xml.setTag("processes");
                        for (var it = this.process_map_.begin(); !it.equals(this.process_map_.end()); it = it.next())
                            processes_xml.push(it.second.toXML());
                    }
                    xml.push(processes_xml);
                    return xml;
                };
                return DistributedSystemArray;
            }(templates.parallel.ParallelSystemArray));
            distributed.DistributedSystemArray = DistributedSystemArray;
        })(distributed = templates.distributed || (templates.distributed = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="DistributedSystemArray".ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var distributed;
        (function (distributed) {
            /**
             * Mediator of Distributed Processing System.
             *
             * The {@link DistributedSystemArrayMediator} class be a master for its slave systems, and be a slave to its master
             * system at the same time. This {@link DistributedSystemArrayMediator} be a master system, containing and managing
             * {@link DistributedSystem} objects, which represent distributed slave systems, by extending
             * {@link DistributedSystemArray} class. Also, be a slave system through {@link getMediator mediator} object, which is
             * derived from the {@link SlaveSystem} class.
             *
             * As a master, you can specify this {@link DistributedSystemArrayMediator} class to be <i>a master server accepting
             * slave clients<i> or <i>a master client to connecting slave servers</i>. Even both of them is possible. Extends one
             * of them below and overrides abstract factory method(s) creating the child {@link DistributedSystem} object.
             *
             * - {@link DistributedClientArrayMediator}: A server accepting {@link DistributedSystem distributed clients}.
             * - {@link DistributedServerArrayMediator}: A client connecting to {@link DistributedServer distributed servers}.
             * - {@link DistributedServerClientArrayMediator}: Both of them. Accepts {@link DistributedSystem distributed clients} and
             *   connects to {@link DistributedServer distributed servers} at the same time.
             *
             * As a slave, you can specify this {@link DistributedSystemArrayMediator} to be <i>a client slave connecting to master
             * server</i> or <i>a server slave accepting master client</i> by overriding the {@link createMediator} method.
             * Overrides the {@link createMediator createMediator()} method and return one of them:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * #### [Inherited] {@link DistributedSystemArray}
             * The {@link DistributedSystemArray} is an abstract class containing and managing remote distributed **slave** system
             * drivers, {@link DistributedSystem} objects. Within framework of network, {@link DistributedSystemArray} represents
             * your system, a **Master** of *Distributed Processing System* that requesting *distributed process* to **slave**
             * systems and the children {@link DistributedSystem} objects represent the remote **slave** systems, who is being
             * requested the *distributed processes*.
             *
             * The {@link DistributedSystemArray} contains {@link DistributedProcess} objects directly. You can request a
             * **distributed process** through the {@link DistributedProcess} object. You can access the
             * {@link DistributedProcess} object(s) with those methods:
             *
             * - {@link hasProcess}
             * - {@link getProcess}
             * - {@link insertProcess}
             * - {@link eraseProcess}
             * - {@link getProcessMap}
             *
             * When you need the **distributed process**, call the {@link DistributedProcess.sendData} method. Then the
             * {@link DistributedProcess} will find the most idle {@link DistributedSystem} object who represents a distributed
             * **slave **system. The {@link Invoke} message will be sent to the most idle {@link DistributedSystem} object. When
             * the **distributed process** has completed, then {@link DistributedSystem.getPerformance performance index} and
             * {@link DistributedProcess.getResource resource index} of related objects will be revaluated.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Parallel Process
             * This {@link DistributedSystemArray} class is derived from the {@link ParallelSystemArray} class, so you can request
             * a **parallel process**, too.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices will
             * be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * #### Proxy Pattern
             * This class {@link DistributedSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link DistributedSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var DistributedSystemArrayMediator = (function (_super) {
                __extends(DistributedSystemArrayMediator, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function DistributedSystemArrayMediator() {
                    var _this = _super.call(this) || this;
                    _this.mediator_ = null;
                    return _this;
                }
                /**
                 * Start mediator.
                 *
                 * If the {@link getMediator mediator} is a type of server, then opens the server accepting master client.
                 * Otherwise, the {@link getMediator mediator} is a type of client, then connects the master server.
                 */
                DistributedSystemArrayMediator.prototype.startMediator = function () {
                    if (this.mediator_ != null)
                        return;
                    this.mediator_ = this.createMediator();
                    this.mediator_.start();
                };
                /* ---------------------------------------------------------
                    ACCESSOR
                --------------------------------------------------------- */
                /**
                 * Get {@link MediatorSystem} object.
                 *
                 * When you need to send an {@link Invoke} message to the master system of this
                 * {@link DistributedSystemArrayMediator}, then send to the {@link MediatorSystem} through this
                 * {@link getMediator}.
                 *
                 * ```typescript
                 * this.getMediator().sendData(...);
                 * ```
                 *
                 * @return The {@link MediatorSystem} object.
                 */
                DistributedSystemArrayMediator.prototype.getMediator = function () {
                    return this.mediator_;
                };
                /* ---------------------------------------------------------
                    MESSAGE CHAIN
                --------------------------------------------------------- */
                /**
                 * @hidden
                 */
                DistributedSystemArrayMediator.prototype._Complete_history = function (history) {
                    var ret = _super.prototype._Complete_history.call(this, history);
                    if (ret == true)
                        this.mediator_._Complete_history(history.getUID());
                    return ret;
                };
                return DistributedSystemArrayMediator;
            }(distributed.DistributedSystemArray));
            distributed.DistributedSystemArrayMediator = DistributedSystemArrayMediator;
        })(distributed = templates.distributed || (templates.distributed = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="../library/XML.ts" />
/// <reference path="../collections/ArrayCollection.ts" />
/// <reference path="../protocol/Entity.ts" />
/// <reference path="../templates/distributed/DistributedSystemArrayMediator.ts" />
try {
    // IF THE CONDITION BE IS_NODE(), THEN CANNOT BE USED IN BROWSERIFY
    module.exports = samchon;
}
catch (exception) { }
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * An abstract, basic class for communicators.
         *
         * {@link CommunicatorBase} is an abstract class implemented from the {@link ICommunicator}. Mechanism of converting
         * raw data to {@link Invoke} messag has realized in this abstract class. Type of this {@link CommunicatorBase} class
         * is specified to as below following which protocol is used.
         *
         * - {@link Communicator}: Samchon Framework's own protocool.
         * - {@link WebCommunicator}: Web-socket protocol
         * - {@link SharedWorkerCommunicator}: SharedWorker's message protocol.
         *
         * #### [Inherited] {@link ICommunicator}
         * {@link ICommunicator} is an interface for communicator classes who take full charge of network communication with
         * remote system, without reference to whether the remote system is a server or a client. Type of the
         * {@link ICommunicator} is specified to {@link IServerConnector} and {@link IClientDriver} whether the remote system
         * is a server (that I've to connect) or a client (a client connected to my server).
         *
         * Whenever a replied message comes from the remote system, the message will be converted to an {@link Invoke} class
         * and the {@link Invoke} object will be shifted to the {@link IProtocol listener}'s
         * {@link IProtocol.replyData IProtocol.replyData()} method.
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link IClientDriver}, {@link IServerConnector}, {@link IProtocol}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#icommunicator)
         * @author Jeongho Nam <http://samchon.org>
         */
        var CommunicatorBase = (function () {
            function CommunicatorBase(listener) {
                if (listener === void 0) { listener = null; }
                // BASIC MEMBERS
                this.listener_ = listener;
                this.onClose = null;
                // BINARY INVOKE MEMBERS
                this.binary_invoke_ = null;
                this.binary_parameters_ = new std.Queue();
                this.unhandled_invokes_ = new std.Deque();
            }
            /* ---------------------------------------------------------
                ACCESSORS
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            CommunicatorBase.prototype.isConnected = function () {
                return this.connected_;
            };
            /**
             * @hidden
             */
            CommunicatorBase.prototype.is_binary_invoke = function () {
                return (this.binary_invoke_ != null);
            };
            /**
             * @inheritdoc
             */
            CommunicatorBase.prototype.replyData = function (invoke) {
                if (this.listener_ == null)
                    this.unhandled_invokes_.push_back(invoke);
                else {
                    if (this.listener_._Reply_data instanceof Function)
                        this.listener_._Reply_data(invoke);
                    else
                        this.listener_.replyData(invoke);
                }
            };
            /**
             * @hidden
             */
            CommunicatorBase.prototype.handle_string = function (str) {
                // REPLIED DATA IS CLEARY BE AN INVOKE MESSAGE
                var invoke = new protocol.Invoke();
                invoke.construct(new samchon.library.XML(str));
                for (var i = 0; i < invoke.size(); i++) {
                    var parameter = invoke.at(i);
                    if (parameter.getType() != "ByteArray")
                        continue;
                    if (this.binary_invoke_ == null)
                        this.binary_invoke_ = invoke; // INIT BINARY_INVOKE
                    this.binary_parameters_.push(parameter); // ENROLL TO PARAMETERS' QUEUE
                }
                // NO BINARY, THEN REPLY DIRECTLY
                if (this.binary_invoke_ == null)
                    this.replyData(invoke);
            };
            /**
             * @hidden
             */
            CommunicatorBase.prototype.handle_binary = function (binary) {
                // FETCH A PARAMETER
                var parameter = this.binary_parameters_.front();
                {
                    parameter.setValue(binary);
                }
                this.binary_parameters_.pop();
                if (this.binary_parameters_.empty() == true) {
                    // NO BINARY PARAMETER LEFT,
                    var invoke = this.binary_invoke_;
                    this.binary_invoke_ = null;
                    // THEN REPLY
                    this.replyData(invoke);
                }
            };
            return CommunicatorBase;
        }());
        protocol.CommunicatorBase = CommunicatorBase;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A communicator following Samchon Framework's own protocol.
         *
         * {@link Communicator} is an abstract class following Samchon Framework's own protocol. This {@link Communicator}
         * class is specified to {@link ServerConnector} and {@link ClientDriver} whether the remote system is a server (that
         * my system is connecting to) or a client (a client conneting to to my server).
         *
         * Note that, if one of this or remote system is web-browser based, then you don't have to use this
         * {@link Communicator} class who follows Samchon Framework's own protocol. Web-browser supports only Web-socket
         * protocol. Thus in that case, you have to use {@link WebCommunicator} instead.
         *
         * #### [Inherited] {@link ICommunicator}
         * {@link ICommunicator} is an interface for communicator classes who take full charge of network communication with
         * remote system, without reference to whether the remote system is a server or a client. Type of the
         * {@link ICommunicator} is specified to {@link IServerConnector} and {@link IClientDriver} whether the remote system
         * is a server (that I've to connect) or a client (a client connected to my server).
         *
         * Whenever a replied message comes from the remote system, the message will be converted to an {@link Invoke} class
         * and the {@link Invoke} object will be shifted to the {@link IProtocol listener}'s
         * {@link IProtocol.replyData IProtocol.replyData()} method.
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link ClientDriver}, {@link ServerConnector}, {@link IProtocol}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#icommunicator)
         * @author Jeongho Nam <http://samchon.org>
         */
        var Communicator = (function (_super) {
            __extends(Communicator, _super);
            function Communicator() {
                var _this = _super.apply(this, arguments) || this;
                // SOCKET AND RECEIVED DATA
                /**
                 * @hidden
                 */
                _this.socket_ = null;
                /**
                 * @hidden
                 */
                _this.header_bytes_ = null;
                /**
                 * @hidden
                 */
                _this.data_ = null;
                /**
                 * @hidden
                 */
                _this.data_index_ = -1;
                /**
                 * @hidden
                 */
                _this.listening_ = false;
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /**
             * @inheritdoc
             */
            Communicator.prototype.close = function () {
                this.socket_.end();
            };
            /**
             * @hidden
             */
            Communicator.prototype.start_listen = function () {
                if (this.listening_ == true)
                    return;
                this.listening_ = true;
                this.socket_.on("data", this._Listen_piece.bind(this));
                this.socket_.on("error", this._Handle_error.bind(this));
                this.socket_.on("end", this._Handle_close.bind(this));
                this.socket_.on("close", this._Handle_close.bind(this));
            };
            /**
             * @hidden
             */
            Communicator.prototype._Handle_error = function () {
                // WHEN ERROR EXISTS, CLOSE EVENT ALSO BEING DISPATCHED
                // THUS, ANYTHING NEED TO DO
            };
            /**
             * @hidden
             */
            Communicator.prototype._Handle_close = function () {
                this.connected_ = false;
                if (this.onClose != null)
                    this.onClose();
            };
            /* =========================================================
                SEND & REPLY DATA
                    - INVOKE MESSAGE CHAIN
                    - LISTENERS
            ============================================================
                INVOKE MESSAGE CHAIN
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            Communicator.prototype.sendData = function (invoke) {
                ////////
                // SEND STRING
                ////////
                var str_header = new Buffer(8);
                var str = invoke.toXML().toString();
                // WRITE CONTENT SIZE TO HEADER BUFFER
                str_header.writeUInt32BE(0, 0);
                str_header.writeUInt32BE(Buffer.byteLength(str, "utf8"), 4);
                this.socket_.write(str_header); // SEND SIZE HEADER
                this.socket_.write(str, "utf8"); // TEXT IS AFTER
                for (var i = 0; i < invoke.size(); i++) {
                    var parameter = invoke.at(i);
                    if (parameter.getType() != "ByteArray")
                        continue;
                    ////////
                    // SEND BINARY
                    ////////
                    var binary_header = new Buffer(8);
                    var binary = parameter.getValue();
                    binary_header.writeUInt32BE(0, 0);
                    binary_header.writeUInt32BE(binary.byteLength, 4);
                    this.socket_.write(binary_header); // SEND SIZE HEADER
                    this.socket_.write(binary); // BINARY IS AFTER
                }
            };
            /* ---------------------------------------------------------
                LISTENERS
            --------------------------------------------------------- */
            /**
             * @hidden
             */
            Communicator.prototype._Listen_piece = function (piece) {
                // DETERMINE WHICH TO LISTEN
                if (this.data_ == null)
                    this._Listen_header(piece, 0);
                else
                    this._Listen_data(piece, 0);
            };
            /**
             * @hidden
             */
            Communicator.prototype._Listen_header = function (piece, piece_index) {
                if (this.header_bytes_ != null) {
                    // ATTACH RESERVED HEADER BYTE TO PIECE
                    this.header_bytes_.copy(piece, piece_index, // FRONT OF THE PIECE
                    0, this.header_bytes_.byteLength // ALL BYTES FROM this.header_bytes
                    );
                    this.header_bytes_ = null; // TRUNCATE
                }
                if (piece_index > piece.byteLength - 8) {
                    // IF LEFT BYTES ARE UNDER 8, THEN RESERVE THE LEFT BYTES
                    this.header_bytes_ = new Buffer(8);
                    piece.copy(this.header_bytes_, 0, // TO THE NEWLY CREATED HEADER
                    piece_index, piece.byteLength - piece_index // LEFT BYTES
                    );
                    return;
                }
                // READ CONTENT SIZE AND INIT DATA
                var content_size = piece.readUInt32BE(piece_index + 4);
                piece_index += 8;
                if (content_size != 0) {
                    this.data_ = new Buffer(content_size);
                    this.data_index_ = 0;
                }
                // IF LEFT BYTES ARE, THEN LISTEN DATA
                if (piece_index < piece.byteLength)
                    if (content_size != 0)
                        this._Listen_data(piece, piece_index);
                    else
                        this._Listen_header(piece, piece_index);
            };
            /**
             * @hidden
             */
            Communicator.prototype._Listen_data = function (piece, piece_index) {
                // BYTES TO INSERT
                var inserted_bytes = Math.min(this.data_.byteLength - this.data_index_, // LEFT BYTES TO FILL
                piece.byteLength - piece_index // LEFT BYTES IN THE PIECE
                );
                // INSERT PIECE TO THE DATA
                piece.copy(this.data_, this.data_index_, // COPY TO THE DATA,
                piece_index, piece_index + inserted_bytes // LEFT BYTES OF THE PIECE OR FILL
                );
                this.data_index_ += inserted_bytes; // INCREASE OFFSET
                piece_index += inserted_bytes; // INCREASE OFFSET
                if (this.data_index_ == this.data_.byteLength) {
                    /////
                    // THE DATA IS FULLY FILLED
                    /////
                    if (this.is_binary_invoke() == false)
                        this.handle_string(this.data_.toString());
                    else
                        this.handle_binary(this.data_);
                    // TRUNCATE DATA
                    this.data_ = null;
                    this.data_index_ = -1;
                }
                // THE PIECE IS NOT EXHAUSTED, THEN CONTINUE READING
                if (piece_index < piece.byteLength)
                    this._Listen_header(piece, piece_index);
            };
            return Communicator;
        }(protocol.CommunicatorBase));
        protocol.Communicator = Communicator;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A communicator following Web-socket protocol.
         *
         * {@link WebCommunicator} is an abstract class following Web-socket protocol. This {@link WebCommunicator} class is
         * specified to {@link WebServerConnector} and {@link WebClientDriver} whether the remote system is a server (that my
         * system is connecting to) or a client (a client conneting to to my server).
         *
         * Note that, one of this or remote system is web-browser based, then there's not any alternative choice. Web browser
         * supports only Web-socket protocol. In that case, you've use this {@link WebCommunicator} class.
         *
         * #### [Inherited] {@link ICommunicator}
         * {@link ICommunicator} is an interface for communicator classes who take full charge of network communication with
         * remote system, without reference to whether the remote system is a server or a client. Type of the
         * {@link ICommunicator} is specified to {@link IServerConnector} and {@link IClientDriver} whether the remote system
         * is a server (that I've to connect) or a client (a client connected to my server).
         *
         * Whenever a replied message comes from the remote system, the message will be converted to an {@link Invoke} class
         * and the {@link Invoke} object will be shifted to the {@link IProtocol listener}'s
         * {@link IProtocol.replyData IProtocol.replyData()} method.
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link WebClientDriver}, {@link WebServerConnector}, {@link IProtocol}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#icommunicator)
         * @author Jeongho Nam <http://samchon.org>
         */
        var WebCommunicator = (function (_super) {
            __extends(WebCommunicator, _super);
            function WebCommunicator() {
                var _this = _super.apply(this, arguments) || this;
                // SOCKET MEMBER
                /**
                 * @hidden
                 */
                _this.connection_ = null;
                return _this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /**
             * @inheritdoc
             */
            WebCommunicator.prototype.close = function () {
                this.connection_.close();
            };
            /* ---------------------------------------------------------
                INVOKE MESSAGE I/O
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            WebCommunicator.prototype.sendData = function (invoke) {
                this.connection_.sendUTF(invoke.toXML().toString());
                for (var i = 0; i < invoke.size(); i++)
                    if (invoke.at(i).getType() == "ByteArray")
                        this.connection_.sendBytes(invoke.at(i).getValue());
            };
            /**
             * @hidden
             */
            WebCommunicator.prototype.handle_message = function (message) {
                // EXCEPTION HANDLING IS REQUIRED
                //	- BINARY_INVOKE EXISTS, BUT TEXT MESSAGE HAS COME
                //	- BINARY_INVOKE IS NOT, BUT BINARY MESSAGE HAS COME
                if (message.type == "utf8")
                    this.handle_string(message.utf8Data);
                else
                    this.handle_binary(message.binaryData);
            };
            /**
             * @hidden
             */
            WebCommunicator.prototype.handle_close = function () {
                this.connected_ = false;
                if (this.onClose != null)
                    this.onClose();
            };
            return WebCommunicator;
        }(protocol.CommunicatorBase));
        protocol.WebCommunicator = WebCommunicator;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A communicator for shared worker.
         *
         * {@link DedicatedWorkerCommunicator} is an abstract class for communication between DedicatedWorker and Web-browser.
         * This {@link DedicatedWorkerCommunicator} is specified to {@link DedicatedWorkerServerConnector} and
         * {@link DedicatedWorkerClientDriver} whether the remote system is a server (that my system is connecting to) or a
         * client (a client conneting to to my server).
         *
         * #### Why DedicatedWorker be a server?
         * In JavaScript environment, there's no way to implement multi-threading function. Instead, JavaScript supports the
         * **Worker**, creating a new process. However, the **Worker** does not shares memory addresses. To integrate the
         * **Worker** with its master, only communication with string or binary data is allowed. Doesn't it seem like a network
         * communication? Furthermore, there's not any difference between the worker communication and network communication.
         * It's the reason why Samchon Framework considers the **Worker** as a network node.
         *
         * The class {@link DedicatedWorkerCommunicator} is designed make such relationship. From now on, DedicatedWorker is a
         * {@link DedicatedWorkerServer server} and {@link DedicatedWorkerServerConnector browser} is a client. Integrate the
         * server and clients with this {@link DedicatedWorkerCommunicator}.
         *
         * #### [Inherited] {@link ICommunicator}
         * {@link ICommunicator} is an interface for communicator classes who take full charge of network communication with
         * remote system, without reference to whether the remote system is a server or a client. Type of the
         * {@link ICommunicator} is specified to {@link IServerConnector} and {@link IClientDriver} whether the remote system
         * is a server (that I've to connect) or a client (a client connected to my server).
         *
         * Whenever a replied message comes from the remote system, the message will be converted to an {@link Invoke} class
         * and the {@link Invoke} object will be shifted to the {@link IProtocol listener}'s
         * {@link IProtocol.replyData IProtocol.replyData()} method.
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link DedicatedWorkerClientDriver}, {@link DedicatedWorkerServerConnector}, {@link IProtocol}
         * @reference https://developer.mozilla.org/en-US/docs/Web/API/DedicatedWorker
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#icommunicator)
         * @author Jeongho Nam <http://samchon.org>
         */
        var DedicatedWorkerCommunicator = (function (_super) {
            __extends(DedicatedWorkerCommunicator, _super);
            function DedicatedWorkerCommunicator() {
                return _super.apply(this, arguments) || this;
            }
            /**
             * @hidden
             */
            DedicatedWorkerCommunicator.prototype.handle_message = function (event) {
                if (this.is_binary_invoke() == false)
                    this.handle_string(event.data);
                else
                    this.handle_binary(event.data);
            };
            return DedicatedWorkerCommunicator;
        }(protocol.CommunicatorBase));
        protocol.DedicatedWorkerCommunicator = DedicatedWorkerCommunicator;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A communicator for shared worker.
         *
         * {@link SharedWorkerCommunicator} is an abstract class for communication between SharedWorker and Web-browser. This
         * {@link SharedWorkerCommunicator} is specified to {@link SharedWorkerServerConnector} and
         * {@link SharedWorkerClientDriver} whether the remote system is a server (that my system is connecting to) or a client
         * (a client conneting to to my server).
         *
         * Note that, SharedWorker is a conception only existed in web-browser. This {@link SharedWorkerCommunicator} is not
         * supported in NodeJS. Only web-browser environment can utilize this {@link SharedWorkerCommunicator}.
         *
         * #### Why SharedWorker be a server?
         * SharedWorker, it allows only an instance (process) to be created whether the SharedWorker is declared in a browser
         * or multiple browsers. To integrate them, messages are being sent and received. Doesn't it seem like a relationship
         * between a server and clients? Thus, Samchon Framework consider the SharedWorker as a server and browsers as
         * clients.
         *
         * The class {@link SharedWorkerCommunicator} is designed make such relationship. From now on, SharedWorker is a
         * {@link SharedWorkerServer server} and {@link SharedWorkerServerConnector browsers} are clients. Integrate the
         * server and clients with this {@link SharedWorkerCommunicator}.
         *
         * #### [Inherited] {@link ICommunicator}
         * {@link ICommunicator} is an interface for communicator classes who take full charge of network communication with
         * remote system, without reference to whether the remote system is a server or a client. Type of the
         * {@link ICommunicator} is specified to {@link IServerConnector} and {@link IClientDriver} whether the remote system
         * is a server (that I've to connect) or a client (a client connected to my server).
         *
         * Whenever a replied message comes from the remote system, the message will be converted to an {@link Invoke} class
         * and the {@link Invoke} object will be shifted to the {@link IProtocol listener}'s
         * {@link IProtocol.replyData IProtocol.replyData()} method.
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link SharedWorkerClientDriver}, {@link SharedWorkerServerConnector}, {@link IProtocol}
         * @reference https://developer.mozilla.org/en-US/docs/Web/API/SharedWorker
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#icommunicator)
         * @author Jeongho Nam <http://samchon.org>
         */
        var SharedWorkerCommunicator = (function (_super) {
            __extends(SharedWorkerCommunicator, _super);
            function SharedWorkerCommunicator() {
                return _super.apply(this, arguments) || this;
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
            /**
             * @inheritdoc
             */
            SharedWorkerCommunicator.prototype.close = function () {
                this.connected_ = false;
                this.port_.close();
                if (this.onClose != null)
                    this.onClose();
            };
            /* ---------------------------------------------------------
                INVOKE MESSAGE I/O
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            SharedWorkerCommunicator.prototype.sendData = function (invoke) {
                this.port_.postMessage(invoke.toXML().toString());
                for (var i = 0; i < invoke.size(); i++)
                    if (invoke.at(i).getType() == "ByteaArray")
                        this.port_.postMessage(invoke.at(i).getValue());
            };
            /**
             * @hidden
             */
            SharedWorkerCommunicator.prototype.handle_message = function (event) {
                if (this.is_binary_invoke() == false)
                    this.handle_string(event.data);
                else
                    this.handle_binary(event.data);
            };
            return SharedWorkerCommunicator;
        }(protocol.CommunicatorBase));
        protocol.SharedWorkerCommunicator = SharedWorkerCommunicator;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="Communicator.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * Communicator with remote client.
         *
         * {@link ClientDriver} is a class taking full charge of network communication with remote client who follows Samchon
         * Framework's own protocol. This {@link ClientDriver} object is always created by {@link Server} class. When you got
         * this {@link ClientDriver} object from the {@link Server.addClient Server.addClient()}, then specify
         * {@link IProtocol listener} with the {@link ClientDriver.listen ClientDriver.listen()} method.
         *
         * #### [Inherited] {@link IClientDriver}
         * {@link IClientDriver} is a type of {@link ICommunicator}, specified for communication with remote client who has
         * connected in a {@link IServer server}. It takes full charge of network communication with the remote client.
         *
         * The {@link IClientDriver} object is created and delivered from {@link IServer} and
         * {@link IServer.addClient IServer.addClient()}. Those are derived types from this {@link IClientDriver}, being
         * created by the matched {@link IServer} object.
         *
         * Protocol                | Derived Type                        | Created By
         * ------------------------|-------------------------------------|----------------------------
         * Samchon Framework's own | {@link ClientDriver}                | {@link Server}
         * Web-socket protocol     | {@link WebClientDriver}             | {@link WebServer}
         * DedicatedWorker         | {@link DedicatedWorkerClinetDriver} | {@link DedicatedWorkerServer}
         * SharedWorker            | {@link SharedWorkerClientDriver}    | {@link SharedWorkerServer}
         *
         * When you've got an {@link IClientDriver} object from the {@link IServer.addClient IServer.addClient()}, then
         * specify {@link IProtocol listener} with {@link IClient.listen IClient.listen()}. Whenever a replied message comes
         * from the remote system, the message will be converted to an {@link Invoke} class and the {@link Invoke} object
         * will be shifted to the {@link IProtocol listener}'s {@link IProtocol.replyData IProtocol.replyData()} method.
         * Below code is an example specifying and managing the {@link IProtocol listener} objects.
         *
         * - https://github.com/samchon/framework/blob/master/ts/examples/calculator/calculator-server.ts
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link Server}, {@link IProtocol}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iclientdriver)
         * @author Jeongho Nam <http://samchon.org>
         */
        var ClientDriver = (function (_super) {
            __extends(ClientDriver, _super);
            /**
             * Construct from a socket.
             */
            function ClientDriver(socket) {
                var _this = _super.call(this) || this;
                _this.socket_ = socket;
                _this.connected_ = true;
                return _this;
            }
            /**
             * @inheritdoc
             */
            ClientDriver.prototype.listen = function (listener) {
                this.listener_ = listener;
                this.start_listen();
            };
            return ClientDriver;
        }(protocol.Communicator));
        protocol.ClientDriver = ClientDriver;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * Communicator with remote web-client.
         *
         * {@link WebClientDriver} is a class taking full charge of network communication with remote client who follows
         * Web-socket protocol. This {@link WebClientDriver} object is always created by {@link WebServer} class. When you
         * got this {@link WebClientDriver} object from the {@link WebServer.addClient WebServer.addClient()}, then specify
         * {@link IProtocol listener} with the {@link WebClientDriver.listen WebClientDriver.listen()} method.
         *
         * Unlike other protocol, Web-socket protocol's clients notify two parameters on their connection;
         * {@link getSessionID session-id} and {@link getPath path}. The {@link getSessionID session-id} can be used to
         * identify *user* of each client, and the {@link getPath path} can be used which type of *service* that client wants.
         * In {@link service} module, you can see the best utilization case of them.
         * - {@link service.User}: utlization of the {@link getSessionID session-id}.
         * - {@link service.Service}: utilization of the {@link getPath path}.
         *
         * #### [Inherited] {@link IClientDriver}
         * {@link IClientDriver} is a type of {@link ICommunicator}, specified for communication with remote client who has
         * connected in a {@link IServer server}. It takes full charge of network communication with the remote client.
         *
         * The {@link IClientDriver} object is created and delivered from {@link IServer} and
         * {@link IServer.addClient IServer.addClient()}. Those are derived types from this {@link IClientDriver}, being
         * created by the matched {@link IServer} object.
         *
         * Protocol                | Derived Type                        | Created By
         * ------------------------|-------------------------------------|----------------------------
         * Samchon Framework's own | {@link ClientDriver}                | {@link Server}
         * Web-socket protocol     | {@link WebClientDriver}             | {@link WebServer}
         * DedicatedWorker         | {@link DedicatedWorkerClinetDriver} | {@link DedicatedWorkerServer}
         * SharedWorker            | {@link SharedWorkerClientDriver}    | {@link SharedWorkerServer}
         *
         * When you've got an {@link IClientDriver} object from the {@link IServer.addClient IServer.addClient()}, then
         * specify {@link IProtocol listener} with {@link IClient.listen IClient.listen()}. Whenever a replied message comes
         * from the remote system, the message will be converted to an {@link Invoke} class and the {@link Invoke} object
         * will be shifted to the {@link IProtocol listener}'s {@link IProtocol.replyData IProtocol.replyData()} method.
         * Below code is an example specifying and managing the {@link IProtocol listener} objects.
         *
         * - https://github.com/samchon/framework/blob/master/ts/examples/calculator/calculator-server.ts
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link WebServer}, {@link IProtocol}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iclientdriver)
         * @author Jeongho Nam <http://samchon.org>
         */
        var WebClientDriver = (function (_super) {
            __extends(WebClientDriver, _super);
            /**
             * Initialization Constructor.
             *
             * @param connection Connection driver, a socket for web-socket.
             * @param path Requested path.
             * @param session_id Session ID, an identifier of the remote client.
             */
            function WebClientDriver(connection, path, session_id) {
                var _this = _super.call(this) || this;
                _this.connection_ = connection;
                _this.path_ = path;
                _this.session_id_ = session_id;
                _this.listening_ = false;
                return _this;
            }
            /**
             * @inheritdoc
             */
            WebClientDriver.prototype.listen = function (listener) {
                this.listener_ = listener;
                if (this.listening_ == true)
                    return;
                this.listening_ = true;
                this.connection_.on("message", this.handle_message.bind(this));
                this.connection_.on("close", this.handle_close.bind(this));
                this.connection_.on("error", this.handle_close.bind(this));
            };
            /**
             * Get requested path.
             */
            WebClientDriver.prototype.getPath = function () {
                return this.path_;
            };
            /**
             * Get session ID, an identifier of the remote client.
             */
            WebClientDriver.prototype.getSessionID = function () {
                return this.session_id_;
            };
            return WebClientDriver;
        }(protocol.WebCommunicator));
        protocol.WebClientDriver = WebClientDriver;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * Communicator with master web-browser.
         *
         * {@link DedicatedWorkerClientDriver} is a class taking full charge of network communication with web browsers. This
         * {@link DedicatedWorkerClientDriver} object is always created by {@link DedicatedWorkerServer} class. When you got
         * this {@link DedicatedWorkerClientDriver} object from
         * {@link DedicatedWorkerServer.addClient DedicatedWorkerServer.addClient()}, then specify {@link IProtocol listener}
         * with the {@link DedicatedWorkerClientDriver.listen DedicatedWorkerClientDriver.listen()} method.
         *
         * #### Why DedicatedWorker be a server?
         * In JavaScript environment, there's no way to implement multi-threading function. Instead, JavaScript supports the
         * **Worker**, creating a new process. However, the **Worker** does not shares memory addresses. To integrate the
         * **Worker** with its master, only communication with string or binary data is allowed. Doesn't it seem like a network
         * communication? Furthermore, there's not any difference between the worker communication and network communication.
         * It's the reason why Samchon Framework considers the **Worker** as a network node.
         *
         * The class {@link DedicatedWorkerCommunicator} is designed make such relationship. From now on, DedicatedWorker is a
         * {@link DedicatedWorkerServer server} and {@link DedicatedWorkerServerConnector browser} is a client. Integrate the
         * server and clients with this {@link DedicatedWorkerCommunicator}.
         *
         * #### [Inherited] {@link IClientDriver}
         * {@link IClientDriver} is a type of {@link ICommunicator}, specified for communication with remote client who has
         * connected in a {@link IServer server}. It takes full charge of network communication with the remote client.
         *
         * The {@link IClientDriver} object is created and delivered from {@link IServer} and
         * {@link IServer.addClient IServer.addClient()}. Those are derived types from this {@link IClientDriver}, being
         * created by the matched {@link IServer} object.
         *
         * Protocol                | Derived Type                        | Created By
         * ------------------------|-------------------------------------|----------------------------
         * Samchon Framework's own | {@link ClientDriver}                | {@link Server}
         * Web-socket protocol     | {@link WebClientDriver}             | {@link WebServer}
         * DedicatedWorker         | {@link DedicatedWorkerClinetDriver} | {@link DedicatedWorkerServer}
         * SharedWorker            | {@link SharedWorkerClientDriver}    | {@link SharedWorkerServer}
         *
         * When you've got an {@link IClientDriver} object from the {@link IServer.addClient IServer.addClient()}, then
         * specify {@link IProtocol listener} with {@link IClient.listen IClient.listen()}. Whenever a replied message comes
         * from the remote system, the message will be converted to an {@link Invoke} class and the {@link Invoke} object
         * will be shifted to the {@link IProtocol listener}'s {@link IProtocol.replyData IProtocol.replyData()} method.
         * Below code is an example specifying and managing the {@link IProtocol listener} objects.
         *
         * - https://github.com/samchon/framework/blob/master/ts/examples/calculator/calculator-server.ts
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link DedicatedWorkerServer}, {@link IProtocol}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iclientdriver)
         * @author Jeongho Nam <http://samchon.org>
         */
        var DedicatedWorkerClientDriver = (function (_super) {
            __extends(DedicatedWorkerClientDriver, _super);
            /**
             * Default Constructor.
             */
            function DedicatedWorkerClientDriver() {
                var _this = _super.call(this) || this;
                onmessage = _this.handle_message.bind(_this);
                _this.connected_ = true;
                return _this;
            }
            /**
             * @inheritdoc
             */
            DedicatedWorkerClientDriver.prototype.listen = function (listener) {
                this.listener_ = listener;
            };
            /**
             * @inheritdoc
             */
            DedicatedWorkerClientDriver.prototype.close = function () {
                close();
            };
            /**
             * @inheritdoc
             */
            DedicatedWorkerClientDriver.prototype.sendData = function (invoke) {
                postMessage(invoke.toXML().toString(), "");
                for (var i = 0; i < invoke.size(); i++)
                    if (invoke.at(i).getType() == "ByteArray")
                        postMessage(invoke.at(i).getValue(), "");
            };
            return DedicatedWorkerClientDriver;
        }(protocol.DedicatedWorkerCommunicator));
        protocol.DedicatedWorkerClientDriver = DedicatedWorkerClientDriver;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * Communicator with remote web-browser.
         *
         * {@link SharedWorkerClientDriver} is a class taking full charge of network communication with web browsers. This
         * {@link SharedWorkerClientDriver} object is always created by {@link SharedWorkerServer} class. When you got this
         * {@link SharedWorkerClientDriver} object from {@link SharedWorkerServer.addClient SharedWorkerServer.addClient()},
         * then specify {@link IProtocol listener} with the
         * {@link SharedWorkerClientDriver.listen SharedWorkerClientDriver.listen()} method.
         *
         * #### Why SharedWorker be a server?
         * SharedWorker, it allows only an instance (process) to be created whether the SharedWorker is declared in a browser
         * or multiple browsers. To integrate them, messages are being sent and received. Doesn't it seem like a relationship
         * between a server and clients? Thus, Samchon Framework consider the SharedWorker as a server and browsers as
         * clients.
         *
         * The class {@link SharedWorkerCommunicator} is designed make such relationship. From now on, SharedWorker is a
         * {@link SharedWorkerServer server} and {@link SharedWorkerServerConnector browsers} are clients. Integrate the
         * server and clients with this {@link SharedWorkerCommunicator}.
         *
         * #### [Inherited] {@link IClientDriver}
         * {@link IClientDriver} is a type of {@link ICommunicator}, specified for communication with remote client who has
         * connected in a {@link IServer server}. It takes full charge of network communication with the remote client.
         *
         * The {@link IClientDriver} object is created and delivered from {@link IServer} and
         * {@link IServer.addClient IServer.addClient()}. Those are derived types from this {@link IClientDriver}, being
         * created by the matched {@link IServer} object.
         *
         * Protocol                | Derived Type                        | Created By
         * ------------------------|-------------------------------------|----------------------------
         * Samchon Framework's own | {@link ClientDriver}                | {@link Server}
         * Web-socket protocol     | {@link WebClientDriver}             | {@link WebServer}
         * DedicatedWorker         | {@link DedicatedWorkerClinetDriver} | {@link DedicatedWorkerServer}
         * SharedWorker            | {@link SharedWorkerClientDriver}    | {@link SharedWorkerServer}
         *
         * When you've got an {@link IClientDriver} object from the {@link IServer.addClient IServer.addClient()}, then
         * specify {@link IProtocol listener} with {@link IClient.listen IClient.listen()}. Whenever a replied message comes
         * from the remote system, the message will be converted to an {@link Invoke} class and the {@link Invoke} object
         * will be shifted to the {@link IProtocol listener}'s {@link IProtocol.replyData IProtocol.replyData()} method.
         * Below code is an example specifying and managing the {@link IProtocol listener} objects.
         *
         * - https://github.com/samchon/framework/blob/master/ts/examples/calculator/calculator-server.ts
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link SharedWorkerServer}, {@link IProtocol}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iclientdriver)
         * @author Jeongho Nam <http://samchon.org>
         */
        var SharedWorkerClientDriver = (function (_super) {
            __extends(SharedWorkerClientDriver, _super);
            /**
             * Construct from a MessagePort object.
             */
            function SharedWorkerClientDriver(port) {
                var _this = _super.call(this) || this;
                _this.port_ = port;
                _this.connected_ = true;
                _this.listening_ = false;
                return _this;
            }
            /**
             * @inheritdoc
             */
            SharedWorkerClientDriver.prototype.listen = function (listener) {
                this.listener_ = listener;
                if (this.listening_ == true)
                    return;
                this.listening_ = true;
                this.port_.onmessage = this.handle_message.bind(this);
            };
            return SharedWorkerClientDriver;
        }(protocol.SharedWorkerCommunicator));
        protocol.SharedWorkerClientDriver = SharedWorkerClientDriver;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * @hidden
         */
        var IEntityGroup;
        (function (IEntityGroup) {
            /* ------------------------------------------------------------------
                ENTITY <-> XML CONVERSION
            ------------------------------------------------------------------ */
            /**
             * @hidden
             */
            function construct(entityGroup, xml) {
                var prohibited_names = [];
                for (var _i = 2; _i < arguments.length; _i++) {
                    prohibited_names[_i - 2] = arguments[_i];
                }
                entityGroup.clear();
                // MEMBER VARIABLES
                protocol.IEntity.construct.apply(protocol.IEntity, [entityGroup, xml].concat(prohibited_names));
                // CHILDREN
                if (xml.has(entityGroup.CHILD_TAG()) == false)
                    return;
                var children = new std.Vector();
                var xml_list = xml.get(entityGroup.CHILD_TAG());
                for (var i = 0; i < xml_list.size(); i++) {
                    var child = entityGroup.createChild(xml_list.at(i));
                    if (child == null)
                        continue;
                    child.construct(xml_list.at(i));
                    children.push(child);
                }
                entityGroup.assign(children.begin(), children.end());
            }
            IEntityGroup.construct = construct;
            /**
             * @hidden
             */
            function toXML(entityGroup) {
                var prohibited_names = [];
                for (var _i = 1; _i < arguments.length; _i++) {
                    prohibited_names[_i - 1] = arguments[_i];
                }
                // MEMBERS
                var xml = protocol.IEntity.toXML.apply(protocol.IEntity, [entityGroup].concat(prohibited_names));
                // CHILDREN
                for (var it = entityGroup.begin(); !it.equals(entityGroup.end()); it = it.next())
                    xml.push(it.value.toXML());
                return xml;
            }
            IEntityGroup.toXML = toXML;
            /* ------------------------------------------------------------------
                ACCESSORS
            ------------------------------------------------------------------ */
            function has(entityGroup, key) {
                return std.any_of(entityGroup.begin(), entityGroup.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            }
            IEntityGroup.has = has;
            function count(entityGroup, key) {
                return std.count_if(entityGroup.begin(), entityGroup.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            }
            IEntityGroup.count = count;
            function get(entityGroup, key) {
                for (var it = entityGroup.begin(); !it.equals(entityGroup.end()); it = it.next())
                    if (std.equal_to(it.value.key(), key) == true)
                        return it.value;
                throw new std.OutOfRange("out of range");
            }
            IEntityGroup.get = get;
        })(IEntityGroup = protocol.IEntityGroup || (protocol.IEntityGroup = {}));
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * @inheritdoc
         */
        var EntityArray = (function (_super) {
            __extends(EntityArray, _super);
            function EntityArray() {
                return _super.apply(this, arguments) || this;
            }
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            // using super::super;
            /**
             * @inheritdoc
             */
            EntityArray.prototype.construct = function (xml) {
                protocol.IEntityGroup.construct(this, xml);
            };
            /* ------------------------------------------------------------------
                GETTERS
            ------------------------------------------------------------------ */
            /**
             * @inheritdoc
             */
            EntityArray.prototype.key = function () {
                return "";
            };
            /**
             * @inheritdoc
             */
            EntityArray.prototype.has = function (key) {
                return protocol.IEntityGroup.has(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityArray.prototype.count = function (key) {
                return protocol.IEntityGroup.count(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityArray.prototype.get = function (key) {
                return protocol.IEntityGroup.get(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityArray.prototype.toXML = function () {
                return protocol.IEntityGroup.toXML(this, "length");
            };
            return EntityArray;
        }(std.Vector));
        protocol.EntityArray = EntityArray;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * @inheritdoc
         */
        var EntityList = (function (_super) {
            __extends(EntityList, _super);
            function EntityList() {
                return _super.apply(this, arguments) || this;
            }
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            // using super::super;
            /**
             * @inheritdoc
             */
            EntityList.prototype.construct = function (xml) {
                protocol.IEntityGroup.construct(this, xml);
            };
            /* ------------------------------------------------------------------
                GETTERS
            ------------------------------------------------------------------ */
            /**
             * @inheritdoc
             */
            EntityList.prototype.key = function () {
                return "";
            };
            /**
             * @inheritdoc
             */
            EntityList.prototype.has = function (key) {
                return protocol.IEntityGroup.has(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityList.prototype.count = function (key) {
                return protocol.IEntityGroup.count(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityList.prototype.get = function (key) {
                return protocol.IEntityGroup.get(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityList.prototype.toXML = function () {
                return protocol.IEntityGroup.toXML(this);
            };
            return EntityList;
        }(std.List));
        protocol.EntityList = EntityList;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * @inheritdoc
         */
        var EntityDeque = (function (_super) {
            __extends(EntityDeque, _super);
            function EntityDeque() {
                return _super.apply(this, arguments) || this;
            }
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            // using super::super;
            /**
             * @inheritdoc
             */
            EntityDeque.prototype.construct = function (xml) {
                protocol.IEntityGroup.construct(this, xml);
            };
            /* ------------------------------------------------------------------
                GETTERS
            ------------------------------------------------------------------ */
            /**
             * @inheritdoc
             */
            EntityDeque.prototype.key = function () {
                return "";
            };
            /**
             * @inheritdoc
             */
            EntityDeque.prototype.has = function (key) {
                return protocol.IEntityGroup.has(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityDeque.prototype.count = function (key) {
                return protocol.IEntityGroup.count(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityDeque.prototype.get = function (key) {
                return protocol.IEntityGroup.get(this, key);
            };
            /**
             * @inheritdoc
             */
            EntityDeque.prototype.toXML = function () {
                return protocol.IEntityGroup.toXML(this);
            };
            return EntityDeque;
        }(std.Deque));
        protocol.EntityDeque = EntityDeque;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="EntityArray.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * Standard message of network I/O.
         *
         * {@link Invoke} is a class used in network I/O in protocol package of Samchon Framework.
         *
         * The Invoke message has an XML structure like the result screen of provided example in below.
         * We can enjoy lots of benefits by the normalized and standardized message structure used in
         * network I/O.
         *
         * The greatest advantage is that we can make any type of network system, even how the system
         * is enourmously complicated. As network communication message is standardized, we only need to
         * concentrate on logical relationships between network systems. We can handle each network system
         * like a object (class) in OOD. And those relationships can be easily designed by using design
         * pattern.
         *
         * In Samchon Framework, you can make any type of network system with basic componenets
         * (IProtocol, IServer and ICommunicator) by implemens or inherits them, like designing
         * classes of S/W architecture.
         *
         * ![Class Diagram](http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_message_protocol.png)
         *
         * @see {@link IProtocol}
         * @author Jeongho Nam <http://samchon.org>
         */
        var Invoke = (function (_super) {
            __extends(Invoke, _super);
            function Invoke() {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var _this = _super.call(this) || this;
                /**
                 * Listener, represent function's name.
                 */
                _this.listener = "";
                if (args.length == 0) {
                    _this.listener = "";
                }
                else {
                    _this.listener = args[0];
                    for (var i = 1; i < args.length; i++)
                        _this.push_back(new protocol.InvokeParameter(args[i]));
                }
                return _this;
            }
            /**
             * @inheritdoc
             */
            Invoke.prototype.createChild = function (xml) {
                return new protocol.InvokeParameter();
            };
            /* -------------------------------------------------------------------
                GETTERS
            ------------------------------------------------------------------- */
            /**
             * Get listener.
             */
            Invoke.prototype.getListener = function () {
                return this.listener;
            };
            /**
             * Get arguments for Function.apply().
             *
             * @return An array containing values of the contained parameters.
             */
            Invoke.prototype.getArguments = function () {
                var args = [];
                for (var i = 0; i < this.size(); i++)
                    if (this.at(i).getName() == "_History_uid")
                        continue;
                    else
                        args.push(this.at(i).getValue());
                return args;
            };
            Invoke.prototype.apply = function (thisArg, func) {
                if (func === void 0) { func = null; }
                var argArray = this.getArguments();
                if (func == null) {
                    if (!(this.listener in thisArg && thisArg[this.listener] instanceof Function))
                        return false;
                    func = thisArg[this.listener];
                }
                func.apply(thisArg, argArray);
                return true;
            };
            /* -------------------------------------------------------------------
                EXPORTERS
            ------------------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            Invoke.prototype.TAG = function () {
                return "invoke";
            };
            /**
             * @inheritdoc
             */
            Invoke.prototype.CHILD_TAG = function () {
                return "parameter";
            };
            return Invoke;
        }(protocol.EntityArray));
        protocol.Invoke = Invoke;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="Entity.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A parameter belongs to an Invoke.
         *
         * ![Class Diagram](http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_message_protocol.png)
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var InvokeParameter = (function (_super) {
            __extends(InvokeParameter, _super);
            /* -------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------- */
            function InvokeParameter() {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                var _this = _super.call(this) || this;
                /**
                 * Name of the parameter.
                 *
                 * @details Optional property, can be omitted.
                 */
                _this.name = "";
                /**
                 * Type of the parameter.
                 */
                _this.type = "";
                /**
                 * Value of the parameter.
                 */
                _this.value = null;
                // DEFAULT CONSTRUCTOR
                if (args.length == 0)
                    return _this;
                // INITIALIZATION CONSTRUCTOR
                if (args.length == 1) {
                    _this.name = "";
                    _this.setValue(args[0]);
                }
                else {
                    _this.name = args[0];
                    _this.setValue(args[1]);
                }
                return _this;
            }
            /**
             * @inheritdoc
             */
            InvokeParameter.prototype.construct = function (xml) {
                this.name = (xml.hasProperty("name")) ? xml.getProperty("name") : "";
                this.type = xml.getProperty("type");
                if (this.type == "XML")
                    this.value = xml.begin().second.front();
                else if (this.type == "boolean")
                    this.value = Boolean(xml.getValue());
                else if (this.type == "number")
                    this.value = Number(xml.getValue());
                else if (this.type == "string")
                    this.value = xml.getValue();
            };
            InvokeParameter.prototype.setValue = function (value) {
                this.value = value;
                if (value instanceof samchon.library.XML)
                    this.type = "XML";
                else if (value instanceof Uint8Array)
                    this.type = "ByteArray";
                else
                    this.type = typeof value;
            };
            /* -------------------------------------------------------------------
                GETTERS
            ------------------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            InvokeParameter.prototype.key = function () {
                return this.name;
            };
            /**
             * Get name.
             */
            InvokeParameter.prototype.getName = function () {
                return this.name;
            };
            /**
             * Get type.
             */
            InvokeParameter.prototype.getType = function () {
                return this.type;
            };
            /**
             * Get value.
             */
            InvokeParameter.prototype.getValue = function () {
                return this.value;
            };
            /* -------------------------------------------------------------------
                EXPORTERS
            ------------------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            InvokeParameter.prototype.TAG = function () {
                return "parameter";
            };
            /**
             * @inheritdoc
             */
            InvokeParameter.prototype.toXML = function () {
                var xml = new samchon.library.XML();
                xml.setTag(this.TAG());
                if (this.name != "")
                    xml.setProperty("name", this.name);
                xml.setProperty("type", this.type);
                // NOT CONSIDERED ABOUT THE BINARY DATA
                if (this.type == "XML")
                    xml.push(this.value);
                else if (this.type != "ByteArray")
                    xml.setValue(this.value + "");
                return xml;
            };
            return InvokeParameter;
        }(protocol.Entity));
        protocol.InvokeParameter = InvokeParameter;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A server.
         *
         * The {@link Server} is an abstract class designed to open a server and accept clients who are following Samchon
         * Framework's own protocol. Extends this {@link Server} class and overrides {@link addClient addClient()} method to
         * define what to do with newly connected {@link ClientDriver remote clients}.
         *
         * #### [Inherited] {@link IServer}
         * {@link IServer} is an interfaec for server classes who are providing methods for {@link open opening a server} and
         * {@link IClientDriver accepting clients}.
         *
         * To open a server, extends one of derived class under below considedring which protocol to follow first. At next,
         * overrides {@link addClient addClient()} method who accepts a newly connected client as an {@link IClientDriver}
         * object. Then at last, call {@link open open()} method with specified port number.
         *
         * Protocol                | Derived Type                  | Related {@link IClientDriver}
         * ------------------------|-------------------------------|-------------------------------------
         * Samchon Framework's own | {@link Server}                | {@link ClientDriver}
         * Web-socket protocol     | {@link WebServer}             | {@link WebClientDriver}
         * DedicatedWorker         | {@link DedicatedWorkerServer} | {@link DedicatedWorkerClientDriver}
         * SharedWorker            | {@link SharedWorkerServer}    | {@link SharedWorkerClientDriver}
         *
         * Below codes and classes will be good examples for comprehending how to open a server and handle remote clients.
         * - https://github.com/samchon/framework/blob/master/ts/examples/calculator/calculator-server.ts
         * - https://github.com/samchon/framework/blob/master/ts/examples/chat-server/server.ts
         * - {@link service.Server}
         * - {@link external.ExternalClientArray}
         * - {@link slave.SlaveServer}
         *
         * If you're embarrased because your class already extended another one, then use {@link IServerBase}.
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link ClientDriver}, {@link ServerBase}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iserver)
         * @author Jeongho Nam <http://samchon.org>
         */
        var Server = (function () {
            function Server() {
            }
            /**
             * @inheritdoc
             */
            Server.prototype.open = function (port) {
                this.net_driver_ = net.createServer(this._Handle_connect.bind(this));
                this.net_driver_.listen(port);
            };
            /**
             * @inheritdoc
             */
            Server.prototype.close = function () {
                this.net_driver_.close();
            };
            /**
             * @hidden
             */
            Server.prototype._Handle_connect = function (socket) {
                var clientDriver = new protocol.ClientDriver(socket);
                ;
                this.addClient(clientDriver);
            };
            return Server;
        }());
        protocol.Server = Server;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A web server.
         *
         * The {@link WebServer} is an abstract class designed to open a server and accept clients who are following
         * web-socket protocol. Extends this {@link WebServer} class and overrides {@link addClient addClient()} method to
         * define what to do with newly connected {@link WebClientDriver remote clients}.
         *
         * #### [Inherited] {@link IServer}
         * {@link IServer} is an interfaec for server classes who are providing methods for {@link open opening a server} and
         * {@link IClientDriver accepting clients}.
         *
         * To open a server, extends one of derived class under below considedring which protocol to follow first. At next,
         * overrides {@link addClient addClient()} method who accepts a newly connected client as an {@link IClientDriver}
         * object. Then at last, call {@link open open()} method with specified port number.
         *
         * Protocol                | Derived Type                  | Related {@link IClientDriver}
         * ------------------------|-------------------------------|-------------------------------------
         * Samchon Framework's own | {@link Server}                | {@link ClientDriver}
         * Web-socket protocol     | {@link WebServer}             | {@link WebClientDriver}
         * DedicatedWorker         | {@link DedicatedWorkerServer} | {@link DedicatedWorkerClientDriver}
         * SharedWorker            | {@link SharedWorkerServer}    | {@link SharedWorkerClientDriver}
         *
         * Below codes and classes will be good examples for comprehending how to open a server and handle remote clients.
         * - https://github.com/samchon/framework/blob/master/ts/examples/calculator/calculator-server.ts
         * - https://github.com/samchon/framework/blob/master/ts/examples/chat-server/server.ts
         * - {@link service.Server}
         * - {@link external.ExternalClientArray}
         * - {@link slave.SlaveServer}
         *
         * If you're embarrased because your class already extended another one, then use {@link IServerBase}.
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link WebClientDriver}, {@link WebServerBase}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iserver)
         * @author Jeongho Nam <http://samchon.org>
         */
        var WebServer = (function () {
            /**
             * Default Constructor.
             */
            function WebServer() {
                this.sequence_ = 0;
            }
            /**
             * @inheritdoc
             */
            WebServer.prototype.open = function (port) {
                this.my_port_ = port;
                this.http_server_ = http.createServer();
                this.http_server_.listen(port);
                var ws_server = new websocket.server({ httpServer: this.http_server_ });
                ws_server.on("request", this._Handle_request.bind(this));
            };
            /**
             * @inheritdoc
             */
            WebServer.prototype.close = function () {
                this.http_server_.close();
            };
            /**
             * @hidden
             */
            WebServer.prototype._Handle_request = function (request) {
                //--------
                // Handle request from a client system.
                // 
                // This method "handle_request()" will be called when a client is connected. It will call an abstract method 
                // "addClient()" who handles an accepted client. If the newly connected client doesn't have its own session 
                // id, then a new session id will be issued.
                // 
                // @param request Requested header.
                //--------
                var path = request.resource.substr(1);
                var session_id = this._Get_session_id(request.cookies);
                var connection = request.accept("", request.origin, [{ name: "SESSION_ID", value: session_id }]);
                var driver = new protocol.WebClientDriver(connection, path, session_id);
                this.addClient(driver);
            };
            /**
             * @hidden
             */
            WebServer.prototype._Get_session_id = function (cookies) {
                //--------
                // Get session id from a newly connected.
                // 
                // Queries ordinary session id from cookies of a newly connected client. If the client has not, a new session 
                // id will be issued.
                // 
                // @param cookies Cookies from the remote client.
                // @return Session id
                //--------
                for (var i = 0; i < cookies.length; i++)
                    if (cookies[i].name == "SESSION_ID")
                        return cookies[i].value;
                return this._Issue_session_id();
            };
            /**
             * @hidden
             */
            WebServer.prototype._Issue_session_id = function () {
                // Issue a new session id.
                var port = this.my_port_;
                var uid = ++this.sequence_;
                var linux_time = new Date().getTime();
                var rand = Math.floor(Math.random() * 0xffffffff);
                return port.toString(16) + uid.toString(16) + linux_time.toString(16) + rand.toString(16);
            };
            return WebServer;
        }());
        protocol.WebServer = WebServer;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A SharedWorker server.
         *
         * The {@link DedicatedWorkerServer} is an abstract class is realized to open a DedicatedWorker server and accept
         * web-browser client (master). Extends this {@link DedicatedWorkerServer} class and overrides
         * {@link addClient addClient()} method to define what to do with a newly connected
         * {@link DedicatedWorkerClientDriver remote client}.
         *
         * #### Why DedicatedWorker be a server?
         * In JavaScript environment, there's no way to implement multi-threading function. Instead, JavaScript supports the
         * **Worker**, creating a new process. However, the **Worker** does not shares memory addresses. To integrate the
         * **Worker** with its master, only communication with string or binary data is allowed. Doesn't it seem like a network
         * communication? Furthermore, there's not any difference between the worker communication and network communication.
         * It's the reason why Samchon Framework considers the **Worker** as a network node.
         *
         * The class {@link DedicatedWorkerCommunicator} is designed make such relationship. From now on, DedicatedWorker is a
         * {@link DedicatedWorkerServer server} and {@link DedicatedWorkerServerConnector browser} is a client. Integrate the
         * server and clients with this {@link DedicatedWorkerCommunicator}.
         *
         * #### [Inherited] {@link IServer}
         * {@link IServer} is an interfaec for server classes who are providing methods for {@link open opening a server} and
         * {@link IClientDriver accepting clients}.
         *
         * To open a server, extends one of derived class under below considedring which protocol to follow first. At next,
         * overrides {@link addClient addClient()} method who accepts a newly connected client as an {@link IClientDriver}
         * object. Then at last, call {@link open open()} method with specified port number.
         *
         * Protocol                | Derived Type                  | Related {@link IClientDriver}
         * ------------------------|-------------------------------|-------------------------------------
         * Samchon Framework's own | {@link Server}                | {@link ClientDriver}
         * Web-socket protocol     | {@link WebServer}             | {@link WebClientDriver}
         * DedicatedWorker         | {@link DedicatedWorkerServer} | {@link DedicatedWorkerClientDriver}
         * SharedWorker            | {@link SharedWorkerServer}    | {@link SharedWorkerClientDriver}
         *
         * Below codes and classes will be good examples for comprehending how to open a server and handle remote clients.
         * - https://github.com/samchon/framework/blob/master/ts/examples/calculator/calculator-server.ts
         * - https://github.com/samchon/framework/blob/master/ts/examples/chat-server/server.ts
         * - {@link service.Server}
         * - {@link external.ExternalClientArray}
         * - {@link slave.SlaveServer}
         *
         * If you're embarrased because your class already extended another one, then use {@link IServerBase}.
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link DedicatedWorkerClientDriver}, {@link DedicatedWorkerServerBase}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iserver)
         * @author Jeongho Nam <http://samchon.org>
         */
        var DedicatedWorkerServer = (function () {
            function DedicatedWorkerServer() {
            }
            /**
             * @inheritdoc
             */
            DedicatedWorkerServer.prototype.open = function () {
                this.addClient(new protocol.DedicatedWorkerClientDriver());
            };
            /**
             * @inheritdoc
             */
            DedicatedWorkerServer.prototype.close = function () {
                close();
            };
            return DedicatedWorkerServer;
        }());
        protocol.DedicatedWorkerServer = DedicatedWorkerServer;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A SharedWorker server.
         *
         * The {@link SharedWorker} is an abstract class is realized to open a SharedWorker server and accept web-browser
         * clients. Extends this {@link SharedWorkerServer} class and overrides {@link addClient addClient()} method to
         * define what to do with newly connected {@link SharedWorkerClientDriver remote clients}.
         *
         * #### Why SharedWorker be a server?
         * SharedWorker, it allows only an instance (process) to be created whether the SharedWorker is declared in a browser
         * or multiple browsers. To integrate them, messages are being sent and received. Doesn't it seem like a relationship
         * between a server and clients? Thus, Samchon Framework consider the SharedWorker as a server and browsers as
         * clients.
         *
         * The class {@link SharedWorkerCommunicator} is designed make such relationship. From now on, SharedWorker is a
         * {@link SharedWorkerServer server} and {@link SharedWorkerServerConnector browsers} are clients. Integrate the
         * server and clients with this {@link SharedWorkerCommunicator}.
         *
         * #### [Inherited] {@link IServer}
         * {@link IServer} is an interfaec for server classes who are providing methods for {@link open opening a server} and
         * {@link IClientDriver accepting clients}.
         *
         * To open a server, extends one of derived class under below considedring which protocol to follow first. At next,
         * overrides {@link addClient addClient()} method who accepts a newly connected client as an {@link IClientDriver}
         * object. Then at last, call {@link open open()} method with specified port number.
         *
         * Protocol                | Derived Type                  | Related {@link IClientDriver}
         * ------------------------|-------------------------------|-------------------------------------
         * Samchon Framework's own | {@link Server}                | {@link ClientDriver}
         * Web-socket protocol     | {@link WebServer}             | {@link WebClientDriver}
         * DedicatedWorker         | {@link DedicatedWorkerServer} | {@link DedicatedWorkerClientDriver}
         * SharedWorker            | {@link SharedWorkerServer}    | {@link SharedWorkerClientDriver}
         *
         * Below codes and classes will be good examples for comprehending how to open a server and handle remote clients.
         * - https://github.com/samchon/framework/blob/master/ts/examples/calculator/calculator-server.ts
         * - https://github.com/samchon/framework/blob/master/ts/examples/chat-server/server.ts
         * - {@link service.Server}
         * - {@link external.ExternalClientArray}
         * - {@link slave.SlaveServer}
         *
         * If you're embarrased because your class already extended another one, then use {@link IServerBase}.
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link SharedWorkerClientDriver}, {@link SharedWorkerServerBase}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iserver)
         * @author Jeongho Nam <http://samchon.org>
         */
        var SharedWorkerServer = (function () {
            function SharedWorkerServer() {
            }
            /**
             * @inheritdoc
             */
            SharedWorkerServer.prototype.open = function () {
                self.addEventListener("connect", this._Handle_connect.bind(this));
            };
            /**
             * @inheritdoc
             */
            SharedWorkerServer.prototype.close = function () {
                // MAY IMPOSSIBLE
                close();
            };
            /**
             * @hidden
             */
            SharedWorkerServer.prototype._Handle_connect = function (event) {
                var port = event.ports[event.ports.length - 1];
                var driver = new protocol.SharedWorkerClientDriver(port);
                this.addClient(driver);
            };
            return SharedWorkerServer;
        }());
        protocol.SharedWorkerServer = SharedWorkerServer;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="Server.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A substitute {@link Server}.
         *
         * The {@link ServerBase} is a substitute class who subrogates {@link Server}'s responsibility.
         *
         * #### [Inherited] {@link IServerBase}
         * {@link IServerBase} is an interface for substitue server classes who subrogate server's role.
         *
         * The easiest way to defining a server class is to extending one of them below, who implemented the {@link IServer}.
         * However, it is impossible (that is, if the class is already extending another class), you can instead implement
         * the {@link IServer} interface, create an {@link IServerBase} member, and write simple hooks to route calls into
         * the aggregated {@link IServerBase}.
         *
         * Protocol                | {@link IServer}               | {@link IServerBase}               | {@link IClientDriver}
         * ------------------------|-------------------------------|-----------------------------------|-------------------------------------
         * Samchon Framework's own | {@link Server}                | {@link ServerBase}                | {@link ClientDriver}
         * Web-socket protocol     | {@link WebServer}             | {@link WebServerBase}             | {@link WebClientDriver}
         * DedicatedWorker         | {@link DedicatedWorkerServer} | {@link DedicatedWorkerServerBase} | {@link DedicatedWorkerClientDriver}
         * SharedWorker            | {@link SharedWorkerServer}    | {@link SharedWorkerServerBase}    | {@link SharedWorkerClientDriver}
         *
         * After the hooking to aggregated {@link IServerBase} object, overrides {@link addClient addClient()} method who
         * accepts a newly connected client as an {@link IClientDriver} object. At last, call {@link open open()} method with
         * specified port number.
         *
         * ```typescript
         * class MyServer extends Something implements IServer
         * {
         * 	private server_base_: IServerBase = new WebServerBase(this);
         *
         * 	public addClient(driver: IClientDriver): void
         * 	{
         * 		// WHAT TO DO WHEN A CLIENT HAS CONNECTED
         * 	}
         *
         * 	public open(port: number): void
         * 	{
         * 		this.server_base_.open();
         * 	}
         * 	public close(): void
         * 	{
         * 		this.server_base_.close();
         * 	}
         * }
         * ```
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         *
         * @see {@link Server}, {@link ClientDriver}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iserverbase)
         * @author Jeongho Nam <http://samchon.org>
         */
        var ServerBase = (function (_super) {
            __extends(ServerBase, _super);
            /**
             * Construct from a *hooker*.
             *
             * @param hooker A hooker throwing responsibility of server's role.
             */
            function ServerBase(hooker) {
                var _this = _super.call(this) || this;
                _this.hooker_ = hooker;
                return _this;
            }
            /**
             * @inheritdoc
             */
            ServerBase.prototype.addClient = function (driver) {
                this.hooker_.addClient(driver);
            };
            return ServerBase;
        }(protocol.Server));
        protocol.ServerBase = ServerBase;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A substitute {@link WebServer}.
         *
         * The {@link WebServerBase} is a substitute class who subrogates {@link WebServer}'s responsibility.
         *
         * #### [Inherited] {@link IServerBase}
         * {@link IServerBase} is an interface for substitue server classes who subrogate server's role.
         *
         * The easiest way to defining a server class is to extending one of them below, who implemented the {@link IServer}.
         * However, it is impossible (that is, if the class is already extending another class), you can instead implement
         * the {@link IServer} interface, create an {@link IServerBase} member, and write simple hooks to route calls into
         * the aggregated {@link IServerBase}.
         *
         * Protocol                | {@link IServer}               | {@link IServerBase}               | {@link IClientDriver}
         * ------------------------|-------------------------------|-----------------------------------|-------------------------------------
         * Samchon Framework's own | {@link Server}                | {@link ServerBase}                | {@link ClientDriver}
         * Web-socket protocol     | {@link WebServer}             | {@link WebServerBase}             | {@link WebClientDriver}
         * DedicatedWorker         | {@link DedicatedWorkerServer} | {@link DedicatedWorkerServerBase} | {@link DedicatedWorkerClientDriver}
         * SharedWorker            | {@link SharedWorkerServer}    | {@link SharedWorkerServerBase}    | {@link SharedWorkerClientDriver}
         *
         * After the hooking to aggregated {@link IServerBase} object, overrides {@link addClient addClient()} method who
         * accepts a newly connected client as an {@link IClientDriver} object. At last, call {@link open open()} method with
         * specified port number.
         *
         * ```typescript
         * class MyServer extends Something implements IServer
         * {
         * 	private server_base_: IServerBase = new WebServerBase(this);
         *
         * 	public addClient(driver: IClientDriver): void
         * 	{
         * 		// WHAT TO DO WHEN A CLIENT HAS CONNECTED
         * 	}
         *
         * 	public open(port: number): void
         * 	{
         * 		this.server_base_.open();
         * 	}
         * 	public close(): void
         * 	{
         * 		this.server_base_.close();
         * 	}
         * }
         * ```
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link WebServer}, {@link WebClientDriver}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iserverbase)
         * @author Jeongho Nam <http://samchon.org>
         */
        var WebServerBase = (function (_super) {
            __extends(WebServerBase, _super);
            /**
             * Construct from a *hooker*.
             *
             * @param hooker A hooker throwing responsibility of server's role.
             */
            function WebServerBase(hooker) {
                var _this = _super.call(this) || this;
                _this.hooker_ = hooker;
                return _this;
            }
            /**
             * @inheritdoc
             */
            WebServerBase.prototype.addClient = function (driver) {
                this.hooker_.addClient(driver);
            };
            return WebServerBase;
        }(protocol.WebServer));
        protocol.WebServerBase = WebServerBase;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A substitute {@link DedicatedWorkerServer}.
         *
         * The {@link DedicatedWorkerServerBase} is a substitute class who subrogates {@link DedicatedWorkerServer}'s
         * responsibility.
         *
         * #### [Inherited] {@link IServerBase}
         * {@link IServerBase} is an interface for substitue server classes who subrogate server's role.
         *
         * The easiest way to defining a server class is to extending one of them below, who implemented the {@link IServer}.
         * However, it is impossible (that is, if the class is already extending another class), you can instead implement
         * the {@link IServer} interface, create an {@link IServerBase} member, and write simple hooks to route calls into
         * the aggregated {@link IServerBase}.
         *
         * Protocol                | {@link IServer}               | {@link IServerBase}               | {@link IClientDriver}
         * ------------------------|-------------------------------|-----------------------------------|-------------------------------------
         * Samchon Framework's own | {@link Server}                | {@link ServerBase}                | {@link ClientDriver}
         * Web-socket protocol     | {@link WebServer}             | {@link WebServerBase}             | {@link WebClientDriver}
         * DedicatedWorker         | {@link DedicatedWorkerServer} | {@link DedicatedWorkerServerBase} | {@link DedicatedWorkerClientDriver}
         * SharedWorker            | {@link SharedWorkerServer}    | {@link SharedWorkerServerBase}    | {@link SharedWorkerClientDriver}
         *
         * After the hooking to aggregated {@link IServerBase} object, overrides {@link addClient addClient()} method who
         * accepts a newly connected client as an {@link IClientDriver} object. At last, call {@link open open()} method with
         * specified port number.
         *
         * ```typescript
         * class MyServer extends Something implements IServer
         * {
         * 	private server_base_: IServerBase = new WebServerBase(this);
         *
         * 	public addClient(driver: IClientDriver): void
         * 	{
         * 		// WHAT TO DO WHEN A CLIENT HAS CONNECTED
         * 	}
         *
         * 	public open(port: number): void
         * 	{
         * 		this.server_base_.open();
         * 	}
         * 	public close(): void
         * 	{
         * 		this.server_base_.close();
         * 	}
         * }
         * ```
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link DedicatedWorkerServer}, {@link DedicatedWorkerClientDriver}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iserverbase)
         * @author Jeongho Nam <http://samchon.org>
         */
        var DedicatedWorkerServerBase = (function (_super) {
            __extends(DedicatedWorkerServerBase, _super);
            /**
             * Construct from a *hooker*.
             *
             * @param hooker A hooker throwing responsibility of server's role.
             */
            function DedicatedWorkerServerBase(hooker) {
                var _this = _super.call(this) || this;
                _this.hooker_ = hooker;
                return _this;
            }
            /**
             * @inheritdoc
             */
            DedicatedWorkerServerBase.prototype.addClient = function (driver) {
                this.hooker_.addClient(driver);
            };
            return DedicatedWorkerServerBase;
        }(protocol.DedicatedWorkerServer));
        protocol.DedicatedWorkerServerBase = DedicatedWorkerServerBase;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A substitute {@link SharedWorkerServer}.
         *
         * The {@link SharedWorkerServerBase} is a substitute class who subrogates {@link SharedWorkerServer}'s
         * responsibility.
         *
         * #### [Inherited] {@link IServerBase}
         * {@link IServerBase} is an interface for substitue server classes who subrogate server's role.
         *
         * The easiest way to defining a server class is to extending one of them below, who implemented the {@link IServer}.
         * However, it is impossible (that is, if the class is already extending another class), you can instead implement
         * the {@link IServer} interface, create an {@link IServerBase} member, and write simple hooks to route calls into
         * the aggregated {@link IServerBase}.
         *
         * Protocol                | {@link IServer}               | {@link IServerBase}               | {@link IClientDriver}
         * ------------------------|-------------------------------|-----------------------------------|-------------------------------------
         * Samchon Framework's own | {@link Server}                | {@link ServerBase}                | {@link ClientDriver}
         * Web-socket protocol     | {@link WebServer}             | {@link WebServerBase}             | {@link WebClientDriver}
         * DedicatedWorker         | {@link DedicatedWorkerServer} | {@link DedicatedWorkerServerBase} | {@link DedicatedWorkerClientDriver}
         * SharedWorker            | {@link SharedWorkerServer}    | {@link SharedWorkerServerBase}    | {@link SharedWorkerClientDriver}
         *
         * After the hooking to aggregated {@link IServerBase} object, overrides {@link addClient addClient()} method who
         * accepts a newly connected client as an {@link IClientDriver} object. At last, call {@link open open()} method with
         * specified port number.
         *
         * ```typescript
         * class MyServer extends Something implements IServer
         * {
         * 	private server_base_: IServerBase = new WebServerBase(this);
         *
         * 	public addClient(driver: IClientDriver): void
         * 	{
         * 		// WHAT TO DO WHEN A CLIENT HAS CONNECTED
         * 	}
         *
         * 	public open(port: number): void
         * 	{
         * 		this.server_base_.open();
         * 	}
         * 	public close(): void
         * 	{
         * 		this.server_base_.close();
         * 	}
         * }
         * ```
         *
         * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		  target="_blank">
         *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png"
         *		 style="max-width: 100%" />
         * </a>
         *
         * @see {@link SharedWorkerServer}, {@link SharedWorkerClientDriver}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iserverbase)
         * @author Jeongho Nam <http://samchon.org>
         */
        var SharedWorkerServerBase = (function (_super) {
            __extends(SharedWorkerServerBase, _super);
            /**
             * Construct from a *hooker*.
             *
             * @param hooker A hooker throwing responsibility of server's role.
             */
            function SharedWorkerServerBase(hooker) {
                var _this = _super.call(this) || this;
                _this.hooker_ = hooker;
                return _this;
            }
            /**
             * @inheritdoc
             */
            SharedWorkerServerBase.prototype.addClient = function (driver) {
                this.hooker_.addClient(driver);
            };
            return SharedWorkerServerBase;
        }(protocol.SharedWorkerServer));
        protocol.SharedWorkerServerBase = SharedWorkerServerBase;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="Communicator.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * Server connnector.
         *
         * {@link ServerConnector} is a class connecting to remote server who follows Samchon Framework's own protocol and
         * taking full charge of network communication with the remote server. Create a {@link ServerConnector} instance from
         * the {@IProtocol listener} and call the {@link connect connect()} method.
         *
         * #### [Inherited] {@link IServerConnector}
         * {@link IServerConnector} is a type of {@link ICommunicator}, specified for server connector classes who connect to
         * the remote server as a client. {@link IServerConnector} provides {@link connect connection method} and takes full
         * charge of network communication with the remote server.
         *
         * Declare specific type of {@link IServerConnector} from {@link IProtocol listener} and call the
         * {@link connect connect()} method. Then whenever a replied message comes from the remote system, the message will
         * be converted to an {@link Invoke} object and the {@link Invoke} object will be shifted to the
         * {@link IProtocol listener}'s {@link IProtocol.replyData IProtocol.replyData()} method.
         *
         * Note that, protocol of this client and remote server must be matched. Thus, before determining specific type of
         * this {@link IServerConnector}, you've to consider which protocol and type the remote server follows.
         *
         * Protocol                | Derived Type                           | Connect to
         * ------------------------|----------------------------------------|-------------------------------
         * Samchon Framework's own | {@link ServerConnector}                | {@link Server}
         * Web-socket protocol     | {@link WebServerConnector}             | {@link WebServer}
         * DedicatedWorker         | {@link DedicatedWorkerServerConnector} | {@link DedicatedWorkerServer}
         * SharedWorker            | {@link SharedWorkerServerConnector}    | {@link SharedWorkerServer}
         *
         * ![Class Diagram](http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png)
         *
         * @see {@link Server}, {@link IProtocol}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iserverconnector)
         * @author Jeongho Nam <http://samchon.org>
         */
        var ServerConnector = (function (_super) {
            __extends(ServerConnector, _super);
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            /**
             * Construct from *listener*.
             *
             * @param listener A listener object to listen replied message from newly connected client in
             *				   {@link IProtocol.replyData replyData()} as an {@link Invoke} object.
             */
            function ServerConnector(listener) {
                var _this = _super.call(this, listener) || this;
                _this.connected_ = false;
                return _this;
            }
            /**
             * @inheritdoc
             */
            ServerConnector.prototype.connect = function (ip, port) {
                this.socket_ = net.connect({ host: ip, port: port }, this._Handle_connect.bind(this));
            };
            /* ---------------------------------------------------------
                HANDLERS
            --------------------------------------------------------- */
            /**
             * @hidden
             */
            ServerConnector.prototype._Handle_connect = function () {
                var arg = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    arg[_i] = arguments[_i];
                }
                this.connected_ = true;
                this.start_listen();
                this._Send_dummy_packet_repeatedly();
                if (this.onConnect != null)
                    this.onConnect();
            };
            /**
             * @hidden
             */
            ServerConnector.prototype._Send_dummy_packet_repeatedly = function () {
                setInterval(function () {
                    // WRITE A HEADER BUFFER WHICH MEANS CONTENT SIZE IS ZERO.
                    var packet = new Buffer(8);
                    packet.writeUInt32BE(0, 0);
                    packet.writeUInt32BE(0, 4);
                    // SEND
                    try {
                        this.socket_.write(packet);
                    }
                    catch (exception) {
                        return;
                    }
                }.bind(this), 5000);
            };
            return ServerConnector;
        }(protocol.Communicator));
        protocol.ServerConnector = ServerConnector;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A server connector for web-socket protocol.
         *
         * {@link WebServerConnector} is a class connecting to remote server who follows Web-socket protocol and taking full
         * charge of network communication with the remote server. Create an {@link WebServerConnector} instance from the
         * {@IProtocol listener} and call the {@link connect connect()} method.
         *
         * #### [Inherited] {@link IServerConnector}
         * {@link IServerConnector} is a type of {@link ICommunicator}, specified for server connector classes who connect to
         * the remote server as a client. {@link IServerConnector} provides {@link connect connection method} and takes full
         * charge of network communication with the remote server.
         *
         * Declare specific type of {@link IServerConnector} from {@link IProtocol listener} and call the
         * {@link connect connect()} method. Then whenever a replied message comes from the remote system, the message will
         * be converted to an {@link Invoke} class and the {@link Invoke} object will be shifted to the
         * {@link IProtocol listener}'s {@link IProtocol.replyData IProtocol.replyData()} method.
         *
         * Note that, protocol of this client and remote server must be matched. Thus, before determining specific type of
         * this {@link IServerConnector}, you've to consider which protocol and type the remote server follows.
         *
         * Protocol                | Derived Type                           | Connect to
         * ------------------------|----------------------------------------|-------------------------------
         * Samchon Framework's own | {@link ServerConnector}                | {@link Server}
         * Web-socket protocol     | {@link WebServerConnector}             | {@link WebServer}
         * DedicatedWorker         | {@link DedicatedWorkerServerConnector} | {@link DedicatedWorkerServer}
         * SharedWorker            | {@link SharedWorkerServerConnector}    | {@link SharedWorkerServer}
         *
         * ![Class Diagram](http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png)
         *
         * @see {@link WebServer}, {@link IProtocol}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iserverconnector)
         * @author Jeongho Nam <http://samchon.org>
         */
        var WebServerConnector = (function (_super) {
            __extends(WebServerConnector, _super);
            /* ----------------------------------------------------
                CONSTRUCTORS
            ---------------------------------------------------- */
            /**
             * Construct from *listener*.
             *
             * @param listener A listener object to listen replied message from newly connected client in
             *				   {@link IProtocol.replyData replyData()} as an {@link Invoke} object.
             */
            function WebServerConnector(listener) {
                var _this = _super.call(this, listener) || this;
                _this.browser_socket_ = null;
                _this.node_client_ = null;
                _this.connected_ = false;
                _this.onConnect = null;
                return _this;
            }
            /**
             * Connect to a web server.
             *
             * Connects to a server with specified *host* address, *port* number and *path*. After the connection has
             * succeeded, callback function {@link onConnect} is called. Listening data from the connected server also begins.
             * Replied messages from the connected server will be converted to {@link Invoke} classes and will be shifted to
             * the {@link WebCommunicator.listener listener}'s {@link IProtocol.replyData replyData()} method.
             *
             * If the connection fails immediately, either an event is dispatched or an exception is thrown: an error
             * event is dispatched if a host was specified, and an exception is thrown if no host was specified. Otherwise,
             * the status of the connection is reported by an event. If the socket is already connected, the existing
             * connection is closed first.
             *
             * @param ip The name or IP address of the host to connect to.
             *			 If no host is specified, the host that is contacted is the host where the calling file resides.
             *			 If you do not specify a host, use an event listener to determine whether the connection was
             *			 successful.
             * @param port The port number to connect to.
             * @param path Path of service which you want.
             */
            WebServerConnector.prototype.connect = function (ip, port, path) {
                if (path === void 0) { path = ""; }
                // COMPOSITE FULL-ADDRESS
                var address;
                if (ip.indexOf("ws://") == -1)
                    if (ip.indexOf("://") != -1)
                        throw "only websocket is possible";
                    else
                        ip = "ws://" + ip;
                address = ip + ":" + port + "/" + path;
                // CONNECTION BRANCHES
                if (samchon.is_node() == true) {
                    this.node_client_ = new websocket.client();
                    this.node_client_.on("connect", this._Handle_node_connect.bind(this));
                    this.node_client_.connect(address);
                }
                else {
                    this.browser_socket_ = new WebSocket(address);
                    this.browser_socket_.onopen = this._Handle_browser_connect.bind(this);
                    this.browser_socket_.onerror = this.handle_close.bind(this);
                    this.browser_socket_.onclose = this.handle_close.bind(this);
                    this.browser_socket_.onmessage = this._Handle_browser_message.bind(this);
                }
            };
            /**
             * @inheritdoc
             */
            WebServerConnector.prototype.close = function () {
                if (samchon.is_node() == true)
                    _super.prototype.close.call(this);
                else
                    this.browser_socket_.close();
            };
            /* ----------------------------------------------------
                IPROTOCOL'S METHOD
            ---------------------------------------------------- */
            /**
             * @inheritdoc
             */
            WebServerConnector.prototype.sendData = function (invoke) {
                if (this.browser_socket_ != null) {
                    this.browser_socket_.send(invoke.toXML().toString());
                    for (var i = 0; i < invoke.size(); i++)
                        if (invoke.at(i).getType() == "ByteArray")
                            this.browser_socket_.send(invoke.at(i).getValue());
                }
                else {
                    _super.prototype.sendData.call(this, invoke);
                }
            };
            /**
             * @hidden
             */
            WebServerConnector.prototype._Handle_browser_connect = function (event) {
                this.connected_ = true;
                if (this.onConnect != null)
                    this.onConnect();
            };
            /**
             * @hidden
             */
            WebServerConnector.prototype._Handle_browser_message = function (event) {
                if (this.is_binary_invoke() == false)
                    this.handle_string(event.data);
                else
                    this.handle_binary(event.data);
            };
            /**
             * @hidden
             */
            WebServerConnector.prototype._Handle_node_connect = function (connection) {
                this.connected_ = true;
                this.connection_ = connection;
                this.connection_.on("message", this.handle_message.bind(this));
                this.connection_.on("close", this.handle_close.bind(this));
                this.connection_.on("error", this.handle_close.bind(this));
                if (this.onConnect != null)
                    this.onConnect();
            };
            return WebServerConnector;
        }(protocol.WebCommunicator));
        protocol.WebServerConnector = WebServerConnector;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A server connector for DedicatedWorker.
         *
         * {@link DedicatedWorkerServerConnector} is a class connecting to SharedWorker and taking full charge of network
         * communication with the SharedWorker. Create an {@link DedicatedWorkerServer} instance from the
         * {@IProtocol listener} and call the {@link connect connect()} method.
         *
         * #### Why DedicatedWorker be a server?
         * In JavaScript environment, there's no way to implement multi-threading function. Instead, JavaScript supports the
         * **Worker**, creating a new process. However, the **Worker** does not shares memory addresses. To integrate the
         * **Worker** with its master, only communication with string or binary data is allowed. Doesn't it seem like a network
         * communication? Furthermore, there's not any difference between the worker communication and network communication.
         * It's the reason why Samchon Framework considers the **Worker** as a network node.
         *
         * The class {@link DedicatedWorkerCommunicator} is designed make such relationship. From now on, DedicatedWorker is a
         * {@link DedicatedWorkerServer server} and {@link DedicatedWorkerServerConnector browser} is a client. Integrate the
         * server and clients with this {@link DedicatedWorkerCommunicator}.
         *
         * #### [Inherited] {@link IServerConnector}
         * {@link IServerConnector} is a type of {@link ICommunicator}, specified for server connector classes who connect to
         * the remote server as a client. {@link IServerConnector} provides {@link connect connection method} and takes full
         * charge of network communication with the remote server.
         *
         * Declare specific type of {@link IServerConnector} from {@link IProtocol listener} and call the
         * {@link connect connect()} method. Then whenever a replied message comes from the remote system, the message will
         * be converted to an {@link Invoke} class and the {@link Invoke} object will be shifted to the
         * {@link IProtocol listener}'s {@link IProtocol.replyData IProtocol.replyData()} method.
         *
         * Note that, protocol of this client and remote server must be matched. Thus, before determining specific type of
         * this {@link IServerConnector}, you've to consider which protocol and type the remote server follows.
         *
         * Protocol                | Derived Type                           | Connect to
         * ------------------------|----------------------------------------|-------------------------------
         * Samchon Framework's own | {@link ServerConnector}                | {@link Server}
         * Web-socket protocol     | {@link WebServerConnector}             | {@link WebServer}
         * DedicatedWorker         | {@link DedicatedWorkerServerConnector} | {@link DedicatedWorkerServer}
         * SharedWorker            | {@link SharedWorkerServerConnector}    | {@link SharedWorkerServer}
         *
         * ![Class Diagram](http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png)
         *
         * @see {@link DedicatedWorkerServer}, {@link IProtocol}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iserverconnector)
         * @author Jeongho Nam <http://samchon.org>
         */
        var DedicatedWorkerServerConnector = (function (_super) {
            __extends(DedicatedWorkerServerConnector, _super);
            /**
             * Construct from *listener*.
             *
             * @param listener A listener object to listen replied message from newly connected client in
             *				   {@link IProtocol.replyData replyData()} as an {@link Invoke} object.
             */
            function DedicatedWorkerServerConnector(listener) {
                var _this = _super.call(this, listener) || this;
                _this.worker_ = null;
                return _this;
            }
            /**
             * @inheritdoc
             */
            DedicatedWorkerServerConnector.prototype.connect = function (jsFile) {
                // CONSTRUCT WORKER AND START LISTENING
                this.worker_ = new Worker(jsFile);
                this.worker_.onmessage = this.handle_message.bind(this);
                // NOTIFY THE CONNECTION
                this.connected_ = true;
                if (this.onConnect != null)
                    this.onConnect();
            };
            /**
             * @inheritdoc
             */
            DedicatedWorkerServerConnector.prototype.close = function () {
                this.worker_.terminate();
                if (this.onClose != null)
                    this.onClose();
            };
            /**
             * @inheritdoc
             */
            DedicatedWorkerServerConnector.prototype.sendData = function (invoke) {
                this.worker_.postMessage(invoke.toXML().toString(), "");
                for (var i = 0; i < invoke.size(); i++)
                    if (invoke.at(i).getType() == "ByteArray")
                        this.worker_.postMessage(invoke.at(i).getValue(), "");
            };
            return DedicatedWorkerServerConnector;
        }(protocol.DedicatedWorkerCommunicator));
        protocol.DedicatedWorkerServerConnector = DedicatedWorkerServerConnector;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * A server connector for SharedWorker.
         *
         * {@link SharedWorkerServerConnector} is a class connecting to SharedWorker and taking full charge of network
         * communication with the SharedWorker. Create an {@link SharedWorkerServerConnector} instance from the
         * {@IProtocol listener} and call the {@link connect connect()} method.
         *
         * #### Why SharedWorker be a server?
         * SharedWorker, it allows only an instance (process) to be created whether the SharedWorker is declared in a browser
         * or multiple browsers. To integrate them, messages are being sent and received. Doesn't it seem like a relationship
         * between a server and clients? Thus, Samchon Framework consider the SharedWorker as a server and browsers as
         * clients.
         *
         * The class {@link SharedWorkerCommunicator} is designed make such relationship. From now on, SharedWorker is a
         * {@link SharedWorkerServer server} and {@link SharedWorkerServerConnector browsers} are clients. Integrate the
         * server and clients with this {@link SharedWorkerCommunicator}.
         *
         * #### [Inherited] {@link IServerConnector}
         * {@link IServerConnector} is a type of {@link ICommunicator}, specified for server connector classes who connect to
         * the remote server as a client. {@link IServerConnector} provides {@link connect connection method} and takes full
         * charge of network communication with the remote server.
         *
         * Declare specific type of {@link IServerConnector} from {@link IProtocol listener} and call the
         * {@link connect connect()} method. Then whenever a replied message comes from the remote system, the message will
         * be converted to an {@link Invoke} class and the {@link Invoke} object will be shifted to the
         * {@link IProtocol listener}'s {@link IProtocol.replyData IProtocol.replyData()} method.
         *
         * Note that, protocol of this client and remote server must be matched. Thus, before determining specific type of
         * this {@link IServerConnector}, you've to consider which protocol and type the remote server follows.
         *
         * Protocol                | Derived Type                           | Connect to
         * ------------------------|----------------------------------------|-------------------------------
         * Samchon Framework's own | {@link ServerConnector}                | {@link Server}
         * Web-socket protocol     | {@link WebServerConnector}             | {@link WebServer}
         * DedicatedWorker         | {@link DedicatedWorkerServerConnector} | {@link DedicatedWorkerServer}
         * SharedWorker            | {@link SharedWorkerServerConnector}    | {@link SharedWorkerServer}
         *
         * ![Class Diagram](http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_basic_components.png)
         *
         * @see {@link SharedWorkerServer}, {@link IProtocol}
         * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components#iserverconnector)
         * @author Jeongho Nam <http://samchon.org>
         */
        var SharedWorkerServerConnector = (function (_super) {
            __extends(SharedWorkerServerConnector, _super);
            /* ---------------------------------------------------------
                CONSTRUCTORS AND CONNECTORS
            --------------------------------------------------------- */
            /**
             * Construct from *listener*.
             *
             * @param listener A listener object to listen replied message from newly connected client in
             *				   {@link IProtocol.replyData replyData()} as an {@link Invoke} object.
             */
            function SharedWorkerServerConnector(listener) {
                var _this = _super.call(this, listener) || this;
                _this.connected_ = false;
                _this.onConnect = null;
                return _this;
            }
            /**
             * Connect to a SharedWorker.
             *
             * Connects to a server with specified *jstFile* path. If a SharedWorker instance of the *jsFile* is not
             * constructed yet, then the SharedWorker will be newly constructed. Otherwise the SharedWorker already exists,
             * then connect to the SharedWorker. After those processes, callback function {@link onConnect} is called.
             * Listening data from the connected server also begins. Replied messages from the connected server will be
             * converted to {@link Invoke} classes and will be shifted to the {@link WebCommunicator.listener listener}'s
             * {@link IProtocol.replyData replyData()} method.
             *
             * If the connection fails immediately, either an event is dispatched or an exception is thrown: an error
             * event is dispatched if a host was specified, and an exception is thrown if no host was specified. Otherwise,
             * the status of the connection is reported by an event. If the socket is already connected, the existing
             * connection is closed first.
             *
             * @param jsFile Path of JavaScript file to execute who defines SharedWorker.
             */
            SharedWorkerServerConnector.prototype.connect = function (jsFile) {
                // CONSTRUCT AND START SHARED-WORKER-SERVER
                var worker = new SharedWorker(jsFile);
                // LISTEN MESSAGE
                this.port_ = worker.port;
                this.port_.onmessage = this.handle_message.bind(this);
                // NOTIFY THE CONNECTION
                this.connected_ = true;
                if (this.onConnect != null)
                    this.onConnect();
            };
            return SharedWorkerServerConnector;
        }(protocol.SharedWorkerCommunicator));
        protocol.SharedWorkerServerConnector = SharedWorkerServerConnector;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="../../API.ts" />
/// <reference path="DistributedSystemArray.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var distributed;
        (function (distributed) {
            /**
             * Master of Distributed Processing System, a server accepting slave clients.
             *
             * The {@link DistributedClientArray} is an abstract class, derived from the {@link DistributedSystemArray} class,
             * opening a server accepting {@link DistributedSystem distributed clients}.
             *
             * Extends this {@link DistributedClientArray}, overrides {@link createServerBase createServerBase()} to determine
             * which protocol to follow and {@link createExternalClient createExternalClient()} creating child
             * {@link DistributedSystem} object. After the extending and overridings, open this server using the
             * {@link open open()} method.
             *
             * #### [Inherited] {@link DistributedSystemArray}
             * The {@link DistributedSystemArray} is an abstract class containing and managing remote distributed **slave** system
             * drivers, {@link DistributedSystem} objects. Within framework of network, {@link DistributedSystemArray} represents
             * your system, a **Master** of *Distributed Processing System* that requesting *distributed process* to **slave**
             * systems and the children {@link DistributedSystem} objects represent the remote **slave** systems, who is being
             * requested the *distributed processes*.
             *
             * The {@link DistributedSystemArray} contains {@link DistributedProcess} objects directly. You can request a
             * **distributed process** through the {@link DistributedProcess} object. You can access the
             * {@link DistributedProcess} object(s) with those methods:
             *
             * - {@link hasProcess}
             * - {@link getProcess}
             * - {@link insertProcess}
             * - {@link eraseProcess}
             * - {@link getProcessMap}
             *
             * When you need the **distributed process**, call the {@link DistributedProcess.sendData} method. Then the
             * {@link DistributedProcess} will find the most idle {@link DistributedSystem} object who represents a distributed
             * **slave **system. The {@link Invoke} message will be sent to the most idle {@link DistributedSystem} object. When
             * the **distributed process** has completed, then {@link DistributedSystem.getPerformance performance index} and
             * {@link DistributedProcess.getResource resource index} of related objects will be revaluated.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Parallel Process
             * This {@link DistributedSystemArray} class is derived from the {@link ParallelSystemArray} class, so you can request
             * a **parallel process**, too.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices will
             * be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * #### Proxy Pattern
             * This class {@link DistributedSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link DistributedSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var DistributedClientArray = (function (_super) {
                __extends(DistributedClientArray, _super);
                /* =========================================================
                    CONSTRUCTORS
                        - MEMBER
                        - FACTORY METHOD FOR CHILDREN
                ============================================================
                    MEMBER
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function DistributedClientArray() {
                    return _super.call(this) || this;
                }
                /* ---------------------------------------------------------
                    FACTORY METHOD FOR CHILDREN
                --------------------------------------------------------- */
                /**
                 * Add a newly connected remote client.
                 *
                 * When a {@link IClientDriver remote client} connects to this *master server of parallel processing system*,
                 * then this {@link ParallelClientArray} creates a child {@link ParallelSystem parallel client} object through
                 * the {@link createExternalClient createExternalClient()} method and {@link insert inserts} it.
                 *
                 * @param driver A communicator for external client.
                 */
                DistributedClientArray.prototype.addClient = function (driver) {
                    var system = this.createExternalClient(driver);
                    if (system == null)
                        return;
                    this.push_back(system);
                };
                /**
                 * (Deprecated) Factory method creating child object.
                 *
                 * The method {@link createChild createChild()} is deprecated. Don't use and override this.
                 *
                 * Note that, the {@link ParallelClientArray} is a server accepting {@link ParallelSystem parallel clients}.
                 * There's no way to creating the {@link ParallelSystem parallel clients} in advance before opening the server.
                 *
                 * @param xml An {@link XML} object represents the child {@link ParallelSystem} object.
                 * @return ```null```
                 */
                DistributedClientArray.prototype.createChild = function (xml) { return null; };
                /* ---------------------------------------------------------
                    SERVER's METHOD
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                DistributedClientArray.prototype.open = function (port) {
                    this.server_base_ = this.createServerBase();
                    if (this.server_base_ == null)
                        return;
                    this.server_base_.open(port);
                };
                /**
                 * @inheritdoc
                 */
                DistributedClientArray.prototype.close = function () {
                    if (this.server_base_ == null)
                        return;
                    this.server_base_.close();
                    this.clear();
                };
                return DistributedClientArray;
            }(distributed.DistributedSystemArray));
            distributed.DistributedClientArray = DistributedClientArray;
        })(distributed = templates.distributed || (templates.distributed = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="DistributedSystemArrayMediator.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var distributed;
        (function (distributed) {
            /**
             * Mediator of Distributed Processing System, a server accepting slave clients.
             *
             * The {@link DistributedClientArrayMediator} is an abstract class, derived from {@link DistributedSystemArrayMediator}
             * class, opening a server accepting {@link DistributedSystem distributed clients} as a **master**.
             *
             * Extends this {@link DistributedClientArrayMediator}, overrides {@link createServerBase createServerBase()} to
             * determine which protocol to follow and {@link createExternalClient createExternalClient()} creating child
             * {@link DistributedSystem} object. After the extending and overridings, open this server using the
             * {@link open open()} method.
             *
             * #### [Inherited] {@link DistributedSystemArrayMediator}
             * The {@link DistributedSystemArrayMediator} class be a master for its slave systems, and be a slave to its master
             * system at the same time. This {@link DistributedSystemArrayMediator} be a master system, containing and managing
             * {@link DistributedSystem} objects, which represent distributed slave systems, by extending
             * {@link DistributedSystemArray} class. Also, be a slave system through {@link getMediator mediator} object, which is
             * derived from the {@link SlaveSystem} class.
             *
             * As a slave, you can specify this {@link DistributedSystemArrayMediator} to be <i>a client slave connecting to master
             * server</i> or <i>a server slave accepting master client</i> by overriding the {@link createMediator} method.
             * Overrides the {@link createMediator createMediator()} method and return one of them:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * #### [Inherited] {@link DistributedSystemArray}
             * The {@link DistributedSystemArray} is an abstract class containing and managing remote distributed **slave** system
             * drivers, {@link DistributedSystem} objects. Within framework of network, {@link DistributedSystemArray} represents
             * your system, a **Master** of *Distributed Processing System* that requesting *distributed process* to **slave**
             * systems and the children {@link DistributedSystem} objects represent the remote **slave** systems, who is being
             * requested the *distributed processes*.
             *
             * The {@link DistributedSystemArray} contains {@link DistributedProcess} objects directly. You can request a
             * **distributed process** through the {@link DistributedProcess} object. You can access the
             * {@link DistributedProcess} object(s) with those methods:
             *
             * - {@link hasProcess}
             * - {@link getProcess}
             * - {@link insertProcess}
             * - {@link eraseProcess}
             * - {@link getProcessMap}
             *
             * When you need the **distributed process**, call the {@link DistributedProcess.sendData} method. Then the
             * {@link DistributedProcess} will find the most idle {@link DistributedSystem} object who represents a distributed
             * **slave **system. The {@link Invoke} message will be sent to the most idle {@link DistributedSystem} object. When
             * the **distributed process** has completed, then {@link DistributedSystem.getPerformance performance index} and
             * {@link DistributedProcess.getResource resource index} of related objects will be revaluated.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Parallel Process
             * This {@link DistributedSystemArray} class is derived from the {@link ParallelSystemArray} class, so you can request
             * a **parallel process**, too.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices will
             * be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * #### Proxy Pattern
             * This class {@link DistributedSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link DistributedSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var DistributedClientArrayMediator = (function (_super) {
                __extends(DistributedClientArrayMediator, _super);
                /* =========================================================
                    CONSTRUCTORS
                        - MEMBER
                        - FACTORY METHOD FOR CHILDREN
                ============================================================
                    MEMBER
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function DistributedClientArrayMediator() {
                    return _super.call(this) || this;
                }
                /* ---------------------------------------------------------
                    FACTORY METHOD FOR CHILDREN
                --------------------------------------------------------- */
                /**
                 * Add a newly connected remote client.
                 *
                 * When a {@link IClientDriver remote client} connects to this *master server of distributed processing system*,
                 * then this {@link DistributedClientArrayMediator} creates a child {@link Distributed distributed client} object
                 * through the {@link createExternalClient createExternalClient()} method.
                 *
                 * @param driver A communicator for external client.
                 */
                DistributedClientArrayMediator.prototype.addClient = function (driver) {
                    var system = this.createExternalClient(driver);
                    if (system == null)
                        return;
                    this.push_back(system);
                };
                /**
                 * (Deprecated) Factory method creating child object.
                 *
                 * The method {@link createChild createChild()} is deprecated. Don't use and override this.
                 *
                 * Note that, the {@link DistributedClientArrayMediator} is a server accepting {@link DistributedSystem distributed
                 * clients} as a master. There's no way to creating the {@link DistributedSystem distributed clients} in advance
                 * before opening the server.
                 *
                 * @param xml An {@link XML} object represents the child {@link DistributedSystem} object.
                 * @return null
                 */
                DistributedClientArrayMediator.prototype.createChild = function (xml) { return null; };
                /* ---------------------------------------------------------
                    SERVER's METHOD
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                DistributedClientArrayMediator.prototype.open = function (port) {
                    this.server_base_ = this.createServerBase();
                    if (this.server_base_ == null)
                        return;
                    this.server_base_.open(port);
                };
                /**
                 * @inheritdoc
                 */
                DistributedClientArrayMediator.prototype.close = function () {
                    if (this.server_base_ == null)
                        return;
                    this.server_base_.close();
                    this.clear();
                };
                return DistributedClientArrayMediator;
            }(distributed.DistributedSystemArrayMediator));
            distributed.DistributedClientArrayMediator = DistributedClientArrayMediator;
        })(distributed = templates.distributed || (templates.distributed = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../../protocol/Entity.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var distributed;
        (function (distributed) {
            /**
             * A process of Distributed Processing System.
             *
             * The {@link DistributedProcess} is an abstract class who represents a **process**, *SOMETHING TO DISTRIBUTE* in a Distributed
             * Processing System. Overrides the {@link DistributedProcess} and defines the *SOMETHING TO DISTRIBUTE*.
             *
             * Relationship between {@link DistributedSystem} and {@link DistributedProcess} objects are **M: N Associative**.
             * Unlike {@link ExternalSystemRole}, the {@link DistributedProcess} objects are not belonged to a specific
             * {@link DistributedSystem} object. The {@link DistributedProcess} objects are belonged to the
             * {@link DistributedSystemArrayMediator} directly.
             *
             * When you need the **distributed process**, then call {@link sendData sendData()}. The {@link sendData} will find
             * the most idle {@link DistributedSystem slave system} considering not only number of processes on progress, but also
             * {@link DistributedSystem.getPerformance performance index} of each {@link DistributedSystem} object and
             * {@link getResource resource index} of this {@link DistributedProcess} object. The {@link Invoke} message
             * requesting the **distributed process** will be sent to the most idle {@link DistributedSystem slave system}.
             *
             * Those {@link DistributedSystem.getPerformance performance index} and {@link getResource resource index} are
             * revaluated whenever the **distributed process** has completed basis on the execution time.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var DistributedProcess = (function (_super) {
                __extends(DistributedProcess, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Constrct from parent {@link DistributedSystemArray} object.
                 *
                 * @param systemArray The parent {@link DistributedSystemArray} object.
                 */
                function DistributedProcess(systemArray) {
                    var _this = _super.call(this) || this;
                    _this.system_array_ = systemArray;
                    _this.name = "";
                    // PERFORMANCE INDEX
                    _this.resource = 1.0;
                    _this.progress_list_ = new std.HashMap();
                    _this.history_list_ = new std.HashMap();
                    return _this;
                }
                /* ---------------------------------------------------------
                    ACCESSORS
                --------------------------------------------------------- */
                /**
                 * Identifier of {@link ParallelProcess} is its {@link name}.
                 */
                DistributedProcess.prototype.key = function () {
                    return this.name;
                };
                DistributedProcess.prototype.getSystemArray = function () {
                    return this.system_array_;
                };
                /**
                 * Get name, who represents and identifies this process.
                 */
                DistributedProcess.prototype.getName = function () {
                    return this.name;
                };
                /**
                 * Get resource index.
                 *
                 * Get *resource index* that indicates how much this {@link DistributedProcess process} is heavy.
                 *
                 * If this {@link DistributedProcess process} does not have any	{@link Invoke} message had handled, then the
                 * *resource index* will be ```1.0```, which means default and average value between all
                 * {@link DistributedProcess} instances (that are belonged to a same {@link DistributedSystemArray} object).
                 *
                 * You can specify the *resource index* by yourself, but notice that, if the *resource index* is higher than
                 * other {@link DistributedProcess} objects, then this {@link DistributedProcess process} will be ordered to
                 * handle less processes than other {@link DistributedProcess} objects. Otherwise, the *resource index* is
                 * lower than others, of course, much processes will be requested.
                 *
                 * - {@link setResource setResource()}
                 * - {@link enforceResource enforceResource()}
                 *
                 * Unless {@link enforceResource enforceResource()} is called, This *resource index* is **revaluated** whenever
                 * {@link sendData sendData()} is called.
                 *
                 * @return Resource index.
                 */
                DistributedProcess.prototype.getResource = function () {
                    return this.resource;
                };
                /**
                 * Set resource index.
                 *
                 * Set *resource index* that indicates how much this {@link DistributedProcess process} is heavy. This
                 * *resource index* can be **revaulated**.
                 *
                 * Note that, initial and average *resource index* of {@link DistributedProcess} objects are ```1.0```. If the
                 * *resource index* is higher than other {@link DistributedProcess} objects, then this
                 * {@link DistributedProcess} will be ordered to handle more processes than other {@link DistributedProcess}
                 * objects. Otherwise, the *resource index* is lower than others, of course, less processes will be requested.
                 *
                 * Unlike {@link enforceResource}, configuring *resource index* by this {@link setResource} allows the
                 * **revaluation**. This **revaluation** prevents wrong valuation from user. For example, you *mis-valuated* the
                 * *resource index*. The {@link DistributedProcess process} is much heavier than any other, but you estimated it
                 * to the lightest one. It looks like a terrible case that causes
                 * {@link DistributedSystemArray entire distributed processing system} to be slower, however, don't mind. The
                 * {@link DistributedProcess process} will the direct to the *propriate resource index* eventually with the
                 * **revaluation**.
                 *
                 * - The **revaluation** is caused by the {@link sendData sendData()} method.
                 *
                 * @param val New resource index, but can be revaluated.
                 */
                DistributedProcess.prototype.setResource = function (val) {
                    this.resource = val;
                    this.enforced_ = false;
                };
                /**
                 * Enforce resource index.
                 *
                 * Enforce *resource index* that indicates how much heavy the {@link DistributedProcess process is}. The
                 * *resource index* will be fixed, never be **revaluated**.
                 *
                 * Note that, initial and average *resource index* of {@link DistributedProcess} objects are ```1.0```. If the
                 * *resource index* is higher than other {@link DistributedProcess} objects, then this
                 * {@link DistributedProcess} will be ordered to handle more processes than other {@link DistributedProcess}
                 * objects. Otherwise, the *resource index* is lower than others, of course, less processes will be requested.
                 *
                 * The difference between {@link setResource} and this {@link enforceResource} is allowing **revaluation** or not.
                 * This {@link enforceResource} does not allow the **revaluation**. The *resource index* is clearly fixed and
                 * never be changed by the **revaluation**. But you've to keep in mind that, you can't avoid the **mis-valuation**
                 * with this {@link enforceResource}.
                 *
                 * For example, there's a {@link DistributedProcess process} much heavier than any other, but you
                 * **mis-estimated** it to the lightest. In that case, there's no way. The
                 * {@link DistributedSystemArray entire distributed processing system} will be slower by the **mis-valuation**.
                 * By the reason, using {@link enforceResource}, it's recommended only when you can clearly certain the
                 * *resource index*. If you can't certain the *resource index* but want to recommend, then use {@link setResource}
                 * instead.
                 *
                 * @param val New resource index to be fixed.
                 */
                DistributedProcess.prototype.enforceResource = function (val) {
                    this.resource = val;
                    this.enforced_ = true;
                };
                /**
                 * @hidden
                 */
                DistributedProcess.prototype._Compute_average_elapsed_time = function () {
                    var sum = 0;
                    for (var it = this.history_list_.begin(); !it.equals(this.history_list_.end()); it = it.next()) {
                        var history_1 = it.second;
                        var elapsed_time = history_1.computeElapsedTime() / history_1.getWeight();
                        // THE SYSTEM'S PERFORMANCE IS 5. THE SYSTEM CAN HANDLE A PROCESS VERY QUICKLY
                        //	AND ELAPSED TIME OF THE PROCESS IS 3 SECONDS
                        //	THEN I CONSIDER THE ELAPSED TIME AS 15 SECONDS.
                        sum += elapsed_time * history_1.getSystem().getPerformance();
                    }
                    return sum / this.history_list_.size();
                };
                DistributedProcess.prototype.sendData = function (invoke, weight) {
                    if (weight === void 0) { weight = 1.0; }
                    if (this.system_array_.empty() == true)
                        return null;
                    // ADD UID FOR ARCHIVING HISTORY
                    var uid;
                    if (invoke.has("_History_uid") == false) {
                        // ISSUE UID AND ATTACH IT TO INVOKE'S LAST PARAMETER
                        uid = ++this.system_array_.history_sequence_;
                        invoke.push_back(new samchon.protocol.InvokeParameter("_History_uid", uid));
                    }
                    else {
                        // INVOKE MESSAGE ALREADY HAS ITS OWN UNIQUE ID
                        //	- system_array_ IS A TYPE OF DistributedSystemArrayMediator. THE MESSAGE HAS COME FROM ITS MASTER
                        //	- A Distributed HAS DISCONNECTED. THE SYSTEM SHIFTED ITS CHAIN TO ANOTHER SLAVE.
                        uid = invoke.get("_History_uid").getValue();
                        // FOR CASE 1. UPDATE HISTORY_SEQUENCE TO MAXIMUM
                        this.system_array_.history_sequence_ = uid;
                        // FOR CASE 2. ERASE ORDINARY PROGRESSIVE HISTORY FROM THE DISCONNECTED
                        this.progress_list_.erase(uid);
                    }
                    // ADD PROCESS NAME AND WEIGHT FOR MEDIATOR
                    if (invoke.has("_Process_name") == false)
                        invoke.push_back(new samchon.protocol.InvokeParameter("_Process_name", this.name));
                    if (invoke.has("_Process_weight") == false)
                        invoke.push_back(new samchon.protocol.InvokeParameter("_Process_weight", weight));
                    else
                        weight = invoke.get("_Process_name").getValue();
                    // FIND THE MOST IDLE SYSTEM
                    var idle_system = null;
                    for (var i = 0; i < this.system_array_.size(); i++) {
                        var system = this.system_array_.at(i);
                        if (system.exclude_ == true)
                            continue; // BEING REMOVED SYSTEM
                        if (idle_system == null ||
                            system.history_list_.empty() == true ||
                            system.progress_list_.size() < idle_system.progress_list_.size() ||
                            (system.progress_list_.size() == idle_system.progress_list_.size() &&
                                system.getPerformance() > idle_system.getPerformance()) ||
                            (system.progress_list_.size() == idle_system.progress_list_.size() &&
                                system.getPerformance() == idle_system.getPerformance() &&
                                system.history_list_.size() < idle_system.history_list_.size()))
                            if (idle_system == null || idle_system.history_list_.empty() == false)
                                idle_system = system;
                    }
                    if (idle_system == null)
                        throw new std.OutOfRange("No remote system to send data");
                    // ARCHIVE HISTORY ON PROGRESS_LIST (IN SYSTEM AND ROLE AT THE SAME TIME)
                    var history = new distributed.DSInvokeHistory(idle_system, this, invoke, weight);
                    this.progress_list_.insert([uid, history]);
                    idle_system.progress_list_.insert([uid, std.make_pair(invoke, history)]);
                    // SEND DATA
                    idle_system.sendData(invoke);
                    // RETURN THE IDLE SYSTEM, WHO SENT THE INVOKE MESSAGE.
                    return idle_system;
                };
                /**
                 * @hidden
                 */
                DistributedProcess.prototype._Complete_history = function (history) {
                    // ERASE FROM ORDINARY PROGRESS AND MIGRATE TO THE HISTORY
                    this.progress_list_.erase(history.getUID());
                    this.history_list_.insert([history.getUID(), history]);
                };
                /* ---------------------------------------------------------
                    EXPORTERS
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                DistributedProcess.prototype.TAG = function () {
                    return "process";
                };
                return DistributedProcess;
            }(samchon.protocol.Entity));
            distributed.DistributedProcess = DistributedProcess;
        })(distributed = templates.distributed || (templates.distributed = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../../protocol/EntityCollection.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var external;
        (function (external) {
            /**
             * An external system driver.
             *
             * The {@link ExternalSystem} class represents an external system, connected and interact with this system.
             * {@link ExternalSystem} takes full charge of network communication with the remote, external system have connected.
             * Replied {@link Invoke} messages from the external system is shifted to and processed in, children elements of this
             * class, {@link ExternalSystemRole} objects.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Bridge & Proxy Pattern
             * The {@link ExternalSystem} class can be a *bridge* for *logical proxy*. In framework within user,
             * which {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Bridge Pattern* and *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/TypeScript-Templates-External_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ExternalSystem = (function (_super) {
                __extends(ExternalSystem, _super);
                function ExternalSystem(systemArray, communicator) {
                    if (communicator === void 0) { communicator = null; }
                    var _this = _super.call(this) || this;
                    _this.system_array_ = systemArray;
                    _this.communicator = communicator;
                    if (communicator != null)
                        communicator.listen(_this);
                    _this.name = "";
                    return _this;
                }
                /**
                 * Default Destructor.
                 *
                 * This {@link destructor destructor()} method is called when the {@link ExternalSystem} object is destructed and
                 * the {@link ExternalSystem} object is destructed when connection with the remote system is closed or this
                 * {@link ExternalSystem} object is {@link ExternalSystemArray.erase erased} from its parent
                 * {@link ExternalSystemArray} object.
                 *
                 * Note that, don't call this {@link destructor destructor()} method by yourself. It must be called automatically
                 * by those *destruction* cases. Also, if your derived {@link ExternalSystem} class has something to do on the
                 * *destruction*, then overrides this {@link destructor destructor()} method and defines the something to do.
                 * Overriding this {@link destructor destructor()}, don't forget to calling ```super.destructor();``` on tail.
                 *
                 * ```typescript
                 * class SomeSystem extends templates.external.ExternalSystem
                 * {
                 *     protected destructor(): void
                 *     {
                 *         // DO SOMETHING
                 *         this.do_something();
                 *
                 *         // CALL SUPER.DESTRUCTOR() ON TAIL. DON'T FORGET THIS
                 *         super.destructor();
                 *     }
                 * }
                 * ```
                 */
                ExternalSystem.prototype.destructor = function () {
                    if (this.communicator != null && this.communicator.isConnected() == true) {
                        this.communicator.onClose = null;
                        this.communicator.close();
                    }
                };
                /**
                 * @hidden
                 */
                ExternalSystem.prototype._Handle_close = function () {
                    if (this.system_array_ == null)
                        return;
                    else
                        std.remove(this.system_array_.begin(), this.system_array_.end(), this);
                };
                ExternalSystem.prototype.getSystemArray = function () {
                    return this.system_array_;
                };
                /**
                 * Identifier of {@link ExternalSystem} is its {@link name}.
                 *
                 * @return name.
                 */
                ExternalSystem.prototype.key = function () {
                    return this.name;
                };
                /**
                 * Get {@link name}.
                 */
                ExternalSystem.prototype.getName = function () {
                    return this.name;
                };
                Object.defineProperty(ExternalSystem.prototype, "communicator", {
                    /**
                     * @hidden
                     */
                    get: function () {
                        return this.communicator_;
                    },
                    /**
                     * @hidden
                     */
                    set: function (val) {
                        this.communicator_ = val;
                        if (this.communicator_ != null)
                            this.communicator.onClose = this._Handle_close.bind(this);
                    },
                    enumerable: true,
                    configurable: true
                });
                /* ---------------------------------------------------------
                    NETWORK & MESSAGE CHAIN
                --------------------------------------------------------- */
                /**
                 * Close connection.
                 */
                ExternalSystem.prototype.close = function () {
                    this.communicator.close();
                };
                /**
                 * Send {@link Invoke} message to external system.
                 *
                 * @param invoke An {@link Invoke} message to send.
                 */
                ExternalSystem.prototype.sendData = function (invoke) {
                    this.communicator.sendData(invoke);
                };
                /**
                 * Handle an {@Invoke} message has received.
                 *
                 * @param invoke An {@link Invoke} message have received.
                 */
                ExternalSystem.prototype.replyData = function (invoke) {
                    // SHIFT TO SYSTEM_ARRAY
                    this.system_array_.replyData(invoke);
                    // SHIFT TO ROLES
                    for (var i = 0; i < this.size(); i++)
                        this.at(i).replyData(invoke);
                };
                /* ---------------------------------------------------------
                    EXPORTERS
                --------------------------------------------------------- */
                /**
                 * Tag name of the {@link ExternalSystem} in {@link XML}.
                 *
                 * @return *system*.
                 */
                ExternalSystem.prototype.TAG = function () {
                    return "system";
                };
                /**
                 * Tag name of {@link ExternalSystemRole children elements} belonged to the {@link ExternalSystem} in {@link XML}.
                 *
                 * @return *role*.
                 */
                ExternalSystem.prototype.CHILD_TAG = function () {
                    return "role";
                };
                return ExternalSystem;
            }(samchon.protocol.EntityDequeCollection));
            external.ExternalSystem = ExternalSystem;
        })(external = templates.external || (templates.external = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../external/ExternalSystem.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * A driver for a parallel slave system.
             *
             * The {@link ParallelSystem} is an abstract class represents a **slave** system in *Parallel Processing System*,
             * connected with this **master** system. This {@link ParallelSystem} takes full charge of network communication with
             * the remote, parallel **slave** system has connected.
             *
             * When a *parallel process* is requested (by {@link ParallelSystemArray.sendSegmentData} or
             * {@link ParallelSystemArray.sendPieceData}), the number of pieces to be allocated to a {@link ParallelSystem} is
             * turn on its {@link getPerformance performance index}. Higher {@link getPerformance performance index}, then
             * more pieces are requested. The {@link getPerformance performance index} is revaluated whenever a *parallel process*
             * has completed, basic on the execution time and number of pieces. You can sugguest or enforce the
             * {@link getPerformance performance index} with {@link setPerformance} or {@link enforcePerformance}.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Bridge & Proxy Pattern
             * This class {@link ParallelSystem} is derived from the {@link ExternalSystem} class. Thus, you can take advantage
             * of the *Bridge & Proxy Pattern* in this {@link ParallelSystem} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Bridge & Proxy Pattern*:
             *
             * The {@link ExternalSystem} class can be a *bridge* for *logical proxy*. In framework within user,
             * which {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Bridge Pattern* and *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ParallelSystem = (function (_super) {
                __extends(ParallelSystem, _super);
                function ParallelSystem(systemArray, communicator) {
                    if (communicator === void 0) { communicator = null; }
                    var _this = _super.call(this, systemArray, communicator) || this;
                    // HIDDEN MEMBERS
                    _this.progress_list_ = new std.HashMap();
                    _this.history_list_ = new std.HashMap();
                    _this.enforced_ = false;
                    _this.exclude_ = false;
                    // PERFORMANCE INDEX
                    _this.performance = 1.0;
                    return _this;
                }
                /**
                 * Default Destructor.
                 *
                 * This {@link destructor destructor()} method is called when the {@link ParallelSystem} object is destructed and
                 * the {@link ParallelSystem} object is destructed when connection with the remote system is closed or this
                 * {@link ParallelSystem} object is {@link ParallelSystemArray.erase erased} from its parent
                 * {@link ParallelSystemArray} object.
                 *
                 * You may think if there're some *parallel processes* have requested but not completed yet, then it would be a
                 * critical problem because the *parallel processes* will not complete forever. Do not worry. The critical problem
                 * does not happen. After the destruction, the remained *parallel processes* will be shifted to and proceeded in
                 * other {@link ParallelSystem} objects.
                 *
                 * Note that, don't call this {@link destructor destructor()} method by yourself. It must be called automatically
                 * by those *destruction* cases. Also, if your derived {@link ParallelSystem} class has something to do on the
                 * *destruction*, then overrides this {@link destructor destructor()} method and defines the something to do.
                 * Overriding this {@link destructor destructor()}, don't forget to calling ```super.destructor();``` on tail.
                 *
                 * ```typescript
                 * class SomeSystem extends protocol.external.ExternalSystem
                 * {
                 *     protected destructor(): void
                 *     {
                 *         // DO SOMETHING
                 *         this.do_something();
                 *
                 *         // CALL SUPER.DESTRUCTOR() ON TAIL. DON'T FORGET THIS
                 *         super.destructor();
                 *     }
                 * }
                 * ```
                 */
                ParallelSystem.prototype.destructor = function () {
                    this.exclude_ = true;
                    for (var it = this.progress_list_.begin(); !it.equals(this.progress_list_.end()); it = it.next()) {
                        // AN INVOKE AND HISTORY HAD PROGRESSED
                        var invoke = it.second.first;
                        var history_2 = it.second.second;
                        this._Send_back_history(invoke, history_2);
                    }
                    _super.prototype.destructor.call(this);
                };
                ParallelSystem.prototype.getSystemArray = function () {
                    return this.system_array_;
                };
                /**
                 * Get performance index.
                 *
                 * Get *performance index* that indicates how much fast the remote system is.
                 *
                 * If this {@link ParallelSystem parallel system} does not have any {@link Invoke} message had handled, then the
                 * *performance index* will be ```1.0```, which means default and average value between all {@link ParallelSystem}
                 * instances (that are belonged to a same {@link ParallelSystemArray} object).
                 *
                 * You can specify this *performance index* by yourself but notice that, if the *performance index* is higher
                 * than other {@link ParallelSystem} objects, then this {@link ParallelSystem parallel system} will be ordered to
                 * handle more processes than other {@link ParallelSystem} objects. Otherwise, the *performance index* is lower
                 * than others, of course, less processes will be delivered.
                 *
                 * - {@link setPerformance setPerformance()}
                 * - {@link enforcePerformance enforcePerformance()}
                 *
                 * Unless {@link enforcePerformance enforcePerformance()} is called, This *performance index* is **revaluated**
                 * whenever user calls one of them below.
                 *
                 * - {@link ParallelSystemArray.sendSegmentData ParallelSystemArray.sendSegmentData()}
                 * - {@link ParallelSystemArray.sendPieceData ParallelSystemArray.sendPieceData()}
                 * - {@link DistributedProcess.sendData DistributedProcess.sendData()}.
                 *
                 * @return Performance index.
                 */
                ParallelSystem.prototype.getPerformance = function () {
                    return this.performance;
                };
                /**
                 * Set performance index.
                 *
                 * Set *performance index* that indicates how much fast the remote system is. This *performance index* can be
                 * **revaulated**.
                 *
                 * Note that, initial and average *performance index* of {@link ParallelSystem} objects are ```1.0```. If the
                 * *performance index* is higher than other {@link ParallelSystem} objects, then this {@link ParallelSystem} will
                 * be ordered to handle more processes than other {@link ParallelSystem} objects. Otherwise, the
                 * *performance index* is lower than others, of course, less processes will be delivered.
                 *
                 * Unlike {@link enforcePerformance}, configuring *performance index* by this {@link setPerformance} allows
                 * **revaluation**. This **revaluation** prevents wrong valuation from user. For example, you *mis-valuated* the
                 * *performance index*. The remote system is much faster than any other, but you estimated it to the slowest one.
                 * It looks like a terrible case that causes {@link ParallelSystemArray entire parallel systems} to be slower,
                 * however, don't mind. The system will direct to the *propriate performance index* eventually with the
                 * **revaluation** by following methods.
                 *
                 * - {@link ParallelSystemArray.sendSegmentData ParallelSystemArray.sendSegmentData()}
                 * - {@link ParallelSystemArray.sendPieceData ParallelSystemArray.sendPieceData()}
                 * - {@link DistributedProcess.sendData DistributedProcess.sendData()}.
                 *
                 * @param val New performance index, but can be revaluated.
                 */
                ParallelSystem.prototype.setPerformance = function (val) {
                    this.performance = val;
                    this.enforced_ = false;
                };
                /**
                 * Enforce performance index.
                 *
                 * Enforce *performance index* that indicates how much fast the remote system is. The *performance index* will be
                 * fixed, never be **revaluated**.
                 *
                 * Note that, initial and average *performance index* of {@link ParallelSystem} objects are ```1.0```. If the
                 * *performance index* is higher than other {@link ParallelSystem} objects, then this {@link ParallelSystem} will
                 * be ordered to handle more processes than other {@link ParallelSystem} objects. Otherwise, the
                 * *performance index* is lower than others, of course, less processes will be delivered.
                 *
                 * The difference between {@link setPerformance} and this {@link enforcePerformance} is allowing **revaluation**
                 * or not. This {@link enforcePerformance} does not allow the **revaluation**. The *performance index* is clearly
                 * fixed and never be changed by the **revaluation**. But you've to keep in mind that, you can't avoid the
                 * **mis-valuation** with this {@link enforcePerformance}.
                 *
                 * For example, there's a remote system much faster than any other, but you **mis-estimated** it to the slowest.
                 * In that case, there's no way. The {@link ParallelSystemArray entire parallel systems} will be slower by the
                 * **mis-valuation**. By the reason, using {@link enforcePerformance}, it's recommended only when you can clearly
                 * certain the *performance index*. If you can't certain the *performance index* but want to recommend, then use
                 * {@link setPerformance} instead.
                 *
                 * @param val New performance index to be fixed.
                 */
                ParallelSystem.prototype.enforcePerformance = function (val) {
                    this.performance = val;
                    this.enforced_ = true;
                };
                /* ---------------------------------------------------------
                    MESSAGE CHAIN
                --------------------------------------------------------- */
                /**
                 * @hidden
                 */
                ParallelSystem.prototype._Send_piece_data = function (invoke, first, last) {
                    // DUPLICATE INVOKE AND ATTACH PIECE INFO
                    var my_invoke = new samchon.protocol.Invoke(invoke.getListener());
                    {
                        my_invoke.assign(invoke.begin(), invoke.end());
                        my_invoke.push_back(new samchon.protocol.InvokeParameter("_Piece_first", first));
                        my_invoke.push_back(new samchon.protocol.InvokeParameter("_Piece_last", last));
                    }
                    // REGISTER THE UID AS PROGRESS
                    var history = new parallel.PRInvokeHistory(my_invoke);
                    this.progress_list_.insert([
                        history.getUID(),
                        std.make_pair(my_invoke, history) // VALUE: PAIR OF INVOKE AND ITS HISTORY
                    ]);
                    // SEND DATA
                    this.sendData(my_invoke);
                };
                /**
                 * @hidden
                 */
                ParallelSystem.prototype._Reply_data = function (invoke) {
                    if (invoke.getListener() == "_Report_history") {
                        this._Report_history(invoke.front().getValue());
                    }
                    else if (invoke.getListener() == "_Send_back_history") {
                        var uid = invoke.front().getValue();
                        var it = this.progress_list_.find(uid);
                        if (it.equals(this.progress_list_.end()) == true)
                            return;
                        this._Send_back_history(it.second.first, it.second.second);
                    }
                    else
                        this.replyData(invoke);
                };
                /**
                 * @hidden
                 */
                ParallelSystem.prototype._Report_history = function (xml) {
                    //--------
                    // CONSTRUCT HISTORY
                    //--------
                    var history = new parallel.PRInvokeHistory();
                    history.construct(xml);
                    // IF THE HISTORY IS NOT EXIST IN PROGRESS, THEN TERMINATE REPORTING
                    var progress_it = this.progress_list_.find(history.getUID());
                    if (progress_it.equals(this.progress_list_.end()) == true)
                        return;
                    // ARCHIVE FIRST AND LAST INDEX
                    history.first = progress_it.second.second.getFirst();
                    history.last = progress_it.second.second.computeSize();
                    // ERASE FROM ORDINARY PROGRESS AND MIGRATE TO THE HISTORY
                    this.progress_list_.erase(progress_it);
                    this.history_list_.insert([history.getUID(), history]);
                    // NOTIFY TO THE MANAGER, SYSTEM_ARRAY
                    this.getSystemArray()._Complete_history(history);
                };
                /**
                 * @hidden
                 */
                ParallelSystem.prototype._Send_back_history = function (invoke, history) {
                    if (history instanceof parallel.PRInvokeHistory) {
                        // REMOVE UID AND FIRST, LAST INDEXES
                        std.remove_if(invoke.begin(), invoke.end(), function (param) {
                            return param.getName() == "_History_uid"
                                || param.getName() == "_Piece_first"
                                || param.getName() == "_Piece_last";
                        });
                        // RE-SEND (DISTRIBUTE) THE PIECE TO OTHER SLAVES
                        this.getSystemArray().sendPieceData(invoke, history.getFirst(), history.getLast());
                    }
                    // ERASE FROM THE PROGRESS LIST
                    this.progress_list_.erase(history.getUID());
                };
                return ParallelSystem;
            }(templates.external.ExternalSystem));
            parallel.ParallelSystem = ParallelSystem;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../parallel/ParallelSystem.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var distributed;
        (function (distributed) {
            /**
             * A driver for a distributed slave system.
             *
             * The {@link DistributedSystem} is an abstract class represents a **slave** system in *Distributed Processing System*,
             * connected with this **master** system. This {@link DistributedSystem} takes full charge of network communication
             * with the remote, distributed **slave** system has connected.
             *
             * This {@link DistributedSystem} has a {@link getPerformance performance index} that indicates how much the **slave**
             * system is fast. The {@link getPerformance performance index} is referenced and revaluated whenever those methods
             * are called:
             *
             * - Requesting a *parallel process*
             *   - {@link DistributedSystemArray.sendSegmentData}
             *   - {@link DistributedSystemArray.sendPieceData}
             * - Requesting a *distributed process*: {@link DistributedProcess.sendData}
             *
             * Note that, this {@link DistributedSystem} class derived from the {@link ExternalSystem} class. Thus, this
             * {@link DistributedSystem} can also have children {@link ExternalSystemRole} objects exclusively. However, the
             * children {@link ExternalSystemRole roles} objects are different with the {@link DistributedProcess}. The
             * domestic {@link ExternalSystemRole roles} are belonged to only a specific {@link DistributedSystem} object.
             * Otherwise, the {@link DistributedProcess} objects are belonged to a {@link DistributedSystemArray} object.
             * Furthermore, the relationship between this {@link DistributedSystem} and {@link DistributedProcess} classes are
             * **M: N Associative**.
             *
             * Articles     | {@link DistributedProcess}     | {@link ExternalSystemRole}
             * -------------|--------------------------------|----------------------------
             * Belonged to  | {@link DistributedSystemArray} | {@link DistributedSystem}
             * Relationship | M: N Associative               | 1: N Composite
             * Ownership    | References                     | Exclusive possession
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var DistributedSystem = (function (_super) {
                __extends(DistributedSystem, _super);
                function DistributedSystem(systemArray, communicator) {
                    if (communicator === void 0) { communicator = null; }
                    return _super.call(this, systemArray, communicator) || this;
                }
                // using super::destructor
                /**
                 * Factory method creating a {@link ExternalSystemRole child} object.
                 *
                 * In {@link distributed} module, the process class {@link DistributedProcess} is not belonged to a specific
                 * {@link DistributedSystem} object. It only belongs to a {@link DistributedSystemArray} object and has a
                 * **M: N Associative Relationship** between this {@link DistributedSystem} class.
                 *
                 * By that reason, it's the normal case that the {@link DistributedSystem} object does not have any children
                 * {@link ExternalSystemRole} object. Thus, default {@link createChild} returns ```null```.
                 *
                 * However, if you want a {@link DistributedSystem} to have its own domestic {@link ExternalSystemRole} objects
                 * without reference to the {@link DistributedProcess} objects, it is possible. Creates and returns the
                 * domestic {@link ExternalSystemRole} object.
                 *
                 * @param xml {@link XML} represents the {@link ExternalSystemRole child} object.
                 * @return A newly created {@link ExternalSystemRole} object or ```null```.
                 */
                DistributedSystem.prototype.createChild = function (xml) {
                    return null;
                };
                DistributedSystem.prototype.getSystemArray = function () {
                    return this.system_array_;
                };
                /**
                 * @hidden
                 */
                DistributedSystem.prototype._Compute_average_elapsed_time = function () {
                    var sum = 0;
                    var denominator = 0;
                    for (var it = this.history_list_.begin(); !it.equals(this.history_list_.end()); it = it.next()) {
                        var history_3 = it.second;
                        if (history_3 instanceof distributed.DSInvokeHistory == false)
                            continue;
                        var elapsed_time = history_3.computeElapsedTime() / history_3.getWeight();
                        sum += elapsed_time / history_3.getProcess().getResource();
                        denominator++;
                    }
                    if (denominator == 0)
                        return -1;
                    else
                        return sum / denominator;
                };
                /* ---------------------------------------------------------
                    MESSAGE CHAIN
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                DistributedSystem.prototype.replyData = function (invoke) {
                    // SHIFT TO ROLES
                    var role_map = this.getSystemArray().getProcessMap();
                    for (var it = role_map.begin(); !it.equals(role_map.end()); it = it.next())
                        it.second.replyData(invoke);
                    // SHIFT TO PARENT AND CHIILDREN, EXCLUSIVE ROLES
                    _super.prototype.replyData.call(this, invoke);
                };
                /**
                 * @hidden
                 */
                DistributedSystem.prototype._Report_history = function (xml) {
                    // ParallelSystem's history -> PRInvokeHistory
                    if (xml.hasProperty("_Piece_first") == true)
                        return _super.prototype._Report_history.call(this, xml);
                    //--------
                    // DistributedProcess's history -> DSInvokeHistory
                    //--------
                    // CONSTRUCT HISTORY
                    var history = new distributed.DSInvokeHistory(this);
                    history.construct(xml);
                    // IF THE HISTORY HAS NOT EXISTED IN PROGRESS, THEN TERMINATE REPORTING
                    var progress_it = this.progress_list_.find(history.getUID());
                    if (progress_it.equals(this.progress_list_.end()) == true)
                        return;
                    history.process_ = progress_it.second.second.getProcess();
                    history.weight_ = progress_it.second.second.getWeight();
                    // ERASE FROM ORDINARY PROGRESS AND MIGRATE TO THE HISTORY
                    this.progress_list_.erase(progress_it);
                    this.history_list_.insert([history.getUID(), history]);
                    // REPORT TO THE ROLE
                    if (history.getProcess() != null)
                        history.getProcess()._Complete_history(history);
                    // COMPLETE THE HISTORY IN THE BELONGED SYSTEM_ARRAY
                    this.getSystemArray()._Complete_history(history);
                };
                /**
                 * @hidden
                 */
                DistributedSystem.prototype._Send_back_history = function (invoke, history) {
                    if (history instanceof distributed.DSInvokeHistory) {
                        // RE-SEND INVOKE MESSAGE TO ANOTHER SLAVE VIA ROLE
                        history.getProcess().sendData(invoke, history.getWeight());
                    }
                    // ERASE THE HISTORY
                    _super.prototype._Send_back_history.call(this, invoke, history);
                };
                return DistributedSystem;
            }(templates.parallel.ParallelSystem));
            distributed.DistributedSystem = DistributedSystem;
        })(distributed = templates.distributed || (templates.distributed = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="DistributedSystem.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var distributed;
        (function (distributed) {
            /**
             * A driver for distributed slave server.
             *
             * The {@link DistributedServer} is an abstract class, derived from the {@link DistributedSystem} class, connecting to
             * remote, distributed **slave** server. Extends this {@link DistributedServer} class and overrides the
             * {@link createServerConnector createServerConnector()} method following which protocol the **slave** server uses.
             *
             * #### [Inheritdoc] {@link DistributedSystem}
             * The {@link DistributedSystem} is an abstract class represents a **slave** system in *Distributed Processing System*,
             * connected with this **master** system. This {@link DistributedSystem} takes full charge of network communication
             * with the remote, distributed **slave** system has connected.
             *
             * This {@link DistributedSystem} has a {@link getPerformance performance index} that indicates how much the **slave**
             * system is fast. The {@link getPerformance performance index} is referenced and revaluated whenever those methods
             * are called:
             *
             * - Requesting a *parallel process*
             *   - {@link DistributedSystemArray.sendSegmentData}
             *   - {@link DistributedSystemArray.sendPieceData}
             * - Requesting a *distributed process*: {@link DistributedProcess.sendData}
             *
             * Note that, this {@link DistributedSystem} class derived from the {@link ExternalSystem} class. Thus, this
             * {@link DistributedSystem} can also have children {@link ExternalSystemRole} objects exclusively. However, the
             * children {@link ExternalSystemRole roles} objects are different with the {@link DistributedProcess}. The
             * domestic {@link ExternalSystemRole roles} are belonged to only a specific {@link DistributedSystem} object.
             * Otherwise, the {@link DistributedProcess} objects are belonged to a {@link DistributedSystemArray} object.
             * Furthermore, the relationship between this {@link DistributedSystem} and {@link DistributedProcess} classes are
             * **M: N Associative**.
             *
             * Articles     | {@link DistributedProcess}     | {@link ExternalSystemRole}
             * -------------|--------------------------------|----------------------------
             * Belonged to  | {@link DistributedSystemArray} | {@link DistributedSystem}
             * Relationship | M: N Associative               | 1: N Composite
             * Ownership    | References                     | Exclusive possession
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var DistributedServer = (function (_super) {
                __extends(DistributedServer, _super);
                /**
                 * Construct from parent {@link DistributedSystemArray}.
                 *
                 * @param systemArray The parent {@link DistributedSystemArray} object.
                 */
                function DistributedServer(systemArray) {
                    var _this = _super.call(this, systemArray) || this;
                    _this.ip = "";
                    _this.port = 0;
                    return _this;
                }
                /**
                 * @inheritdoc
                 */
                DistributedServer.prototype.connect = function () {
                    if (this.communicator != null)
                        return;
                    this.communicator = this.createServerConnector();
                    this.communicator.connect(this.ip, this.port);
                };
                return DistributedServer;
            }(distributed.DistributedSystem));
            distributed.DistributedServer = DistributedServer;
        })(distributed = templates.distributed || (templates.distributed = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="DistributedSystemArray.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var distributed;
        (function (distributed) {
            /**
             * Master of Distributed Processing System, a client connecting to slave servers.
             *
             * The {@link DistributedServerArray} is an abstract class, derived from the {@link DistributedSystemArray} class,
             * connecting to {@link IDistributedServer distributed servers}.
             *
             * Extends this {@link DistributedServerArray} and overrides {@link createChild createChild()} method creating child
             * {@link IDistributedServer} object. After the extending and overriding, construct children {@link IDistributedServer}
             * objects and call the {@link connect connect()} method.
             *
             * #### [Inherited] {@link DistributedSystemArray}
             * The {@link DistributedSystemArray} is an abstract class containing and managing remote distributed **slave** system
             * drivers, {@link DistributedSystem} objects. Within framework of network, {@link DistributedSystemArray} represents
             * your system, a **Master** of *Distributed Processing System* that requesting *distributed process* to **slave**
             * systems and the children {@link DistributedSystem} objects represent the remote **slave** systems, who is being
             * requested the *distributed processes*.
             *
             * The {@link DistributedSystemArray} contains {@link DistributedProcess} objects directly. You can request a
             * **distributed process** through the {@link DistributedProcess} object. You can access the
             * {@link DistributedProcess} object(s) with those methods:
             *
             * - {@link hasProcess}
             * - {@link getProcess}
             * - {@link insertProcess}
             * - {@link eraseProcess}
             * - {@link getProcessMap}
             *
             * When you need the **distributed process**, call the {@link DistributedProcess.sendData} method. Then the
             * {@link DistributedProcess} will find the most idle {@link DistributedSystem} object who represents a distributed
             * **slave **system. The {@link Invoke} message will be sent to the most idle {@link DistributedSystem} object. When
             * the **distributed process** has completed, then {@link DistributedSystem.getPerformance performance index} and
             * {@link DistributedProcess.getResource resource index} of related objects will be revaluated.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Parallel Process
             * This {@link DistributedSystemArray} class is derived from the {@link ParallelSystemArray} class, so you can request
             * a **parallel process**, too.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices will
             * be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * #### Proxy Pattern
             * This class {@link DistributedSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link DistributedSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var DistributedServerArray = (function (_super) {
                __extends(DistributedServerArray, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function DistributedServerArray() {
                    return _super.call(this) || this;
                }
                /* ---------------------------------------------------------
                    CONNECTOR's METHOD
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                DistributedServerArray.prototype.connect = function () {
                    for (var i = 0; i < this.size(); i++)
                        this.at(i).connect();
                };
                return DistributedServerArray;
            }(distributed.DistributedSystemArray));
            distributed.DistributedServerArray = DistributedServerArray;
        })(distributed = templates.distributed || (templates.distributed = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="DistributedSystemArrayMediator.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var distributed;
        (function (distributed) {
            /**
             * Mediator of Distributed Processing System, a client connecting to slave servers.
             *
             * The {@link DistributedServerArrayMediator} is an abstract class, derived from {@link DistributedSystemArrayMediator}
             * class, connecting to {@link IDistributedServer distributed servers}.
             *
             * Extends this {@link DistributedServerArrayMediator} and overrides {@link createChild createChild()} method creating
             * child {@link IDistributedServer} object. After the extending and overriding, construct children
             * {@link IDistributedServer} objects and call the {@link connect connect()} method.
             *
             * #### [Inherited] {@link DistributedSystemArrayMediator}
             * The {@link DistributedSystemArrayMediator} class be a master for its slave systems, and be a slave to its master
             * system at the same time. This {@link DistributedSystemArrayMediator} be a master system, containing and managing
             * {@link DistributedSystem} objects, which represent distributed slave systems, by extending
             * {@link DistributedSystemArray} class. Also, be a slave system through {@link getMediator mediator} object, which is
             * derived from the {@link SlaveSystem} class.
             *
             * As a slave, you can specify this {@link DistributedSystemArrayMediator} to be <i>a client slave connecting to master
             * server</i> or <i>a server slave accepting master client</i> by overriding the {@link createMediator} method.
             * Overrides the {@link createMediator createMediator()} method and return one of them:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * #### [Inherited] {@link DistributedSystemArray}
             * The {@link DistributedSystemArray} is an abstract class containing and managing remote distributed **slave** system
             * drivers, {@link DistributedSystem} objects. Within framework of network, {@link DistributedSystemArray} represents
             * your system, a **Master** of *Distributed Processing System* that requesting *distributed process* to **slave**
             * systems and the children {@link DistributedSystem} objects represent the remote **slave** systems, who is being
             * requested the *distributed processes*.
             *
             * The {@link DistributedSystemArray} contains {@link DistributedProcess} objects directly. You can request a
             * **distributed process** through the {@link DistributedProcess} object. You can access the
             * {@link DistributedProcess} object(s) with those methods:
             *
             * - {@link hasProcess}
             * - {@link getProcess}
             * - {@link insertProcess}
             * - {@link eraseProcess}
             * - {@link getProcessMap}
             *
             * When you need the **distributed process**, call the {@link DistributedProcess.sendData} method. Then the
             * {@link DistributedProcess} will find the most idle {@link DistributedSystem} object who represents a distributed
             * **slave **system. The {@link Invoke} message will be sent to the most idle {@link DistributedSystem} object. When
             * the **distributed process** has completed, then {@link DistributedSystem.getPerformance performance index} and
             * {@link DistributedProcess.getResource resource index} of related objects will be revaluated.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Parallel Process
             * This {@link DistributedSystemArray} class is derived from the {@link ParallelSystemArray} class, so you can request
             * a **parallel process**, too.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices will
             * be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * #### Proxy Pattern
             * This class {@link DistributedSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link DistributedSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var DistributedServerArrayMediator = (function (_super) {
                __extends(DistributedServerArrayMediator, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function DistributedServerArrayMediator() {
                    return _super.call(this) || this;
                }
                /* ---------------------------------------------------------
                    CONNECTOR's METHOD
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                DistributedServerArrayMediator.prototype.connect = function () {
                    for (var i = 0; i < this.size(); i++)
                        this.at(i).connect();
                };
                return DistributedServerArrayMediator;
            }(distributed.DistributedSystemArrayMediator));
            distributed.DistributedServerArrayMediator = DistributedServerArrayMediator;
        })(distributed = templates.distributed || (templates.distributed = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="DistributedClientArray.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var distributed;
        (function (distributed) {
            /**
             * Master of Distributed Processing System, be a server and client at the same time.
             *
             * The {@link DistributedServerClientArray} is an abstract class, derived from the {@link DistributedSystemArray}
             * class, opening a server accepting {@link Distributed distributed clients} and being a client connecting to
             * {@link IDistributedServer distributed servers} at the same time.
             *
             * Extends this {@link DistributedServerClientArray} and overrides below methods. After the overridings, open server
             * with {@link open open()} method and connect to {@link IDistributedServer distributed servers} through the
             * {@link connect connect()} method.
             *
             * - {@link createServerBase createServerBase()}
             * - {@link createExternalClient createExternalClient()}
             * - {@link createExternalServer createExternalServer()}
             *
             * #### [Inherited] {@link DistributedSystemArray}
             * The {@link DistributedSystemArray} is an abstract class containing and managing remote distributed **slave** system
             * drivers, {@link DistributedSystem} objects. Within framework of network, {@link DistributedSystemArray} represents
             * your system, a **Master** of *Distributed Processing System* that requesting *distributed process* to **slave**
             * systems and the children {@link DistributedSystem} objects represent the remote **slave** systems, who is being
             * requested the *distributed processes*.
             *
             * The {@link DistributedSystemArray} contains {@link DistributedProcess} objects directly. You can request a
             * **distributed process** through the {@link DistributedProcess} object. You can access the
             * {@link DistributedProcess} object(s) with those methods:
             *
             * - {@link hasProcess}
             * - {@link getProcess}
             * - {@link insertProcess}
             * - {@link eraseProcess}
             * - {@link getProcessMap}
             *
             * When you need the **distributed process**, call the {@link DistributedProcess.sendData} method. Then the
             * {@link DistributedProcess} will find the most idle {@link DistributedSystem} object who represents a distributed
             * **slave **system. The {@link Invoke} message will be sent to the most idle {@link DistributedSystem} object. When
             * the **distributed process** has completed, then {@link DistributedSystem.getPerformance performance index} and
             * {@link DistributedProcess.getResource resource index} of related objects will be revaluated.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Parallel Process
             * This {@link DistributedSystemArray} class is derived from the {@link ParallelSystemArray} class, so you can request
             * a **parallel process**, too.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices will
             * be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * #### Proxy Pattern
             * This class {@link DistributedSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link DistributedSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var DistributedServerClientArray = (function (_super) {
                __extends(DistributedServerClientArray, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function DistributedServerClientArray() {
                    return _super.call(this) || this;
                }
                /**
                 * Factory method of a child Entity.
                 *
                 * This method is migrated to {@link createExternalServer}. Override the {@link createExternalServer} method.
                 *
                 * @param xml An {@link XML} object represents child element, so that can identify the type of child to create.
                 * @return A new child Entity via {@link createExternalServer createExternalServer()}.
                 */
                DistributedServerClientArray.prototype.createChild = function (xml) {
                    return this.createExternalServer(xml);
                };
                /* ---------------------------------------------------------
                    METHOD OF CLIENT
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                DistributedServerClientArray.prototype.connect = function () {
                    for (var i = 0; i < this.size(); i++) {
                        var system = this.at(i);
                        if (system.connect == undefined)
                            continue;
                        system.connect();
                    }
                };
                return DistributedServerClientArray;
            }(distributed.DistributedClientArray));
            distributed.DistributedServerClientArray = DistributedServerClientArray;
        })(distributed = templates.distributed || (templates.distributed = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="DistributedClientArrayMediator.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var distributed;
        (function (distributed) {
            /**
             * Mediator of Distributed Processing System, be a server and client at the same time as a **master**.
             *
             * The {@link DistributedServerClientArrayMediator} is an abstract class, derived from the
             * {@link DistributedSystemArrayMediator} class, opening a server accepting {@link DistributedSystem distributed
             * clients} and being a client connecting to {@link IDistributedServer distributed servers} at the same time.
             *
             * Extends this {@link DistributedServerClientArrayMediator} and overrides below methods. After the overridings, open
             * server with {@link open open()} method and connect to {@link IDistributedServer distributed servers} through the
             * {@link connect connect()} method.
             *
             * - {@link createServerBase createServerBase()}
             * - {@link createExternalClient createExternalClient()}
             * - {@link createExternalServer createExternalServer()}
             *
             * #### [Inherited] {@link DistributedSystemArrayMediator}
             * The {@link DistributedSystemArrayMediator} class be a master for its slave systems, and be a slave to its master
             * system at the same time. This {@link DistributedSystemArrayMediator} be a master system, containing and managing
             * {@link DistributedSystem} objects, which represent distributed slave systems, by extending
             * {@link DistributedSystemArray} class. Also, be a slave system through {@link getMediator mediator} object, which is
             * derived from the {@link SlaveSystem} class.
             *
             * As a slave, you can specify this {@link DistributedSystemArrayMediator} to be <i>a client slave connecting to master
             * server</i> or <i>a server slave accepting master client</i> by overriding the {@link createMediator} method.
             * Overrides the {@link createMediator createMediator()} method and return one of them:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * #### [Inherited] {@link DistributedSystemArray}
             * The {@link DistributedSystemArray} is an abstract class containing and managing remote distributed **slave** system
             * drivers, {@link DistributedSystem} objects. Within framework of network, {@link DistributedSystemArray} represents
             * your system, a **Master** of *Distributed Processing System* that requesting *distributed process* to **slave**
             * systems and the children {@link DistributedSystem} objects represent the remote **slave** systems, who is being
             * requested the *distributed processes*.
             *
             * The {@link DistributedSystemArray} contains {@link DistributedProcess} objects directly. You can request a
             * **distributed process** through the {@link DistributedProcess} object. You can access the
             * {@link DistributedProcess} object(s) with those methods:
             *
             * - {@link hasProcess}
             * - {@link getProcess}
             * - {@link insertProcess}
             * - {@link eraseProcess}
             * - {@link getProcessMap}
             *
             * When you need the **distributed process**, call the {@link DistributedProcess.sendData} method. Then the
             * {@link DistributedProcess} will find the most idle {@link DistributedSystem} object who represents a distributed
             * **slave **system. The {@link Invoke} message will be sent to the most idle {@link DistributedSystem} object. When
             * the **distributed process** has completed, then {@link DistributedSystem.getPerformance performance index} and
             * {@link DistributedProcess.getResource resource index} of related objects will be revaluated.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Parallel Process
             * This {@link DistributedSystemArray} class is derived from the {@link ParallelSystemArray} class, so you can request
             * a **parallel process**, too.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices will
             * be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * #### Proxy Pattern
             * This class {@link DistributedSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link DistributedSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var DistributedServerClientArrayMediator = (function (_super) {
                __extends(DistributedServerClientArrayMediator, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function DistributedServerClientArrayMediator() {
                    return _super.call(this) || this;
                }
                /**
                 * Factory method of a child Entity.
                 *
                 * This method is migrated to {@link createExternalServer}. Override the {@link createExternalServer} method.
                 *
                 * @param xml An {@link XML} object represents child element, so that can identify the type of child to create.
                 * @return A new child Entity via {@link createExternalServer createExternalServer()}.
                 */
                DistributedServerClientArrayMediator.prototype.createChild = function (xml) {
                    return this.createExternalServer(xml);
                };
                /* ---------------------------------------------------------
                    METHOD OF CLIENT
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                DistributedServerClientArrayMediator.prototype.connect = function () {
                    for (var i = 0; i < this.size(); i++) {
                        var system = this.at(i);
                        if (system.connect == undefined)
                            continue;
                        system.connect();
                    }
                };
                return DistributedServerClientArrayMediator;
            }(distributed.DistributedClientArrayMediator));
            distributed.DistributedServerClientArrayMediator = DistributedServerClientArrayMediator;
        })(distributed = templates.distributed || (templates.distributed = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../../protocol/Entity.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var slave;
        (function (slave) {
            /**
             * History of an {@link Invoke} message.
             *
             * The {@link InvokeHistory} is a class archiving history log of an {@link Invoke} message with elapsed time. This
             * {@link InvokeHistory} class is used to report elapsed time of handling a requested process from **slave** to
             * **master** system.
             *
             * The **master** system utilizes derived {@link InvokeHistory} objects to compute performance indices.
             * - {@link ParallelSytem.getPerformance}
             * - {@link DistributedProcess.getResource}
             *
             * @author Jeongho Nam <http://samchon.org>
             */
            var InvokeHistory = (function (_super) {
                __extends(InvokeHistory, _super);
                function InvokeHistory(invoke) {
                    if (invoke === void 0) { invoke = null; }
                    var _this = _super.call(this) || this;
                    if (invoke == null) {
                        // DEFAULT CONSTRUCTOR
                        _this.uid = 0;
                        _this.listener = "";
                    }
                    else {
                        // CONSTRUCT FROM AN INVOKE MESSAGE
                        _this.uid = invoke.get("_History_uid").getValue();
                        _this.listener = invoke.getListener();
                        _this.start_time_ = new Date();
                    }
                    return _this;
                }
                /**
                 * @inheritdoc
                 */
                InvokeHistory.prototype.construct = function (xml) {
                    _super.prototype.construct.call(this, xml);
                    this.start_time_ = new Date(parseInt(xml.getProperty("startTime")));
                    this.end_time_ = new Date(parseInt(xml.getProperty("endTime")));
                };
                /**
                 * Complete the history.
                 *
                 * Completes the history and determines the {@link getEndTime end time}.
                 */
                InvokeHistory.prototype.complete = function () {
                    this.end_time_ = new Date();
                };
                /* ---------------------------------------------------------
                    ACCESSORS
                --------------------------------------------------------- */
                InvokeHistory.prototype.key = function () {
                    return this.uid;
                };
                /**
                 * Get unique ID.
                 */
                InvokeHistory.prototype.getUID = function () {
                    return this.uid;
                };
                /**
                 * Get {@link Invoke.getListener listener} of the {@link Invoke} message.
                 */
                InvokeHistory.prototype.getListener = function () {
                    return this.listener;
                };
                /**
                 * Get start time.
                 */
                InvokeHistory.prototype.getStartTime = function () {
                    return this.start_time_;
                };
                /**
                 * Get end time.
                 */
                InvokeHistory.prototype.getEndTime = function () {
                    return this.end_time_;
                };
                /**
                 * Compute elapsed time.
                 *
                 * @return nanoseconds.
                 */
                InvokeHistory.prototype.computeElapsedTime = function () {
                    return Math.max(this.end_time_.getTime() - this.start_time_.getTime(), 1);
                };
                /* ---------------------------------------------------------
                    EXPORTERS
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                InvokeHistory.prototype.TAG = function () {
                    return "history";
                };
                /**
                 * @inheritdoc
                 */
                InvokeHistory.prototype.toXML = function () {
                    var xml = _super.prototype.toXML.call(this);
                    xml.setProperty("startTime", this.start_time_.getTime() + "");
                    xml.setProperty("endTime", this.end_time_.getTime() + "");
                    return xml;
                };
                /**
                 * Convert to an {@link Invoke} message.
                 *
                 * Creates and returns an {@link Invoke} message that is used to reporting to the **master**.
                 */
                InvokeHistory.prototype.toInvoke = function () {
                    return new samchon.protocol.Invoke("_Report_history", this.toXML());
                };
                return InvokeHistory;
            }(samchon.protocol.Entity));
            slave.InvokeHistory = InvokeHistory;
        })(slave = templates.slave || (templates.slave = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../slave/InvokeHistory.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var distributed;
        (function (distributed) {
            /**
             * History of an {@link Invoke} message.
             *
             * The {@link PRInvokeHistory} is a class archiving history log of an {@link Invoke} message which requests the
             * *distributed process*, created whenever {@link DistributedProcess.sendData} is called.
             *
             * When the *distributed process* has completed, then {@link complete complete()} is called and the *elapsed time* is
             * determined. The elapsed time is utilized for computation of {@link DistributedSystem.getPerformance performance index}
             * and {@link DistributedProcess.getResource resource index} of related objects.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_distributed_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var DSInvokeHistory = (function (_super) {
                __extends(DSInvokeHistory, _super);
                function DSInvokeHistory(system, process, invoke, weight) {
                    if (process === void 0) { process = null; }
                    if (invoke === void 0) { invoke = null; }
                    if (weight === void 0) { weight = 1; }
                    var _this = _super.call(this, invoke) || this;
                    _this.system_ = system;
                    _this.process_ = process;
                    _this.weight_ = weight;
                    return _this;
                }
                /**
                 * @inheritdoc
                 */
                DSInvokeHistory.prototype.construct = function (xml) {
                    _super.prototype.construct.call(this, xml);
                    if (xml.hasProperty("process") == false) {
                        this.process_ = null;
                        return;
                    }
                    var system_array = this.system_.getSystemArray();
                    var process_name = xml.getProperty("process");
                    if (system_array.hasProcess(process_name) == true)
                        this.process_ = system_array.getProcess(process_name);
                    else
                        this.process_ = null;
                };
                /* ---------------------------------------------------------
                    ACCESSORS
                --------------------------------------------------------- */
                /**
                 * Get the related {@link DistributedSystem} object.
                 */
                DSInvokeHistory.prototype.getSystem = function () {
                    return this.system_;
                };
                /**
                 * Get the related {@link DistributedProcess} object.
                 */
                DSInvokeHistory.prototype.getProcess = function () {
                    return this.process_;
                };
                /**
                 * Get weight.
                 *
                 * Gets weight of resource which indicates how heavy this {@link Invoke} message is. Default is 1.
                 */
                DSInvokeHistory.prototype.getWeight = function () {
                    return this.weight_;
                };
                /* ---------------------------------------------------------
                    EXPORTERS
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                DSInvokeHistory.prototype.toXML = function () {
                    var xml = _super.prototype.toXML.call(this);
                    if (this.process_ != null)
                        xml.setProperty("process", this.process_.getName());
                    return xml;
                };
                return DSInvokeHistory;
            }(templates.slave.InvokeHistory));
            distributed.DSInvokeHistory = DSInvokeHistory;
        })(distributed = templates.distributed || (templates.distributed = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="ExternalSystemArray.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var external;
        (function (external) {
            /**
             * An array and manager of {@link ExternalSystem external clients} as a server.
             *
             * The {@link ExternalClientArray} is an abstract class, derived from the {@link ExternalSystemArray} class, opening
             * a server accepting {@link ExternalSystem external clients}.
             *
             * Extends this {@link ExternalClientArray}, overrides {@link createServerBase createServerBase()} to determine which
             * protocol to follow and {@link createExternalClient createExternalClient()} creating child {@link ExternalSystem}
             * object. After the extending and overridings, open this server using the {@link open open()} method.
             *
             * #### [Inherited] {@link ExternalSystemArray}
             * The {@link ExternalSystemArray} is an abstract class containing and managing external system drivers,
             * {@link ExternalSystem} objects. Within framewokr of network, {@link ExternalSystemArray} represents your system
             * and children {@link ExternalSystem} objects represent remote, external systems connected with your system.
             * With this {@link ExternalSystemArray}, you can manage multiple external systems as a group.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/TypeScript-Templates-External_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ExternalClientArray = (function (_super) {
                __extends(ExternalClientArray, _super);
                /* =========================================================
                    CONSTRUCTORS
                        - MEMBER
                        - FACTORY METHOD FOR CHILDREN
                ============================================================
                    MEMBER
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function ExternalClientArray() {
                    var _this = _super.call(this) || this;
                    _this.server_base_ = null;
                    return _this;
                }
                /* ---------------------------------------------------------
                    FACTORY METHOD FOR CHILDREN
                --------------------------------------------------------- */
                /**
                 * Add a newly connected remote client.
                 *
                 * When a {@link IClientDriver remote client} connects to this *server* {@link ExternalClientArray} object,
                 * then this {@link ExternalClientArray} creates a child {@link ExternalSystem external client} object through
                 * the {@link createExternalClient createExternalClient()} method and {@link insert inserts} it.
                 *
                 * @param driver A communicator for external client.
                 */
                ExternalClientArray.prototype.addClient = function (driver) {
                    var system = this.createExternalClient(driver);
                    if (system == null)
                        return;
                    this.push_back(system);
                };
                /**
                 * (Deprecated) Factory method creating child object.
                 *
                 * The method {@link createChild createChild()} is deprecated. Don't use and override this.
                 *
                 * Note that, the {@link ExternalClientArray} is a server accepting {@link ExternalSystem external clients}.
                 * There's no way to creating the {@link ExternalSystem external clients} in advance before opening the server.
                 *
                 * @param xml An {@link XML} object represents the child {@link ExternalSystem} object.
                 * @return null
                 */
                ExternalClientArray.prototype.createChild = function (xml) { return null; };
                /* ---------------------------------------------------------
                    METHOD OF SERVER
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                ExternalClientArray.prototype.open = function (port) {
                    this.server_base_ = this.createServerBase();
                    if (this.server_base_ == null)
                        return;
                    this.server_base_.open(port);
                };
                /**
                 * @inheritdoc
                 */
                ExternalClientArray.prototype.close = function () {
                    if (this.server_base_ == null)
                        return;
                    this.server_base_.close();
                };
                return ExternalClientArray;
            }(external.ExternalSystemArray));
            external.ExternalClientArray = ExternalClientArray;
        })(external = templates.external || (templates.external = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="ExternalSystem.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var external;
        (function (external) {
            /**
             * An external server driver.
             *
             * The {@link ExternalServer} is an abstract class, derived from the {@link ExternalSystem} class, connecting to
             * remote, external server. Extends this {@link ExternalServer} class and overrides the
             * {@link createServerConnector createServerConnector()} method following which protocol the external server uses.
             *
             * #### [Inherited] {@link ExternalSystem}
             * The {@link ExternalSystem} class represents an external system, connected and interact with this system.
             * {@link ExternalSystem} takes full charge of network communication with the remote, external system have connected.
             * Replied {@link Invoke} messages from the external system is shifted to and processed in, children elements of this
             * class, {@link ExternalSystemRole} objects.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Bridge & Proxy Pattern
             * The {@link ExternalSystem} class can be a *bridge* for *logical proxy*. In framework within user,
             * which {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Bridge Pattern* and *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/TypeScript-Templates-External_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ExternalServer = (function (_super) {
                __extends(ExternalServer, _super);
                /**
                 * Construct from parent {@link ExternalSystemArray}.
                 *
                 * @param systemArray The parent {@link ExternalSystemArray} object.
                 */
                function ExternalServer(systemArray) {
                    var _this = _super.call(this, systemArray) || this;
                    _this.ip = "";
                    _this.port = 0;
                    return _this;
                }
                /**
                 * @inheritdoc
                 */
                ExternalServer.prototype.connect = function () {
                    this.communicator = this.createServerConnector();
                    this.communicator.connect(this.ip, this.port);
                };
                return ExternalServer;
            }(external.ExternalSystem));
            external.ExternalServer = ExternalServer;
        })(external = templates.external || (templates.external = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="ExternalSystemArray.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var external;
        (function (external) {
            /**
             * An array and manager of {@link IExternalServer external servers}.
             *
             * The {@link ExternalServerArray} is an abstract class, derived from the {@link ExternalSystemArray} class,
             * connecting to {@link IExternalServer external servers}.
             *
             * Extends this {@link ExternalServerArray} and overrides {@link createChild createChild()} method creating child
             * {@link IExternalServer} object. After the extending and overriding, construct children {@link IExternalServer}
             * objects and call the {@link connect connect()} method.
             *
             * #### [Inherited] {@link ExternalSystemArray}
             * The {@link ExternalSystemArray} is an abstract class containing and managing external system drivers,
             * {@link ExternalSystem} objects. Within framewokr of network, {@link ExternalSystemArray} represents your system
             * and children {@link ExternalSystem} objects represent remote, external systems connected with your system.
             * With this {@link ExternalSystemArray}, you can manage multiple external systems as a group.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/TypeScript-Templates-External_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ExternalServerArray = (function (_super) {
                __extends(ExternalServerArray, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function ExternalServerArray() {
                    return _super.call(this) || this;
                }
                /* ---------------------------------------------------------
                    CONNECTOR's METHOD
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                ExternalServerArray.prototype.connect = function () {
                    for (var i = 0; i < this.size(); i++)
                        this.at(i).connect();
                };
                return ExternalServerArray;
            }(external.ExternalSystemArray));
            external.ExternalServerArray = ExternalServerArray;
        })(external = templates.external || (templates.external = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="ExternalSystemArray.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var external;
        (function (external) {
            /**
             * An array and manager of {@link IExternalServer external servers} and {@link ExternalSystem external clients}.
             *
             * The {@link ExternalServerClientArray} is an abstract class, derived from the {@link ExternalSystemArray} class,
             * opening a server accepting {@link ExternalSystem external clients} and being a client connecting to
             * {@link IExternalServer external servers} at the same time.
             *
             * Extends this {@link ExternalServerClientArray} and overrides below methods. After the overridings, open server
             * with {@link open open()} method and connect to {@link IExternalServer external servers} through the
             * {@link connect connect()} method.
             *
             * - {@link createServerBase createServerBase()}
             * - {@link createExternalClient createExternalClient()}
             * - {@link createExternalServer createExternalServer()}
             *
             * #### [Inherited] {@link ExternalSystemArray}
             * The {@link ExternalSystemArray} is an abstract class containing and managing external system drivers,
             * {@link ExternalSystem} objects. Within framewokr of network, {@link ExternalSystemArray} represents your system
             * and children {@link ExternalSystem} objects represent remote, external systems connected with your system.
             * With this {@link ExternalSystemArray}, you can manage multiple external systems as a group.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/TypeScript-Templates-External_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ExternalServerClientArray = (function (_super) {
                __extends(ExternalServerClientArray, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function ExternalServerClientArray() {
                    return _super.call(this) || this;
                }
                /**
                 * Factory method of a child Entity.
                 *
                 * This method is migrated to {@link createExternalServer}. Override the {@link createExternalServer} method.
                 *
                 * @param xml An {@link XML} object represents child element, so that can identify the type of child to create.
                 * @return A new child Entity via {@link createExternalServer createExternalServer()}.
                 */
                ExternalServerClientArray.prototype.createChild = function (xml) {
                    return this.createExternalServer(xml);
                };
                /* ---------------------------------------------------------
                    METHOD OF CLIENT
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                ExternalServerClientArray.prototype.connect = function () {
                    for (var i = 0; i < this.size(); i++) {
                        var system = this.at(i);
                        if (system.connect == undefined)
                            continue;
                        system.connect();
                    }
                };
                return ExternalServerClientArray;
            }(external.ExternalClientArray));
            external.ExternalServerClientArray = ExternalServerClientArray;
        })(external = templates.external || (templates.external = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../../protocol/Entity.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var external;
        (function (external) {
            /**
             * A role of an external system.
             *
             * The {@link ExternalSystemRole} class represents a role, *WHAT TO DO*. Extends the {@link ExternalSystemRole} class
             * and overrides {@link replyData replyData()} to define the *WHAT TO DO*. And assign this {@link ExternalSystemRole}
             * object to related {@link ExternalSystem} object.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * The {@link ExternalSystemRole} class can be an *logical proxy*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem} object, via {@link ExternalSystemArray.getRole ExternalSystemArray.getRole()}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/TypeScript-Templates-External_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ExternalSystemRole = (function (_super) {
                __extends(ExternalSystemRole, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Constructor from a system.
                 *
                 * @param system An external system containing this role.
                 */
                function ExternalSystemRole(system) {
                    var _this = _super.call(this) || this;
                    _this.system = system;
                    return _this;
                }
                /* ---------------------------------------------------------
                    ACCESSORS
                --------------------------------------------------------- */
                /**
                 * Identifier of {@link ExternalSystemRole} is its {@link name}.
                 */
                ExternalSystemRole.prototype.key = function () {
                    return this.name;
                };
                /**
                 * Get grandparent {@link ExternalSystemArray}.
                 *
                 * Get the grandparent {@link ExternalSystemArray} object through this parent {@link ExternalSystem},
                 * {@link ExternalSystem.getSystemArray ExternalSystem.getSystemArray()}.
                 *
                 * @return The grandparent {@link ExternalSystemArray} object.
                 */
                ExternalSystemRole.prototype.getSystemArray = function () {
                    return this.system.getSystemArray();
                };
                /**
                 * Get parent {@link ExternalSystemRole} object.
                 */
                ExternalSystemRole.prototype.getSystem = function () {
                    return this.system;
                };
                /**
                 * Get name, who represents and identifies this role.
                 */
                ExternalSystemRole.prototype.getName = function () {
                    return this.name;
                };
                /* ---------------------------------------------------------
                    MESSAGE CHAIN
                --------------------------------------------------------- */
                /**
                 * Send an {@link Invoke} message.
                 *
                 * Sends an {@link Invoke} message to remote system through the parent {@link ExternalSystem} object.
                 *
                 * @param invoke An {@link Invoke} message to send to the external system.
                 */
                ExternalSystemRole.prototype.sendData = function (invoke) {
                    this.system.sendData(invoke);
                };
                /* ---------------------------------------------------------
                    EXPORTERS
                --------------------------------------------------------- */
                /**
                 * Tag name of the {@link ExternalSytemRole} in {@link XML}.
                 *
                 * @return *role*.
                 */
                ExternalSystemRole.prototype.TAG = function () {
                    return "role";
                };
                return ExternalSystemRole;
            }(samchon.protocol.Entity));
            external.ExternalSystemRole = ExternalSystemRole;
        })(external = templates.external || (templates.external = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../external/ExternalSystem.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var slave;
        (function (slave) {
            var SlaveSystem = (function () {
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function SlaveSystem() {
                    this.communicator_ = null;
                }
                /* ---------------------------------------------------------
                    INVOKE MSSAGE CHIAN
                --------------------------------------------------------- */
                SlaveSystem.prototype.sendData = function (invoke) {
                    this.communicator_.sendData(invoke);
                };
                /**
                 * @hidden
                 */
                SlaveSystem.prototype._Reply_data = function (invoke) {
                    // INTERCEPT INVOKE MESSAGE
                    if (invoke.has("_History_uid")) {
                        // INIT HISTORY - WITH START TIME
                        var history_4 = new slave.InvokeHistory(invoke);
                        std.remove_if(invoke.begin(), invoke.end(), function (parameter) {
                            return parameter.getName() == "_History_uid"
                                || parameter.getName() == "_Process_name"
                                || parameter.getName() == "_Process_weight";
                        }); // DETACH THE UID FOR FUNCTION AUTO-MATCHING
                        // MAIN PROCESS - REPLY_DATA
                        var pInvoke = new slave.PInvoke(invoke, history_4, this);
                        this.replyData(pInvoke);
                        // NOTIFY - WITH END TIME
                        if (pInvoke.isHold() == false)
                            pInvoke.complete();
                    }
                    else
                        this.replyData(invoke);
                };
                SlaveSystem.prototype.replyData = function (invoke) {
                    invoke.apply(this);
                };
                return SlaveSystem;
            }());
            slave.SlaveSystem = SlaveSystem;
        })(slave = templates.slave || (templates.slave = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../slave/SlaveSystem.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * A mediator, the master driver.
             *
             * The {@link MediatorSystem} is an abstract class helping {@link ParallelSystemArrayMediator} can be a **slave**
             * system. The {@link MediatorSystem} interacts and communicates with the **master** system as a role of **slave**.
             *
             * This {@link MediatorSystem} object is created in {@link ParallelSystemArrayMediator.createMediator}. Override the
             * method and return one of them, which are derived from this {@link MediatorSystem} class, considering which
             * type and protocol the **master** system follows:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * When the **master** orders a *parallel process* to this **slave**, then the {@link MediatorSystem} delivers the
             * *parallel process* to its parent {@link ParallelSystemArrayMediator} object. The
             * {@link ParallelSystemArrayMediator} object distributes the *parallel process* to its slaves system,
             * {@link ParallelSystem} objects. When the *parallel process* has completed, then {@link MediatorSystem} reports the
             * result to its **master**.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System),
             *			 [Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var MediatorSystem = (function (_super) {
                __extends(MediatorSystem, _super);
                function MediatorSystem(systemArray) {
                    var _this = _super.call(this) || this;
                    _this.system_array_ = systemArray;
                    _this.progress_list_ = new std.HashMap();
                    return _this;
                }
                MediatorSystem.prototype.getSystemArray = function () {
                    return this.system_array_;
                };
                /* ---------------------------------------------------------
                    MESSAGE CHAIN
                --------------------------------------------------------- */
                /**
                 * @hidden
                 */
                MediatorSystem.prototype._Complete_history = function (uid) {
                    // NO SUCH HISTORY; THE PROCESS HAD DONE ONLY IN THIS MEDIATOR LEVEL.
                    if (this.progress_list_.has(uid) == false)
                        return;
                    // COMPLETE THE HISTORY
                    var history = this.progress_list_.get(uid);
                    var start_time = null;
                    var end_time = null;
                    // DETERMINE WHEN STARTED AND COMPLETED TIME
                    for (var i = 0; i < this.system_array_.size(); i++) {
                        var system = this.system_array_.at(i);
                        var it = system.history_list_.find(uid);
                        if (it.equals(system.history_list_.end()) == true)
                            continue;
                        var my_history = it.second;
                        if (start_time == null || my_history.getStartTime() < start_time)
                            start_time = my_history.getStartTime();
                        if (end_time == null || my_history.getEndTime() > end_time)
                            end_time = my_history.getEndTime();
                    }
                    history.start_time_ = start_time;
                    history.end_time_ = end_time;
                    // ERASE THE HISTORY ON PROGRESS LIST
                    this.progress_list_.erase(uid);
                    // REPORT THE HISTORY TO MASTER
                    this.sendData(history.toInvoke());
                };
                /**
                 * @hidden
                 */
                MediatorSystem.prototype._Reply_data = function (invoke) {
                    if (invoke.has("_History_uid") == true) {
                        // INIT HISTORY OBJECT
                        var history_5 = new templates.slave.InvokeHistory(invoke);
                        if (this.system_array_.empty() == true) {
                            // NO BELONGED SLAVE, THEN SEND BACK
                            this.sendData(new samchon.protocol.Invoke("_Send_back_history", history_5.getUID()));
                            return;
                        }
                        // REGISTER THIS PROCESS ON HISTORY LIST
                        this.progress_list_.insert([history_5.getUID(), history_5]);
                        if (invoke.has("_Piece_first") == true) {
                            // PARALLEL PROCESS
                            var first = invoke.get("_Piece_first").getValue();
                            var last = invoke.get("_Piece_last").getValue();
                            invoke.erase(invoke.end().advance(-2), invoke.end());
                            this.system_array_.sendPieceData(invoke, first, last);
                        }
                        else if (this.system_array_ instanceof templates.distributed.DistributedSystemArrayMediator
                            && invoke.has("_Process_name") == true) {
                            // FIND THE MATCHED ROLE
                            var process_name = invoke.get("_Process_name").getValue();
                            if (this.system_array_.hasProcess(process_name) == false)
                                return;
                            // SEND DATA VIA THE ROLE
                            var process_4 = this.system_array_.getProcess(process_name);
                            process_4.sendData(invoke);
                        }
                    }
                    else
                        this.replyData(invoke);
                };
                /**
                 * @inheritdoc
                 */
                MediatorSystem.prototype.replyData = function (invoke) {
                    this.system_array_.sendData(invoke);
                };
                return MediatorSystem;
            }(templates.slave.SlaveSystem));
            parallel.MediatorSystem = MediatorSystem;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * A mediator server, driver for the master client.
             *
             * The {@link MediatorServer} is a class opening a server accepting the **master** client, following the protocol of
             * Samchon Framework's own.
             *
             * #### [Inherited] {@link MediatorSystem}
             * The {@link MediatorSystem} is an abstract class helping {@link ParallelSystemArrayMediator} can be a **slave**
             * system. The {@link MediatorSystem} interacts and communicates with the **master** system as a role of **slave**.
             *
             * This {@link MediatorSystem} object is created in {@link ParallelSystemArrayMediator.createMediator}. Override the
             * method and return one of them, which are derived from this {@link MediatorSystem} class, considering which
             * type and protocol the **master** system follows:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * When the **master** orders a *parallel process* to this **slave**, then the {@link MediatorSystem} delivers the
             * *parallel process* to its parent {@link ParallelSystemArrayMediator} object. The
             * {@link ParallelSystemArrayMediator} object distributes the *parallel process* to its slaves system,
             * {@link ParallelSystem} objects. When the *parallel process* has completed, then {@link MediatorSystem} reports the
             * result to its **master**.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System),
             *			 [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var MediatorServer = (function (_super) {
                __extends(MediatorServer, _super);
                function MediatorServer(systemArray, port) {
                    var _this = _super.call(this, systemArray) || this;
                    _this.port = port;
                    return _this;
                }
                /**
                 * Factory method creating {@link IServerBase} object.
                 *
                 * This method {@link createServerBase createServerBase()} determines which protocol is used in this server,
                 * {@link MediatorServer}. Note that, **slave** (this {@link MediatorServer} object) must follow the **master**'s
                 * protocol.
                 *
                 * Overrides and return one of them considering the which protocol to follow:
                 *
                 * - {@link ServerBase}
                 * - {@link WebServerBase}
                 * - {@link SharedWorkerServerBase}
                 */
                MediatorServer.prototype.createServerBase = function () {
                    return new samchon.protocol.ServerBase(this);
                };
                /**
                 * Add a newly connected remote client.
                 *
                 * {@link MediatorServer} represents a **slave** dedicating to its **master**. In that reason, the
                 * {@link MediatorServer} does not accept multiple **master** clients. It accepts only one. Thus, *listener* of
                 * the *communicator* is {@link MediatorSystem} object, itself.
                 *
                 * @param driver A communicator with remote client.
                 */
                MediatorServer.prototype.addClient = function (driver) {
                    this.communicator_ = driver;
                    driver.listen(this);
                };
                /* ---------------------------------------------------------
                    SERVER's METHOD
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                MediatorServer.prototype.start = function () {
                    this.open(this.port);
                };
                /**
                 * @inheritdoc
                 */
                MediatorServer.prototype.open = function (port) {
                    this.server_base_ = this.createServerBase();
                    if (this.server_base_ == null)
                        return;
                    this.server_base_.open(port);
                };
                /**
                 * @inheritdoc
                 */
                MediatorServer.prototype.close = function () {
                    if (this.server_base_ != null)
                        this.server_base_.close();
                };
                return MediatorServer;
            }(parallel.MediatorSystem));
            parallel.MediatorServer = MediatorServer;
            /**
             * A mediator server, driver for the master client.
             *
             * The {@link MediatorWebServer} is a class opening a server accepting the **master** client, following the
             * web-socket protocol.
             *
             * #### [Inherited] {@link MediatorSystem}
             * The {@link MediatorSystem} is an abstract class helping {@link ParallelSystemArrayMediator} can be a **slave**
             * system. The {@link MediatorSystem} interacts and communicates with the **master** system as a role of **slave**.
             *
             * This {@link MediatorSystem} object is created in {@link ParallelSystemArrayMediator.createMediator}. Override the
             * method and return one of them, which are derived from this {@link MediatorSystem} class, considering which
             * type and protocol the **master** system follows:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * When the **master** orders a *parallel process* to this **slave**, then the {@link MediatorSystem} delivers the
             * *parallel process* to its parent {@link ParallelSystemArrayMediator} object. The
             * {@link ParallelSystemArrayMediator} object distributes the *parallel process* to its slaves system,
             * {@link ParallelSystem} objects. When the *parallel process* has completed, then {@link MediatorSystem} reports the
             * result to its **master**.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System),
             *			 [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var MediatorWebServer = (function (_super) {
                __extends(MediatorWebServer, _super);
                function MediatorWebServer() {
                    return _super.apply(this, arguments) || this;
                }
                /**
                 * @inheritdoc
                 */
                MediatorWebServer.prototype.createServerBase = function () {
                    return new samchon.protocol.WebServerBase(this);
                };
                return MediatorWebServer;
            }(MediatorServer));
            parallel.MediatorWebServer = MediatorWebServer;
            /**
             * A mediator server, driver for the master client.
             *
             * The {@link MediatorDedicatedWorkerServer} is a class opening a server accepting the **master** client, following
             * the DedicatedWorker's protocol.
             *
             * #### [Inherited] {@link MediatorSystem}
             * The {@link MediatorSystem} is an abstract class helping {@link ParallelSystemArrayMediator} can be a **slave**
             * system. The {@link MediatorSystem} interacts and communicates with the **master** system as a role of **slave**.
             *
             * This {@link MediatorSystem} object is created in {@link ParallelSystemArrayMediator.createMediator}. Override the
             * method and return one of them, which are derived from this {@link MediatorSystem} class, considering which
             * type and protocol the **master** system follows:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * When the **master** orders a *parallel process* to this **slave**, then the {@link MediatorSystem} delivers the
             * *parallel process* to its parent {@link ParallelSystemArrayMediator} object. The
             * {@link ParallelSystemArrayMediator} object distributes the *parallel process* to its slaves system,
             * {@link ParallelSystem} objects. When the *parallel process* has completed, then {@link MediatorSystem} reports the
             * result to its **master**.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System),
             *			 [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var MediatorDedicatedWorkerServer = (function (_super) {
                __extends(MediatorDedicatedWorkerServer, _super);
                function MediatorDedicatedWorkerServer() {
                    return _super.apply(this, arguments) || this;
                }
                /**
                 * @inheritdoc
                 */
                MediatorDedicatedWorkerServer.prototype.createServerBase = function () {
                    return new samchon.protocol.DedicatedWorkerServerBase(this);
                };
                return MediatorDedicatedWorkerServer;
            }(MediatorServer));
            parallel.MediatorDedicatedWorkerServer = MediatorDedicatedWorkerServer;
            /**
             * A mediator server, driver for the master client.
             *
             * The {@link MediatorSharedWorkerServer} is a class opening a server accepting the **master** client, following the
             * SharedWorker's protocol.
             *
             * #### [Inherited] {@link MediatorSystem}
             * The {@link MediatorSystem} is an abstract class helping {@link ParallelSystemArrayMediator} can be a **slave**
             * system. The {@link MediatorSystem} interacts and communicates with the **master** system as a role of **slave**.
             *
             * This {@link MediatorSystem} object is created in {@link ParallelSystemArrayMediator.createMediator}. Override the
             * method and return one of them, which are derived from this {@link MediatorSystem} class, considering which
             * type and protocol the **master** system follows:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * When the **master** orders a *parallel process* to this **slave**, then the {@link MediatorSystem} delivers the
             * *parallel process* to its parent {@link ParallelSystemArrayMediator} object. The
             * {@link ParallelSystemArrayMediator} object distributes the *parallel process* to its slaves system,
             * {@link ParallelSystem} objects. When the *parallel process* has completed, then {@link MediatorSystem} reports the
             * result to its **master**.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System),
             *			 [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var MediatorSharedWorkerServer = (function (_super) {
                __extends(MediatorSharedWorkerServer, _super);
                function MediatorSharedWorkerServer() {
                    return _super.apply(this, arguments) || this;
                }
                /**
                 * @inheritdoc
                 */
                MediatorSharedWorkerServer.prototype.createServerBase = function () {
                    return new samchon.protocol.SharedWorkerServerBase(this);
                };
                return MediatorSharedWorkerServer;
            }(MediatorServer));
            parallel.MediatorSharedWorkerServer = MediatorSharedWorkerServer;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * A mediator client, driver for the master server.
             *
             * The {@link MediatorServer} is a class being a client connecting to the **master** server, following the protocol
             * of Samchon Framework's own.
             *
             * #### [Inherited] {@link MediatorSystem}
             * The {@link MediatorSystem} is an abstract class helping {@link ParallelSystemArrayMediator} can be a **slave**
             * system. The {@link MediatorSystem} interacts and communicates with the **master** system as a role of **slave**.
             *
             * This {@link MediatorSystem} object is created in {@link ParallelSystemArrayMediator.createMediator}. Override the
             * method and return one of them, which are derived from this {@link MediatorSystem} class, considering which
             * type and protocol the **master** system follows:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * When the **master** orders a *parallel process* to this **slave**, then the {@link MediatorSystem} delivers the
             * *parallel process* to its parent {@link ParallelSystemArrayMediator} object. The
             * {@link ParallelSystemArrayMediator} object distributes the *parallel process* to its slaves system,
             * {@link ParallelSystem} objects. When the *parallel process* has completed, then {@link MediatorSystem} reports the
             * result to its **master**.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System),
             *			 [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var MediatorClient = (function (_super) {
                __extends(MediatorClient, _super);
                function MediatorClient(systemArray, ip, port) {
                    var _this = _super.call(this, systemArray) || this;
                    _this.ip = ip;
                    _this.port = port;
                    return _this;
                }
                /**
                 * Factory method creating {@link IServerConnector} object.
                 *
                 * The {@link createServerConnector createServerConnector()} is an abstract method creating
                 * {@link IServerConnector} object. Overrides and returns one of them, considering which protocol the **master**
                 * server follows:
                 *
                 * - {@link ServerConnector}
                 * - {@link WebServerConnector}
                 * - {@link SharedWorkerServerConnector}
                 *
                 * @return A newly created {@link IServerConnector} object.
                 */
                MediatorClient.prototype.createServerConnector = function () {
                    return new samchon.protocol.ServerConnector(this);
                };
                /* ---------------------------------------------------------
                    METHOD OF CONNECTOR
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                MediatorClient.prototype.start = function () {
                    this.connect();
                };
                /**
                 * @inheritdoc
                 */
                MediatorClient.prototype.connect = function () {
                    if (this.communicator_ != null)
                        return;
                    this.communicator_ = this.createServerConnector();
                    this.communicator_.connect(this.ip, this.port);
                };
                return MediatorClient;
            }(parallel.MediatorSystem));
            parallel.MediatorClient = MediatorClient;
            /**
             * A mediator client, driver for the master server.
             *
             * The {@link MediatorWebClient} is a class being a client connecting to the **master** server, following the
             * web-socket protocol.
             *
             * #### [Inherited] {@link MediatorSystem}
             * The {@link MediatorSystem} is an abstract class helping {@link ParallelSystemArrayMediator} can be a **slave**
             * system. The {@link MediatorSystem} interacts and communicates with the **master** system as a role of **slave**.
             *
             * This {@link MediatorSystem} object is created in {@link ParallelSystemArrayMediator.createMediator}. Override the
             * method and return one of them, which are derived from this {@link MediatorSystem} class, considering which
             * type and protocol the **master** system follows:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * When the **master** orders a *parallel process* to this **slave**, then the {@link MediatorSystem} delivers the
             * *parallel process* to its parent {@link ParallelSystemArrayMediator} object. The
             * {@link ParallelSystemArrayMediator} object distributes the *parallel process* to its slaves system,
             * {@link ParallelSystem} objects. When the *parallel process* has completed, then {@link MediatorSystem} reports the
             * result to its **master**.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System),
             *			 [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var MediatorWebClient = (function (_super) {
                __extends(MediatorWebClient, _super);
                function MediatorWebClient() {
                    return _super.apply(this, arguments) || this;
                }
                /**
                 * @inheritdoc
                 */
                MediatorWebClient.prototype.createServerConnector = function () {
                    return new samchon.protocol.WebServerConnector(this);
                };
                return MediatorWebClient;
            }(MediatorClient));
            parallel.MediatorWebClient = MediatorWebClient;
            /**
             * A mediator client, driver for the master server.
             *
             * The {@link MediatorSharedWorkerClient} is a class being a client connecting to the **master** server, following
             * the SharedWorker's protocol.
             *
             * #### [Inherited] {@link MediatorSystem}
             * The {@link MediatorSystem} is an abstract class helping {@link ParallelSystemArrayMediator} can be a **slave**
             * system. The {@link MediatorSystem} interacts and communicates with the **master** system as a role of **slave**.
             *
             * This {@link MediatorSystem} object is created in {@link ParallelSystemArrayMediator.createMediator}. Override the
             * method and return one of them, which are derived from this {@link MediatorSystem} class, considering which
             * type and protocol the **master** system follows:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * When the **master** orders a *parallel process* to this **slave**, then the {@link MediatorSystem} delivers the
             * *parallel process* to its parent {@link ParallelSystemArrayMediator} object. The
             * {@link ParallelSystemArrayMediator} object distributes the *parallel process* to its slaves system,
             * {@link ParallelSystem} objects. When the *parallel process* has completed, then {@link MediatorSystem} reports the
             * result to its **master**.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System),
             *			 [Templates - Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var MediatorSharedWorkerClient = (function (_super) {
                __extends(MediatorSharedWorkerClient, _super);
                function MediatorSharedWorkerClient() {
                    return _super.apply(this, arguments) || this;
                }
                /**
                 * @inheritdoc
                 */
                MediatorSharedWorkerClient.prototype.createServerConnector = function () {
                    return new samchon.protocol.SharedWorkerServerConnector(this);
                };
                return MediatorSharedWorkerClient;
            }(MediatorClient));
            parallel.MediatorSharedWorkerClient = MediatorSharedWorkerClient;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="ParallelSystemArray.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * Master of Parallel Processing System, a server accepting slave clients.
             *
             * The {@link ParallelClientArray} is an abstract class, derived from the {@link ParallelSystemArray} class, opening
             * a server accepting {@link ParallelSystem parallel clients}.
             *
             * Extends this {@link ParallelClientArray}, overrides {@link createServerBase createServerBase()} to determine which
             * protocol to follow and {@link createExternalClient createExternalClient()} creating child {@link ParallelSystem}
             * object. After the extending and overridings, open this server using the {@link open open()} method.
             *
             * #### [Inherited] {@link ParallelSystemArray}
             * The {@link ParallelSystemArray} is an abstract class containing and managing remote parallel **slave** system
             * drivers, {@link ParallelSystem} objects. Within framework of network, {@link ParallelSystemArray} represents your
             * system, a **Master** of *Parallel Processing System* that requesting *parallel process* to slave systems and the
             * children {@link ParallelSystem} objects represent the remote slave systems, who is being requested the
             * *parallel processes*.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices
             * will be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * This class {@link ParallelSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link ParallelSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ParallelClientArray = (function (_super) {
                __extends(ParallelClientArray, _super);
                /* =========================================================
                    CONSTRUCTORS
                        - MEMBER
                        - FACTORY METHOD FOR CHILDREN
                ============================================================
                    MEMBER
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function ParallelClientArray() {
                    return _super.call(this) || this;
                }
                /* ---------------------------------------------------------
                    FACTORY METHOD FOR CHILDREN
                --------------------------------------------------------- */
                /**
                 * Add a newly connected remote client.
                 *
                 * When a {@link IClientDriver remote client} connects to this *master server of parallel processing system*,
                 * then this {@link ParallelClientArray} creates a child {@link ParallelSystem parallel client} object through
                 * the {@link createExternalClient createExternalClient()} method and {@link insert inserts} it.
                 *
                 * @param driver A communicator for external client.
                 */
                ParallelClientArray.prototype.addClient = function (driver) {
                    var system = this.createExternalClient(driver);
                    if (system == null)
                        return;
                    this.push_back(system);
                };
                /**
                 * (Deprecated) Factory method creating child object.
                 *
                 * The method {@link createChild createChild()} is deprecated. Don't use and override this.
                 *
                 * Note that, the {@link ParallelClientArray} is a server accepting {@link ParallelSystem parallel clients}.
                 * There's no way to creating the {@link ParallelSystem parallel clients} in advance before opening the server.
                 *
                 * @param xml An {@link XML} object represents the child {@link ParallelSystem} object.
                 * @return ```null```
                 */
                ParallelClientArray.prototype.createChild = function (xml) { return null; };
                /* ---------------------------------------------------------
                    SERVER's METHOD
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                ParallelClientArray.prototype.open = function (port) {
                    this.server_base_ = this.createServerBase();
                    if (this.server_base_ == null)
                        return;
                    this.server_base_.open(port);
                };
                /**
                 * @inheritdoc
                 */
                ParallelClientArray.prototype.close = function () {
                    if (this.server_base_ == null)
                        return;
                    this.server_base_.close();
                };
                return ParallelClientArray;
            }(parallel.ParallelSystemArray));
            parallel.ParallelClientArray = ParallelClientArray;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="ParallelSystemArray".ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * Mediator of Parallel Processing System.
             *
             * The {@link ParallelSystemArrayMediator} class be a **master** for its slave systems, and be a **slave** to its
             * master system at the same time. This {@link ParallelSystemArrayMediator} be a **master **system, containing and
             * managing {@link ParallelSystem} objects, which represent parallel slave systems, by extending
             * {@link ParallelSystemArray} class. Also, be a **slave** system through {@link getMediator mediator} object, which is
             * derived from the {@link SlaveSystem} class.
             *
             * As a **master**, you can specify this {@link ParallelSystemArrayMediator} class to be <i>a master server accepting
             * slave clients<i> or <i>a master client to connecting slave servers</i>. Even both of them is possible. Extends one
             * of them below and overrides abstract factory method(s) creating the child {@link ParallelSystem} object.
             *
             * - {@link ParallelClientArrayMediator}: A server accepting {@link ParallelSystem parallel clients}.
             * - {@link ParallelServerArrayMediator}: A client connecting to {@link ParallelServer parallel servers}.
             * - {@link ParallelServerClientArrayMediator}: Both of them. Accepts {@link ParallelSystem parallel clients} and
             *   connects to {@link ParallelServer parallel servers} at the same time.
             *
             * As a **slave**, you can specify this {@link ParallelSystemArrayMediator} to be <i>a client slave connecting to
             * master server</i> or <i>a server slave accepting master client</i> by overriding the {@link createMediator} method.
             * Overrides the {@link createMediator createMediator()} method and return one of them:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * #### [Inherited] {@link ParallelSystemArray}
             * The {@link ParallelSystemArray} is an abstract class containing and managing remote parallel **slave** system
             * drivers, {@link ParallelSystem} objects. Within framework of network, {@link ParallelSystemArray} represents your
             * system, a **Master** of *Parallel Processing System* that requesting *parallel process* to slave systems and the
             * children {@link ParallelSystem} objects represent the remote slave systems, who is being requested the
             * *parallel processes*.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices
             * will be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * This class {@link ParallelSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link ParallelSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ParallelSystemArrayMediator = (function (_super) {
                __extends(ParallelSystemArrayMediator, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function ParallelSystemArrayMediator() {
                    var _this = _super.call(this) || this;
                    _this.mediator_ = null;
                    return _this;
                }
                /**
                 * Start mediator.
                 *
                 * If the {@link getMediator mediator} is a type of server, then opens the server accepting master client.
                 * Otherwise, the {@link getMediator mediator} is a type of client, then connects the master server.
                 */
                ParallelSystemArrayMediator.prototype.startMediator = function () {
                    if (this.mediator_ != null)
                        return;
                    this.mediator_ = this.createMediator();
                    this.mediator_.start();
                };
                /* ---------------------------------------------------------
                    ACCESSORS
                --------------------------------------------------------- */
                /**
                 * Get {@link MediatorSystem} object.
                 *
                 * When you need to send an {@link Invoke} message to the master system of this
                 * {@link ParallelSystemArrayMediator}, then send to the {@link MediatorSystem} through this {@link getMediator}.
                 *
                 * ```typescript
                 * this.getMediator().sendData(...);
                 * ```
                 *
                 * @return The {@link MediatorSystem} object.
                 */
                ParallelSystemArrayMediator.prototype.getMediator = function () {
                    return this.mediator_;
                };
                /* ---------------------------------------------------------
                    INVOKE MESSAGE CHAIN
                --------------------------------------------------------- */
                /**
                 * @hidden
                 */
                ParallelSystemArrayMediator.prototype._Complete_history = function (history) {
                    var ret = _super.prototype._Complete_history.call(this, history);
                    if (ret == true)
                        this.mediator_._Complete_history(history.getUID());
                    return ret;
                };
                return ParallelSystemArrayMediator;
            }(parallel.ParallelSystemArray));
            parallel.ParallelSystemArrayMediator = ParallelSystemArrayMediator;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="ParallelSystemArrayMediator.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * Mediator of Parallel Processing System, a server accepting slave clients.
             *
             * The {@link ParallelClientArrayMediator} is an abstract class, derived from the {@link ParallelSystemArrayMediator}
             * class, opening a server accepting {@link ParallelSystem parallel clients} as a **master**.
             *
             * Extends this {@link ParallelClientArrayMediator}, overrides {@link createServerBase createServerBase()} to
             * determine which protocol to follow and {@link createExternalClient createExternalClient()} creating child
             * {@link ParallelSystem} object. After the extending and overridings, open this server using the
             * {@link open open()} method.
             *
             * #### [Inherited] {@link ParallelSystemArrayMediator}
             * The {@link ParallelSystemArrayMediator} class be a **master** for its slave systems, and be a **slave** to its
             * master system at the same time. This {@link ParallelSystemArrayMediator} be a **master **system, containing and
             * managing {@link ParallelSystem} objects, which represent parallel slave systems, by extending
             * {@link ParallelSystemArray} class. Also, be a **slave** system through {@link getMediator mediator} object, which is
             * derived from the {@link SlaveSystem} class.
             *
             * As a **slave**, you can specify this {@link ParallelSystemArrayMediator} to be <i>a client slave connecting to
             * master server</i> or <i>a server slave accepting master client</i> by overriding the {@link createMediator} method.
             * Overrides the {@link createMediator createMediator()} method and return one of them:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * #### [Inherited] {@link ParallelSystemArray}
             * The {@link ParallelSystemArray} is an abstract class containing and managing remote parallel **slave** system
             * drivers, {@link ParallelSystem} objects. Within framework of network, {@link ParallelSystemArray} represents your
             * system, a **Master** of *Parallel Processing System* that requesting *parallel process* to slave systems and the
             * children {@link ParallelSystem} objects represent the remote slave systems, who is being requested the
             * *parallel processes*.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices
             * will be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * This class {@link ParallelSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link ParallelSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ParallelClientArrayMediator = (function (_super) {
                __extends(ParallelClientArrayMediator, _super);
                /* =========================================================
                    CONSTRUCTORS
                        - MEMBER
                        - FACTORY METHOD FOR CHILDREN
                ============================================================
                    MEMBER
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function ParallelClientArrayMediator() {
                    return _super.call(this) || this;
                }
                /* ---------------------------------------------------------
                    FACTORY METHOD FOR CHILDREN
                --------------------------------------------------------- */
                /**
                 * Add a newly connected remote client.
                 *
                 * When a {@link IClientDriver remote client} connects to this *master server of parallel processing system*,
                 * then this {@link ParallelClientArrayMediator} creates a child {@link ParallelSystem parallel client} object
                 * through the {@link createExternalClient createExternalClient()} method and {@link insert inserts} it.
                 *
                 * @param driver A communicator for parallel client.
                 */
                ParallelClientArrayMediator.prototype.addClient = function (driver) {
                    var system = this.createExternalClient(driver);
                    if (system == null)
                        return;
                    this.push_back(system);
                };
                /**
                 * (Deprecated) Factory method creating child object.
                 *
                 * The method {@link createChild createChild()} is deprecated. Don't use and override this.
                 *
                 * Note that, the {@link ParallelClientArrayMediator} is a server accepting {@link ParallelSystem parallel
                 * clients} as a master. There's no way to creating the {@link ParallelSystem parallel clients} in advance before
                 * opening the server.
                 *
                 * @param xml An {@link XML} object represents the child {@link ParallelSystem} object.
                 * @return null
                 */
                ParallelClientArrayMediator.prototype.createChild = function (xml) { return null; };
                /* ---------------------------------------------------------
                    SERVER's METHOD
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                ParallelClientArrayMediator.prototype.open = function (port) {
                    this.server_base_ = this.createServerBase();
                    if (this.server_base_ == null)
                        return;
                    this.server_base_.open(port);
                    this.startMediator();
                };
                /**
                 * @inheritdoc
                 */
                ParallelClientArrayMediator.prototype.close = function () {
                    if (this.server_base_ == null)
                        return;
                    this.server_base_.close();
                    this.clear();
                };
                return ParallelClientArrayMediator;
            }(parallel.ParallelSystemArrayMediator));
            parallel.ParallelClientArrayMediator = ParallelClientArrayMediator;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="ParallelSystem.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * A driver for parallel slave server.
             *
             * The {@link ParallelServer} is an abstract class, derived from the {@link ParallelSystem} class, connecting to
             * remote, parallel **slave** server. Extends this {@link ParallelServer} class and overrides the
             * {@link createServerConnector createServerConnector()} method following which protocol the **slave** server uses.
             *
             * #### [Inherited] {@link ParallelSystem}
             * The {@link ParallelSystem} is an abstract class represents a **slave** system in *Parallel Processing System*,
             * connected with this **master** system. This {@link ParallelSystem} takes full charge of network communication with
             * the remote, parallel **slave** system has connected.
             *
             * When a *parallel process* is requested (by {@link ParallelSystemArray.sendSegmentData} or
             * {@link ParallelSystemArray.sendPieceData}), the number of pieces to be allocated to a {@link ParallelSystem} is
             * turn on its {@link getPerformance performance index}. Higher {@link getPerformance performance index}, then
             * more pieces are requested. The {@link getPerformance performance index} is revaluated whenever a *parallel process*
             * has completed, basic on the execution time and number of pieces. You can sugguest or enforce the
             * {@link getPerformance performance index} with {@link setPerformance} or {@link enforcePerformance}.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Bridge & Proxy Pattern
             * This class {@link ParallelSystem} is derived from the {@link ExternalSystem} class. Thus, you can take advantage
             * of the *Bridge & Proxy Pattern* in this {@link ParallelSystem} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Bridge & Proxy Pattern*:
             *
             * The {@link ExternalSystem} class can be a *bridge* for *logical proxy*. In framework within user,
             * which {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Bridge Pattern* and *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ParallelServer = (function (_super) {
                __extends(ParallelServer, _super);
                /**
                 * Construct from parent {@link ParallelSystemArray}.
                 *
                 * @param systemArray The parent {@link ParallelSystemArray} object.
                 */
                function ParallelServer(systemArray) {
                    var _this = _super.call(this, systemArray) || this;
                    _this.ip = "";
                    _this.port = 0;
                    return _this;
                }
                /**
                 * @inheritdoc
                 */
                ParallelServer.prototype.connect = function () {
                    if (this.communicator != null)
                        return;
                    this.communicator = this.createServerConnector();
                    this.communicator.connect(this.ip, this.port);
                };
                return ParallelServer;
            }(parallel.ParallelSystem));
            parallel.ParallelServer = ParallelServer;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="ParallelSystemArray.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * Master of Parallel Processing System, a client connecting to slave servers.
             *
             * The {@link ParallelServerArray} is an abstract class, derived from the {@link ParallelSystemArray} class,
             * connecting to {@link IParallelServer parallel servers}.
             *
             * Extends this {@link ParallelServerArray} and overrides {@link createChild createChild()} method creating child
             * {@link IParallelServer} object. After the extending and overriding, construct children {@link IParallelServer}
             * objects and call the {@link connect connect()} method.
             *
             * #### [Inherited] {@link ParallelSystemArray}
             * The {@link ParallelSystemArray} is an abstract class containing and managing remote parallel **slave** system
             * drivers, {@link ParallelSystem} objects. Within framework of network, {@link ParallelSystemArray} represents your
             * system, a **Master** of *Parallel Processing System* that requesting *parallel process* to slave systems and the
             * children {@link ParallelSystem} objects represent the remote slave systems, who is being requested the
             * *parallel processes*.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices
             * will be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * This class {@link ParallelSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link ParallelSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ParallelServerArray = (function (_super) {
                __extends(ParallelServerArray, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function ParallelServerArray() {
                    return _super.call(this) || this;
                }
                /* ---------------------------------------------------------
                    CONNECTOR's METHOD
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                ParallelServerArray.prototype.connect = function () {
                    for (var i = 0; i < this.size(); i++)
                        this.at(i).connect();
                };
                return ParallelServerArray;
            }(parallel.ParallelSystemArray));
            parallel.ParallelServerArray = ParallelServerArray;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="ParallelSystemArrayMediator.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * Mediator of Parallel Processing System, a client connecting to slave servers.
             *
             * The {@link ParallelServerArrayMediator} is an abstract class, derived from the {@link ParallelSystemArrayMediator}
             * class, connecting to {@link IParallelServer parallel servers}.
             *
             * Extends this {@link ParallelServerArrayMediator} and overrides {@link createChild createChild()} method creating
             * child {@link IParallelServer} object. After the extending and overriding, construct children
             * {@link IParallelServer} objects and call the {@link connect connect()} method.
             *
             * #### [Inherited] {@link ParallelSystemArrayMediator}
             * The {@link ParallelSystemArrayMediator} class be a **master** for its slave systems, and be a **slave** to its
             * master system at the same time. This {@link ParallelSystemArrayMediator} be a **master **system, containing and
             * managing {@link ParallelSystem} objects, which represent parallel slave systems, by extending
             * {@link ParallelSystemArray} class. Also, be a **slave** system through {@link getMediator mediator} object, which is
             * derived from the {@link SlaveSystem} class.
             *
             * As a **master**, you can specify this {@link ParallelSystemArrayMediator} class to be <i>a master server accepting
             * slave clients<i> or <i>a master client to connecting slave servers</i>. Even both of them is possible. Extends one
             * of them below and overrides abstract factory method(s) creating the child {@link ParallelSystem} object.
             *
             * - {@link ParallelClientArrayMediator}: A server accepting {@link ParallelSystem parallel clients}.
             * - {@link ParallelServerArrayMediator}: A client connecting to {@link ParallelServer parallel servers}.
             * - {@link ParallelServerClientArrayMediator}: Both of them. Accepts {@link ParallelSystem parallel clients} and
             *                                              connects to {@link ParallelServer parallel servers} at the same time.
             *
             * As a **slave**, you can specify this {@link ParallelSystemArrayMediator} to be <i>a client slave connecting to
             * master server</i> or <i>a server slave accepting master client</i> by overriding the {@link createMediator} method.
             * Overrides the {@link createMediator createMediator()} method and return one of them:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * #### [Inherited] {@link ParallelSystemArray}
             * The {@link ParallelSystemArray} is an abstract class containing and managing remote parallel **slave** system
             * drivers, {@link ParallelSystem} objects. Within framework of network, {@link ParallelSystemArray} represents your
             * system, a **Master** of *Parallel Processing System* that requesting *parallel process* to slave systems and the
             * children {@link ParallelSystem} objects represent the remote slave systems, who is being requested the
             * *parallel processes*.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices
             * will be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * This class {@link ParallelSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link ParallelSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ParallelServerArrayMediator = (function (_super) {
                __extends(ParallelServerArrayMediator, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function ParallelServerArrayMediator() {
                    return _super.call(this) || this;
                }
                /* ---------------------------------------------------------
                    CONNECTOR's METHOD
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                ParallelServerArrayMediator.prototype.connect = function () {
                    for (var i = 0; i < this.size(); i++) {
                        this.at(i).connect();
                    }
                };
                return ParallelServerArrayMediator;
            }(parallel.ParallelSystemArrayMediator));
            parallel.ParallelServerArrayMediator = ParallelServerArrayMediator;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="ParallelClientArray.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * Master of Parallel Processing System, be a server and client at the same time.
             *
             * The {@link ParallelServerClientArray} is an abstract class, derived from the {@link ParallelSystemArray} class,
             * opening a server accepting {@link ParallelSystem parallel clients} and being a client connecting to
             * {@link IParallelServer parallel servers} at the same time.
             *
             * Extends this {@link ParallelServerClientArray} and overrides below methods. After the overridings, open server
             * with {@link open open()} method and connect to {@link IParallelServer parallel servers} through the
             * {@link connect connect()} method.
             *
             * - {@link createServerBase createServerBase()}
             * - {@link createExternalClient createExternalClient()}
             * - {@link createExternalServer createExternalServer()}
             *
             * #### [Inherited] {@link ParallelSystemArray}
             * The {@link ParallelSystemArray} is an abstract class containing and managing remote parallel **slave** system
             * drivers, {@link ParallelSystem} objects. Within framework of network, {@link ParallelSystemArray} represents your
             * system, a **Master** of *Parallel Processing System* that requesting *parallel process* to slave systems and the
             * children {@link ParallelSystem} objects represent the remote slave systems, who is being requested the
             * *parallel processes*.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices
             * will be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * This class {@link ParallelSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link ParallelSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ParallelServerClientArray = (function (_super) {
                __extends(ParallelServerClientArray, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function ParallelServerClientArray() {
                    return _super.call(this) || this;
                }
                /**
                 * Factory method of a child Entity.
                 *
                 * This method is migrated to {@link createExternalServer}. Override the {@link createExternalServer} method.
                 *
                 * @param xml An {@link XML} object represents child element, so that can identify the type of child to create.
                 * @return A new child Entity via {@link createExternalServer createExternalServer()}.
                 */
                ParallelServerClientArray.prototype.createChild = function (xml) {
                    return this.createExternalServer(xml);
                };
                /* ---------------------------------------------------------
                    METHOD OF CLIENT
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                ParallelServerClientArray.prototype.connect = function () {
                    for (var i = 0; i < this.size(); i++) {
                        var system = this.at(i);
                        if (system.connect == undefined)
                            continue;
                        system.connect();
                    }
                };
                return ParallelServerClientArray;
            }(parallel.ParallelClientArray));
            parallel.ParallelServerClientArray = ParallelServerClientArray;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="ParallelClientArrayMediator.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * Mediator of Parallel Processing System, be a server and client at the same time as a **master**.
             *
             * The {@link ParallelServerClientArrayMediator} is an abstract class, derived from the
             * {@link ParallelSystemArrayMediator} class, opening a server accepting {@link ParallelSystem parallel clients} and
             * being a client connecting to {@link IParallelServer parallel servers} at the same time.
             *
             * Extends this {@link ParallelServerClientArrayMediator} and overrides below methods. After the overridings, open
             * server with {@link open open()} method and connect to {@link IParallelServer parallel servers} through the
             * {@link connect connect()} method.
             *
             * - {@link createServerBase createServerBase()}
             * - {@link createExternalClient createExternalClient()}
             * - {@link createExternalServer createExternalServer()}
             *
             * #### [Inherited] {@link ParallelSystemArrayMediator}
             * The {@link ParallelSystemArrayMediator} class be a **master** for its slave systems, and be a **slave** to its
             * master system at the same time. This {@link ParallelSystemArrayMediator} be a **master **system, containing and
             * managing {@link ParallelSystem} objects, which represent parallel slave systems, by extending
             * {@link ParallelSystemArray} class. Also, be a **slave** system through {@link getMediator mediator} object, which is
             * derived from the {@link SlaveSystem} class.
             *
             * As a **master**, you can specify this {@link ParallelSystemArrayMediator} class to be <i>a master server accepting
             * slave clients<i> or <i>a master client to connecting slave servers</i>. Even both of them is possible. Extends one
             * of them below and overrides abstract factory method(s) creating the child {@link ParallelSystem} object.
             *
             * - {@link ParallelClientArrayMediator}: A server accepting {@link ParallelSystem parallel clients}.
             * - {@link ParallelServerArrayMediator}: A client connecting to {@link ParallelServer parallel servers}.
             * - {@link ParallelServerClientArrayMediator}: Both of them. Accepts {@link ParallelSystem parallel clients} and
             *                                              connects to {@link ParallelServer parallel servers} at the same time.
             *
             * As a **slave**, you can specify this {@link ParallelSystemArrayMediator} to be <i>a client slave connecting to
             * master server</i> or <i>a server slave accepting master client</i> by overriding the {@link createMediator} method.
             * Overrides the {@link createMediator createMediator()} method and return one of them:
             *
             * - A client slave connecting to master server:
             *   - {@link MediatorClient}
             *   - {@link MediatorWebClient}
             *   - {@link MediatorSharedWorkerClient}
             * - A server slave accepting master client:
             *   - {@link MediatorServer}
             *   - {@link MediatorWebServer}
             *   - {@link MediatorDedicatedWorkerServer}
             *   - {@link MediatorSharedWorkerServer}
             *
             * #### [Inherited] {@link ParallelSystemArray}
             * The {@link ParallelSystemArray} is an abstract class containing and managing remote parallel **slave** system
             * drivers, {@link ParallelSystem} objects. Within framework of network, {@link ParallelSystemArray} represents your
             * system, a **Master** of *Parallel Processing System* that requesting *parallel process* to slave systems and the
             * children {@link ParallelSystem} objects represent the remote slave systems, who is being requested the
             * *parallel processes*.
             *
             * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
             * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
             * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices
             * will be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * #### Proxy Pattern
             * This class {@link ParallelSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
             * advantage of the *Proxy Pattern* in the {@link ParallelSystemArray} class. If a process to request is not the
             * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
             * may better to utilizing the *Proxy Pattern*:
             *
             * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
             * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
             * important. Only interested in user's perspective is *which can be done*.
             *
             * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
             * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
             * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
             *
             * <ul>
             *	<li>
             *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
             *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
             *	</li>
             *	<li>
             *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
             *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
             *		external system.
             *	</li>
             *	<li> Those strategy is called *Proxy Pattern*. </li>
             * </ul>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var ParallelServerClientArrayMediator = (function (_super) {
                __extends(ParallelServerClientArrayMediator, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function ParallelServerClientArrayMediator() {
                    return _super.call(this) || this;
                }
                /**
                 * Factory method of a child Entity.
                 *
                 * This method is migrated to {@link createExternalServer}. Override the {@link createExternalServer} method.
                 *
                 * @param xml An {@link XML} object represents child element, so that can identify the type of child to create.
                 * @return A new child Entity via {@link createExternalServer createExternalServer()}.
                 */
                ParallelServerClientArrayMediator.prototype.createChild = function (xml) {
                    return this.createExternalServer(xml);
                };
                /* ---------------------------------------------------------
                    METHOD OF CLIENT
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                ParallelServerClientArrayMediator.prototype.connect = function () {
                    for (var i = 0; i < this.size(); i++) {
                        var system = this.at(i);
                        if (system.connect == undefined)
                            continue;
                        system.connect();
                    }
                };
                return ParallelServerClientArrayMediator;
            }(parallel.ParallelClientArrayMediator));
            parallel.ParallelServerClientArrayMediator = ParallelServerClientArrayMediator;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../slave/InvokeHistory.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var parallel;
        (function (parallel) {
            /**
             * History of an {@link Invoke} message.
             *
             * The {@link PRInvokeHistory} is a class archiving history log of an {@link Invoke} message which requests the
             * *parallel process*, created whenever {@link ParallelSystemArray.sendSegmentData} or
             * {@link ParallelSystemArray.sendSegmentData} is called.
             *
             * When the *parallel process* has completed, then {@link complete complete()} is called and the *elapsed time* is
             * determined. The elapsed time is utilized for computation of {@link ParallelSystem.getPerformance performance index}
             * of each {@link ParallelSystem parallel slave system}.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		  target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_parallel_system.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel_System)
             * @author Jeongho Nam <http://samchon.org>
             */
            var PRInvokeHistory = (function (_super) {
                __extends(PRInvokeHistory, _super);
                function PRInvokeHistory(invoke) {
                    if (invoke === void 0) { invoke = null; }
                    var _this = _super.call(this, invoke) || this;
                    if (invoke == null) {
                        _this.first = 0;
                        _this.last = 0;
                    }
                    else {
                        _this.first = invoke.get("_Piece_first").getValue();
                        _this.last = invoke.get("_Piece_last").getValue();
                    }
                    return _this;
                }
                /**
                 * Get initial piece's index.
                 *
                 * Returns initial piece's index in the section of requested *parallel process*.
                 *
                 * @return The initial index.
                 */
                PRInvokeHistory.prototype.getFirst = function () {
                    return this.first;
                };
                /**
                 * Get final piece's index.
                 *
                 * Returns initial piece's index in the section of requested *parallel process*. The range used is
                 * [*first*, *last*), which contains all the pieces' indices between *first* and *last*, including the piece
                 * pointed by index *first*, but not the piece pointed by the index *last*.
                 *
                 * @return The final index.
                 */
                PRInvokeHistory.prototype.getLast = function () {
                    return this.last;
                };
                /**
                 * Compute number of allocated pieces.
                 */
                PRInvokeHistory.prototype.computeSize = function () {
                    return this.last;
                };
                return PRInvokeHistory;
            }(templates.slave.InvokeHistory));
            parallel.PRInvokeHistory = PRInvokeHistory;
        })(parallel = templates.parallel || (templates.parallel = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../APi.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var service;
        (function (service_1) {
            /**
             * A driver of remote client.
             *
             * The {@link Client} is an abstract class representing and interacting with a remote client. It deals the network
             * communication with the remote client and shifts {@link Invoke} message to related {@link User} and {@link Service}
             * objects.
             *
             * Extends this {@link Client} class and override the {@link createService} method, a factory method creating a child
             * {@link Service} object. Note that, {@link Client} represents a remote client, not *an user*, a specific *web page*
             * or *service*. Do not define logics about user or account information. It must be declared in the parent
             * {@link User} class. Also, don't define processes of a specific a web page or service. Defines them in the child
             * {@link Service} class.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_cloud_service.png" target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_cloud_service.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Cloud Service](https://github.com/samchon/framework/wiki/TypeScript-Templates-Cloud_Service)
             * @author Jeongho Nam <http://samchon.org>
             */
            var Client = (function () {
                /* ------------------------------------------------------------------
                    CONSTRUCTORS
                ------------------------------------------------------------------ */
                /**
                 * Construct from parent {@link User} and communicator.
                 *
                 * @param user Parent {@link User} object.
                 * @param driver Communicator with remote client.
                 */
                function Client(user, driver) {
                    this.user_ = user;
                    this.no_ = ++user.sequence_;
                    // ENROLL COMMUNICATOR
                    this.communicator_ = driver;
                    this.communicator_.listen(this);
                    // CREATE SERVICE DIRECLTY
                    this.service_ = this.createService(driver.getPath());
                }
                /**
                 * Default Destructor.
                 *
                 * This {@link destructor destructor()} method is called when the {@link Client} object is destructed and this
                 * {@link Client} object is destructed when connection with the remote client is closed or this {@link Client}
                 * object is {@link User.erase erased} from its parent {@link User} object.
                 *
                 * Note that, don't call this {@link destructor destructor()} method by yourself. It must be called automatically
                 * by those *destruction* cases. Also, if your derived {@link Client} class has something to do on the
                 * *destruction*, then overrides this {@link destructor destructor()} method and defines the something to do.
                 * Overriding this {@link destructor destructor()}, don't forget to calling ```super.destructor();``` on tail.
                 *
                 * ```typescript
                 * class MyUser extends protocol.service.Client
                 * {
                 *     protected destructor(): void
                 *     {
                 *         // DO SOMETHING
                 *         this.do_something();
                 *
                 *         // CALL SUPER.DESTRUCTOR() ON TAIL. DON'T FORGET THIS
                 *         super.destructor();
                 *     }
                 * }
                 * ```
                 */
                Client.prototype.destructor = function () {
                    if (this.service_ != null)
                        this.service_.destructor();
                };
                /**
                 * Close connection.
                 */
                Client.prototype.close = function () {
                    this.user_.erase(this.no_);
                };
                /* ------------------------------------------------------------------
                    ACCESSORS
                ------------------------------------------------------------------ */
                /**
                 * Get parent {@link User} object.
                 *
                 * Get the parent {@link User} object, who is groupping {@link Client} objects with same session id.
                 *
                 * @return The parent {@link User} object.
                 */
                Client.prototype.getUser = function () {
                    return this.user_;
                };
                /**
                 * Get child {@link Service} object.
                 *
                 * @return The child {@link Service} object.
                 */
                Client.prototype.getService = function () {
                    return this.service_;
                };
                /**
                 * Get sequence number.
                 *
                 * Get sequence number of this {@link Client} object in the parent {@link User} object. This sequence number also
                 * be a *key* in the parent {@link User} object, who extended the ```std.HashMap<number, Client>```.
                 *
                 * @return Sequence number.
                 */
                Client.prototype.getNo = function () {
                    return this.no_;
                };
                Client.prototype.changeService = function (arg) {
                    if (this.service_ != null)
                        this.service_.destructor();
                    if (arg instanceof service_1.Service)
                        this.service_ = arg;
                    else
                        this.service_ = this.createService(arg);
                };
                /* ------------------------------------------------------------------
                    MESSAGE CHAIN
                ------------------------------------------------------------------ */
                /**
                 * Send an {@link Invoke} message.
                 *
                 * Sends an {@link Invoke} message to remote client.
                 *
                 * @param invoke An {@link Invoke} messgae to send to remote client.
                 */
                Client.prototype.sendData = function (invoke) {
                    this.communicator_.sendData(invoke);
                };
                /**
                 * Handle a replied {@link Invoke} message.
                 *
                 * The default {@link Client.replyData Client.replyData()} shifts chain to its parent {@link User} and belonged
                 * {@link Service} objects, by calling the the {@link User.replyData User.replyData()} and
                 * {@link Service.replyData Service.replyData()} methods.
                 *
                 * Note that, {@link Client} represents a remote client, not *an user*, a specific *web page* or *service*. Do not
                 * define logics about user or account information. It must be declared in the parent {@link User} class. Also,
                 * don't define processes of a specific a web page or service. Defines them in the child {@link Service} class.
                 *
                 * ```typescript
                 * class protocol.service.Client
                 * {
                 *     public replyData(invoke: protocol.Invoke): void
                 *     {
                 *         // SHIFT TO PARENT USER
                 *         // THE PARENT USER ALSO MAY SHIFT TO ITS PARENT SERVER
                 *         this.getUser().replyData(invoke);
                 *
                 *         // SHIFT TO BELOGED SERVICE
                 *         if (this.getService() != null)
                 *             this.getService().replyData(invoke);
                 *     }
                 * }
                 *
                 * class MyClient extends protocol.service.Client
                 * {
                 *     public replyData(invoke: protocol.Invoke): void
                 *     {
                 *         if (invoke.getListener() == "do_something_in_client_level")
                 *             this.do_something_in_client_level();
                 *         else
                 *             super.replyData(invoke);
                 *     }
                 * }
                 * ```
                 *
                 * @param invoke An {@link Invoke invoke} message to be handled in {@link Client} level.
                 */
                Client.prototype.replyData = function (invoke) {
                    // SHIFT CHAIN TO USER
                    this.user_.replyData(invoke);
                    // AND SERVICE
                    if (this.service_ != null)
                        this.service_.replyData(invoke);
                };
                return Client;
            }());
            service_1.Client = Client;
        })(service = templates.service || (templates.service = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../../protocol/Server.ts" />
/**
 * A system template for Cloud Service.
 *
 * @handbook [Templates - Cloud Service](https://github.com/samchon/framework/wiki/TypeScript-Templates-Cloud_Service)
 * @author Jeongho Nam <http://samchon.org>
 */
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var service;
        (function (service) {
            /**
             * A cloud server.
             *
             * The {@link Server} is an abstract server class, who can build a real-time cloud server, that is following the
             * web-socket protocol. Extends this {@link Server} and related classes and overrides abstract methods under below.
             * After the overridings, open this {@link Server cloud server} using the {@link open open()} method.
             *
             * - Objects in composite relationship and their factory methods
             *   - {@link User}: {@link Server.createUser Server.createUser()}
             *   - {@link Client}: {@link User.createClient User.createClient()}
             *   - {@link Service}: {@link Client.createService Client.createService()}
             * - {@link Invoke} message chains; {@link IProtocol.replyData replyData}
             *   - {@link Server.replyData}
             *   - {@link User.replyData}
             *   - {@link Client.replyData}
             *   - {@link Service.replyData}
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_cloud_service.png" target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_cloud_service.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Cloud Service](https://github.com/samchon/framework/wiki/TypeScript-Templates-Cloud_Service)
             * @author Jeongho Nam <http://samchon.org>
             */
            var Server = (function (_super) {
                __extends(Server, _super);
                /* ------------------------------------------------------------------
                    CONSTRUCTORS
                ------------------------------------------------------------------ */
                /**
                 * Default Constructor.
                 */
                function Server() {
                    var _this = _super.call(this) || this;
                    // INITIALIZE USER MAPS
                    _this.session_map_ = new std.HashMap();
                    _this.account_map_ = new std.HashMap();
                    return _this;
                }
                /* ------------------------------------------------------------------
                    ACCESSORS
                ------------------------------------------------------------------ */
                /**
                 * Test wheter an {@link User} exists with the *accountID*.
                 *
                 * @param accountID Account id of {@link User} to find.
                 * @return Exists or not.
                 */
                Server.prototype.has = function (accountID) {
                    return this.account_map_.has(accountID);
                };
                /**
                 * Get an {@link User} object by its *accountID*.
                 *
                 * @param accountID Account id of {@link User} to get.
                 * @return An {@link User} object.
                 */
                Server.prototype.get = function (accountID) {
                    return this.account_map_.get(accountID);
                };
                /* ------------------------------------------------------------------
                    MESSAGE CHAIN
                ------------------------------------------------------------------ */
                /**
                 * Send an {@link Invoke} message.
                 *
                 * Sends an {@link Invoke} message to all remote clients through the belonged {@link User} and {@link Client}
                 * objects. Sending the {@link Invoke} message to all remote clients, it's came true by passing through
                 * {@link User.sendData User.sendData()}. And the {@link User.sendData} also pass through the
                 * {@link Client.sendData Client.sendData()}.
                 *
                 * ```typescript
                 * class protocol.service.Server
                 * {
                 *     public sendData(invoke: Invoke): void
                 *     {
                 *         for (user: User in this)
                 *             for (client: Client in user)
                 *                 client.sendData(invoke);
                 *     }
                 * }
                 * ```
                 *
                 * @param invoke {@link Invoke} message to send to all remote clients.
                 */
                Server.prototype.sendData = function (invoke) {
                    for (var it = this.session_map_.begin(); !it.equals(this.session_map_.end()); it = it.next())
                        it.second.sendData(invoke);
                };
                /* ------------------------------------------------------------------
                    CLIENT I/O
                ------------------------------------------------------------------ */
                /**
                 * Add a newly connected remote client.
                 *
                 * When a {@link WebClientDriver remote client} connects to this cloud server, then {@link Server} queries the
                 * {WebClientDriver.getSessionID session id} of the {@link WebClientDriver remote client}. If the
                 * {WebClientDriver.getSessionID session id} is new one, then creates a new {@link User} object.
                 *
                 * At next, creates a {@link Client} object who represents the newly connected remote client and insert the
                 * {@link Client} object to the matched {@link User} object which is new or ordinary one following the
                 * {WebClientDriver.getSessionID session id}. At last, a {@link Service} object can be created with referencing
                 * the {@link WebClientDriver.getPath path}.
                 *
                 * List of objects can be created by this method.
                 * - {@link User} by {@link createUser createUser()}.
                 * - {@link Client} by {@link User.createClient User.createClient()}.
                 * - {@link Service} by {@link Client.createService Client.createService()}.
                 *
                 * @param driver A web communicator for remote client.
                 */
                Server.prototype.addClient = function (driver) {
                    //--------
                    // CREATE CHILDREN OBJECTS
                    //--------
                    // USER
                    var user;
                    if (this.session_map_.has(driver.getSessionID()) == true)
                        user = this.session_map_.get(driver.getSessionID());
                    else {
                        user = this.createUser();
                        user.session_id_ = (driver.getSessionID());
                        this.session_map_.insert(std.make_pair(driver.getSessionID(), user));
                    }
                    //--------
                    // CLIENT
                    //--------
                    // SERVICE IS CREEATED IN CLIENT'S CONSTRUCTOR
                    var client = user.createClient(driver);
                    user.insert(std.make_pair(client.getNo(), client));
                    // CLOSE HANDLER
                    driver.onClose = function () {
                        // WHEN DISCONNECTED, THEN ERASE THE CLIENT.
                        // OF COURSE, IT CAN CAUSE DELETION OF THE RELATED USER.
                        user.erase(client.getNo());
                        // ALSO, DESTRUCTORS OF THE SERVICE ARE CALLED.
                        if (client.getService() != null)
                            client.getService().destructor(); // SERVICE
                        client.destructor(); // AND CLIENT
                    };
                };
                /**
                 * @hidden
                 */
                Server.prototype._Erase_user = function (user) {
                    // USER DOESN'T BE ERASED AT THAT TIME
                    // IT WAITS UNTIL 30 SECONDS TO KEEP SESSION
                    setTimeout(function () {
                        var server = this;
                        if (user.empty() == false)
                            return; // USER IS NOT EMPTY, THEN RETURNS
                        // ERASE USER FROM
                        server.session_map_.erase(user.session_id_); // SESSION-ID MAP
                        if (user.getAccountID() != "")
                            server.account_map_.erase(user.getAccountID());
                        // CALL DESTRUCTOR
                        user.destructor();
                    }.bind(this), 30000 // KEEP USER 30 SECONDS
                    );
                };
                return Server;
            }(samchon.protocol.WebServer));
            service.Server = Server;
        })(service = templates.service || (templates.service = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var service;
        (function (service) {
            /**
             * A service.
             *
             * The {@link Service} is an abstract class who represents a service, that is providing functions a specific page.
             *
             * Extends the {@link Service} class and defines its own service, which to be provided for the specific weg page,
             * by overriding the {@link replyData replyData()} method. Note that, the service, functions for the specific page
             * should be defined in this {@link Service} class, not its parent {@link Client} class who represents a remote client
             * and takes communication responsibility.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_cloud_service.png" target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_cloud_service.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Cloud Service](https://github.com/samchon/framework/wiki/TypeScript-Templates-Cloud_Service)
             * @author Jeongho Nam <http://samchon.org>
             */
            var Service = (function () {
                /* ------------------------------------------------------------------
                    CONSTRUCTORS
                ------------------------------------------------------------------ */
                /**
                 * Construct from parent {@link Client} and requested path.
                 *
                 * @param client Driver of remote client.
                 * @param path Requested path that identifies this {@link Service}.
                 */
                function Service(client, path) {
                    this.client_ = client;
                    this.path_ = path;
                }
                /**
                 * Default Destructor.
                 *
                 * This {@link destructor destructor()} method is call when the {@link Service} object is destructed and the
                 * {@link Service} object is destructed when its parent {@link Client} object has
                 * {@link Client.destructor destructed} or the {@link Client} object {@link Client.changeService changed} its
                 * child {@link Service service} object to another one.
                 *
                 * Note that, don't call this {@link destructor destructor()} method by yourself. It must be called automatically
                 * by those *destruction* cases. Also, if your derived {@link Service} class has something to do on the
                 * *destruction*, then overrides this {@link destructor destructor()} method and defines the something to do.
                 */
                Service.prototype.destructor = function () {
                };
                /* ------------------------------------------------------------------
                    ACCESSORS
                ------------------------------------------------------------------ */
                /**
                 * Get client.
                 */
                Service.prototype.getClient = function () {
                    return this.client_;
                };
                /**
                 * Get requested path.
                 */
                Service.prototype.getPath = function () {
                    return this.path_;
                };
                /* ------------------------------------------------------------------
                    MESSAGE CHAIN
                ------------------------------------------------------------------ */
                /**
                 * Send an {@link Invoke} message.
                 *
                 * Sends an {@link Invoke} message to remote system through parent {@link Client} object ({@link Client.sendData}).
                 *
                 * @param invoke An {@link Invoke} message to send to the remte system.
                 */
                Service.prototype.sendData = function (invoke) {
                    return this.client_.sendData(invoke);
                };
                return Service;
            }());
            service.Service = Service;
        })(service = templates.service || (templates.service = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <refernece path="../../collection/HashMapCollection.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var service;
        (function (service) {
            /**
             * An user.
             *
             * The {@link User} is an abstract class groupping {@link Client} objects, who communicates with remote client, with
             * same *session id*. This {@link User} represents a *remote user* literally. Within framework of remote system,
             * an {@link User} corresponds to a web-browser and a {@link Client} represents a window in the web-browser.
             *
             * Extends this {@link User} class and override the {@link createClient} method, a factory method creating a child
             * {@link Client} object. I repeat, the {@link User} class represents a *remote user*, groupping {@link Client}
             * objects with same *session id*. If your cloud server has some processes to be handled in the **user level**, then
             * defines method in this {@link User} class. Methods managing **account** under below are some of them:
             *
             * - {@link setAccount setAccount()}
             * - {@link getAccountID getAccountID()}
             * - {@link getAuthority getAuthority()}
             *
             * The children {@link Client} objects, they're contained with their key, the {@link Client.getNo sequence number}.
             * If you {@link User.erase erase} the children {@link Client} object by yourself, then their connection with the
             * remote clients will be {@link Client.close closed} and their {@link Client.destructor destruction method} will be
             * called. If you remove {@link clear all children}, then this {@link User} object will be also
             * {@link destructor destructed} and erased from the parent {@link Server} object.
             *
             * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_cloud_service.png" target="_blank">
             *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_cloud_service.png"
             *		 style="max-width: 100%" />
             * </a>
             *
             * @handbook [Templates - Cloud Service](https://github.com/samchon/framework/wiki/TypeScript-Templates-Cloud_Service)
             * @author Jeongho Nam <http://samchon.org>
             */
            var User = (function (_super) {
                __extends(User, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Construct from its parent {@link Server}.
                 *
                 * @param server The parent {@link Server} object.
                 */
                function User(server) {
                    var _this = _super.call(this) || this;
                    _this.server_ = server;
                    _this.account_id_ = "guest";
                    _this.authority_ = 0;
                    _this.session_id_ = "";
                    _this.sequence_ = 0;
                    _this.addEventListener("erase", _this._Handle_erase_client, _this);
                    return _this;
                }
                /**
                 * Default Destructor.
                 *
                 * This {@link destructor destructor()} method is called when the {@link User} object is destructed. The
                 * {@link User} object is destructed when connections with the remote clients are all closed, that is all the
                 * children {@link Client} objects are all removed, and 30 seconds has left. If some remote client connects
                 * within the 30 seconds, then the {@link User} object doesn't be destructed.
                 *
                 * Note that, don't call this {@link destructor destructor()} method by yourself. It must be called automatically
                 * by those *destruction* cases. Also, if your derived {@link User} class has something to do on the
                 * *destruction*, then overrides this {@link destructor destructor()} method and defines the something to do.
                 * Overriding this {@link destructor destructor()}, don't forget to calling ```super.destructor();``` on tail.
                 *
                 * ```typescript
                 * class MyUser extends protocol.service.User
                 * {
                 *     protected destructor(): void
                 *     {
                 *         // DO SOMETHING
                 *         this.do_something();
                 *
                 *         // CALL SUPER.DESTRUCTOR() ON TAIL. DON'T FORGET THIS
                 *         super.destructor();
                 *     }
                 * }
                 * ```
                 */
                User.prototype.destructor = function () {
                };
                /**
                 * @hidden
                 */
                User.prototype._Handle_erase_client = function (event) {
                    for (var it = event.first; !it.equals(event.last); it = it.next())
                        it.second.close();
                    if (this.empty() == true)
                        this.server_._Erase_user(this);
                };
                /* ---------------------------------------------------------
                    ACCESSORS
                --------------------------------------------------------- */
                /**
                 * Get parent {@lin Server} object.
                 *
                 * @return Parent {@link Server} object.
                 */
                User.prototype.getServer = function () {
                    return this.server_;
                };
                /**
                 * Get account id.
                 *
                 * @return Account ID.
                 */
                User.prototype.getAccountID = function () {
                    return this.account_id_;
                };
                /**
                 * Get authority.
                 *
                 * @return Authority
                 */
                User.prototype.getAuthority = function () {
                    return this.authority_;
                };
                /**
                 * Set *account id* and *authority*.
                 *
                 * The {@link setAccount setAccount()} is a method configuring *account id* and *authority* of this {@link User}.
                 *
                 * After the configuring, the {@link getAccountID account id} is enrolled into the parent {@link Server} as a
                 * **key** for this {@link User} object. You can test existence and access this {@link User} object from
                 * {@link Server.has Server.has()} and {@link Server.get Server.get()} with the {@link getAccountID account id}.
                 * Of course, if ordinary {@link getAccountID account id} had existed, then the ordinary **key** will be
                 * replaced.
                 *
                 * As you suggest, this {@link setAccount setAccount()} is something like a **log-in** function. If what you want
                 * is not **logging-in**, but **logging-out**, then configure the *account id* to empty string ``""```` or call
                 * the {@link lgout logout()} method.
                 *
                 * @param id To be account id.
                 * @param authority To be authority.
                 */
                User.prototype.setAccount = function (id, authority) {
                    if (this.account_id_ == id)
                        return;
                    else if (this.account_id_ != "")
                        this.server_.account_map_.erase(this.account_id_); // ERASE FROM ORDINARY ACCOUNT_MAP
                    // SET
                    this.account_id_ = id;
                    this.authority_ = authority;
                    // REGISTER TO ACCOUNT_MAP IN ITS SERVER
                    if (id != "")
                        this.server_.account_map_.set(id, this);
                };
                /**
                 * Log-out.
                 *
                 * This {@link logout logout()} method configures {@link getAccountID account id} to empty string and
                 * {@link getAuthority authority} to zero.
                 *
                 * The ordinary {@link getAccountID account id} will be also erased from the parent {@link Server} object. You
                 * can't access this {@link User} object from {@link Server.has Server.has()} and {@link Server.get Server.get()}
                 * with the ordinary {@link getAccountID account id} more.
                 */
                User.prototype.logout = function () {
                    if (this.account_id_ != "")
                        this.server_.account_map_.erase(this.account_id_);
                    this.setAccount("", 0);
                };
                /* ---------------------------------------------------------
                    MESSAGE CHAIN
                --------------------------------------------------------- */
                /**
                 * Send an {@link Invoke} message.
                 *
                 * Sends an {@link Invoke} message to all remote clients through the belonged {@link Client} objects. Sending the
                 * {@link Invoke} message to all remote clients, it's came true by passing through the
                 * {@link Client.sendData Client.sendData()} methods.
                 *
                 * ```typescript
                 * class protocol.service.User
                 * {
                 *     public sendData(invoke: Invoke): void
                 *     {
                 *         for (let it = this.begin(); !it.equals(this.end()); it = it.next())
                 *             it.second.sendData(invoke);
                 *     }
                 * }
                 * ```
                 *
                 * @param invoke {@link Invoke} message to send to all remote clients.
                 */
                User.prototype.sendData = function (invoke) {
                    for (var it = this.begin(); !it.equals(this.end()); it = it.next())
                        it.second.sendData(invoke);
                };
                /**
                 * Handle a replied {@link Invoke} message.
                 *
                 * The default {@link User.replyData User.replyData()} shifts chain to its parent {@link Server} object, by
                 * calling the {@link Server.replyData Server.replyData()} method. If there're some {@link Invoke} message to be
                 * handled in this {@link User} level, then override this method and defines what to do with the {@link Invoke}
                 * message in this {@link User} level.
                 *
                 * ```typescript
                 * class protocol.service.User
                 * {
                 *     public replyData(invoke: protocol.Invoke): void
                 *     {
                 *         this.getServer().replyData(invoke);
                 *     }
                 * }
                 *
                 * class MyUser extends protocol.service.User
                 * {
                 *     public replyData(invoke: protocol.Invoke): void
                 *     {
                 *          if (invoke.apply(this) == false) // IS TARGET TO BE HANDLED IN THIS USER LEVEL
                 *              super.replyData(invoke); // SHIFT TO SERVER
                 *     }
                 * }
                 * ```
                 *
                 * @param invoke An {@link Invoke invoke} message to be handled in {@link User} level.
                 */
                User.prototype.replyData = function (invoke) {
                    this.server_.replyData(invoke);
                };
                return User;
            }(samchon.collections.HashMapCollection));
            service.User = User;
        })(service = templates.service || (templates.service = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../../protocol/Invoke.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var slave;
        (function (slave) {
            var PInvoke = (function (_super) {
                __extends(PInvoke, _super);
                function PInvoke(invoke, history, masterDriver) {
                    var _this = _super.call(this, invoke.getListener()) || this;
                    _this.assign(invoke.begin(), invoke.end());
                    _this.history_ = history;
                    _this.master_driver_ = masterDriver;
                    _this.hold_ = false;
                    return _this;
                }
                PInvoke.prototype.getHistory = function () {
                    return this.history_;
                };
                PInvoke.prototype.isHold = function () {
                    return this.hold_;
                };
                /**
                 * Hold reporting completion to master.
                 */
                PInvoke.prototype.hold = function () {
                    this.hold_ = true;
                };
                /**
                 * Report completion.
                 */
                PInvoke.prototype.complete = function () {
                    this.history_.complete();
                    this.master_driver_.sendData(this.history_.toInvoke());
                };
                return PInvoke;
            }(samchon.protocol.Invoke));
            slave.PInvoke = PInvoke;
        })(slave = templates.slave || (templates.slave = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="SlaveSystem.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var slave;
        (function (slave) {
            var SlaveClient = (function (_super) {
                __extends(SlaveClient, _super);
                /* ---------------------------------------------------------
                    CONSTRUCTORS
                --------------------------------------------------------- */
                /**
                 * Default Constructor.
                 */
                function SlaveClient() {
                    return _super.call(this) || this;
                }
                /* ---------------------------------------------------------
                    METHOD OF CONNECTOR
                --------------------------------------------------------- */
                /**
                 * @inheritdoc
                 */
                SlaveClient.prototype.connect = function (ip, port) {
                    if (this.communicator_ != null)
                        return;
                    this.communicator_ = this.createServerConnector();
                    this.communicator_.connect(ip, port);
                };
                return SlaveClient;
            }(slave.SlaveSystem));
            slave.SlaveClient = SlaveClient;
        })(slave = templates.slave || (templates.slave = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="SlaveSystem.ts" />
var samchon;
(function (samchon) {
    var templates;
    (function (templates) {
        var slave;
        (function (slave) {
            var SlaveServer = (function (_super) {
                __extends(SlaveServer, _super);
                /* =========================================================
                    CONSTRUCTORS
                        - MEMBER
                        - FACTORY METHOD FOR CHILDREN
                ============================================================
                    MEMBER
                --------------------------------------------------------- */
                function SlaveServer() {
                    var _this = _super.call(this) || this;
                    _this.server_base_ = null;
                    return _this;
                }
                /* ---------------------------------------------------------
                    SERVER's METHOD
                --------------------------------------------------------- */
                SlaveServer.prototype.open = function (port) {
                    this.server_base_ = this.createServerBase();
                    if (this.server_base_ == null)
                        return;
                    this.server_base_.open(port);
                };
                SlaveServer.prototype.close = function () {
                    if (this.server_base_ != null)
                        this.server_base_.close();
                };
                /* ---------------------------------------------------------
                    OVERRIDINGS
                --------------------------------------------------------- */
                SlaveServer.prototype.addClient = function (driver) {
                    this.communicator_ = driver;
                    driver.listen(this);
                };
                return SlaveServer;
            }(slave.SlaveSystem));
            slave.SlaveServer = SlaveServer;
        })(slave = templates.slave || (templates.slave = {}));
    })(templates = samchon.templates || (samchon.templates = {}));
})(samchon || (samchon = {}));
