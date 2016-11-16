#pragma once
#include <samchon/protocol/Entity.hpp>

#include <random>
#include <cmath>
#include <samchon/library/Math.hpp>
#include <samchon/library/StringUtil.hpp>
#include <samchon/library/XML.hpp>

namespace samchon
{
namespace examples
{
namespace tsp
{
	using namespace std;

	using namespace library;
	using namespace protocol;

	/**
	 * @brief A geometry coordinates (point)
	 *
	 * @details
	 * <p> @image html cpp/example_tsp.png
	 * @image latex cpp/example_tsp.png </p>
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class GeometryPoint
		: public Entity<int>
	{
	private:
		typedef Entity super;

	protected:
		/**
		 * @brief An unique id for uniqueness
		 */
		int uid;

		/**
		 * @brief The longitude; coordinates X
		 */
		double longitude;

		/**
		 * @brief The latitude, coordinates Y
		 */
		double latitude;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 */
		GeometryPoint()
			: super()
		{
		};

		/**
		 * @brief Construct from uid.
		 * 		  
		 * @details Geometry coordinates, longitude and latitude will have a random value between
		 *	\li Longitude between 0 and 180.
		 *	\li Latitude between -90 and 90.
		 */
		GeometryPoint(int uid)
			: super()
		{
			this->uid = uid;

			this->longitude = Math::random() * 180.0;
			this->latitude = Math::random() * 180 - 90.0;
		};

		/**
		 * @brief Construct from uid and geometry coordinates.
		 *
		 * @param longitude Longitude, a coordinate of the geometry point
		 * @param latitude Latitude, a coordinate of the geometry point
		 */
		GeometryPoint(int uid, double longitude, double latitude)
			: super()
		{
			this->uid = uid;
			this->longitude = longitude;
			this->latitude = latitude;
		};

		virtual ~GeometryPoint() = default;

		virtual void construct(shared_ptr<XML> xml) override
		{
			uid = xml->getProperty<int>("uid");
			longitude = xml->getProperty<double>("longitude");
			latitude = xml->getProperty<double>("latitude");
		};

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		virtual auto key() const -> int override
		{
			return uid;
		};

		/**
		 * @brief Calculate distance between target Branch
		 *
		 * @details Referenced from http://thesunrises.tistory.com/958 (Geometry points to killometers)
		 * @param branch The target Branch to calculate
		 */
		auto calcDistance(const GeometryPoint &point) const -> double
		{
			if (longitude == point.longitude && latitude == point.latitude)
				return 0.0;

			double latitude_radian1 = Math::degree_to_radian(this->latitude);
			double latitude_radian2 = Math::degree_to_radian(point.latitude);
			double theta = this->longitude - point.longitude;

			double val =
				sin(latitude_radian1) * sin(latitude_radian2)
				+ cos(latitude_radian1) * cos(latitude_radian2) * cos(Math::degree_to_radian(theta));

			val = acos(val);
			val = Math::radian_to_degree(val);
			val = val * 60 * 1.1515;
			val = val * 1.609344;

			return val;
		};

		/* -----------------------------------------------------------
			EXPORTER
		----------------------------------------------------------- */
		virtual auto TAG() const -> string
		{
			return "point";
		};

		auto toXML() const -> shared_ptr<XML> override
		{
			shared_ptr<XML> &xml = super::toXML();
			xml->setProperty("uid", uid);
			xml->setProperty("longitude", longitude);
			xml->setProperty("latitude", latitude);

			return xml;
		};

		/**
		 * @brief Convert the Branch to String
		 *
		 * @details Have of form of tab and enter delimeters for Excel.
		 *  \li <pre> {$uid}	{$longitude}	{$latitude} </pre>
		 *
		 * @return A string represents the GeometryPoint
		 */
		auto toString() const -> string
		{
			return StringUtil::substitute
				(
					"{1}\t{2}\t{3}",
					uid, longitude, latitude
				);
		};
	};			
};
};
};