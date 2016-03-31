#pragma once
#include <samchon/API.hpp>

#include <map>
#include <samchon/IndexPair.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief Utility of Math
	 *
	 * @details
	 * <p> Provides some static values and mathmatical methods. </p>
	 *
	 * @image html cpp/library_math.png
	 * @image latex cpp/library_math.png
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API Math
	{
	public:
		/* ========================================================
			STATIC VARIABLES
		======================================================== */
		/**
		 * @brief Exponent
		 *
		 * @details
		 * A mathematical constant for the base of natural logarithms,
		 * expressed as e. The approximate value of e is 2.71828182845905.
		 */
		static const double E;

		/**
		 * @brief ¥ð, Number Pi
		 *
		 * @details
		 * A mathematical constant for the ratio of the circumference of a circle to
		 * its diameter, expressed as pi, with a value of 3.141592653589793.
		 */
		static const double PI;

		/**
		 * @brief log<sub>e</sub>2
		 *
		 * @details
		 * A mathematical constant for the natural logarithm of 2, expressed as
		 * log<sub>e</sub>2, with an approximate value of 0.6931471805599453.
		 */
		static const double LN2;

		/**
		 * @brief log<sub>e</sub>10
		 *
		 * @details
		 * A mathematical constant for the natural logarithm of 10, expressed as
		 * log<sub>e</sub>10, with an approximate value of 2.302585092994046.
		 */
		static const double LN10;

		/**
		 * @brief log<sub>2</sub>e
		 *
		 * @details
		 * A mathematical constant for the base-2 logarithm of the constant e,
		 * expressed as log<sub>2</sub>e, with an approximate value of 1.442695040888963387.
		 */
		static const double LOG2E;

		/**
		 * @brief log<sub>10</sub>e
		 *
		 * @details
		 * A mathematical constant for the base-10 logarithm of the constant e (Math.E),
		 * expressed as log<sub>10</sub>e, with an approximate value of 0.4342944819032518.
		 * The Math.log() method computes the natural logarithm of a number.
		 * Multiply the result of Math.log() by Math.LOG10E to obtain the base-10 logarithm.
		 */
		static const double LOG10E;


		/**
		 * @brief Squart root of 0.5
		 *
		 * @details
		 * A mathematical constant for the square root of one-half,
		 * with an approximate value of 0.7071067811865476.
		 */
		static const double SQRT1_2;

		/**
		 * @brief Squart root of 2
		 *
		 * @details
		 * A mathematical constant for the square root of 2,
		 * with an approximate value of 1.4142135623730951.
		 */
		static const double SQRT2;

		/* ========================================================
			RANDOM
		======================================================== */
		/**
		 * @brief Get a random value
		 *
		 * @details Returns a random value following uniform distribution
		 * @return The random value
		 */
		static auto random() -> double;

		/* ========================================================
			PIE
		======================================================== */
		/**
		 * @brief Convert degree to radian
		 */
		static auto degree_to_radian(double) -> double;

		/**
		 * @brief Convert radian to degree
		 */
		static auto radian_to_degree(double) -> double;

		/* ========================================================
			STATISTICS
		======================================================== */
		/**
		 * @brief Calculate minimum value with its index
		 *
		 * @tparam _Cont Type of the container
		 * @tparam _Ty
		 *	<p> Value type of the container. </p>
		 *	<p> Do not modify the default template argument. </p>
		 *
		 * @param container Container to find its minimum value
		 * @return Pair of minimum value and its index
		 */
		template <typename _Cont, typename T = _Cont::value_type>
		static auto minimum(const _Cont &container) -> IndexPair<T>
		{
			auto it = container.begin();
			size_t i = 0, position = 0;

			const T *ptr = &(*it);

			for (; it != container.end(); it++)
			{
				if (*it < *ptr)
				{
					ptr = &(*it);
					position = i;
				}
				i++;
			}
			return{ position, *ptr };
		};

		/**
		 * @brief Calculate maximum value with its index
		 *
		 * @tparam _Cont Type of the container
		 * @tparam _Ty
		 *	<p> Value type of the container. </p>
		 *	<p> Do not modify the default template argument. </p>
		 *
		 * @param container Container to find its minimum value
		 * @return Pair of maximum value and its index
		 */
		template <typename _Cont, typename T = _Cont::value_type>
		static auto maximum(const _Cont &container) -> IndexPair<T>
		{
			auto it = container.begin();
			size_t i = 0, position = 0;

			const T *ptr = &(*it);

			for (; it != container.end(); it++)
			{
				if (*it > *ptr)
				{
					ptr = &(*it);
					position = i;
				}
				i++;
			}
			return{ position, *ptr };
		};

		/**
		 * @brief Calculate average
		 *
		 * @tparam _Cont Type of the container
		 * @tparam _Ty
		 *	<p> Value type of the container. </p>
		 *	<p> Do not modify the default template argument. </p>
		 *
		 * @param container Container to find its mean
		 * @return Average(mean) of the contaier
		 */
		template <typename _Cont, typename T = _Cont::value_type>
		static auto mean(const _Cont &container) -> double
		{
			double val = 0.0;
			for (auto it = container.begin(); it != container.end(); it++)
				val += *it;

			return val / (double)container.size();
		};

		/**
		 * @brief Get median value
		 *
		 * @tparam _Cont Type of the container
		 * @tparam _Ty
		 *	<p> Value type of the container. </p>
		 *	<p> Do not modify the default template argument. </p>
		 *
		 * @param container Container to find its median
		 * @return Median value of the container
		 */
		template <typename _Cont, typename T = _Cont::value_type>
		static auto median(const _Cont &container) -> double
		{
			auto it = container.begin();

			if (container.size() % 2 == 1)
			{
				advance(it, container.size() / 2);

				return (double)*it;
			}
			else
			{
				advance(it, container.size() / 2 - 1);
				auto next_it = it;
				next_it++;

				return ((double)*it + (double)*next_it) / 2.0;
			}
		};

		/**
		 * @brief Find mode value
		 *
		 * @tparam _Cont Type of the container
		 * @tparam _Ty
		 *	<p> Value type of the container. </p>
		 *	<p> Do not modify the default template argument. </p>
		 *
		 * @param container Container to find its mode
		 * @return Most appeared value in the container
		 */
		template <typename _Cont, typename T = _Cont::value_type>
		static auto mode(const _Cont &container) -> T
		{
			std::map<T, size_t> frequencyMap;
			for (auto it = container.begin(); it != container.end(); it++)
				if (frequencyMap.find(*it) == frequencyMap.end())
					frequencyMap[*it] = 1;
				else
					frequencyMap[*it]++;

			auto det_it = frequencyMap.begin();
			for (auto it = frequencyMap.begin(); it != frequencyMap.end(); it++)
				if (it->second > det_it->second)
					det_it = it;

			return det_it->first;
		};

		/**
		 * @brief Square root of a variance_p
		 *
		 * <p> \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$ </p>
		 * @copydetails variance_p()
		 */
		template <typename _Cont, typename T = _Cont::value_type>
		static auto stdev_p(const _Cont &container) -> double
		{
			return sqrt(variance_p(contaier));
		};

		/**
		 * @brief Square root of a variance_s
		 *
		 * @copydetails variance_s()
		 */
		template <typename _Cont, typename T = _Cont::value_type>
		static auto stdev_s(const _Cont &container) -> double
		{
			return sql(variance_s(container));
		};

		/**
		 * @brief Calculate standard deviation for all population
		 *
		 * @tparam _Cont Type of the container
		 * @tparam _Ty
		 *	<p> Value type of the container. </p>
		 *	<p> Do not modify the default template argument. </p>
		 *
		 * @param container Container to find its standard deviation
		 * @return Standard deviation of the container
		 */
		template <typename _Cont, typename T = _Cont::value_type>
		static auto variance_p(const _Cont &container) -> double
		{
			double val = 0.0;
			double mean = Math::mean(container);

			for (auto it = container.begin(); it != container.end(); it++)
				val += *it - mean;

			return val / (double)container.size();
		};

		/**
		 * @brief Calculate variance for sample
		 *
		 * @tparam _Cont Type of the container
		 * @tparam _Ty
		 *	<p> Value type of the container. </p>
		 *	<p> Do not modify the default template argument. </p>
		 *
		 * @param container Container to find its variance
		 * @return Variance of the container
		 */
		template <typename _Cont, typename T = _Cont::value_type>
		static auto variance_s(const _Cont &container) -> double
		{
			double val = 0.0;
			double mean = Math::mean(container);

			for (auto it = container.begin(); it != container.end(); it++)
				val += *it - mean;

			return val / (double)(container.size() - 1);
		};
	};
};
};