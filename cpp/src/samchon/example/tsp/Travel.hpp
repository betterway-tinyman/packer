#pragma once
#include <samchon/protocol/SharedEntityArray.hpp>
#include <samchon/example/tsp/GeometryPoint.hpp>

namespace samchon
{
namespace example
{
namespace tsp
{
	using namespace std;

	using namespace library;
	using namespace protocol;

	/**
	 * @brief Represent a travel containning Point(s)
	 *
	 * @details 
	 * <p> Travel class is not only an array containing Point(s) but also a sequence listing of the Point(s). </p>
	 *
	 * <p> @image html cpp/example_tsp.png
	 * @image latex cpp/example_tsp.png </p>
	 *
	 * @author Jeongho Nam
	 */
	class Travel
		: public SharedEntityArray<GeometryPoint>
	{
	private:
		typedef SharedEntityArray<GeometryPoint> super;

	protected:
		/**
		 * @brief Estimated hours to move
		 *
		 * @details A variable for avoiding duplicated calculation
		 * @see calcDistance()
		 */
		double distance;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 */
		Travel()
			: super()
		{
			distance = INT_MIN;
		};

		/**
		 * @brief Copy Constructor
		 *
		 * @details
		 * <p> Travel is an array of Point(s) as gene(s). The copy constructor of Travel doesn't copy
		 * member variables which are related to G.A. because the sequence of gene(Point)s can be shuffled
		 * by process of evolution of genetic algorithm. </p>
		 */
		Travel(const Travel &travel)
			: super(travel)
		{
			distance = INT_MIN;
		};

		/**
		 * @brief Move Constructor
		 */
		Travel(Travel &&travel)
			: super(move(travel))
		{
			distance = travel.distance;
		};

		virtual ~Travel() = default;

		virtual void construct(shared_ptr<XML> xml) override
		{
			super::construct(xml);

			if (xml->hasProperty("distance") == true)
				distance = xml->getProperty<double>("distance");
			else
				distance = INT_MIN;
		};

	protected:
		virtual auto createChild(shared_ptr<XML>) -> GeometryPoint*
		{
			return new GeometryPoint();
		};

		/* -----------------------------------------------------------
			CALCULATORS
		----------------------------------------------------------- */
	public:
		/**
		 * @brief Calculate distance to move
		 */
		auto calcDistance() const -> double
		{
			if(this->distance != INT_MIN)
				return this->distance;

			double distance = 0.0;
			for(size_t i = 1; i < size(); i++)
				distance += at(i-1)->calcDistance(*at(i));

			((Travel*)this)->distance = distance;
			return distance;
		};

	public:
		/**
		 * @brief Compare which object is less
		 * @details Compares distances to move. The result is used to optimize and evolve genes by G.A.
		 *
		 * @param Target object to compare
		 * @return Whether this object is less than target
		 */
		auto operator<(const Travel &travel) const -> bool
		{
			return this->calcDistance() < travel.calcDistance();
		};

		/* -----------------------------------------------------------
			EXPORTER
		----------------------------------------------------------- */
		virtual auto TAG() const -> string override
		{
			return "travel";
		};
		virtual auto CHILD_TAG() const -> string override
		{
			return "point";
		};

		virtual auto toXML() const -> shared_ptr<XML> override
		{
			shared_ptr<XML> &xml = super::toXML();
			if (distance != INT_MIN)
				xml->setProperty("distance", distance);

			return xml;
		};

		/**
		 * @brief Convert the Travel to String
		 *
		 * @details 
		 * <p> Have of form of tab and enter delimeters for Excel. </p>
		 * <p><pre> {$uid1}	{$x}	{$y} </pre><br/>
		 * <pre> {$uid2}	{$x}	{$y} </pre><br/>
		 * <pre> {$uid3}	{$x}	{$y} </pre><br/>
		 * ...</p>
		 *
		 * @return A string can represent the Travel
		 */
		auto toString() const -> string
		{
			string str =
				"Distance: " + StringUtil::numberFormat(calcDistance()) + " km\n" + 
				"uid	longitude	latitude\n";

			for(size_t i = 0; i < size(); i++)
				str += at(i)->toString() + "\n";

			return move(str);
		};
	};			
};
};
};