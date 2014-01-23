#include <iostream>
#include <fstream>
#include <memory>
#include <cctype>
#include <map>
#include <regex>

using namespace std;

/*
mali sme vytvorit telefonny zoznam podporujuci nasledovne prikazy
a Jan Novak 775436985 // a=add
a Marek Tlusty 12345 
a Jan Novak 074580 // Janov Novakov je viac, pridam dalsieho
a Jan Novak 074580 // pozor nato, aby mi nepridalo dvakrat tu istu osobu
d Jan Novak 074580 // d=delete. Cloveka jednoznacne identifikujem az ked viem aj jeho cislo
? No*k //?=query najde vsetky polozky, ktore viem namatchovat na tento "regular"
q // q = quit

teda vyhladavanie je podla prijmeni a dokaze vyhladavat s pouzitim hviezdicky. Hviezdicka sa vo vyraze moze vyskytnut maximalne raz.
Zastupuje lubovolny pocet lubovolnych znakov
*/

class Person
{
public:
	Person (int id, string &name, string &surname)
		: id (id), name (name), surname (surname) {};

	const int id;
	const string name;
	const string surname;
};

multimap<string, Person> people;

void printPeople ()
{
	cout << "----------------" << endl;
	for (auto x : people) {
		auto p = x.second;
		cout << "- " << p.name << "\t" << p.surname << "\t" << p.id << endl;
	}
}

bool exist (const Person &p)
{
	auto range = people.equal_range(p.surname);

	for (auto it = range.first; it != range.second; ++it) {
		auto res = it->second;
		if (res.id == p.id && res.name == p.name && res.surname == p.surname) {
			return true;
		}
	}

	return false;
}

void add (const Person &p)
{
	people.insert( {p.surname, p} );
}

void del (const Person &p)
{
	auto range = people.equal_range(p.surname);

	for (auto it = range.first; it != range.second; ++it) {
		auto res = it->second;
		if (res.id == p.id && res.name == p.name && res.surname == p.surname) {
			people.erase(it);
			break;
		}
	}
}

void printBySurname(const string surname)
{
	cout << "---------------" << endl;

	const int pos = surname.find("*");
	const int length = surname.size();

	if (pos == -1) {	// regular
		auto range = people.equal_range(surname);

		for (auto it = range.first; it != range.second; ++it) {
			auto p = it->second;
			cout << surname << ")\t" << p.name << "\t" << p.surname << "\t" << p.id << endl;
		}
		return;
	}

	string token = surname.substr(0, pos) + ".*" + surname.substr(pos+1, length);
	cout << token << endl;

	regex reg(token);
	for (auto it = people.begin(); it != people.end(); ++it) {
		if (regex_match(it->first, reg)) {
			auto p = it->second;
			cout << p.name << "\t" << p.surname << "\t" << p.id << endl;
		}
	}
}

int main (int argc, char ** argv)
{
	string command, name, surname, nr, query;

	while (true) {
		cin >> command;

		if (command == "a") {
			cin >> name >> surname >> nr;

			Person p(atoi(nr.c_str()), name, surname);

			if (exist(p)) {
				cout << "a) collision:\t" << p.name << "\t" << p.surname << "\t" << p.id << endl;
				continue;
			}

			add(p);
			cout << "a) added:\t" << p.name << "\t" << p.surname << "\t" << p.id << endl;
		}
		else if (command == "d") {
			cin >> name >> surname >> nr;

			Person p(atoi(nr.c_str()), name, surname);

			if (!exist(p)) {
				cout << "d) not found:\t" << p.name << "\t" << p.surname << "\t" << p.id << endl;
				continue;
			}

			del(p);
			cout << "d) deleted:\t" << p.name << "\t" << p.surname << "\t" << p.id << endl;

		}
		else if (command == "?") {
			cin >> surname;
			cout << endl << "q)\t" << surname << endl;
			printBySurname(surname);
		}
		else if (command == "q") {
			printPeople();
			break;
		}
		else if (command[0] == '#') {
			cout << "ignoring" << endl;
			cin >> name >> surname >> nr;
			continue;
		}
		else {
			cout << "unknown command '" << command << "'" << endl;
		}
	}

	return 0;
}