#include <iostream>
#include <stack>
#include <string>
#include <string.h>
#include <map>
#include <regex>

using namespace std;

bool is_number(const string& s)
{
    auto it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

class Exception {};

class Scope
{
public:
	int get (string name) const
	{
		auto it = vars.find(name);
		if (it != end(vars)) {
			return it->second;
		}
		return 0;
	}
	
	void set (string name, int value) 
	{
		auto it = vars.find(name);
		if (it != end(vars)) {
			it->second = value;
		}
		else {
			vars.insert(make_pair(name, value));
		}
	}
	
	void print (ostream & out) const
	{
		for (auto it : vars) {
			out << it.first << " " << it.second << endl;
		}
	}
protected:
	map<string, int> vars;
};

class Tree
{
public:

	Tree (const string &orig, const Scope &scope)
		: scope_ (scope)
	{
		string inf = trimSpaces(orig);
		val_ = parse(inf);

		if (val_.size() == 0) throw Exception();
		else if (val_ == "+") {
			type_ = PLUS;
		}
		else if (val_ == "-") {
			type_ = MINUS;
		}
		else if (val_ == "*") {
			type_ = TIMES;
		}
		else if (val_ == "/") {
			type_ = DIVIDE;
		}
		else if (is_number(val_)) {
			type_ = NUM;
		}
		else {
			type_ = VAR;
		}
	};

	void print ()
	{
		if (left_ == NULL || right_ == NULL) {
			cout << val_;
			return;
		}

		cout << "(";
		left_->print();
		cout << ") ";

		cout << val_;

		cout << "(";
		right_->print();
		cout << ")";
	};

	int evaluate ()
	{
		if (type_ == NUM) {
			return atoi(val_.c_str());
		}
		else if (type_ == VAR) {
			return scope_.get(val_);
		}
		else if (type_ == PLUS) {
			return left_->evaluate() + right_->evaluate();
		}
		else if (type_ == MINUS) {
			return left_->evaluate() - right_->evaluate();
		}
		else if (type_ == TIMES) {
			return left_->evaluate() * right_->evaluate();
		}
		else if (type_ == DIVIDE) {
			return left_->evaluate() / right_->evaluate();
		}
		return 0;
	};

	virtual ~Tree() {
		if (type_ == NUM || type_ == VAR) return;
		delete left_;
		delete right_;
	};

protected:
	//	get value
	string parse (string &inf)
	{
		int bestBig = -1;
		int bestSmall = -1;
		int opts = 0;

		int depth = 0;
		
		for (size_t i = 0; i < inf.size(); i++) {
			switch (inf[i]) {

				case '+': case '-':
					opts++;
					if (depth == 0) {
						bestSmall = i;
					}
					break;

				case '*': case '/':
					opts++;
					if (depth == 0) {
						bestBig = i;
					}
					break;

				case '(':
					depth++;
					break;

				case ')':
					depth--;
					if (depth < 0) throw Exception();
					break;

				default:
					break;
			}
		}

		if (depth > 0) throw Exception();

		if (opts == 0) {
			if (inf[0] == '(' && inf[inf.size()-1] == ')') {
				inf = inf.substr(1, inf.size()-2);
				return parse(inf);
			}
			// pokud obsahuje závorku teď, exception
			regex rl(".*\\(.*");
			regex rr(".*\\).*");
			if (regex_match(inf, rl) || regex_match(inf, rr)) throw Exception();

			left_ = NULL;
			right_ = NULL;
			return inf;
		}

		string res;
		if (bestSmall != -1) {	//	nejmenší priorita +-
			left_ = new Tree(inf.substr(0, bestSmall), scope_);
			right_ = new Tree(inf.substr(bestSmall + 1, inf.size()), scope_);
			res += inf[bestSmall];
			return res;
		}
		else if (bestBig != -1) {	//	nejmenší priorita 
			left_ = new Tree(inf.substr(0, bestBig), scope_);
			right_ = new Tree(inf.substr(bestBig + 1, inf.size()), scope_);
			res += inf[bestBig];
			return res;
		}
		else {	// celé v závorkách
			inf = inf.substr(1, inf.size()-2);
			return parse(inf);
		}
	};

	string trimSpaces (const string& inf)
	{
		string res;
		for (size_t i = 0; i < inf.size(); i++) {
			if (inf[i] != ' ') res += inf[i];
		}
		return res;
	};

	enum Type { NUM, VAR, PLUS, MINUS, TIMES, DIVIDE };

	Tree * left_;
	Tree * right_;
	string val_;
	Type type_;
	Scope scope_;

};


int main(int argc, char* argv[])
{
	Scope s;
	s.set("ABC", 125);
	string inf = "((((12/2+1))-15+ABC))";

	Tree t(inf, s);
	t.print();
	cout << " = " << t.evaluate() << endl;

	return 0;
}
