#pragma once

#include <iostream>
#include <samchon/library/HTTPLoader.hpp>

namespace samchon
{
namespace examples
{
namespace http
{
	void main()
	{
		using namespace std;
		using namespace samchon::library;

		HTTPLoader loader("http://samchon.org/simulation/php/corporate/list.php", HTTPLoader::GET);
		URLVariables data;
		{
			data["c"] = "order";
			data["a"] = "outsourcing";
			data["page"] = "2";
		}

		//cout << "#size: " << loader.load(data).size() << endl;
		string &str = loader.load(data).read<string>();
		cout << str;
	};
};
};
};