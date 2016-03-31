#include <bws/packer/GAWrapperArray.hpp>

#include <bws/packer/Packer.hpp>
#include <bws/packer/WrapperGroup.hpp>
#include <bws/packer/InstanceArray.hpp>

#include <iostream>

using namespace std;
using namespace bws::packer;
using namespace samchon::library;
using namespace samchon::protocol;

/* -----------------------------------------------------------
	CONSTRUCTORS
----------------------------------------------------------- */
GAWrapperArray::GAWrapperArray(std::shared_ptr<InstanceArray> instanceArray)
	: super()
{
	this->instanceArray = instanceArray;
	valid = true;
	price = 0.0;
}
GAWrapperArray::GAWrapperArray(const GAWrapperArray &obj)
	: GAWrapperArray(obj.instanceArray)
{
	assign(obj.begin(), obj.end());
}

void GAWrapperArray::constructResult()
{
	if (result.empty() == false)
		return;
	
	// 제품과 포장지 그룹, Product와 WrapperGroup의 1:1 매칭
	for (size_t i = 0; i < size(); i++)
	{
		const shared_ptr<Wrapper> &wrapper = at(i);
		if (result.count(wrapper->key()) == 0)
		{
			WrapperGroup *wrapperGroup = new WrapperGroup(wrapper);
			result.insert({ wrapper->key(), shared_ptr<WrapperGroup>(wrapperGroup) });
		}

		shared_ptr<WrapperGroup> wrapperGroup = result.at(wrapper->key());
		shared_ptr<Instance> instance = instanceArray->at(i);
		
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
}

/* -----------------------------------------------------------
	GETTERS
----------------------------------------------------------- */
auto GAWrapperArray::getResult() const -> const unordered_map<string, shared_ptr<WrapperGroup>>&
{
	((GAWrapperArray*)this)->constructResult();

	return result;
}

auto GAWrapperArray::operator<(const GAWrapperArray &obj) const -> bool
{
	((GAWrapperArray*)this)->constructResult();
	((GAWrapperArray&)obj) . constructResult();

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
}