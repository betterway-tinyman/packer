#pragma once

#include <iostream>
#include <samchon/library/XML.hpp>

using namespace std;
using namespace samchon::library;

namespace samchon
{
namespace examples
{
namespace xml
{
	struct Member
	{
		std::string id;
		std::string name;
		int age;
		int grade;

		auto toXML() const -> std::shared_ptr<library::XML>
		{
			std::shared_ptr<library::XML> xml(new library::XML());
			xml->setTag("member");

			xml->setProperty("id", id);
			xml->setProperty("name", name);
			xml->setProperty("age", age);
			xml->setProperty("grade", grade);

			return xml;
		};
	};

	void main()
	{
		using namespace std;
		using namespace samchon::library;

		//DECLARE An XML WITH MEMBER_ARRAY TAG
		shared_ptr<XML> xml(new XML());
		xml->setTag("memberArray");

		//ADD MEMBER(S) TO XML
		vector<struct Member> memberArray =
		{
			{ "samchon", "Jeongho Nam", 27, 1 },
			{ "gkyu", "Kwangkyu Ko", 25, 1 },
			{ "guest", "John Doe", 99, 4 }
		};
		for (size_t i = 0; i < memberArray.size(); i++)
			xml->push_back(memberArray[i].toXML());

		//ADD FILE_LIST BY STRING DIRECTLY
		xml->push_back
		(
			string("") +
			"<fileList>\n" +
			"<file extension='pdf' name='API' />" +
			"<file extension='pdf' name='Guidance+For+Developer'>Damaged</file>" +
			"<file extension='docx' name='Resume' />" +
			"<file extension='jpg' name='My+House' />" +
			"<file extension='xlsx' name='Grades' />" +
			"</fileList>"
		);

		cout << "-----------------------------------------------------------------" << endl;
		cout << " Get Properties And Values" << endl;
		cout << "-----------------------------------------------------------------" << endl;
		cout << "Age of 2nd member: " << xml->get("member")->at(1)->getProperty<int>("age") << endl;
		cout << "Age of 1st member: " << xml->get("member")->at(0)->getProperty("id") << endl << endl;

		cout << "File name and extension of 5th: "
			<< xml->get("fileList")->at(0)->get("file")->at(4)->getProperty("name") << "."
			<< xml->get("fileList")->at(0)->get("file")->at(4)->getProperty("extension") << endl;
		cout << "Value of 2nd file: " << xml->get("fileList")->at(0)->get("file")->at(1)->getValue() << endl << endl;

		cout << "-----------------------------------------------------------------" << endl;
		cout << " XML to String: " << endl;
		cout << "-----------------------------------------------------------------" << endl;
		cout << xml->toString() << endl;
	};
};
};
};