var __extends = (this && this.__extends) || function (d, b) {
    for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
    function __() { this.constructor = d; }
    d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
};
if (typeof (exports) != "undefined")
    try {
        global["std"] = require("typescript-stl");
    }
    catch (e) {
    }
/// <reference path="../miscellaneous/node/requires.ts" /> 
/// <reference path="../../samchon/API.ts" />
var samchon;
(function (samchon) {
    var example;
    (function (example) {
        function test_file_reference() {
            var file = new samchon.library.FileReference();
            file.addEventListener("select", handle_select);
            file.addEventListener("complete", handle_complete);
            file.browse("*.js", "*.ts", "*.txt");
        }
        example.test_file_reference = test_file_reference;
        function handle_select(event) {
            var file = event.target;
            file.load();
        }
        function handle_complete(event) {
            var file = event.target;
            console.log(file.name, file.extension, file.size, file.modificationDate);
        }
    })(example = samchon.example || (samchon.example = {}));
})(samchon || (samchon = {}));
var samchon;
(function (samchon) {
    var example;
    (function (example) {
        var WebClient = (function () {
            function WebClient() {
                var this_ = this;
                this.connector = new samchon.protocol.ServerConnector(this);
                this.connector.onopen =
                    function (event) {
                        console.log("connected");
                        this_.sendData(new samchon.protocol.Invoke("sendMessage", 99999, "I am JavaScript Client", 3, 7));
                    };
                this.connector.connect("127.0.0.1", 37888, "simulation");
            }
            WebClient.prototype.rotate_interval = function () {
                console.log("send message");
                this.sendData(new samchon.protocol.Invoke("sendMessage", "I am JavaScript Client", 3, 7));
            };
            WebClient.prototype.sendData = function (invoke) {
                console.log("sendData: #" + invoke.toXML().toString());
                this.connector.sendData(invoke);
            };
            WebClient.prototype.replyData = function (invoke) {
                console.log("message from cpp:", invoke.getListener());
            };
            return WebClient;
        }());
        function test_web_client() {
            var webClient = new WebClient();
        }
        example.test_web_client = test_web_client;
    })(example = samchon.example || (samchon.example = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var library;
    (function (library) {
        /**
         * <p> XML is a class representing a tree structued xml objects. </p>
         * <p> The XML class provides methods and properties for working with XML objects. </p>
         *
         * <p> The XML class (along with the XMLList and Namespace) implements
         * the powerful XML-handling standard defined in ECMAScript for XML (E4X) specification. </p>
         *
         * <p> XML class has a recursive, hierarchical relationship. </p>
         *
         * <p> Relationships between XML and XMLList </p>
         * <ul>
         *	<li> XML contains XMLList from dictionary of XMLList. </li>
         *  <li> XMLList contains XML from vector of XML. </li>
         * </ul>
         *
         * <h4> Note </h4>
         * <p> Do not abuse values for expressing member variables. </p>
         *
         * <table>
         *	<tr>
         *		<th>Standard Usage</th>
         *		<th>Non-standard usage abusing value</th>
         *	</tr>
         *	<tr>
         *		<td>
         *			&lt;memberList&gt;<br/>
         *			&nbsp;&nbsp;&nbsp;&nbsp; &lt;member id='jhnam88' name='Jeongho+Nam' birthdate='1988-03-11' /&gt;<br/>
         *			&nbsp;&nbsp;&nbsp;&nbsp; &lt;member id='master' name='Administartor' birthdate='2011-07-28' /&gt;<br/>
         *			&lt;/memberList&gt;
         *		</td>
         *		<td>
         *			&lt;member&gt;<br/>
         *			&nbsp;&nbsp;&nbsp;&nbsp; &lt;id&gt;jhnam88&lt;/id&gt;<br/>
         *			&nbsp;&nbsp;&nbsp;&nbsp; &lt;name&gt;Jeongho+Nam&lt;/name&gt;<br/>
         *			&nbsp;&nbsp;&nbsp;&nbsp; &lt;birthdate&gt;1988-03-11&lt;/birthdate&gt;<br/>
         *			&lt;/member&gt;
         *		</td>
         *	</tr>
         * </table>
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var XML = (function (_super) {
            __extends(XML, _super);
            /* -------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------- */
            /**
             * <p> Default Constructor. </p>
             *
             * <p> If the string parameter is not omitted, constructs its tag, value and
             * properties by parsing the string. If there's children, then construct the
             * children XML, XMLList objects, too. </p>
             *
             * @param str A string to be parsed
             */
            function XML(str) {
                if (str === void 0) { str = ""; }
                _super.call(this);
                this.properties = new std.HashMap();
                this.value = "";
                if (str.indexOf("<") == -1)
                    return;
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
                this.construct(str);
            }
            /**
             * <p> Construct XML objects by parsing a string. </p>
             */
            XML.prototype.construct = function (str) {
                this.parseTag(str);
                this.parseProperty(str);
                var res = this.parseValue(str);
                if (res.second == true)
                    this.parseChildren(res.first);
            };
            /**
             * <p> Parse and fetch a tag. </p>
             */
            XML.prototype.parseTag = function (str) {
                var start = str.indexOf("<") + 1;
                var end = this.calcMinIndex(str.indexOf(" ", start), str.indexOf("\r\n", start), str.indexOf("\n", start), str.indexOf("\t", start), str.indexOf(">", start), str.indexOf("/", start));
                if (start == 0 || end == -1)
                    return;
                this.tag = str.substring(start, end);
            };
            /**
             * <p> Parse and fetch properties. </p>
             */
            XML.prototype.parseProperty = function (str) {
                var start = str.indexOf("<" + this.tag) + this.tag.length + 1;
                var end = this.calcMinIndex(str.lastIndexOf("/"), str.indexOf(">", start));
                if (start == -1 || end == -1 || start >= end)
                    return;
                //<comp label='ABCD' /> : " label='ABCD' "
                var line = str.substring(start, end);
                if (line.indexOf("=") == -1)
                    return;
                var label;
                var value;
                var helpers = new Array();
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
                        helpers.push({ "type": quoteType, "start": start, "end": i });
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
                        equal = line.indexOf("=", helpers[i - 1]["end"] + 1);
                        label = line.substring(helpers[i - 1]["end"] + 1, equal).trim();
                    }
                    value = line.substring(helpers[i]["start"] + 1, helpers[i]["end"]);
                    this.setProperty(label, XML.decodeProperty(value));
                }
            };
            /**
             * <p> Parse and fetch a value. </p>
             */
            XML.prototype.parseValue = function (str) {
                var end_slash = str.lastIndexOf("/");
                var end_block = str.indexOf(">");
                if (end_slash < end_block || end_slash + 1 == str.lastIndexOf("<")) {
                    //STATEMENT1: <TAG />
                    //STATEMENT2: <TAG></TAG> -> SAME WITH STATEMENT1: <TAG />
                    this.value = "";
                    return new std.Pair(str, false);
                }
                var start = end_block + 1;
                var end = str.lastIndexOf("<");
                str = str.substring(start, end); //REDEFINE WEAK_STRING -> IN TO THE TAG
                if (str.indexOf("<") == -1)
                    this.value = XML.decodeValue(str.trim());
                else
                    this.value = "";
                return new std.Pair(str, true);
            };
            /**
             * <p> Parse and construct children XML objects. </p>
             */
            XML.prototype.parseChildren = function (str) {
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
                        xml.construct(str.substring(start, end + 1));
                        if (this.has(xml.tag) == true)
                            xmlList = this.get(xml.tag);
                        else {
                            xmlList = new XMLList();
                            this.set(xml.tag, xmlList);
                        }
                        xmlList.push(xml);
                        i = end;
                        start = end + 1;
                        blockStart = 0;
                        blockEnd = 0;
                    }
                }
            };
            /* -------------------------------------------------------------
                ACCESSORS
            ------------------------------------------------------------- */
            /**
             * <p> Get tag. </p>
             */
            XML.prototype.getTag = function () {
                return this.tag;
            };
            /**
             * <p> Get value. </p>
             */
            XML.prototype.getValue = function () {
                return this.value;
            };
            /**
             * <p> Test whether a property exists or not. </p>
             */
            XML.prototype.hasProperty = function (key) {
                return this.properties.has(key);
            };
            /**
             * <p> Get property by its key. </p>
             */
            XML.prototype.getProperty = function (key) {
                return this.properties.get(key);
            };
            XML.prototype.getPropertyMap = function () {
                return this.properties;
            };
            /* -------------------------------------------------------------
                SETTERS
            ------------------------------------------------------------- */
            /**
             * <p> Set tag (identifier) of the XML. </p>
             */
            XML.prototype.setTag = function (str) {
                this.tag = str;
            };
            /**
             * <p> Set value of the XML. </p>
             *
             * <p> Do not abuse values for expressing member variables. </p>
             * <table>
             *	<tr>
             *		<th>Standard Usage</th>
             *		<th>Non-standard usage abusing value</th>
             *	</tr>
             *	<tr>
             *		<td>
             *			\<memberList\>\n
             *			&nbsp;&nbsp;&nbsp;&nbsp;\<member id='jhnam88' name='Jeongho+Nam' birthdate='1988-03-11' /\>\n
             *			&nbsp;&nbsp;&nbsp;&nbsp;\<member id='master' name='Administartor' birthdate='2011-07-28' /\>\n
             *			\</memberList\>
             *		</td>
             *		<td>
             *			\<member\>\n
             *				\<id\>jhnam88\</id\>\n
             *				\<name\>Jeongho+Nam\</name\>\n
             *				\<birthdate\>1988-03-11\</birthdate\>\n
             *			\</member\>
             *		</td>
             *	</tr>
             * </table>
             *
             * @param val A value to set
             */
            XML.prototype.setValue = function (str) {
                this.value = str;
            };
            /**
             * <p> Set a property with its key. </p>
             */
            XML.prototype.setProperty = function (key, value) {
                this.properties.set(key, value);
            };
            /**
             * <p> Erase a property by its key. </p>
             *
             * @param key The key of the property to erase
             * @throw exception out of range
             */
            XML.prototype.eraseProperty = function (key) {
                if (this.properties.has(key) == false)
                    throw Error("out of range");
                else
                    this.properties.erase(key);
            };
            XML.prototype.push = function () {
                var items = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    items[_i - 0] = arguments[_i];
                }
                for (var i = 0; i < items.length; i++) {
                    if (items[i] instanceof XML) {
                        var xml = items[i];
                        if (this.has(xml.tag) == true)
                            this.get(xml.tag).push(xml);
                        else {
                            var xmlList = new XMLList();
                            xmlList.push(xml);
                            this.set(xml.tag, xmlList);
                        }
                    }
                    else if (items[i] instanceof XMLList) {
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
            XML.prototype.addAllProperties = function (xml) {
                for (var it = xml.properties.begin(); it.equal_to(xml.properties.end()) == false; it = it.next())
                    this.setProperty(it.first, it.second);
            };
            XML.prototype.clearProperties = function () {
                this.properties.clear();
            };
            /* -------------------------------------------------------------
                FILTERS
            ------------------------------------------------------------- */
            XML.prototype.calcMinIndex = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i - 0] = arguments[_i];
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
             * <p> Decode a value. </p>
             *
             * <table>
             *	<tr>
             *		<th>Encoded</th>
             *		<th>Decoded</th>
             *	</tr>
             *	<tr>
             *		<td>\&amp;</td>
             *		<td>\&</td>
             *	</tr>
             *	<tr>
             *		<td>\&lt;</td>
             *		<td>\<</td>
             *	</tr>
             *	<tr>
             *		<td>\&gt;</td>
             *		<td>\></td>
             *	</tr>
             * </table>
             *
             * @return A decoded string represents a value
             */
            XML.decodeValue = function (str) {
                var pairs = [
                    new std.Pair("&amp;", "&"),
                    new std.Pair("&lt;", "<"),
                    new std.Pair("&gt;", ">")
                ];
                return library.StringUtil.replaceAll.apply(library.StringUtil, [str].concat(pairs));
            };
            /**
             * <p> Encode a value. </p>
             *
             * <table>
             *	<tr>
             *		<th>Original</th>
             *		<th>Encoded</th>
             *	</tr>
             *	<tr>
             *		<td>\&</td>
             *		<td>\&amp;</td>
             *	</tr>
             *	<tr>
             *		<td>\<</td>
             *		<td>\&lt;</td>
             *	</tr>
             *	<tr>
             *		<td>\></td>
             *		<td>\&gt;</td>
             *	</tr>
             * </table>
             *
             * @return A encoded string represents a value
             */
            XML.encodeValue = function (str) {
                var pairs = [
                    new std.Pair("&", "&amp;"),
                    new std.Pair("<", "&lt;"),
                    new std.Pair(">", "&gt;")
                ];
                return library.StringUtil.replaceAll.apply(library.StringUtil, [str].concat(pairs));
            };
            /**
              * <p> Decode a property. </p>
              *
              * <table>
              *	<tr>
              *		<th>Encoded</th>
              *		<th>Decoded</th>
              *	</tr>
              *	<tr>
              *		<td>\&amp;</td>
              *		<td>\&</td>
              *	</tr>
              *	<tr>
              *		<td>\&lt;</td>
              *		<td>\<</td>
              *	</tr>
              *	<tr>
              *		<td>\&gt;</td>
              *		<td>\></td>
              *	</tr>
              *	<tr>
              *		<td>&quot;</td>
              *		<td>\"</td>
              *	</tr>
              *	<tr>
              *		<td>&apos;</td>
              *		<td>'</td>
              *	</tr>
              *	<tr>
              *		<td>&#x9;</td>
              *		<td>'</td>
              *	</tr>
              *	<tr>
              *		<td>&apos;</td>
              *		<td>\\t</td>
              *	</tr>
              *	<tr>
              *		<td>&#xA;</td>
              *		<td>\\n</td>
              *	</tr>
              *	<tr>
              *		<td>&#xD;</td>
              *		<td>\\r</td>
              *	</tr>
              * </table>
              *
              * @return A decoded string represents a property
              */
            XML.decodeProperty = function (str) {
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
             * <p> Decode a property. </p>
             *
             * <table>
             *	<tr>
             *		<th>Original</th>
             *		<th>Encoded</th>
             *	</tr>
             *	<tr>
             *		<td>\&</td>
             *		<td>\&amp;</td>
             *	</tr>
             *	<tr>
             *		<td>\<</td>
             *		<td>\&lt;</td>
             *	</tr>
             *	<tr>
             *		<td>\></td>
             *		<td>\&gt;</td>
             *	</tr>
             *	<tr>
             *		<td>\"</td>
             *		<td>&quot;</td>
             *	</tr>
             *	<tr>
             *		<td>'</td>
             *		<td>&apos;</td>
             *	</tr>
             *	<tr>
             *		<td>'</td>
             *		<td>&#x9;</td>
             *	</tr>
             *	<tr>
             *		<td>\\t</td>
             *		<td>&apos;</td>
             *	</tr>
             *	<tr>
             *		<td>\\n</td>
             *		<td>&#xA;</td>
             *	</tr>
             *	<tr>
             *		<td>\\r</td>
             *		<td>&#xD;</td>
             *	</tr>
             * </table>
             *
             * @return A encoded string represents a property
             */
            XML.encodeProperty = function (str) {
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
             * <p> Convert the XML to a string. </p>
             */
            XML.prototype.toString = function (level) {
                if (level === void 0) { level = 0; }
                var str = library.StringUtil.tab(level) + "<" + this.tag;
                var childrenString = "";
                //PROPERTIES
                for (var p_it = this.properties.begin(); p_it.equal_to(this.properties.end()) == false; p_it = p_it.next())
                    str += " " + p_it.first + "=\"" + XML.encodeProperty(String(p_it.second)) + "\"";
                if (this.size() == 0) {
                    if (this.value != "")
                        str += ">" + XML.encodeValue(String(this.value)) + "</" + this.tag + ">";
                    else
                        str += " />";
                }
                else {
                    str += ">\n";
                    for (var x_it = this.begin(); x_it.equal_to(this.end()) == false; x_it = x_it.next())
                        str += x_it.second.toString(level + 1);
                    str += library.StringUtil.tab(level) + "</" + this.tag + ">";
                }
                return str;
            };
            /**
             * <p> Convert the XML to HTML string. </p>
             */
            XML.prototype.toHTML = function (level) {
                if (level === void 0) { level = 0; }
                var str = library.StringUtil.htmlTab(level) + "&lt;" + this.tag;
                var childrenString = "";
                //PROPERTIES
                for (var p_it = this.properties.begin(); p_it.equal_to(this.properties.end()) == false; p_it = p_it.next())
                    str += " " + p_it.first + "=&quot;" + XML.encodeProperty(String(p_it.second)) + "&quot;";
                if (this.size() == 0) {
                    if (this.value != "")
                        str += "&gt;" + XML.encodeValue(String(this.value)) + "</" + this.tag + ">";
                    else
                        str += " /&gt;";
                }
                else {
                    str += "&gt;<br>\n";
                    for (var x_it = this.begin(); x_it.equal_to(this.end()) == false; x_it = x_it.next())
                        str += x_it.second.toHTML(level + 1);
                    str += library.StringUtil.htmlTab(level) + "&lt;/" + this.tag + "&gt;";
                }
                return str;
            };
            return XML;
        }(std.HashMap));
        library.XML = XML;
        /**
         * <p> List of XML(s) having same tag. </p>
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var XMLList = (function (_super) {
            __extends(XMLList, _super);
            function XMLList() {
                _super.apply(this, arguments);
            }
            // using super::constructor
            XMLList.prototype.getTag = function () {
                if (this.size() == 0)
                    return null;
                else
                    return this.at(0).getTag();
            };
            /**
             * <p> Convert XMLList to string. </p>
             *
             * @param level Level(depth) of the XMLList.
             */
            XMLList.prototype.toString = function (level) {
                if (level === void 0) { level = 0; }
                var str = "";
                for (var i = 0; i < this.size(); i++)
                    str += this.at(i).toString(level) + "\n";
                return str;
            };
            /**
             * <p> Convert XMLList to HTML string. </p>
             *
             * @param level Level(depth) of the XMLList.
             */
            XMLList.prototype.toHTML = function (level) {
                if (level === void 0) { level = 0; }
                var str = "";
                for (var i = 0; i < this.size(); i++)
                    str += this.at(i).toHTML(level) + "<br>\n";
                return str;
            };
            return XMLList;
        }(std.Vector));
        library.XMLList = XMLList;
    })(library = samchon.library || (samchon.library = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collection;
    (function (collection) {
        /**
         * A {@link Vector} who can detect element I/O events.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var ArrayCollection = (function (_super) {
            __extends(ArrayCollection, _super);
            function ArrayCollection() {
                _super.apply(this, arguments);
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                this.event_dispatcher_ = new samchon.library.EventDispatcher(this);
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
                    items[_i - 0] = arguments[_i];
                }
                var ret = _super.prototype.push.apply(this, items);
                this.notify_insert(this.end().advance(-items.length), this.end());
                return ret;
            };
            /**
             * @inheritdoc
             */
            ArrayCollection.prototype.push_back = function (val) {
                _super.prototype.push.call(this, val);
                this.notify_insert(this.end().prev(), this.end());
            };
            /**
             * @hidden
             */
            ArrayCollection.prototype.insert_by_repeating_val = function (position, n, val) {
                var ret = _super.prototype.insert_by_repeating_val.call(this, position, n, val);
                this.notify_insert(ret, ret.advance(n));
                return ret;
            };
            /**
             * @hidden
             */
            ArrayCollection.prototype.insert_by_range = function (position, begin, end) {
                var n = this.size();
                var ret = _super.prototype.insert_by_range.call(this, position, begin, end);
                n = this.size() - n;
                this.notify_insert(ret, ret.advance(n));
                return ret;
            };
            /* ---------------------------------------------------------
                ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            ArrayCollection.prototype.pop_back = function () {
                this.notify_erase(this.end().prev(), this.end());
                _super.prototype.pop_back.call(this);
            };
            /**
             * @hidden
             */
            ArrayCollection.prototype.erase_by_range = function (first, last) {
                this.notify_erase(first, last);
                return _super.prototype.erase_by_range.call(this, first, last);
            };
            /* ---------------------------------------------------------
                NOTIFIER
            --------------------------------------------------------- */
            /**
             * @hidden
             */
            ArrayCollection.prototype.notify_insert = function (first, last) {
                if (this.hasEventListener(collection.CollectionEvent.INSERT))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.INSERT, first, last));
            };
            /**
             * @hidden
             */
            ArrayCollection.prototype.notify_erase = function (first, last) {
                if (this.hasEventListener(collection.CollectionEvent.ERASE))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.ERASE, first, last));
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
            ArrayCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            ArrayCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            /* =========================================================
                ARRAY'S MEMBERS
                    - INSERT
                    - ERASE
            ============================================================
                INSERT
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            ArrayCollection.prototype.unshift = function () {
                var items = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    items[_i - 0] = arguments[_i];
                }
                var ret = _super.prototype.unshift.apply(this, items);
                this.notify_insert(this.begin(), this.begin().advance(items.length));
                return ret;
            };
            /* ---------------------------------------------------------
                ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            ArrayCollection.prototype.pop = function () {
                this.notify_erase(this.end().prev(), this.end());
                return _super.prototype.pop.call(this);
            };
            ArrayCollection.prototype.splice = function (start, deleteCount) {
                if (deleteCount === void 0) { deleteCount = this.size() - start; }
                var items = [];
                for (var _i = 2; _i < arguments.length; _i++) {
                    items[_i - 2] = arguments[_i];
                }
                // FILTER
                if (start + deleteCount > this.size())
                    deleteCount = this.size() - start;
                // NOTIFY ERASE
                var first = new std.VectorIterator(this, start);
                var last = first.advance(deleteCount);
                this.notify_erase(first, last);
                // CALL SUPER::ERASE
                return _super.prototype.splice.apply(this, [start, deleteCount].concat(items));
            };
            return ArrayCollection;
        }(std.Vector));
        collection.ArrayCollection = ArrayCollection;
    })(collection = samchon.collection || (samchon.collection = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * <p> An entity, a standard data class. </p>
         *
         * <p> Entity is a class for standardization of expression method using on network I/O by XML. If
         * Invoke is a standard message protocol of Samchon Framework which must be kept, Entity is a
         * recommended semi-protocol of message for expressing a data class. Following the semi-protocol
         * Entity is not imposed but encouraged. </p>
         *
         * <p> As we could get advantages from standardization of message for network I/O with Invoke,
         * we can get additional advantage from standardizing expression method of data class with Entity.
         * We do not need to know a part of network communication. Thus, with the Entity, we can only
         * concentrate on entity's own logics and relationships between another entities. Entity does not
         * need to how network communications are being done. </p>
         *
         * <p> I say repeatedly. Expression method of Entity is recommended, but not imposed. It's a semi
         * protocol for network I/O but not a essential protocol must be kept. The expression method of
         * Entity, using on network I/O, is expressed by XML string. </p>
         *
         * <p> If your own network system has a critical performance issue on communication data class,
         * it would be better to using binary communication (with ByteArray).
         * Don't worry about the problem! Invoke also provides methods for binary data (ByteArray). </p>
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
                // MEMBER VARIABLES; ATOMIC
                var propertyMap = xml.getPropertyMap();
                for (var v_it = propertyMap.begin(); v_it.equal_to(propertyMap.end()) != true; v_it = v_it.next())
                    if (this.hasOwnProperty(v_it.first) == true)
                        if (typeof this[v_it.first] == "number")
                            this[v_it.first] = parseFloat(v_it.second);
                        else if (typeof this[v_it.first] == "string")
                            this[v_it.first] = v_it.second;
            };
            /**
             * @inheritdoc
             */
            Entity.prototype.key = function () { return ""; };
            /**
             * @inheritdoc
             */
            Entity.prototype.toXML = function () {
                var xml = new samchon.library.XML();
                xml.setTag(this.TAG());
                // MEMBERS
                for (var key in this)
                    if (typeof key == "string" // NOT STRING, THEN IT MEANS CHILDREN (INT, INDEX)
                        && (typeof this[key] == "string" || typeof this[key] == "number")
                        && this.hasOwnProperty(key)) {
                        xml.setProperty(key, this[key]);
                    }
                return xml;
            };
            return Entity;
        }());
        protocol.Entity = Entity;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../../samchon/API.ts" />
