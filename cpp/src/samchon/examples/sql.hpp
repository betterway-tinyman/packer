#pragma once

#include <iostream>
#include <samchon/library/TSQLi.hpp>

using namespace std;
using namespace samchon::library;

namespace samchon
{
namespace examples
{
namespace sql
{
	void main()
	{
		using namespace std;
		using namespace samchon::library;

		shared_ptr<SQLi> sqli(new TSQLi());

		try
		{
			sqli->connect("211.239.161.211", "REDPRINTING_TEST", "redprinting_test", "redprinting");

			auto stmt = sqli->createStatement();
			size_t uid = 5;
			wstring name = L"고길동";
			wstring department = L"홍보부";

			stmt->prepare(L"INSERT INTO 한글테이블 (번호, 이름, 부서) VALUES (?, ?, ?)", uid, name, department);
			stmt->execute();

			stmt->executeDirectly(u8"SELECT * FROM 한글테이블");

			while (stmt->fetch())
			{
				int uid = stmt->at<int>(0);
				const string &name = stmt->at<string>(1);
				const string &department = stmt->at<string>(2);

				cout << uid << endl;
				cout << name << endl;
				cout << department << endl;
			}
		}
		catch (exception &e)
		{
			cout << e.what() << endl;
		}
	};
}
}
}