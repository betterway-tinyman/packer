#pragma once
#include <bws/packer/API.hpp>

#include <bws/packer/WrapperArray.hpp>

#include <unordered_map>
#include <bws/packer/WrapperGroup.hpp>
#include <bws/packer/InstanceArray.hpp>

namespace bws
{
namespace packer
{
	/**
	 * @brief Sequence list of Wrapper objects.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class GAWrapperArray
		: public WrapperArray
	{
	private:
		typedef WrapperArray super;

	protected:
		/**
		 * @brief Instance objects to be wrapped.
		 */
		std::shared_ptr<InstanceArray> instanceArray;
		
		std::unordered_map<std::string, std::shared_ptr<WrapperGroup>> result;

		/**
		 * @brief Price of this sequence list.
		 *
		 * @details It stores price deducted by the packing process to block repeated operation.
		 */
		double price;

		/**
		 * @brief Validity of this sequence list.
		 *
		 * @details It stores validity of the sequence list to block repeated operation.
		 */
		bool valid;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Construct from instances.
		 *
		 * @param instanceArray Instances to be wrapped.
		 */
		GAWrapperArray(std::shared_ptr<InstanceArray> instanceArray)
			: super()
		{
			this->instanceArray = instanceArray;
			valid = true;
			price = 0.0;
		};
		
		/**
		 * @brief Copy Constructor.
		 */
		GAWrapperArray(const GAWrapperArray &obj)
			: GAWrapperArray(obj.instanceArray)
		{
			assign(obj.begin(), obj.end());
		};

		virtual ~GAWrapperArray() = default;

	protected:
		/**
		 * @brief Construct result, by packing process.
		 */
		void constructResult()
		{
			if (result.empty() == false)
				return;

			// 제품과 포장지 그룹, Product와 WrapperGroup의 1:1 매칭
			for (size_t i = 0; i < size(); i++)
			{
				const std::shared_ptr<Wrapper> &wrapper = at(i);
				if (result.count(wrapper->key()) == 0)
				{
					WrapperGroup *wrapperGroup = new WrapperGroup(wrapper);
					result.insert({ wrapper->key(), std::shared_ptr<WrapperGroup>(wrapperGroup) });
				}

				std::shared_ptr<WrapperGroup> wrapperGroup = result.at(wrapper->key());
				std::shared_ptr<Instance> instance = instanceArray->at(i);

				if (wrapperGroup->allocate(instance) == false)
				{
					// 일개 제품 크기가 포장지보다 커서 포장할 수 없는 경우, 
					// 현재의 염기서열은 유효하지 못하여 폐기됨
					valid = false;
					return;
				}
			}

			// 유효한 염기서열일 때,
			for (auto it = result.begin(); it != result.end(); it++)
			{
				it->second->optimize(); // 세부적(그룹별)으로 bin-packing을 실시함
				price += it->second->getPrice(); // 더불어 가격도 합산해둔다
			}
			valid = true;
		};

	public:
		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * @brief Get optimization result.
		 *
		 * @return Const reference of result map.
		 */
		auto getResult() const -> const std::unordered_map<std::string, std::shared_ptr<WrapperGroup>>&
		{
			((GAWrapperArray*)this)->constructResult();

			return result;
		};

		/**
		 * @brief Whether this sequence list is inferior?
		 */
		auto operator<(const GAWrapperArray &obj) const -> bool
		{
			((GAWrapperArray*)this)->constructResult();
			((GAWrapperArray&)obj).constructResult();

			if (valid == true && obj.valid == true)
			{
				// 가격이 적은 것이 우선
				return this->price < obj.price;
			}
			else if (valid == true && obj.valid == false)
			{
				// 유효한 것이 우선
				return true;
			}
			else
				return false;
		};
	};
};
};