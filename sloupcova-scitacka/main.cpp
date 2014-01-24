#include <iostream>
#include <fstream>
#include <memory>
#include <cctype>
#include <map>
#include <regex>
#include <vector>

using namespace std;

void err (int exitcode, string msg)
{
	cout << msg << endl;
	exit(exitcode);
}


class AbstractStorage {
public:
	virtual void print () = 0;
	virtual void print (ofstream &f) = 0;
};

template< class T >
class Storage : public AbstractStorage
{
public:
	explicit Storage (T value) : value_ (value) {};
	//{
	/*cout << "coonstructed with" << value << endl;*/
		//value_ = new T(value);
	//};
	virtual void print () {cout << value_ << endl;};
	virtual void print (ofstream &f) {f << value_ << endl;};
	T value_;
};


template< typename T >
//void add (AbstractStorage * res, AbstractStorage * obj)
void add (AbstractStorage * res, AbstractStorage * obj)
{

	/*
	cout << endl<< "---" << endl <<"add called to" << endl;
	res->print(); obj->print();
	cout << "---" << endl;
	*/
	//res[index]->print();
	
	//res->print();
	//obj->print();

	Storage<T> * resSt = dynamic_cast< Storage<T> * >(res);
	Storage<T> * objSt = dynamic_cast< Storage<T> * >(obj);

	//cout << "conv ok" << endl;
	//resSt->print();
	//obj->print();
	/*
	cout << objSt->value_ << endl;
	cout << resSt->value_ << endl;

	cout << "expected: " << objSt->value_ + resSt->value_ << endl;
	*/
	resSt->value_ = resSt->value_ + objSt->value_;
	/*
	cout << "real: " << resSt->value_ << endl;
	cout << "saved: "; res->print();
	*/

}



vector<string> explode (string line, char del)
{
	vector<string> res;
	string s = "";
	for (int i = 0; i < line.size(); i++) {
		if (line[i] == del) {
			res.push_back(s);
			s = "";
		}
		else {
			s += line[i];
		}
	}
	return res;
}


int main (int argc, char ** argv)
{
	//vector<char> format_type;
	vector<int> format_pos;

	map<int, char> format;
	for (int i = 0; i <=10; i++) {
		format.insert( {i, 'S'} );
	}

	string inputFile = "", outputFile = "";
	char delim = ' ';

	vector< AbstractStorage * > sum, newLine;
	for (int i = 0; i <= 10; i++) {
		sum.push_back(new Storage<string>(""));
		format.insert( {i, 'S'} );
	}

	int k;
	for (int i = 1; i < argc; i++) {
		string arg = string(argv[i]);
		//cout << arg << endl;

		if (arg == "-i") {
			inputFile = string(argv[i+1]);
			i++;
			continue;
		}
		if (arg == "-o") {
			outputFile = string(argv[i+1]);
			i++;
			continue;
		}
		if (arg[0] == '-' && arg[1] == 't') {
			delim = (arg[2]);
			continue;
		}

		//cout << "->" << i << endl;
		k = (int)((char)arg[1] - '0');
		delete sum[k];

		switch (arg[0]) {
			case 'I':

				//cout << arg[0] << " " << arg[1] <<  " " << (int)((char)arg[1] - '0')<< endl;
				sum[k] = new Storage<int>(0);
				format[k] = 'I';
				//cout << k << " as integer" << endl;
				break;
				//format[]
			case 'S':
				//cout << arg[0] << " " << arg[1] <<  " " << (int)((char)arg[1] - '0')<< endl;
				//k = (int)((char)arg[1] - '0');
				//delete sum[k];
				sum[k] = new Storage<string>("");
				format[k] = 'S';
				//cout << k << " as string" << endl;
				break;
		}

		//format_type.push_back(arg[0]);
		format_pos.push_back(k);

	}

	//for ()

/*
	cout << inputFile << " " << outputFile << " '" << delim << "'" << endl;

	for (int i = 0; i<format_type.size(); i++) {
		printf("%c %d\n", format_type[i], format_pos[i]);
	}
*/





	ifstream f;
	f.open(inputFile, ifstream::in);

	string line;

	while (getline(f, line)) {
		//cout << line << "|";

		//cout << "loaded: " << line << endl;
		auto cols = explode(line, delim);

		for (int i = 0; i < cols.size(); i++) {
			//cout << "for i=" << i << endl;

			/*
			cout << "stat" << endl;
			for (int j = 0; j <10; j++) {
				sum[j]->print();
			}
			cout << "/stat" << endl;
			*/

			auto it = format.find(i);	// always find
			if (it == format.end()) {
				err(1, "invalid arg.");
			}

			//cout << "... "<<it->second<<endl;

			if (it->second == 'S') {
				Storage<string> * tmpS = new Storage<string>(cols[i]);
				//cout << "read "; tmpS->print();
				add<string>(sum[i], tmpS);
				//cout << "after: "; sum[i]->print();
				delete tmpS;
			}
			else if (it->second == 'I') {
				//printf("'%s' -> %d\n", cols[i].c_str(), atoi(cols[i].c_str()));
				Storage<int> * tmpI = new Storage<int>(atoi(cols[i].c_str()));
				add<int>(sum[i], tmpI);
				//cout << "after: "; sum[i]->print();
				delete tmpI;
			}
		}

	}
	f.close();

	if (outputFile == "") {

		cout << "result:" << endl;
		for (auto it = format_pos.begin(); it != format_pos.end(); ++it) {
			cout << *it << " ";
			sum[*it]->print();
		}
	}
	else {
		cout << "into file" << endl;
		ofstream g;
		g.open(outputFile, ofstream::out);
		for (auto it = format_pos.begin(); it != format_pos.end(); ++it) {
			sum[*it]->print(g);
		}
		g.close();
	}

	return 0;
}