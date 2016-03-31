#include <bws/packer/Product.hpp>

using namespace std;
using namespace bws::packer;
using namespace samchon::library;
using namespace samchon::protocol;

auto Product::TYPE() const -> string
{
	return "product";
}