/// <reference path="../../samchon/library/XML.ts" />
/// <reference path="../../samchon/collection/ArrayCollection.ts" />
/// <reference path="../../samchon/protocol/Entity.ts" />
if (typeof (exports) != "undefined") {
    exports.library = samchon.library;
    exports.collection = samchon.collection;
    exports.protocol = samchon.protocol;
}
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var library;
    (function (library) {
        /**
         * An event class.
         *
         * <ul>
         *  <li> Comments from - https://developer.mozilla.org/en-US/docs/Web/API/Event/ </li>
         * </ul>
         *
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
            Object.defineProperty(BasicEvent.prototype, "NONE", {
                /* -------------------------------------------------------------------
                    STATIC CONSTS
                ------------------------------------------------------------------- */
                ///**
                // * @inheritdoc
                // */
                //public static get NONE(): number { return 0; }
                get: function () { return 0; },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "CAPTURING_PHASE", {
                ///**
                // * @inheritdoc
                // */
                //public static get CAPTURING_PHASE(): number { return Event.CAPTURING_PHASE; }
                get: function () { return Event.CAPTURING_PHASE; },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "AT_TARGET", {
                ///**
                // * @inheritdoc
                // */
                //public static get AT_TARGET(): number { return Event.AT_TARGET; }
                get: function () { return Event.AT_TARGET; },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "BUBBLING_PHASE", {
                ///**
                // * @inheritdoc
                // */
                //public static get BUBBLING_PHASE(): number { return Event.BUBBLING_PHASE; }
                get: function () { return Event.BUBBLING_PHASE; },
                enumerable: true,
                configurable: true
            });
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
            BasicEvent.prototype.preventDefault = function () {
            };
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
            Object.defineProperty(BasicEvent.prototype, "isTrusted", {
                /**
                 * @inheritdoc
                 */
                get: function () {
                    return this.isTrusted;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(BasicEvent.prototype, "bubbles", {
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
        var ProgressEvent = (function (_super) {
            __extends(ProgressEvent, _super);
            function ProgressEvent(type, numerator, denominator) {
                _super.call(this, type);
                this.numerator_ = numerator;
                this.denominator_ = denominator;
            }
            Object.defineProperty(ProgressEvent, "PROGRESS", {
                get: function () { return "progress"; },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ProgressEvent.prototype, "numerator", {
                get: function () {
                    return this.numerator_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ProgressEvent.prototype, "denominator", {
                get: function () {
                    return this.denominator_;
                },
                enumerable: true,
                configurable: true
            });
            return ProgressEvent;
        }(library.BasicEvent));
        library.ProgressEvent = ProgressEvent;
    })(library = samchon.library || (samchon.library = {}));
})(samchon || (samchon = {}));
2;
/// <reference path="../API.ts" />
/// <reference path="../library/Event.ts" />
var samchon;
(function (samchon) {
    var collection;
    (function (collection) {
        /**
         *
         */
        var CollectionEvent = (function (_super) {
            __extends(CollectionEvent, _super);
            /**
             *
             *
             * @param type
             * @param first
             * @param last
             */
            function CollectionEvent(type, first, last) {
                _super.call(this, type);
                this.first_ = first;
                this.last_ = last;
            }
            Object.defineProperty(CollectionEvent, "INSERT", {
                get: function () {
                    return "insert";
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(CollectionEvent, "ERASE", {
                get: function () {
                    return "erase";
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(CollectionEvent.prototype, "container", {
                /**
                 *
                 */
                get: function () {
                    return this.target;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(CollectionEvent.prototype, "first", {
                /**
                 *
                 */
                get: function () {
                    return this.first_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(CollectionEvent.prototype, "last", {
                /**
                 *
                 */
                get: function () {
                    return this.last_;
                },
                enumerable: true,
                configurable: true
            });
            return CollectionEvent;
        }(samchon.library.BasicEvent));
        collection.CollectionEvent = CollectionEvent;
    })(collection = samchon.collection || (samchon.collection = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collection;
    (function (collection) {
        /**
         * A {@link Deque} who can detect element I/O events.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var DequeCollection = (function (_super) {
            __extends(DequeCollection, _super);
            function DequeCollection() {
                _super.apply(this, arguments);
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                this.event_dispatcher_ = new samchon.library.EventDispatcher(this);
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
                    items[_i - 0] = arguments[_i];
                }
                var ret = _super.prototype.push.apply(this, items);
                this.notify_insert(this.end().advance(-items.length), this.end());
                return ret;
            };
            /**
             * @inheritdoc
             */
            DequeCollection.prototype.push_back = function (val) {
                _super.prototype.push.call(this, val);
                this.notify_insert(this.end().prev(), this.end());
            };
            /**
             * @hidden
             */
            DequeCollection.prototype.insert_by_repeating_val = function (position, n, val) {
                var ret = _super.prototype.insert_by_repeating_val.call(this, position, n, val);
                this.notify_insert(ret, ret.advance(n));
                return ret;
            };
            /**
             * @hidden
             */
            DequeCollection.prototype.insert_by_range = function (position, begin, end) {
                var n = this.size();
                var ret = _super.prototype.insert_by_range.call(this, position, begin, end);
                n = this.size() - n;
                this.notify_insert(ret, ret.advance(n));
                return ret;
            };
            /* ---------------------------------------------------------
                ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            DequeCollection.prototype.pop_back = function () {
                this.notify_erase(this.end().prev(), this.end());
                _super.prototype.pop_back.call(this);
            };
            /**
             * @hidden
             */
            DequeCollection.prototype.erase_by_range = function (first, last) {
                this.notify_erase(first, last);
                return _super.prototype.erase_by_range.call(this, first, last);
            };
            /* ---------------------------------------------------------
                NOTIFIER
            --------------------------------------------------------- */
            /**
             * @hidden
             */
            DequeCollection.prototype.notify_insert = function (first, last) {
                if (this.hasEventListener(collection.CollectionEvent.INSERT))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.INSERT, first, last));
            };
            /**
             * @hidden
             */
            DequeCollection.prototype.notify_erase = function (first, last) {
                if (this.hasEventListener(collection.CollectionEvent.ERASE))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.ERASE, first, last));
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
        collection.DequeCollection = DequeCollection;
    })(collection = samchon.collection || (samchon.collection = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collection;
    (function (collection) {
        /**
         * A {@link HashMap} who can detect element I/O events.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var HashMapCollection = (function (_super) {
            __extends(HashMapCollection, _super);
            function HashMapCollection() {
                _super.apply(this, arguments);
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                this.event_dispatcher_ = new samchon.library.EventDispatcher(this);
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
            HashMapCollection.prototype.handle_insert = function (first, last) {
                _super.prototype.handle_insert.call(this, first, last);
                if (this.hasEventListener(collection.CollectionEvent.INSERT))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.INSERT, first, last));
            };
            /**
             * @inheritdoc
             */
            HashMapCollection.prototype.handle_erase = function (first, last) {
                _super.prototype.handle_erase.call(this, first, last);
                if (this.hasEventListener(collection.CollectionEvent.ERASE))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.ERASE, first, last));
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
        collection.HashMapCollection = HashMapCollection;
        /**
         * A {@link HashMultiMap} who can detect element I/O events.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var HashMultiMapCollection = (function (_super) {
            __extends(HashMultiMapCollection, _super);
            function HashMultiMapCollection() {
                _super.apply(this, arguments);
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                this.event_dispatcher_ = new samchon.library.EventDispatcher(this);
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
            HashMultiMapCollection.prototype.handle_insert = function (first, last) {
                _super.prototype.handle_insert.call(this, first, last);
                if (this.hasEventListener(collection.CollectionEvent.INSERT))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.INSERT, first, last));
            };
            /**
             * @inheritdoc
             */
            HashMultiMapCollection.prototype.handle_erase = function (first, last) {
                _super.prototype.handle_erase.call(this, first, last);
                if (this.hasEventListener(collection.CollectionEvent.ERASE))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.ERASE, first, last));
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
        collection.HashMultiMapCollection = HashMultiMapCollection;
    })(collection = samchon.collection || (samchon.collection = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collection;
    (function (collection) {
        /**
         * A {@link HashSet} who can detect element I/O events.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var HashSetCollection = (function (_super) {
            __extends(HashSetCollection, _super);
            function HashSetCollection() {
                _super.apply(this, arguments);
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                this.event_dispatcher_ = new samchon.library.EventDispatcher(this);
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
            HashSetCollection.prototype.handle_insert = function (first, last) {
                _super.prototype.handle_insert.call(this, first, last);
                if (this.hasEventListener(collection.CollectionEvent.INSERT))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.INSERT, first, last));
            };
            /**
             * @inheritdoc
             */
            HashSetCollection.prototype.handle_erase = function (first, last) {
                _super.prototype.handle_erase.call(this, first, last);
                if (this.hasEventListener(collection.CollectionEvent.ERASE))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.ERASE, first, last));
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
            HashSetCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            HashSetCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return HashSetCollection;
        }(std.TreeSet));
        collection.HashSetCollection = HashSetCollection;
        var HashMultiSetCollection = (function (_super) {
            __extends(HashMultiSetCollection, _super);
            function HashMultiSetCollection() {
                _super.apply(this, arguments);
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                this.event_dispatcher_ = new samchon.library.EventDispatcher(this);
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
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
            HashMultiSetCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            HashMultiSetCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return HashMultiSetCollection;
        }(std.TreeMultiSet));
        collection.HashMultiSetCollection = HashMultiSetCollection;
    })(collection = samchon.collection || (samchon.collection = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collection;
    (function (collection) {
        /**
         * A {@link List} who can detect element I/O events.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var ListCollection = (function (_super) {
            __extends(ListCollection, _super);
            function ListCollection() {
                _super.apply(this, arguments);
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                this.event_dispatcher_ = new samchon.library.EventDispatcher(this);
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
                    items[_i - 0] = arguments[_i];
                }
                var ret = _super.prototype.push.apply(this, items);
                this.notify_insert(this.end().advance(-items.length), this.end());
                return ret;
            };
            /**
             * @inheritdoc
             */
            ListCollection.prototype.push_front = function (val) {
                _super.prototype.push_front.call(this, val);
                this.notify_insert(this.begin(), this.begin().next());
            };
            /**
             * @inheritdoc
             */
            ListCollection.prototype.push_back = function (val) {
                _super.prototype.push_back.call(this, val);
                this.notify_insert(this.end().prev(), this.end());
            };
            /**
             * @hidden
             */
            ListCollection.prototype.insert_by_repeating_val = function (position, n, val) {
                var ret = _super.prototype.insert_by_repeating_val.call(this, position, n, val);
                this.notify_insert(ret, ret.advance(n));
                return ret;
            };
            /**
             * @hidden
             */
            ListCollection.prototype.insert_by_range = function (position, begin, end) {
                var n = this.size();
                var ret = _super.prototype.insert_by_range.call(this, position, begin, end);
                n = this.size() - n;
                this.notify_insert(ret, ret.advance(n));
                return ret;
            };
            /* ---------------------------------------------------------
                ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            ListCollection.prototype.pop_front = function () {
                var it = this.begin();
                _super.prototype.pop_front.call(this);
                this.notify_erase(it, it.next());
            };
            /**
             * @inheritdoc
             */
            ListCollection.prototype.pop_back = function () {
                var it = this.end().prev();
                _super.prototype.pop_back.call(this);
                this.notify_erase(it, this.end());
            };
            /**
             * @hidden
             */
            ListCollection.prototype.erase_by_range = function (first, last) {
                var ret = _super.prototype.erase_by_range.call(this, first, last);
                this.notify_erase(first, last);
                return ret;
            };
            /* ---------------------------------------------------------
                NOTIFIER
            --------------------------------------------------------- */
            /**
             * @hidden
             */
            ListCollection.prototype.notify_insert = function (first, last) {
                if (this.hasEventListener(collection.CollectionEvent.INSERT))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.INSERT, first, last));
            };
            /**
             * @hidden
             */
            ListCollection.prototype.notify_erase = function (first, last) {
                if (this.hasEventListener(collection.CollectionEvent.ERASE))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.ERASE, first, last));
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
        collection.ListCollection = ListCollection;
    })(collection = samchon.collection || (samchon.collection = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collection;
    (function (collection) {
        /**
         * A {@link TreeMap} who can detect element I/O events.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var TreeMapCollection = (function (_super) {
            __extends(TreeMapCollection, _super);
            function TreeMapCollection() {
                _super.apply(this, arguments);
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                this.event_dispatcher_ = new samchon.library.EventDispatcher(this);
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
            TreeMapCollection.prototype.handle_insert = function (first, last) {
                _super.prototype.handle_insert.call(this, first, last);
                if (this.hasEventListener(collection.CollectionEvent.INSERT))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.INSERT, first, last));
            };
            /**
             * @inheritdoc
             */
            TreeMapCollection.prototype.handle_erase = function (first, last) {
                _super.prototype.handle_erase.call(this, first, last);
                if (this.hasEventListener(collection.CollectionEvent.ERASE))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.ERASE, first, last));
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
            TreeMapCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            TreeMapCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return TreeMapCollection;
        }(std.HashMap));
        collection.TreeMapCollection = TreeMapCollection;
        /**
         * A {@link TreeMultiMap} who can detect element I/O events.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var TreeMultiMapCollection = (function (_super) {
            __extends(TreeMultiMapCollection, _super);
            function TreeMultiMapCollection() {
                _super.apply(this, arguments);
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                this.event_dispatcher_ = new samchon.library.EventDispatcher(this);
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
            TreeMultiMapCollection.prototype.handle_insert = function (first, last) {
                _super.prototype.handle_insert.call(this, first, last);
                if (this.hasEventListener(collection.CollectionEvent.INSERT))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.INSERT, first, last));
            };
            /**
             * @inheritdoc
             */
            TreeMultiMapCollection.prototype.handle_erase = function (first, last) {
                _super.prototype.handle_erase.call(this, first, last);
                if (this.hasEventListener(collection.CollectionEvent.ERASE))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.ERASE, first, last));
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
            TreeMultiMapCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            TreeMultiMapCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            return TreeMultiMapCollection;
        }(std.HashMap));
        collection.TreeMultiMapCollection = TreeMultiMapCollection;
    })(collection = samchon.collection || (samchon.collection = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var collection;
    (function (collection) {
        /**
         * A {@link TreeMap} who can detect element I/O events.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var TreeSetCollection = (function (_super) {
            __extends(TreeSetCollection, _super);
            function TreeSetCollection() {
                _super.apply(this, arguments);
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                this.event_dispatcher_ = new samchon.library.EventDispatcher(this);
            }
            /* ---------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------- */
            // using super::constructor
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
        collection.TreeSetCollection = TreeSetCollection;
        /**
         * A {@link TreeMultiSet} who can detect element I/O events.
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var TreeMultiSetCollection = (function (_super) {
            __extends(TreeMultiSetCollection, _super);
            function TreeMultiSetCollection() {
                _super.apply(this, arguments);
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                this.event_dispatcher_ = new samchon.library.EventDispatcher(this);
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
            TreeMultiSetCollection.prototype.handle_insert = function (first, last) {
                _super.prototype.handle_insert.call(this, first, last);
                if (this.hasEventListener(collection.CollectionEvent.INSERT))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.INSERT, first, last));
            };
            /**
             * @inheritdoc
             */
            TreeMultiSetCollection.prototype.handle_erase = function (first, last) {
                _super.prototype.handle_erase.call(this, first, last);
                if (this.hasEventListener(collection.CollectionEvent.ERASE))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.ERASE, first, last));
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
        collection.TreeMultiSetCollection = TreeMultiSetCollection;
    })(collection = samchon.collection || (samchon.collection = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="../library/XML.ts" />
var samchon;
(function (samchon) {
    var collection;
    (function (collection) {
        var XMLListCollection = (function (_super) {
            __extends(XMLListCollection, _super);
            function XMLListCollection() {
                _super.apply(this, arguments);
                /**
                 * A chain object taking responsibility of dispatching events.
                 */
                this.event_dispatcher_ = new samchon.library.EventDispatcher(this);
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
            XMLListCollection.prototype.push = function () {
                var items = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    items[_i - 0] = arguments[_i];
                }
                var ret = _super.prototype.push.apply(this, items);
                this.notify_insert(this.end().advance(-items.length), this.end());
                return ret;
            };
            /**
             * @inheritdoc
             */
            XMLListCollection.prototype.push_back = function (val) {
                _super.prototype.push.call(this, val);
                this.notify_insert(this.end().prev(), this.end());
            };
            /**
             * @hidden
             */
            XMLListCollection.prototype.insert_by_repeating_val = function (position, n, val) {
                var ret = _super.prototype.insert_by_repeating_val.call(this, position, n, val);
                this.notify_insert(ret, ret.advance(n));
                return ret;
            };
            /**
             * @hidden
             */
            XMLListCollection.prototype.insert_by_range = function (position, begin, end) {
                var n = this.size();
                var ret = _super.prototype.insert_by_range.call(this, position, begin, end);
                n = this.size() - n;
                this.notify_insert(ret, ret.advance(n));
                return ret;
            };
            /* ---------------------------------------------------------
                ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            XMLListCollection.prototype.pop_back = function () {
                this.notify_erase(this.end().prev(), this.end());
                _super.prototype.pop_back.call(this);
            };
            /**
             * @hidden
             */
            XMLListCollection.prototype.erase_by_range = function (first, last) {
                this.notify_erase(first, last);
                return _super.prototype.erase_by_range.call(this, first, last);
            };
            /* ---------------------------------------------------------
                NOTIFIER
            --------------------------------------------------------- */
            /**
             * @hidden
             */
            XMLListCollection.prototype.notify_insert = function (first, last) {
                if (this.hasEventListener(collection.CollectionEvent.INSERT))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.INSERT, first, last));
            };
            /**
             * @hidden
             */
            XMLListCollection.prototype.notify_erase = function (first, last) {
                if (this.hasEventListener(collection.CollectionEvent.ERASE))
                    this.dispatchEvent(new collection.CollectionEvent(collection.CollectionEvent.ERASE, first, last));
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
            XMLListCollection.prototype.hasEventListener = function (type) {
                return this.event_dispatcher_.hasEventListener(type);
            };
            /**
             * @inheritdoc
             */
            XMLListCollection.prototype.dispatchEvent = function (event) {
                return this.event_dispatcher_.dispatchEvent(event);
            };
            XMLListCollection.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.addEventListener(type, listener, thisArg);
            };
            XMLListCollection.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                this.event_dispatcher_.removeEventListener(type, listener, thisArg);
            };
            /* =========================================================
                ARRAY'S MEMBERS
                    - INSERT
                    - ERASE
            ============================================================
                INSERT
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            XMLListCollection.prototype.unshift = function () {
                var items = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    items[_i - 0] = arguments[_i];
                }
                var ret = _super.prototype.unshift.apply(this, items);
                this.notify_insert(this.begin(), this.begin().advance(items.length));
                return ret;
            };
            /* ---------------------------------------------------------
                ERASE
            --------------------------------------------------------- */
            /**
             * @inheritdoc
             */
            XMLListCollection.prototype.pop = function () {
                this.notify_erase(this.end().prev(), this.end());
                return _super.prototype.pop.call(this);
            };
            XMLListCollection.prototype.splice = function (start, deleteCount) {
                if (deleteCount === void 0) { deleteCount = this.size() - start; }
                var items = [];
                for (var _i = 2; _i < arguments.length; _i++) {
                    items[_i - 2] = arguments[_i];
                }
                // FILTER
                if (start + deleteCount > this.size())
                    deleteCount = this.size() - start;
                // NOTIFY ERASE
                var first = new std.VectorIterator(this, start);
                var last = first.advance(deleteCount);
                this.notify_erase(first, last);
                // CALL SUPER::ERASE
                return _super.prototype.splice.apply(this, [start, deleteCount].concat(items));
            };
            return XMLListCollection;
        }(samchon.library.XMLList));
        collection.XMLListCollection = XMLListCollection;
    })(collection = samchon.collection || (samchon.collection = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var library;
    (function (library) {
        /**
         * <p> Case generator. </p>
         *
         * <p> CaseGenerator is an abstract case generator using like a matrix. </p>
         * <ul>
         *  <li> nTTr(n^r) -> CombinedPermutationGenerator </li>
         *  <li> nPr -> PermutationGenerator </li>
         *  <li> n! -> FactorialGenerator </li>
         * </ul>
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var CaseGenerator = (function () {
            /* ---------------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------------- */
            /**
             * <p> Construct from size of N and R. </p>
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
             * <p> Get size of all cases. </p>
             *
             * @return Get a number of the all cases.
             */
            CaseGenerator.prototype.size = function () {
                return this.size_;
            };
            /**
             * <p> Get size of the N. </p>
             */
            CaseGenerator.prototype.n = function () {
                return this.n_;
            };
            /**
             * <p> Get size of the R. </p>
             */
            CaseGenerator.prototype.r = function () {
                return this.r_;
            };
            return CaseGenerator;
        }());
        library.CaseGenerator = CaseGenerator;
        /**
         * <p> A combined-permutation case generator. </p>
         * <p> <sub>n</sub>TT<sub>r</sub> </p>
         *
         * @inheritdoc
         * @author Jeongho Nam <http://samchon.org>
         */
        var CombinedPermutationGenerator = (function (_super) {
            __extends(CombinedPermutationGenerator, _super);
            /* ---------------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------------- */
            /**
             * <p> Construct from size of N and R. </p>
             *
             * @param n Size of candidates.
             * @param r Size of elements of each case.
             */
            function CombinedPermutationGenerator(n, r) {
                _super.call(this, n, r);
                this.size_ = Math.pow(n, r);
                this.divider_array = new Array();
                for (var i = 0; i < r; i++) {
                    var x = r - (i + 1);
                    var val = Math.pow(n, x);
                    this.divider_array.push(val);
                }
            }
            CombinedPermutationGenerator.prototype.at = function (index) {
                var row = new Array();
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
         * <p> A permutation case generator. </p>
         * <p> nPr </p>
         *
         * @author Jeongho Nam <http://samchon.org>
         * @inheritdoc
         */
        var PermuationGenerator = (function (_super) {
            __extends(PermuationGenerator, _super);
            /* ---------------------------------------------------------------
                CONSTRUCTORS
            --------------------------------------------------------------- */
            /**
             * <p> Construct from size of N and R. </p>
             *
             * @param n Size of candidates.
             * @param r Size of elements of each case.
             */
            function PermuationGenerator(n, r) {
                _super.call(this, n, r);
                this.size_ = n;
                for (var i = n - 1; i > n - r; i--)
                    this.size_ *= i;
            }
            /**
             * @inheritdoc
             */
            PermuationGenerator.prototype.at = function (index) {
                var atoms = new Array();
                for (var i = 0; i < this.n_; i++)
                    atoms.push(i);
                var row = new Array();
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
        var FactorialGenerator = (function (_super) {
            __extends(FactorialGenerator, _super);
            /**
             * Construct from factorial size N.
             *
             * @param n Factoria size N.
             */
            function FactorialGenerator(n) {
                _super.call(this, n, n);
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
         * <p> Registers an event listener object with an EventDispatcher object so that the listener
         * receives notification of an event. You can register event listeners on all nodes in the display
         * list for a specific type of event, phase, and priority. </p>
         *
         * <p> After you successfully register an event listener, you cannot change its priority through
         * additional calls to addEventListener(). To change a listener's priority, you must first call
         * removeListener(). Then you can register the listener again with the new priority level. </p>
         *
         * Keep in mind that after the listener is registered, subsequent calls to <code>addEventListener()</code>
         * with a different type or useCapture value result in the creation of a separate listener registration.
         * For example, if you first register a listener with useCapture set to true, it listens only during the
         * capture phase. If you call addEventListener() again using the same listener object, but with
         * useCapture set to false, you have two separate listeners: one that listens during the capture
         * phase and another that listens during the target and bubbling phases.
         *
         * <p> You cannot register an event listener for only the target phase or the bubbling phase. Those
         * phases are coupled during registration because bubbling applies only to the ancestors of the
         * target node. </p>
         *
         * <p> If you no longer need an event listener, remove it by calling <code>removeEventListener()</code>,
         * or memory problems could result. Event listeners are not automatically removed from memory
         * because the garbage collector does not remove the listener as long as the dispatching object
         * exists (unless the useWeakReference parameter is set to true). </p>
         *
         * <p> Copying an EventDispatcher instance does not copy the event listeners attached to it. (If your
         * newly created node needs an event listener, you must attach the listener after creating the
         * node.) However, if you move an EventDispatcher instance, the event listeners attached to it move
         * along with it. </p>
         *
         * <p> If the event listener is being registered on a node while an event is being processed on
         * this node, the event listener is not triggered during the current phase but can be triggered
         * during a later phase in the event flow, such as the bubbling phase. </p>
         *
         * <p> If an event listener is removed from a node while an event is being processed on the node, it is
         * still triggered by the current actions. After it is removed, the event listener is never invoked
         * again (unless registered again for future processing). </p>
         *
         * <ul>
         *  <li> Made by AS3 - http://help.adobe.com/en_US/FlashPlatform/reference/actionscript/3/flash/events/EventDispatcher.html
         * </ul>
         *
         * @author Migrated by Jeongho Nam <http://samchon.org>
         */
        var EventDispatcher = (function () {
            function EventDispatcher(target) {
                if (target === void 0) { target = null; }
                if (target == null)
                    this.target = this;
                else
                    this.target = target;
                this.listeners = new std.HashMap();
            }
            /**
             * @inheritdoc
             */
            EventDispatcher.prototype.hasEventListener = function (type) {
                type = type.toLowerCase();
                return this.listeners.has(type);
            };
            /**
             * @inheritdoc
             */
            EventDispatcher.prototype.dispatchEvent = function (event) {
                if (event instanceof library.BasicEvent)
                    event["target_"] = this.target;
                else
                    event.target = this.target;
                if (this.listeners.has(event.type) == false)
                    return false;
                var listenerSet = this.listeners.get(event.type);
                for (var it = listenerSet.begin(); it.equal_to(listenerSet.end()) == false; it = it.next())
                    it.value.first.apply(it.value.second, [event]);
                //it.value.apply(event);
                return true;
            };
            EventDispatcher.prototype.addEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                type = type.toLowerCase();
                var listenerSet;
                if (this.listeners.has(type) == false) {
                    listenerSet = new std.HashSet();
                    this.listeners.set(type, listenerSet);
                }
                else
                    listenerSet = this.listeners.get(type);
                listenerSet.insert(new std.Pair(listener, thisArg));
            };
            EventDispatcher.prototype.removeEventListener = function (type, listener, thisArg) {
                if (thisArg === void 0) { thisArg = null; }
                type = type.toLowerCase();
                if (this.listeners.has(type) == false)
                    return;
                var listenerSet = this.listeners.get(type);
                var bind = new std.Pair(listener, thisArg);
                if (listenerSet.has(bind) == false)
                    return;
                listenerSet.erase(bind);
                if (listenerSet.empty() == true)
                    this.listeners.erase(type);
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
         * <p> The {@link FileReference} class provides a means to load and save files in browser level. </p>
         *
         * <p> The {@link FileReference} class provides a means to {@link load} and {@link save} files in browser level. A
         * browser-system dialog box prompts the user to select a file to {@link load} or a location for {@link svae}. Each
         * {@link FileReference} object refers to a single file on the user's disk and has properties that contain
         * information about the file's size, type, name, creation date, modification date, and creator type (Macintosh only).
         * </p>
         *
         * <p> FileReference instances are created in the following ways: </p>
         * <ul>
         *	<li>
         *		When you use the new operator with the {@link FileReference} constructor:
         *		<code>var myFileReference = new FileReference();</code>
         *	</li>
         *	<li>
         *		When you call the {@link FileReferenceList.browse} method, which creates an array of {@link FileReference}
         *		objects.
         *	</li>
         * </ul>
         *
         * <p> During a load operation, all the properties of a {@link FileReference} object are populated by calls to the
         * {@link FileReference.browse} or {@link FileReferenceList.browse} methods. During a save operation, the name
         * property is populated when the select event is dispatched; all other properties are populated when the complete
         * event is dispatched. </p>
         *
         * <p> The {@link browse browse()} method opens an browser-system dialog box that prompts the user to select a file
         * for {@link load}. The {@link FileReference.browse} method lets the user select a single file; the
         * {@link FileReferenceList.browse} method lets the user select multiple files. After a successful call to the
         * {@link browse browse()} method, call the {@link FileReference.load} method to load one file at a time. The
         * {@link FileReference.save} method prompts the user for a location to save the file and initiates downloading from
         * a binary or string data. </p>
         *
         * <p> The {@link FileReference} and {@link FileReferenceList} classes do not let you set the default file location
         * for the dialog box that the {@link browse} or {@link save} methods generate. The default location shown in the
         * dialog box is the most recently browsed folder, if that location can be determined, or the desktop. The classes do
         * not allow you to read from or write to the transferred file. They do not allow the browser that initiated the
         * {@link load} or {@link save} to access the loaded or saved file or the file's location on the user's disk. </p>
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
                _super.call(this);
                this.file_ = null;
                this.data_ = null;
            }
            Object.defineProperty(FileReference.prototype, "data", {
                /* ---------------------------------------------------------
                    ACCESSORS
                --------------------------------------------------------- */
                /**
                 * <p> The data from the loaded file after a successful call to the {@link load load()} method. </p>
                 *
                 * <p> If the {@link FileReference} object was not populated (by a valid call to {@link FileReference.browse}),
                 * an {@link LogicError exception} will be thrown when you try to get the value of this property. </p>
                 *
                 * <p> All the properties of a {@link FileReference} object are populated by calling the {@link browse browse()}.
                 * </p>
                 */
                get: function () {
                    return this.data_;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(FileReference.prototype, "name", {
                /**
                 * <p> The name of the file on the local disk. </p>
                 *
                 * <p> If the {@link FileReference} object was not populated (by a valid call to {@link FileReference.browse}),
                 * an {@link LogicError exception} will be thrown when you try to get the value of this property. </p>
                 *
                 * <p> All the properties of a {@link FileReference} object are populated by calling the {@link browse browse()}.
                 * </p>
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
                 * <p> The filename extension. </p>
                 *
                 * <p> A file's extension is the part of the name following (and not including) the final dot (&quot;.&quot;). If
                 * there is no dot in the filename, the extension is <code>null</code>. </p>
                 *
                 * <p> If the {@link FileReference} object was not populated (by a valid call to {@link FileReference.browse}),
                 * an {@link LogicError exception} will be thrown when you try to get the value of this property. </p>
                 *
                 * <p> All the properties of a {@link FileReference} object are populated by calling the {@link browse browse()}.
                 * </p>
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
                 * <p> The file type, metadata of the {@link extension}. </p>
                 *
                 * <p> If the {@link FileReference} object was not populated (by a valid call to {@link FileReference.browse}),
                 * an {@link LogicError exception} will be thrown when you try to get the value of this property. </p>
                 *
                 * <p> All the properties of a {@link FileReference} object are populated by calling the {@link browse browse()}.
                 * </p>
                 */
                get: function () {
                    return this.file_.type;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(FileReference.prototype, "size", {
                /**
                 * <p> The size of the file on the local disk in bytes. </p>
                 *
                 * <p> If the {@link FileReference} object was not populated (by a valid call to {@link FileReference.browse}),
                 * an {@link LogicError exception} will be thrown when you try to get the value of this property. </p>
                 *
                 * <p> All the properties of a {@link FileReference} object are populated by calling the {@link browse browse()}.
                 * </p>
                 */
                get: function () {
                    return this.file_.size;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(FileReference.prototype, "modificationDate", {
                /**
                 * <p> The date that the file on the local disk was last modified. </p>
                 *
                 * <p> If the {@link FileReference} object was not populated (by a valid call to {@link FileReference.browse}),
                 * an {@link LogicError exception} will be thrown when you try to get the value of this property. </p>
                 *
                 * <p> All the properties of a {@link FileReference} object are populated by calling the {@link browse browse()}.
                 * </p>
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
             * <p> Displays a file-browsing dialog box that lets the user select a file to upload. The dialog box is native
             * to the user's browser system. The user can select a file on the local computer or from other systems, for
             * example, through a UNC path on Windows. </p>
             *
             * <p> When you call this method and the user successfully selects a file, the properties of this
             * {@link FileReference} object are populated with the properties of that file. Each subsequent time that the
             * {@link FileReference.browse} method is called, the {@link FileReference} object's properties are reset to
             * the file that the user selects in the dialog box. Only one {@link browse browse()} can be performed at a time
             * (because only one dialog box can be invoked at a time). </p>
             *
             * <p> Using the <i>typeFilter parameter</i>, you can determine which files the dialog box displays. </p>
             *
             * @param typeFilter An array of filter strings used to filter the files that are displayed in the dialog box.
             *					 If you omit this parameter, all files are displayed.
             */
            FileReference.prototype.browse = function () {
                var typeFilter = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    typeFilter[_i - 0] = arguments[_i];
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
             * <p> Starts the load of a local file selected by a user. </p>
             *
             * <p> You must call the {@link FileReference.browse} or {@link FileReferenceList.browse} method before you call
             * the {@link load load()} method. </p>
             *
             * <p> Listeners receive events to indicate the progress, success, or failure of the load. Although you can use
             * the {@link FileReferenceList} object to let users select multiple files to load, you must {@link load} the
             * {@link FileReferenceList files} one by one. To {@link load} the files one by one, iterate through the
             * {@link FileReferenceList.fileList} array of {@link FileReference} objects. </p>
             *
             * <p> If the file finishes loading successfully, its contents are stored in the {@link data} property. </p>
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
             * <p> Save a file to local filesystem. </p>
             *
             * <p> {@link FileReference.save} implemented the save function by downloading a file from a hidden anchor tag.
             * However, the plan, future's {@link FileReference} will follow such rule: </p>
             *
             * <p> Opens a dialog box that lets the user save a file to the local filesystem. </p>
             *
             * <p> The {@link save save()} method first opens an browser-system dialog box that asks the user to enter a
             * filename and select a location on the local computer to save the file. When the user selects a location and
             * confirms the save operation (for example, by clicking Save), the save process begins. Listeners receive events
             * to indicate the progress, success, or failure of the save operation. To ascertain the status of the dialog box
             * and the save operation after calling {@link save save()}, your code must listen for events such as cancel,
             * open, progress, and complete. </p>
             *
             * <p> When the file is saved successfully, the properties of the {@link FileReference} object are populated with
             * the properties of the local file. The complete event is dispatched if the save is successful. </p>
             *
             * <p> Only one {@link browse browse()} or {@link save()} session can be performed at a time (because only one
             * dialog box can be invoked at a time). </p>
             *
             * @param data The data to be saved. The data can be in one of several formats, and will be treated appropriately.
             * @param fileName File name to be saved.
             */
            FileReference.prototype.save = function (data, fileName) {
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
         * <p> The {@link FileReferenceList} class provides a means to let users select one or more files for
         * {@link FileReference.load loading}. A {@link FileReferenceList} object represents a group of one or more local
         * files on the user's disk as an array of {@link FileReference} objects. For detailed information and important
         * considerations about {@link FileReference} objects and the FileReference class, which you use with
         * {@link FileReferenceList}, see the {@link FileReference} class. </p>
         *
         * <p> To work with the {@link FileReferenceList} class: </p>
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
         * <p> The {@link FileReferenceList} class includes a {@link browse browse()} method and a {@link fileList} property
         * for working with multiple files. </p>
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
                _super.call(this);
                this.file_list = new std.Vector();
            }
            Object.defineProperty(FileReferenceList.prototype, "fileList", {
                /**
                 * <p> An array of {@link FileReference} objects. </p>
                 *
                 * <p> When the {@link FileReferenceList.browse} method is called and the user has selected one or more files
                 * from the dialog box that the {@link browse browse()} method opens, this property is populated with an array of
                 * {@link FileReference} objects, each of which represents the files the user selected. </p>
                 *
                 * <p> The {@link fileList} property is populated anew each time {@link browse browse()} is called on that
                 * {@link FileReferenceList} object. </p>
                 */
                get: function () {
                    return this.file_list;
                },
                enumerable: true,
                configurable: true
            });
            /**
             * <p> Displays a file-browsing dialog box that lets the user select one or more local files to upload. The
             * dialog box is native to the user's browser system.  </p>
             *
             * <p> When you call this method and the user successfully selects files, the {@link fileList} property of this
             * {@link FileReferenceList} object is populated with an array of {@link FileReference} objects, one for each
             * file that the user selects. Each subsequent time that the {@link FileReferenceList.browse} method is called,
             * the {@link FileReferenceList.fileList} property is reset to the file(s) that the user selects in the dialog
             * box. </p>
             *
             * <p> Using the <i>typeFilter</i> parameter, you can determine which files the dialog box displays. </p>
             *
             * <p> Only one {@link FileReference.browse}, {@link FileReference.load}, or {@link FileReferenceList.browse}
             * session can be performed at a time on a {@link FileReferenceList} object (because only one dialog box can be
             * opened at a time). </p>
             *
             * @param typeFilter An array of filter strings used to filter the files that are displayed in the dialog box.
             *					 If you omit this parameter, all files are displayed.
             */
            FileReferenceList.prototype.browse = function () {
                var typeFilter = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    typeFilter[_i - 0] = arguments[_i];
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
         * <p> A utility class supporting static methods of string. </p>
         *
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
             * <p> Generate a substring. </p>
             *
             * <p> Extracts a substring consisting of the characters from specified start to end.
             * It's same with str.substring( ? = (str.find(start) + start.size()), str.find(end, ?) ) </p>
             *
             * <code>
             let str = between("ABCD[EFGH]IJK", "[", "]");
             console.log(str); // PRINTS "EFGH"
             * </code>
             *
             * <ul>
             *	<li> If start is not specified, extracts from begin of the string to end. </li>
             *	<li> If end is not specified, extracts from start to end of the string. </li>
             *	<li> If start and end are all omitted, returns str, itself. </li>
             * </ul>
             *
             * @param str Target string to be applied between
             * @param start A string for separating substring at the front
             * @param end A string for separating substring at the end
             *
             * @return substring by specified terms
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
             * <p> Fetch substrings. </p>
             *
             * <p> Splits a string into an array of substrings dividing by specified delimeters of start and end.
             * It's the array of substrings adjusted the between. </p>
             *
             * <ul>
             *	<li> If startStr is omitted, it's same with the split by endStr not having last item. </li>
             *	<li> If endStr is omitted, it's same with the split by startStr not having first item. </li>
             *	<li> If startStr and endStar are all omitted, returns <i>str</i>. </li>
             * </ul>
             *
             * @param str Target string to split by between
             * @param start A string for separating substring at the front.
             *				If omitted, it's same with split(end) not having last item
             * @param end A string for separating substring at the end.
             *			  If omitted, it's same with split(start) not having first item
             * @return An array of substrings
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
             * @param args Additional parameters that can be substituted in the <i>format</i> parameter at each
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
                    var pairs = args[0];
                    if (pairs.length == 0)
                        return str;
                    for (var i = 0; i < pairs.length; i++)
                        str = str.split(pairs[i].first).join(pairs[i].second);
                    return str;
                }
            };
            /* ------------------------------------------------------------------
                MISCELLANEOUS
            ------------------------------------------------------------------ */
            /**
             * <p> Get a tabbed string by specified size. </p>
             */
            StringUtil.tab = function (size) {
                var str = "";
                for (var i = 0; i < size; i++)
                    str += "\t";
                return str;
            };
            /**
             * <p> Get a tabbed HTLM string by specified size. </p>
             */
            StringUtil.htmlTab = function (size) {
                var str = "";
                for (var i = 0; i < size; i++)
                    str += "&nbsp;&nbsp;&nbsp;&nbsp;";
                return str;
            };
            /**
             * Replace all HTML spaces to a literal space.
             *
             * @param str Target string to replace.
             */
            StringUtil.removeHTMLSpaces = function (str) {
                return StringUtil.replaceAll(str, new std.Pair("&nbsp;", " "), new std.Pair("\t", " "), new std.Pair("  ", " "));
            };
            /* ------------------------------------------------------------------
                TRIM
            ------------------------------------------------------------------ */
            /**
             * An array containing whitespaces.
             */
            StringUtil.SPACE_ARRAY = [" ", "\t", "\r", "\n"];
            return StringUtil;
        }());
        library.StringUtil = StringUtil;
    })(library = samchon.library || (samchon.library = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * @inheritdoc
         */
        var EntityArray = (function (_super) {
            __extends(EntityArray, _super);
            function EntityArray() {
                _super.apply(this, arguments);
            }
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            // using super::super;
            /**
             * @inheritdoc
             */
            EntityArray.prototype.construct = function (xml) {
                this.clear();
                // MEMBER VARIABLES; ATOMIC
                var propertyMap = xml.getPropertyMap();
                for (var v_it = propertyMap.begin(); v_it.equal_to(propertyMap.end()) != true; v_it = v_it.next())
                    if (typeof this[v_it.first] == "number" && v_it.first != "length")
                        this[v_it.first] = parseFloat(v_it.second);
                    else if (typeof this[v_it.first] == "string")
                        this[v_it.first] = v_it.second;
                //CHILDREN
                if (xml.has(this.CHILD_TAG()) == false)
                    return;
                var xmlList = xml.get(this.CHILD_TAG());
                for (var i = 0; i < xmlList.size(); i++) {
                    var child = this.createChild(xmlList.at(i));
                    if (child == null)
                        continue;
                    child.construct(xmlList.at(i));
                    this.push(child);
                }
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
            EntityArray.prototype.find = function (key) {
                return std.find_if(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityArray.prototype.has = function (key) {
                return std.any_of(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityArray.prototype.count = function (key) {
                return std.count_if(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityArray.prototype.get = function (key) {
                var it = this.find(key);
                if (it.equal_to(this.end()))
                    throw new std.OutOfRange("out of range");
                return it.value;
            };
            /**
             * @inheritdoc
             */
            EntityArray.prototype.toXML = function () {
                var xml = new samchon.library.XML();
                xml.setTag(this.TAG());
                // MEMBERS
                for (var key in this)
                    if (typeof key == "string" && key != "length" // LENGTH: MEMBER OF AN ARRAY
                        && (typeof this[key] == "string" || typeof this[key] == "number")
                        && this.hasOwnProperty(key)) {
                        // ATOMIC
                        xml.setProperty(key, this[key] + "");
                    }
                // CHILDREN
                for (var it = this.begin(); !it.equal_to(this.end()); it = it.next())
                    xml.push(it.value.toXML());
                return xml;
            };
            return EntityArray;
        }(std.Vector));
        protocol.EntityArray = EntityArray;
        /**
         * @inheritdoc
         */
        var EntityList = (function (_super) {
            __extends(EntityList, _super);
            function EntityList() {
                _super.apply(this, arguments);
            }
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            // using super::super;
            /**
             * @inheritdoc
             */
            EntityList.prototype.construct = function (xml) {
                this.clear();
                // MEMBER VARIABLES; ATOMIC
                var propertyMap = xml.getPropertyMap();
                for (var v_it = propertyMap.begin(); v_it.equal_to(propertyMap.end()) != true; v_it = v_it.next())
                    if (typeof this[v_it.first] == "number" && v_it.first != "length")
                        this[v_it.first] = parseFloat(v_it.second);
                    else if (typeof this[v_it.first] == "string")
                        this[v_it.first] = v_it.second;
                //CHILDREN
                if (xml.has(this.CHILD_TAG()) == false)
                    return;
                var xmlList = xml.get(this.CHILD_TAG());
                for (var i = 0; i < xmlList.size(); i++) {
                    var child = this.createChild(xmlList.at(i));
                    if (child == null)
                        continue;
                    child.construct(xmlList.at(i));
                    this.push(child);
                }
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
            EntityList.prototype.find = function (key) {
                return std.find_if(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityList.prototype.has = function (key) {
                return std.any_of(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityList.prototype.count = function (key) {
                return std.count_if(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityList.prototype.get = function (key) {
                var it = this.find(key);
                if (it.equal_to(this.end()))
                    throw new std.OutOfRange("out of range");
                return it.value;
            };
            /**
             * @inheritdoc
             */
            EntityList.prototype.toXML = function () {
                var xml = new samchon.library.XML();
                xml.setTag(this.TAG());
                // MEMBERS
                for (var key in this)
                    if (typeof key == "string"
                        && (typeof this[key] == "string" || typeof this[key] == "number")
                        && this.hasOwnProperty(key)) {
                        // ATOMIC
                        xml.setProperty(key, this[key] + "");
                    }
                // CHILDREN
                for (var it = this.begin(); !it.equal_to(this.end()); it = it.next())
                    xml.push(it.value.toXML());
                return xml;
            };
            return EntityList;
        }(std.List));
        protocol.EntityList = EntityList;
        /**
         * @inheritdoc
         */
        var EntityDeque = (function (_super) {
            __extends(EntityDeque, _super);
            function EntityDeque() {
                _super.apply(this, arguments);
            }
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            // using super::super;
            /**
             * @inheritdoc
             */
            EntityDeque.prototype.construct = function (xml) {
                this.clear();
                // MEMBER VARIABLES; ATOMIC
                var propertyMap = xml.getPropertyMap();
                for (var v_it = propertyMap.begin(); v_it.equal_to(propertyMap.end()) != true; v_it = v_it.next())
                    if (typeof this[v_it.first] == "number" && v_it.first != "length")
                        this[v_it.first] = parseFloat(v_it.second);
                    else if (typeof this[v_it.first] == "string")
                        this[v_it.first] = v_it.second;
                //CHILDREN
                if (xml.has(this.CHILD_TAG()) == false)
                    return;
                var xmlList = xml.get(this.CHILD_TAG());
                for (var i = 0; i < xmlList.size(); i++) {
                    var child = this.createChild(xmlList.at(i));
                    if (child == null)
                        continue;
                    child.construct(xmlList.at(i));
                    this.push(child);
                }
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
            EntityDeque.prototype.find = function (key) {
                return std.find_if(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityDeque.prototype.has = function (key) {
                return std.any_of(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityDeque.prototype.count = function (key) {
                return std.count_if(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityDeque.prototype.get = function (key) {
                var it = this.find(key);
                if (it.equal_to(this.end()))
                    throw new std.OutOfRange("out of range");
                return it.value;
            };
            /**
             * @inheritdoc
             */
            EntityDeque.prototype.toXML = function () {
                var xml = new samchon.library.XML();
                xml.setTag(this.TAG());
                // MEMBERS
                for (var key in this)
                    if (typeof key == "string"
                        && (typeof this[key] == "string" || typeof this[key] == "number")
                        && this.hasOwnProperty(key)) {
                        // ATOMIC
                        xml.setProperty(key, this[key] + "");
                    }
                // CHILDREN
                for (var it = this.begin(); !it.equal_to(this.end()); it = it.next())
                    xml.push(it.value.toXML());
                return xml;
            };
            return EntityDeque;
        }(std.Deque));
        protocol.EntityDeque = EntityDeque;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="../collection/ArrayCollection.ts" />
/// <reference path="../collection/ListCollection.ts" />
/// <reference path="../collection/DequeCollection.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        ;
        /**
         * @inheritdoc
         */
        var EntityArrayCollection = (function (_super) {
            __extends(EntityArrayCollection, _super);
            function EntityArrayCollection() {
                _super.apply(this, arguments);
            }
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            // using super::super;
            /**
             * @inheritdoc
             */
            EntityArrayCollection.prototype.construct = function (xml) {
                this.clear();
                // MEMBER VARIABLES; ATOMIC
                var propertyMap = xml.getPropertyMap();
                for (var v_it = propertyMap.begin(); v_it.equal_to(propertyMap.end()) != true; v_it = v_it.next())
                    if (typeof this[v_it.first] == "number" && v_it.first != "length")
                        this[v_it.first] = parseFloat(v_it.second);
                    else if (typeof this[v_it.first] == "string")
                        this[v_it.first] = v_it.second;
                //CHILDREN
                if (xml.has(this.CHILD_TAG()) == false)
                    return;
                var xmlList = xml.get(this.CHILD_TAG());
                for (var i = 0; i < xmlList.size(); i++) {
                    var child = this.createChild(xmlList.at(i));
                    if (child == null)
                        continue;
                    child.construct(xmlList.at(i));
                    this.push(child);
                }
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
            EntityArrayCollection.prototype.find = function (key) {
                return std.find_if(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityArrayCollection.prototype.has = function (key) {
                return std.any_of(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityArrayCollection.prototype.count = function (key) {
                return std.count_if(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityArrayCollection.prototype.get = function (key) {
                var it = this.find(key);
                if (it.equal_to(this.end()))
                    throw new std.OutOfRange("out of range");
                return it.value;
            };
            /**
             * @inheritdoc
             */
            EntityArrayCollection.prototype.toXML = function () {
                var xml = new samchon.library.XML();
                xml.setTag(this.TAG());
                // MEMBERS
                for (var key in this)
                    if (typeof key == "string"
                        && (typeof this[key] == "string" || typeof this[key] == "number")
                        && this.hasOwnProperty(key)) {
                        // ATOMIC
                        xml.setProperty(key, this[key] + "");
                    }
                // CHILDREN
                for (var it = this.begin(); !it.equal_to(this.end()); it = it.next())
                    xml.push(it.value.toXML());
                return xml;
            };
            return EntityArrayCollection;
        }(samchon.collection.ArrayCollection));
        protocol.EntityArrayCollection = EntityArrayCollection;
        /**
         * @inheritdoc
         */
        var EntityListCollection = (function (_super) {
            __extends(EntityListCollection, _super);
            function EntityListCollection() {
                _super.apply(this, arguments);
            }
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            // using super::super;
            /**
             * @inheritdoc
             */
            EntityListCollection.prototype.construct = function (xml) {
                this.clear();
                // MEMBER VARIABLES; ATOMIC
                var propertyMap = xml.getPropertyMap();
                for (var v_it = propertyMap.begin(); v_it.equal_to(propertyMap.end()) != true; v_it = v_it.next())
                    if (typeof this[v_it.first] == "number" && v_it.first != "length")
                        this[v_it.first] = parseFloat(v_it.second);
                    else if (typeof this[v_it.first] == "string")
                        this[v_it.first] = v_it.second;
                //CHILDREN
                if (xml.has(this.CHILD_TAG()) == false)
                    return;
                var xmlList = xml.get(this.CHILD_TAG());
                for (var i = 0; i < xmlList.size(); i++) {
                    var child = this.createChild(xmlList.at(i));
                    if (child == null)
                        continue;
                    child.construct(xmlList.at(i));
                    this.push(child);
                }
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
            EntityListCollection.prototype.find = function (key) {
                return std.find_if(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityListCollection.prototype.has = function (key) {
                return std.any_of(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityListCollection.prototype.count = function (key) {
                return std.count_if(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityListCollection.prototype.get = function (key) {
                var it = this.find(key);
                if (it.equal_to(this.end()))
                    throw new std.OutOfRange("out of range");
                return it.value;
            };
            /**
             * @inheritdoc
             */
            EntityListCollection.prototype.toXML = function () {
                var xml = new samchon.library.XML();
                xml.setTag(this.TAG());
                // MEMBERS
                for (var key in this)
                    if (typeof key == "string"
                        && (typeof this[key] == "string" || typeof this[key] == "number")
                        && this.hasOwnProperty(key)) {
                        // ATOMIC
                        xml.setProperty(key, this[key] + "");
                    }
                // CHILDREN
                for (var it = this.begin(); !it.equal_to(this.end()); it = it.next())
                    xml.push(it.value.toXML());
                return xml;
            };
            return EntityListCollection;
        }(samchon.collection.ListCollection));
        protocol.EntityListCollection = EntityListCollection;
        /**
         * @inheritdoc
         */
        var EntityDequeCollection = (function (_super) {
            __extends(EntityDequeCollection, _super);
            function EntityDequeCollection() {
                _super.apply(this, arguments);
            }
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            // using super::super;
            /**
             * @inheritdoc
             */
            EntityDequeCollection.prototype.construct = function (xml) {
                this.clear();
                // MEMBER VARIABLES; ATOMIC
                var propertyMap = xml.getPropertyMap();
                for (var v_it = propertyMap.begin(); v_it.equal_to(propertyMap.end()) != true; v_it = v_it.next())
                    if (typeof this[v_it.first] == "number" && v_it.first != "length")
                        this[v_it.first] = parseFloat(v_it.second);
                    else if (typeof this[v_it.first] == "string")
                        this[v_it.first] = v_it.second;
                //CHILDREN
                if (xml.has(this.CHILD_TAG()) == false)
                    return;
                var xmlList = xml.get(this.CHILD_TAG());
                for (var i = 0; i < xmlList.size(); i++) {
                    var child = this.createChild(xmlList.at(i));
                    if (child == null)
                        continue;
                    child.construct(xmlList.at(i));
                    this.push(child);
                }
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
            EntityDequeCollection.prototype.find = function (key) {
                return std.find_if(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityDequeCollection.prototype.has = function (key) {
                return std.any_of(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityDequeCollection.prototype.count = function (key) {
                return std.count_if(this.begin(), this.end(), function (entity) {
                    return std.equal_to(entity.key(), key);
                });
            };
            /**
             * @inheritdoc
             */
            EntityDequeCollection.prototype.get = function (key) {
                var it = this.find(key);
                if (it.equal_to(this.end()))
                    throw new std.OutOfRange("out of range");
                return it.value;
            };
            /**
             * @inheritdoc
             */
            EntityDequeCollection.prototype.toXML = function () {
                var xml = new samchon.library.XML();
                xml.setTag(this.TAG());
                // MEMBERS
                for (var key in this)
                    if (typeof key == "string"
                        && (typeof this[key] == "string" || typeof this[key] == "number")
                        && this.hasOwnProperty(key)) {
                        // ATOMIC
                        xml.setProperty(key, this[key] + "");
                    }
                // CHILDREN
                for (var it = this.begin(); !it.equal_to(this.end()); it = it.next())
                    xml.push(it.value.toXML());
                return xml;
            };
            return EntityDequeCollection;
        }(samchon.collection.DequeCollection));
        protocol.EntityDequeCollection = EntityDequeCollection;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="EntityArray.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * <p> A network driver for an external system. </p>
         *
         * <p> ExternalSystem is a boundary class interacting with an external system by network communication.
         * Also, ExternalSystem is an abstract class that a network role, which one is server and which one is
         * client, is not determined yet. </p>
         *
         * <p> The ExternalSystem has ExternalSystemRole(s) groupped methods, handling Invoke message
         * interacting with the external system, by subject or unit of a moudle. The ExternalSystemRole is
         * categorized in a 'control'. </p>
         *
         * <h4> Note </h4>
         * <p> The ExternalSystem class takes a role of interaction with external system in network level.
         * However, within a framework of Samchon Framework, a boundary class like the ExternalSystem is
         * not such important. You can find some evidence in a relationship between ExternalSystemArray,
         * ExternalSystem and ExternalSystemRole. </p>
         *
         * <p> Of course, the ExternalSystemRole is belonged to an ExternalSystem. However, if you
         * access an ExternalSystemRole from an ExternalSystemArray directly, not passing by a belonged
         * ExternalSystem, and send an Invoke message even you're not knowing which ExternalSystem is
         * related in, it's called "Proxy pattern".
         *
         * <p> Like the explanation of "Proxy pattern", you can utilize an ExternalSystemRole as a proxy
         * of an ExternalSystem. With the pattern, you can only concentrate on ExternalSystemRole itself,
         * what to do with Invoke message, irrespective of the ExternalSystemRole is belonged to which
         * ExternalSystem. </p>
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var ExternalSystem = (function (_super) {
            __extends(ExternalSystem, _super);
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            /**
             * <p> Default Constructor. </p>
             */
            function ExternalSystem() {
                _super.call(this);
                this.driver = null;
            }
            /**
             * <p> Start interaction. </p>
             * <p> An abstract method starting interaction with an external system. </p>
             *
             * <p> If an external systems are a server, starts connection and listening Inovoke message,
             * else clients, just starts listening only. You also can addict your own procudures of starting
             * the driver, but if you directly override method of abstract ExternalSystem, be careful about
             * virtual inheritance. </p>
             */
            ExternalSystem.prototype.start = function () {
                if (this.driver != null)
                    return;
                this.driver = new protocol.ServerConnector(this);
                this.driver.connect(this.ip, this.port);
            };
            /* ------------------------------------------------------------------
                GETTERS
            ------------------------------------------------------------------ */
            ExternalSystem.prototype.key = function () {
                return this.name;
            };
            /**
             * <p> Get name. </p>
             */
            ExternalSystem.prototype.getName = function () {
                return this.name;
            };
            /**
             * <p> Get ip address of the external system. </p>
             */
            ExternalSystem.prototype.getIP = function () {
                return this.ip;
            };
            /**
             * <p> Get port number of the external system. </p>
             */
            ExternalSystem.prototype.getPort = function () {
                return this.port;
            };
            /* ------------------------------------------------------------------
                CHAIN OF INVOKE MESSAGE
            ------------------------------------------------------------------ */
            ExternalSystem.prototype.sendData = function (invoke) {
                this.driver.sendData(invoke);
            };
            ExternalSystem.prototype.replyData = function (invoke) {
                invoke.apply(this);
                for (var i = 0; i < this.size(); i++)
                    this[i].replyData(invoke);
            };
            /* ------------------------------------------------------------------
               EXPORTERS
           ------------------------------------------------------------------ */
            ExternalSystem.prototype.TAG = function () {
                return "system";
            };
            ExternalSystem.prototype.CHILD_TAG = function () {
                return "role";
            };
            return ExternalSystem;
        }(protocol.EntityArray));
        protocol.ExternalSystem = ExternalSystem;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="EntityArray.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * <p> An array of ExternalSystem(s). </p>
         *
         * <p> ExternalSystemArray is an abstract class containing and managing external system drivers. </p>
         *
         * <p> Also, ExternalSystemArray can access to ExternalSystemRole(s) directly. With the method, you
         * can use an ExternalSystemRole as "logical proxy" of an ExternalSystem. Of course, the
         * ExternalSystemRole is belonged to an ExternalSystem. However, if you access an ExternalSystemRole
         * from an ExternalSystemArray directly, not passing by a belonged ExternalSystem, and send an Invoke
         * message even you're not knowing which ExternalSystem is related in, the ExternalSystemRole acted
         * a role of proxy. </p>
         *
         * <p> It's called as "Proxy pattern". With the pattern, you can only concentrate on
         * ExternalSystemRole itself, what to do with Invoke message, irrespective of the ExternalSystemRole
         * is belonged to which ExternalSystem. </p>
         *
         * <ul>
         *  <li> ExternalSystemArray::getRole("something")->sendData(invoke); </li>
         * </ul>
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var ExternalSystemArray = (function (_super) {
            __extends(ExternalSystemArray, _super);
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            /**
             * Default Constructor.
             */
            function ExternalSystemArray() {
                _super.call(this);
            }
            /**
             * <p> Start interaction. </p>
             * <p> An abstract method starting interaction with external systems. </p>
             *
             * <p> If external systems are servers, starts connection to them, else clients, opens a server
             * and accepts the external systems. You can addict your own procudures of starting drivers, but
             * if you directly override method of abstract ExternalSystemArray, be careful about virtual
             * inheritance. </p>
             */
            ExternalSystemArray.prototype.start = function () {
                for (var i = 0; i < this.size(); i++)
                    this.at(i).start();
            };
            /* ------------------------------------------------------------------
                GETTERS
            ------------------------------------------------------------------ */
            /**
             * <p> Test whether has a role. </p>
             *
             * @param name Name of an ExternalSystemRole.
             * @return Whether has or not.
             */
            ExternalSystemArray.prototype.hasRole = function (key) {
                for (var i = 0; i < this.size(); i++)
                    if (this.at(i).has(key) == true)
                        return true;
                return false;
            };
            /**
             * <p> Get a role. </p>
             *
             * @param name Name of an ExternalSystemRole
             * @return A shared pointer of specialized role
             */
            ExternalSystemArray.prototype.getRole = function (key) {
                for (var i = 0; i < this.size(); i++)
                    if (this.at(i).has(key) == true)
                        return this.at(i).get(key);
                throw Error("out of range");
            };
            /* ------------------------------------------------------------------
                CHAIN OF INVOKE MESSAGE
            ------------------------------------------------------------------ */
            ExternalSystemArray.prototype.sendData = function (invoke) {
                var listener = invoke.getListener();
                for (var i = 0; i < this.size(); i++)
                    for (var j = 0; j < this.at(i).size(); j++)
                        if (this.at(i).at(j).hasSendListener(listener) == true)
                            this.at(i).sendData(invoke);
            };
            ExternalSystemArray.prototype.replyData = function (invoke) {
                invoke.apply(this);
            };
            /* ------------------------------------------------------------------
                EXPORTERS
            ------------------------------------------------------------------ */
            ExternalSystemArray.prototype.TAG = function () {
                return "systemArray";
            };
            ExternalSystemArray.prototype.CHILD_TAG = function () {
                return "system";
            };
            return ExternalSystemArray;
        }(protocol.EntityArray));
        protocol.ExternalSystemArray = ExternalSystemArray;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="Entity.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * <p> A role belongs to an external system. </p>
         *
         * <p> ExternalSystemRole is a 'control' class groupping methods, handling Invoke messages
         * interacting with an external system that the ExternalSystemRole is belonged to, by a subject or
         * unit of a module. <p>
         *
         * <p> ExternalSystemRole can be a "logical proxy" for an ExternalSystem which is containing the
         * ExternalSystemRole. Of course, the ExternalSystemRole is belonged to an ExternalSystem. However,
         * if you access an ExternalSystemRole from an ExternalSystemArray directly, not passing by a
         * belonged ExternalSystem, and send an Invoke message even you're not knowing which ExternalSystem
         * is related in, the ExternalSystemRole acted a role of proxy. </p>
         *
         * <p> It's called as "Proxy pattern". With the pattern, you can only concentrate on
         * ExternalSystemRole itself, what to do with Invoke message, irrespective of the ExternalSystemRole
         * is belonged to which ExternalSystem. </p>
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var ExternalSystemRole = (function (_super) {
            __extends(ExternalSystemRole, _super);
            /* ------------------------------------------------------------------
                CONSTRUCTORS
            ------------------------------------------------------------------ */
            /**
             * <p> Construct from external system driver. </p>
             *
             * @param system A driver of external system the ExternalSystemRole is belonged to.
             */
            function ExternalSystemRole(system) {
                _super.call(this);
                this.system = system;
                this.sendListeners = new std.HashSet();
            }
            ExternalSystemRole.prototype.construct = function (xml) {
                _super.prototype.construct.call(this, xml);
            };
            /* ------------------------------------------------------------------
                GETTERS
            ------------------------------------------------------------------ */
            ExternalSystemRole.prototype.getName = function () {
                return this.name;
            };
            ExternalSystemRole.prototype.hasSendListener = function (key) {
                return this.sendListeners.has(key);
            };
            /* ------------------------------------------------------------------
                CHAIN OF INVOKE MESSAGE
            ------------------------------------------------------------------ */
            ExternalSystemRole.prototype.sendData = function (invoke) {
                this.system.sendData(invoke);
            };
            ExternalSystemRole.prototype.replyData = function (invoke) {
                invoke.apply(this);
            };
            /* ------------------------------------------------------------------
                EXPORTERS
            ------------------------------------------------------------------ */
            ExternalSystemRole.prototype.TAG = function () {
                return "role";
            };
            ExternalSystemRole.prototype.toXML = function () {
                var xml = _super.prototype.toXML.call(this);
                return xml;
            };
            return ExternalSystemRole;
        }(protocol.Entity));
        protocol.ExternalSystemRole = ExternalSystemRole;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
/// <reference path="../API.ts" />
/// <reference path="EntityArray.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * <p> Standard message of network I/O. </p>
         * <p> Invoke is a class used in network I/O in protocol package of Samchon Framework. </p>
         *
         * <p> The Invoke message has an XML structure like the result screen of provided example in below.
         * We can enjoy lots of benefits by the normalized and standardized message structure used in
         * network I/O. </p>
         *
         * <p> The greatest advantage is that we can make any type of network system, even how the system
         * is enourmously complicated. As network communication message is standardized, we only need to
         * concentrate on logical relationships between network systems. We can handle each network system
         * like a object (class) in OOD. And those relationships can be easily designed by using design
         * pattern. </p>
         *
         * <p> In Samchon Framework, you can make any type of network system with basic 3 + 1 componenets
         * (IProtocol, IServer and IClient + ServerConnector), by implemens or inherits them, like designing
         * classes of S/W architecture. </p>
         *
         * @see IProtocol
         * @author Jeongho Nam <http://samchon.org>
         */
        var Invoke = (function (_super) {
            __extends(Invoke, _super);
            function Invoke() {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i - 0] = arguments[_i];
                }
                _super.call(this);
                if (args.length == 0) {
                    this.listener = "";
                }
                else if (args.length == 1 && typeof args[0] == "string") {
                    var listener = args[0];
                    this.listener = listener;
                }
                else if (args.length == 1 && args[0] instanceof samchon.library.XML) {
                    this.listener = "";
                    var xml = args[0];
                    this.construct(xml);
                }
                else if (args.length == 1 && args[0] instanceof Invoke) {
                    var invoke = args[0];
                    this.listener = invoke.listener;
                    this.assign(invoke.begin(), invoke.end());
                }
                else if (args.length == 3 && args[1] instanceof std.VectorIterator && args[2] instanceof std.VectorIterator) {
                    var listener = args[0];
                    var begin = args[1];
                    var end = args[2];
                    this.listener = listener;
                    this.assign(begin, end);
                }
                else if (args.length > 1) {
                    this.listener = args[0];
                    for (var i = 1; i < args.length; i++)
                        this.push_back(new protocol.InvokeParameter("", args[i]));
                }
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
             * <p> Get arguments for Function.apply(). </p>
             *
             * @return An array containing values of the contained parameters.
             */
            Invoke.prototype.getArguments = function () {
                var args = [];
                for (var i = 0; i < this.size(); i++)
                    args.push(this[i].getValue());
                return args;
            };
            /* -------------------------------------------------------------------
                APPLY BY FUNCTION POINTER
            ------------------------------------------------------------------- */
            /**
             * <p> Apply to a matched function. </p>
             */
            Invoke.prototype.apply = function (obj) {
                if (!(this.listener in obj && obj[this.listener] instanceof Function))
                    return false;
                var func = obj[this.listener];
                var args = this.getArguments();
                func.apply(obj, args);
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
         * <p> A history of an Invoke message. </p>
         *
         * <p> InvokeHistory is a class for reporting history log of an Invoke message with elapsed time
         * from a slave to its master.</p>
         *
         * <p> With the elapsed time, consumed time for a process of handling the Invoke message,
         * InvokeHistory is reported to the master. The master utilizies the elapsed time to estimating
         * performances of each slave system. With the estimated performan index, master retrives the
         * optimal solution of distributing processes. </p>
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var InvokeHistory = (function (_super) {
            __extends(InvokeHistory, _super);
            /* -----------------------------------------------------------------
                CONSTRUCTORS
            ----------------------------------------------------------------- */
            /**
             * <p> Construct from an Invoke message. </p>
             *
             * <p> InvokeHistory does not archive entire Invoke message, only archives its listener. </p>
             *
             * @param invoke A message to archive its history log
             */
            function InvokeHistory(invoke) {
                _super.call(this);
                this.uid = invoke.get("invoke_history_uid").getValue();
                this.listener = invoke.getListener();
                this.startTime = new Date();
                //DELETE UID IN INVOKE
                //invoke.erase("invoke_history_uid");
            }
            /**
             * <p> Notify end of the process. </p>
             *
             * <p> Notifies end of a process handling the matched Invoke message to InvokeHistory. </p>
             * <p> InvokeHistory archives the end datetime and calculates elapsed time as nanoseconds. </p>
             */
            InvokeHistory.prototype.notifyEnd = function () {
                this.endTime = new Date();
            };
            /* -----------------------------------------------------------------
                EXPORTERS
            ----------------------------------------------------------------- */
            InvokeHistory.prototype.TAG = function () {
                return "invokeHistory";
            };
            InvokeHistory.prototype.toXML = function () {
                var xml = _super.prototype.toXML.call(this);
                xml.setProperty("startTime", this.startTime.getTime() * Math.pow(10.0, 6) + "");
                xml.setProperty("endTime", this.endTime.getTime() * Math.pow(10.0, 6) + "");
                return xml;
            };
            /**
             * <p> Get an Invoke message. </p>
             *
             * <p> Returns an Invoke message to report to a master that how much time was elapsed on a
             * process handling the Invoke message. In master, those reports are used to estimate
             * performance of each slave system. </p>
             *
             * @return An Invoke message to report master.
             */
            InvokeHistory.prototype.toInvoke = function () {
                return null;
                //return new Invoke("reportInvokeHistory", this.toXML());
            };
            return InvokeHistory;
        }(protocol.Entity));
        protocol.InvokeHistory = InvokeHistory;
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
         * @see Invoke
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
                    args[_i - 0] = arguments[_i];
                }
                _super.call(this);
                /**
                 * <p> Value of the parameter. </p>
                 */
                this.value = null;
                if (args.length == 0)
                    return;
                else if (args.length == 2) {
                    this.name = args[0];
                    this.value = args[1];
                }
                else if (args.length == 3) {
                    this.name = args[0];
                    this.value = args[2];
                }
                this.type = typeof this.value;
                if (this.value instanceof samchon.library.XML)
                    this.type = "XML";
            }
            /**
             * @inheritdoc
             */
            InvokeParameter.prototype.construct = function (xml) {
                this.value = null;
                _super.prototype.construct.call(this, xml);
                if (this.type == "XML")
                    this.value = xml.begin().second.at(0);
            };
            InvokeParameter.prototype.setValue = function (value) {
                this.value = value;
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
                var xml = _super.prototype.toXML.call(this);
                // NOT CONSIDERED ABOUT THE BINARY DATA
                (this.type == "XML");
                xml.push(this.value);
                return xml;
            };
            return InvokeParameter;
        }(protocol.Entity));
        protocol.InvokeParameter = InvokeParameter;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../API.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        /**
         * <p> A server connector for a physical client. </p>
         *
         * <p> ServerConnector is a class for a physical client connecting a server. If you want to connect
         * to a server,  then implements this ServerConnector and just override some methods like
         * getIP(), getPort() and replyData(). That's all. </p>
         *
         * <p> In Samchon Framework, package protocol, There are basic 3 + 1 components that can make any
         * type of network system in Samchon Framework. The basic 3 components are IProtocol, IServer and
         * IClient. The last, surplus one is the ServerConnector. Looking around classes in
         * Samchon Framework, especially module master and slave which are designed for realizing
         * distributed processing systems and parallel processing systems, physical client classes are all
         * derived from this ServerConnector. </p>
         *
         * <img src="interface.png" />
         *
         * @author Jeongho Nam <http://samchon.org>
         */
        var ServerConnector = (function () {
            /**
             * <p> Constructor with parent. </p>
             */
            function ServerConnector(parent) {
                this.parent = parent;
                this.binary_invoke = null;
                this.onopen = null;
            }
            /**
             * <p> Connects to a cloud server with specified host and port. </p>
             *
             * <p> If the connection fails immediately, either an event is dispatched or an exception is thrown:
             * an error event is dispatched if a host was specified, and an exception is thrown if no host
             * was specified. Otherwise, the status of the connection is reported by an event.
             * If the socket is already connected, the existing connection is closed first. </p>
             *
             * @param ip
             * 		The name or IP address of the host to connect to.
             * 		If no host is specified, the host that is contacted is the host where the calling
             * 		file resides. If you do not specify a host, use an event listener to determine whether
             * 		the connection was successful.
             * @param port
             * 		The port number to connect to.
             *
             * @throws IOError
             * 		No host was specified and the connection failed.
             * @throws SecurityError
             * 		This error occurs in SWF content for the following reasons:
             * 		Local untrusted SWF files may not communicate with the Internet. You can work around
             * 		this limitation by reclassifying the file as local-with-networking or as trusted.
             */
            ServerConnector.prototype.connect = function (ip, port, path) {
                if (path === void 0) { path = ""; }
                if (ip.indexOf("ws://") == -1) {
                    if (ip.indexOf("://") != -1)
                        throw "only websocket is possible";
                    else
                        ip = "ws://" + ip;
                }
                this.socket = new WebSocket(ip + ":" + port + "/" + path);
                var this_ = this;
                this.socket.onopen = function (event) {
                    this_.handleConnect(event);
                };
                this.socket.onmessage = function (event) {
                    this_.handleReply(event);
                };
            };
            /* ----------------------------------------------------
                IPROTOCOL'S METHOD
            ---------------------------------------------------- */
            /**
             * <p> Send data to the server. </p>
             */
            ServerConnector.prototype.sendData = function (invoke) {
                var xml = invoke.toXML();
                var str = xml.toString();
                this.socket.send(str);
            };
            /**
             * <p> Shift responsiblity of handling message to parent. </p>
             */
            ServerConnector.prototype.replyData = function (invoke) {
                this.parent.replyData(invoke);
            };
            /* ----------------------------------------------------
                HANDLING CONNECTION AND MESSAGES
            ---------------------------------------------------- */
            ServerConnector.prototype.handleConnect = function (event) {
                if (this.onopen == null)
                    return;
                this.onopen(event);
            };
            /**
             * <p> Handling replied message. </p>
             */
            ServerConnector.prototype.handleReply = function (event) {
                console.log("handle_reply: #" + event.data.length);
                if (this.binary_invoke == null) {
                    var xml = new samchon.library.XML(event.data);
                    var invoke = new protocol.Invoke(xml);
                    // THE INVOKE MESSAGE INCLUDES BINARY DATA?
                    var is_binary = std.any_of(invoke.begin(), invoke.end(), function (parameter) {
                        return parameter.getType() == "ByteArray";
                    });
                    // IF EXISTS, REGISTER AND TERMINATE
                    if (is_binary)
                        this.binary_invoke = invoke;
                    else
                        this.replyData(invoke);
                }
                else {
                    // FIND THE MATCHED PARAMETER
                    var it = std.find_if(this.binary_invoke.begin(), this.binary_invoke.end(), function (parameter) {
                        return parameter.getType() == "ByteArray" && parameter.getValue() == null;
                    });
                    // SET BINARY DATA
                    it.value.setValue(event.data);
                    // FIND THE REMAINED BINARY PARAMETER
                    it = std.find_if(it.next(), this.binary_invoke.end(), function (parameter) {
                        return parameter.getType() == "ByteArray" && parameter.getValue() == null;
                    });
                    if (it.equal_to(this.binary_invoke.end())) {
                        // AND IF NOT, SEND THE INVOKE MESSAGE
                        this.replyData(this.binary_invoke);
                        this.binary_invoke = null;
                    }
                }
            };
            return ServerConnector;
        }());
        protocol.ServerConnector = ServerConnector;
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        var service;
        (function (service) {
            /**
             * <p> An application, the top class in JS-UI. </p>
             *
             * <p> The Application is separated to three part, TopMenu, Movie and ServerConnector. </p>
             * <ul>
             * 	<li> <code>TopMenu</code>: Menu on the top. It's not an essential component. </li>
             * 	<li> <code>Movie</code>: Correspond with Service in Server. Movie has domain UI components(Movie) for the matched Service. </li>
             * 	<li> <code>ServerConnector</code>: The socket connecting to the Server. </li>
             * </ul>
             *
             * <p> The Application and its UI-layout is not fixed, essential component for Samchon Framework in Flex,
             * so it's okay to do not use the provided Application and make your custom Application.
             * But the custom Application, your own, has to contain the Movie and keep the construction routine. </p>
             *
             * <p> <img src="movie.png" /> </p>
             *
             * <h4> THE CONSTRUCTION ROUTINE </h4>
             * <ul>
             * 	<li>Socket Connection</li>
             * 	<ul>
             * 		<li>Connect to the CPP-Server</li>
             * 	</ul>
             * 	<li>Fetch authority</li>
             * 	<ul>
             * 		<li>Send a request to fetching authority</li>
             * 		<li>The window can be navigated to other page by the authority</li>
             * 	</ul>
             * 	<li>Construct Movie</li>
             * 	<ul>
             * 		<li>Determine a Movie by URLVariables::movie and construct it</li>
             * 	</ul>
             * 	<li>All the routines are done</li>
             * </ul>
             *
             * @author Jeongho Nam <http://samchon.org>
             */
            var Application = (function () {
                /**
                 * <p> Construct from arguments. </p>
                 *
                 * @param movie A movie represents a service.
                 * @param ip An ip address of cloud server to connect.
                 * @param port A port number of cloud server to connect.
                 */
                function Application(movie, ip, port) {
                    this.movie = movie;
                    this.socket = new protocol.ServerConnector(this);
                    this.socket.onopen = this.handleConnect;
                    this.socket.connect(ip, port);
                }
                Application.prototype.handleConnect = function (event) {
                };
                /**
                 * <p> Handle replied message or shift the responsibility. </p>
                 */
                Application.prototype.replyData = function (invoke) {
                    if (invoke.apply(this) == false)
                        this.movie.sendData(invoke);
                };
                /**
                 * <p> Send a data to server. </p>
                 */
                Application.prototype.sendData = function (invoke) {
                    this.socket.sendData(invoke);
                };
                return Application;
            }());
            service.Application = Application;
        })(service = protocol.service || (protocol.service = {}));
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        var service;
        (function (service) {
            /**
             * A movie belonged to an Application.
             */
            var Movie = (function () {
                function Movie() {
                }
                /**
                 * Handle replied data.
                 */
                Movie.prototype.replyData = function (invoke) {
                    invoke.apply(this) == false;
                };
                /**
                 * Send data to server.
                 */
                Movie.prototype.sendData = function (invoke) {
                    this.application.sendData(invoke);
                };
                return Movie;
            }());
            service.Movie = Movie;
        })(service = protocol.service || (protocol.service = {}));
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        var service;
        (function (service) {
            /**
             * A sub-movie.
             *
             * @author Jeongho Nam <http://samchon.org>
             */
            var SubMovie = (function () {
                function SubMovie() {
                }
                SubMovie.prototype.replyData = function (invoke) {
                    invoke.apply(this);
                };
                SubMovie.prototype.sendData = function (invoke) {
                    this.parent.sendData(invoke);
                };
                return SubMovie;
            }());
        })(service = protocol.service || (protocol.service = {}));
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
/// <reference path="../../API.ts" />
/// <reference path="../ExternalSystem.ts" />
var samchon;
(function (samchon) {
    var protocol;
    (function (protocol) {
        var slave;
        (function (slave) {
            /**
             * @brief A slave system.
             *
             * @details
             * <p> SlaveSystem, literally, means a slave system belongs to a maste system. </p>
             *
             * <p> The SlaveSystem class is used in opposite side system of master::DistributedSystem
             * and master::ParallelSystem and reports elapsed time of each commmand (by Invoke message)
             * for estimation of its performance. </p>
             *
             * @inheritdoc
             * @author Jeongho Nam <http://samchon.org>
             */
            var SlaveSystem = (function (_super) {
                __extends(SlaveSystem, _super);
                /**
                 * <p> Default Constructor. </p>
                 */
                function SlaveSystem() {
                    _super.call(this);
                }
                /**
                 * @inheritdoc
                 */
                SlaveSystem.prototype.replyData = function (invoke) {
                    var history = new protocol.InvokeHistory(invoke);
                    _super.prototype.replyData.call(this, invoke);
                    history.notifyEnd();
                    this.sendData(history.toInvoke());
                };
                return SlaveSystem;
            }(protocol.ExternalSystem));
            slave.SlaveSystem = SlaveSystem;
        })(slave = protocol.slave || (protocol.slave = {}));
    })(protocol = samchon.protocol || (samchon.protocol = {}));
})(samchon || (samchon = {}));
