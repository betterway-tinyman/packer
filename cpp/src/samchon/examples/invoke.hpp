#pragma once

#include <iostream>
#include <samchon/protocol/Invoke.hpp>

namespace samchon
{
namespace examples
{
namespace invoke
{
	void main()
	{
		using namespace std;
		using namespace samchon::library;
		using namespace samchon::protocol;

		string str = string("") +
			"<memberList>\n" +
			"	<member id='jhnam88' pass='1231' />\n" +
			"	<member id='samchon' pass='1231'>Administrator</member>\n" +
			"	<group>3</group>\n" +
			"</memberList>";

		shared_ptr<XML> xml(new XML(str));
		cout << xml->toString() << endl;

		shared_ptr<Invoke> invoke(new Invoke("login", "jhnam88", "1231", 4, xml));

		cout << "Invoke to XML: " << endl;
		cout << invoke->toXML()->toString() << endl << endl;

		cout << "-------------------------------------------------------------" << endl;
		cout << "	Parameters" << endl;
		cout << "-------------------------------------------------------------" << endl;
		cout << "1st param: " << invoke->at(0)->getValue<string>() << endl;
		cout << "2nd param: " << invoke->at(1)->getValue<string>() << endl;
		cout << "3rd param: " << invoke->at(2)->getValue<string>() << endl;
		cout << "4th param: " << endl << invoke->at(3)->getValue<shared_ptr<XML>>()->toString() << endl;

		cout << shared_ptr<XML>(new XML(invoke->toXML()->toString()))->toString() << endl;
	};
};
};
};