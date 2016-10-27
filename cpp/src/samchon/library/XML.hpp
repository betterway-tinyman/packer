#pragma once
#include <samchon/HashMap.hpp>

#include <vector>
#include <string>
#include <queue>
#include <memory>
#include <sstream>

#include <samchon/WeakString.hpp>
#include <samchon/library/Math.hpp>

namespace samchon
{
namespace library
{
	class XML;
	typedef std::vector<std::shared_ptr<XML>> XMLList;

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
	 * @handbook https://github.com/samchon/framework/wiki/CPP-Library-XML
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class XML
		: public HashMap<std::string, std::shared_ptr<XMLList>>
	{
	private:
		typedef HashMap<std::string, std::shared_ptr<XMLList>> super;

		std::string tag_;
		std::string value_;

		HashMap<std::string, std::string> property_map_;

	public:
		/* =============================================================
			CONSTRUCTORS
				- BASIC CONSTRUCTORS
				- PARSERS
		================================================================
			BASIC CONSTRUCTORS
		------------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 *
		 * @warning Declare XML to managed by shared pointer
		 */
		XML() : super()
		{
		};

		/**
		 * @brief Copy Constructor
		 *
		 * @details
		 * Not copying (shared) pointer of children xml objects,
		 * but copying the real objects of children xml
		 */
		XML(const XML &xml) : super()
		{
			tag_ = xml.tag_;
			value_ = xml.value_;
			property_map_ = xml.property_map_;

			//COPYING CHILDREN OBJECTS
			for (auto it = xml.begin(); it != xml.end(); it++)
			{
				if (it->second->empty() == true)
					continue;

				std::shared_ptr<XMLList> xmlList(new XMLList());
				xmlList->reserve(it->second->size());

				for (size_t i = 0; i < it->second->size(); it++)
					xmlList->emplace_back(new XML(*it->second->at(i)));

				this->set(xmlList->at(0)->tag_, xmlList);
			}
		};

		/**
		 * @brief Move Constructor
		 */
		XML(XML &&xml) : super(move(xml))
		{
			tag_ = move(xml.tag_);
			value_ = move(xml.value_);

			property_map_ = move(xml.property_map_);
		};

		/**
		* Construct from string.
		*
		* Creates {@link XML} object by parsing a string who represents xml structure.
		*
		* @param str A string represents XML structure.
		*/
		XML(WeakString wstr) : super()
		{
			if (wstr.find('<') == std::string::npos)
				return;

			//WHEN COMMENT IS
			std::string replacedStr;
			if (wstr.find("<!--") != std::string::npos)
			{
				std::queue<std::pair<size_t, size_t>> indexPairQueue;
				size_t beginX = 0, endX;

				//CONSTRUCT INDEXES
				replacedStr.reserve(wstr.size());
				while ((beginX = wstr.find("<!--", beginX)) != std::string::npos)
				{
					indexPairQueue.push({ beginX, wstr.find("-->", beginX + 1) + 3 });
					beginX++;
				}

				//INSERT STRINGS
				beginX = 0;
				while (indexPairQueue.empty() == false)
				{
					endX = indexPairQueue.front().first;
					replacedStr.append(wstr.substring(beginX, endX).str());

					beginX = indexPairQueue.front().second;
					indexPairQueue.pop();
				}
				replacedStr.append(wstr.substr(beginX).str());

				//RE-REFERENCE
				wstr = replacedStr;
			}

			//ERASE HEADERS OF XML
			if (wstr.find("<?xml") != std::string::npos)
				wstr = wstr.between("?>");

			parse(wstr);
		};

	private:
		XML(XML *parent, WeakString &wstr) : XML()
		{
			parse(wstr);
		};

		/* -------------------------------------------------------------
			PARSERS
		------------------------------------------------------------- */
		void parse(WeakString &wstr)
		{
			parse_key(wstr);
			parse_properties(wstr);

			if (parse_value(wstr) == true)
				parse_children(wstr);
		};

		void parse_key(WeakString &wstr)
		{
			size_t startX = wstr.find("<") + 1;
			size_t endX =
				calc_min_index
				(
					{
						wstr.find(' ', startX),
						wstr.find("\r\n", startX),
						wstr.find('\n', startX),
						wstr.find('\t', startX),
						wstr.find('>', startX),
						wstr.find('/', startX)
					}
				);

			//Determinate the KEY
			tag_ = move( wstr.substring(startX, endX).str() );
		};
		
		void parse_properties(WeakString &wstr)
		{
			// INLINE CLASS
			class QuotePair
			{
			public:
				enum TYPE : int
				{
					SINGLE = 1,
					DOUBLE = 2
				};

				TYPE type;
				size_t start_index;
				size_t end_index;

				QuotePair(TYPE type, size_t start_index, size_t end_index)
				{
					this->type = type;
					this->start_index = start_index;
					this->end_index = end_index;
				};
			};

			size_t i_begin = wstr.find('<' + tag_) + tag_.size() + 1;
			size_t i_endSlash = wstr.rfind('/');
			size_t i_endBlock = wstr.find('>', i_begin);

			size_t i_end = calc_min_index({ i_endSlash, i_endBlock });
			if (i_end == std::string::npos || i_begin >= i_end)
				return;

			//<comp label='ABCD' /> : " label='ABCD' "
			WeakString &line = wstr.substring(i_begin, i_end); 

			if (line.find('=') == std::string::npos)
				return;

			std::string label, value;
			std::vector<QuotePair*> helpers;
			bool inQuote = false;
			QuotePair::TYPE type;
			size_t startPoint, equalPoint;
			size_t i;

			for (i = 0; i < line.size(); i++) 
			{
				//Start of quote
				if (inQuote == false && (line[i] == '\'' || line[i] == '"'))
				{
					inQuote = true;
					startPoint = i;

					if (line[i] == '\'')
						type = QuotePair::SINGLE;
					else if (line[i] == '"')
						type = QuotePair::DOUBLE;
				}
				else if
					(
						inQuote == true &&
						(
							(type == QuotePair::SINGLE && line[i] == '\'') ||
							(type == QuotePair::DOUBLE && line[i] == '"')
						)
					)
				{
					helpers.push_back(new QuotePair(type, startPoint, i));
					inQuote = false;
				}
			}
			for (i = 0; i < helpers.size(); i++)
			{
				if (i == 0)
				{
					equalPoint = (long long)line.find('=');
					label = move( line.substring(0, equalPoint).trim().str() );
				}
				else
				{
					equalPoint = line.find('=', helpers[i - 1]->end_index + 1);
					label = line.substring(helpers[i - 1]->end_index + 1, equalPoint).trim().str();
				}
		
				value = 
					move
					(
						decodeProperty
						(
							line.substring
							(
								helpers[i]->start_index + 1,
								helpers[i]->end_index
							)
						)
					);
		
				//INSERT INTO PROPERTY_MAP
				property_map_.set(label, move(value));
			}
			for (i = 0; i < helpers.size(); i++)
				delete helpers[i];
		};
		
		auto parse_value(WeakString &wstr) -> bool
		{
			size_t i_endSlash = wstr.rfind('/');
			size_t i_endBlock = wstr.find('>');

			if (i_endSlash < i_endBlock || i_endBlock + 1 == wstr.rfind('<'))
			{
				//STATEMENT1: <TAG />
				//STATEMENT2: <TAG></TAG> -> SAME WITH STATEMENT1: <TAG />
				value_.clear();
				return false;
			}

			size_t startX = i_endBlock + 1;
			size_t endX = wstr.rfind('<');
			wstr = wstr.substring(startX, endX); //REDEFINE WEAK_STRING -> IN TO THE TAG

			if (wstr.find('<') == std::string::npos)
				value_ = wstr.trim();
			else
				value_.clear();

			return true;
		};
		
		void parse_children(WeakString &wstr)
		{
			if (wstr.find('<') == std::string::npos)
				return;

			size_t startX = wstr.find('<');
			size_t endX = wstr.rfind('>') + 1;
			wstr = wstr.substring(startX, endX);

			/*map<std::string, queue<XML *>> xmlQueueMap;
			queue<XML*> *xmlQueue;
			XML *xml;*/

			int blockStartCount = 0;
			int blockEndCount = 0;
			size_t start = 0;
			size_t end;
			size_t i;

			//FIND BLOCKS, CREATES XML AND PUT IN TEMPORARY CONTAINER
			for (i = 0; i < wstr.size(); i++) 
			{
				if (wstr[i] == '<' && wstr.substr(i, 2) != "</")
					blockStartCount++;
				else if (wstr.substr(i, 2) == "/>" || wstr.substr(i, 2) == "</")
					blockEndCount++;

				if (blockStartCount >= 1 && blockStartCount == blockEndCount) 
				{
					//NO PROBLEM TO AVOID COMMENT
					end = wstr.find('>', i);

					/*xml = new XML(this, wstr.substring(start, end + 1));
					xmlQueueMap[xml->tag].push(xml);*/

					std::shared_ptr<XML> xml(new XML(this, wstr.substring(start, end + 1)));
					push_back(xml);

					i = end; //WHY NOT END+1? 
					start = end + 1;
					blockStartCount = 0;
					blockEndCount = 0;
				}
			}

			//RESERVE
			/*for (auto it = xmlQueueMap.begin(); it != xmlQueueMap.end(); it++)
			{
				std::string tag = move(it->first); //GET KEY
				shared_ptr<XMLList> xmlList(new XMLList());

				xmlQueue = &(it->second);
				xmlList->reserve(xmlQueue->size()); //RESERVE

				//MOVE QUEUE TO XML_LIST
				while (xmlQueue->empty() == false)
				{
					xml = xmlQueue->front();
					xmlList->push_back(shared_ptr<XML>(xml));

					xmlQueue->pop();
				}
				//INSERT IN MAP BY KEY
				insert({ tag, xmlList });
			}*/

			if (size() > 0)
				value_.clear();
		};

	public:
		/* =============================================================
			ACCESSORS
				- GETTERS
					- VALUE TEMPLATES
					- PROPERTY TEMPLATES
				- SETTERS
					- VALUE TEMPLATES
					- PROPERTY TEMPLATES
				- ELEMENTS I/O
		================================================================
			GETTERS
		------------------------------------------------------------- */
		/**
		 * @brief Get tag.
		 * 
		 * @details
		 * ```xml
		 * <TAG property_key={property_value}>{value}</TAG>
		 * ```
		 * 
		 * @return tag.
		 */
		auto getTag() const -> std::string
		{
			return tag_;
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
		auto hasProperty(const std::string &key) const -> bool
		{
			return property_map_.has(key);
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
		auto getPropertyMap() const -> const HashMap<std::string, std::string>&
		{
			return property_map_;
		};

		/* -------------------------------------------------------------
			GETTERS - VALUE TEMPLATES
		------------------------------------------------------------- */
		/** 
		 * @brief Get value.
		 * 
		 * @details
		 * ```xml
		 * <tag property_key={property_value}>{VALUE}</tag>
		 * ```
		 * 
		 * @return value.
		 */
		template<class T = std::string> auto getValue() const -> T
		{
			double val = std::stod(value_);

			return (T)val;
		};

		template<> auto getValue() const -> bool
		{
			return value_ == "true";
		};

		template<> auto getValue() const -> std::string
		{
			return value_;
		};
		template<> auto getValue() const -> WeakString
		{
			return value_;
		};

		/* -------------------------------------------------------------
			GETTERS - PROPERTY TEMPLATES
		------------------------------------------------------------- */
		/**
		 * @brief Get property.
		 * 
		 * @details
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
		template<class T = std::string> auto getProperty(const std::string &key) const -> T
		{
			double val = std::stod(property_map_.get(key));

			return (T)val;
		};

		template<> auto getProperty(const std::string &key) const -> bool
		{
			return property_map_.get(key) == "true";
		};

		template<> auto getProperty(const std::string &key) const -> std::string
		{
			return property_map_.get(key);
		};
		template<> auto getProperty(const std::string &key) const -> WeakString
		{
			return property_map_.get(key);
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
		auto findProperty(const std::string &key) ->HashMap<std::string, std::string>::iterator
		{
			return property_map_.find(key);
		};

		auto findProperty(const std::string &key) const -> HashMap<std::string, std::string>::const_iterator
		{
			return property_map_.find(key);
		};

		template <typename T = std::string>
		auto fetchProperty(const std::string &key, const T &def = T()) const -> T
		{
			if (hasProperty(key))
				return getProperty<T>(key);
			else
				return def;
		};

		/* -----------------------------------------------------------
			SETTERS
		----------------------------------------------------------- */
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
		void setTag(const std::string &val)
		{
			tag_ = val;
		};

		/* -------------------------------------------------------------
			SETTERS - VALUE TEMPLATES
		------------------------------------------------------------- */
		/**
		 * Set value.
		 *
		 * ```xml
		 * <tag property_key={property_value}>{VALUE}</tag>
		 * ```
		 * 
		 * @param val To be new {@link getValue value}.
		 */
		template <typename T>
		void setValue(const T &val)
		{
			value_ = std::to_string(val);
		};

		template<> void setValue(const bool &flag)
		{
			this->value_ = flag ? "true" : "false";
		};

		template<> void setValue(const std::string &val)
		{
			this->value_ = val;
		};
		template<> void setValue(const WeakString &val)
		{
			this->value_ = val.str();
		};
		void setValue(const char *ptr)
		{
			this->value_ = ptr;
		};

		/* -------------------------------------------------------------
			SETTERS - PROPERTY TEMPLATES
		------------------------------------------------------------- */
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
		template<typename T>
		void setProperty(const std::string &key, const T &val)
		{
			property_map_.set(key, std::to_string(val));
		};

		template<> void setProperty(const std::string &key, const bool &flag)
		{
			property_map_.set(key, flag ? "true" : "false");
		};

		template<> void setProperty(const std::string &key, const std::string &val)
		{
			property_map_.set(key, val);
		};
		template<> void setProperty(const std::string &key, const WeakString &val)
		{
			property_map_.set(key, val.str());
		};
		void setProperty(const std::string &key, const char *ptr)
		{
			property_map_.set(key, ptr);
		};

		/* -----------------------------------------------------------
			ELEMENTS I/O
		----------------------------------------------------------- */
		/**
		 * @brief Add children xml objects by string representing them
		 *
		 * @param str A string representing xml objects whould be belonged to this XML
		 */
		void push_back(WeakString wstr)
		{
			if (wstr.empty() == true)
				return;

			std::shared_ptr<XML> xml(new XML(this, wstr));
			auto it = find(xml->tag_);

			//if not exists
			if (it == end())
			{
				set(xml->tag_, std::make_shared<XMLList>());
				it = find(xml->tag_);
			}

			//insert
			it->second->push_back(xml);
		};

		/**
		 * @brief Add children xml
		 *
		 * @param xml An xml object you want to add
		 */
		void push_back(const std::shared_ptr<XML> xml)
		{
			std::string &tag = xml->tag_;
			
			if (this->has(tag) == false)
				this->set(tag, std::make_shared<XMLList>());

			this->get(tag)->push_back(xml);
		};

		template <typename T>
		void insertValue(const std::string &tag, const T &val)
		{
			std::shared_ptr<XML> xml(new XML());
			xml->setTag(tag);
			xml->setValue(val);

			push_back(xml);
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
		void insertAllProperties(const std::shared_ptr<XML> xml)
		{
			for (auto it = xml->property_map_.begin(); it != xml->property_map_.end(); it++)
				property_map_[it->first] = it->second;
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
		void eraseProperty(const std::string &key)
		{
			property_map_.erase(key);
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
		void clearProperties()
		{
			property_map_.clear();
		};

	private:
		/* -----------------------------------------------------------
			FILTERS
		----------------------------------------------------------- */
		auto calc_min_index(const std::vector<size_t> &vec) const -> size_t
		{
			size_t val = std::string::npos;
			for (size_t i = 0; i < vec.size(); i++)
				if (vec[i] != std::string::npos && vec[i] < val)
					val = vec[i];

			return val;
		};

		auto encode_value(const WeakString &wstr) const -> std::string
		{
			static std::vector<std::pair<std::string, std::string>> pairArray =
			{
				{ "&", "&amp;" },
				{ "<", "&lt;" },
				{ ">", "&gt;" },
				{ "\"", "&quot;" },
				{ "'", "&apos;" },
				{ "\t", "&#x9;" }, //9
				{ "\n", "&#xA;" }, //10
				{ "\r", "&#xD;" } //13
			};

			return wstr.replaceAll(pairArray);
		};
		
		auto decode_value(const WeakString &wstr) const -> std::string
		{
			static std::vector<std::pair<std::string, std::string>> pairArray =
			{
				{ "&amp;", "&" },
				{ "&lt;", "<" },
				{ "&gt;", ">" },
				{ "&quot;", "\"" },
				{ "&apos;", "'" },
				{ "&#x9;", "\t" }, //9
				{ "&#xA;", "\n" }, //10
				{ "&#xD;", "\r" } //13
			};

			return wstr.replaceAll(pairArray);
		};
		
		auto encode_property(const WeakString &wstr) const -> std::string
		{
			static std::vector<std::pair<std::string, std::string>> pairArray =
			{
				{ "&", "&amp;" },
				{ "<", "&lt;" },
				{ ">", "&gt;" }
			};

			return wstr.trim().replaceAll(pairArray);
		};
		
		auto decodeProperty(const WeakString &wstr) const -> std::string
		{
			static std::vector<std::pair<std::string, std::string>> pairArray =
			{
				{ "&amp;", "&" },
				{ "&lt;", "<" },
				{ "&gt;", ">" }
			};

			return wstr.replaceAll(pairArray);
		};

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
	public:
		/**
		 * {@link XML} object to xml string.
		 * 
		 * Returns a string representation of the {@link XML} object.
		 * 
		 * @param tab Number of tabs to spacing.
		 * @return The string representation of the {@link XML} object.
		 */
		auto toString(size_t level = 0) const -> std::string
		{
			// KEY
			std::string str = std::string(level, '\t') + "<" + tag_;

			// PROPERTIES
			for (auto it = property_map_.begin(); it != property_map_.end(); it++)
				str += " " + it->first + "=\"" + encode_property(it->second) + "\"";

			if (this->empty() == true)
			{
				// VALUE
				if (value_.empty() == true)
					str += " />";
				else
					str += ">" + encode_value(value_) + "</" + tag_ + ">";
			}
			else
			{
				// CHILDREN
				str += ">\n";

				for (auto it = begin(); it != end(); it++)
					for (size_t i = 0; i < it->second->size(); i++)
						str += it->second->at(i)->toString(level + 1);

				str += std::string(level, '\t') + "</" + tag_ + ">";
			}

			return str + "\n";
		};
	};
};
};
