#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <cctype>

struct Scope {
public:
	int get (std::string name) const
	{
		auto it = vars.find(name);
		if (it != end(vars)) {
			return it->second;
		}
		return 0;
	}
	
	int set (std::string name, int value) 
	{
		auto it = vars.find(name);
		if (it != end(vars)) {
			it->second = value;
		} else {
			vars.insert(std::make_pair(name, value));
		}
	}
	
	void print (std::ostream & out) const
	{
		for (auto it: vars) {
			out << it.first << " " << it.second << std::endl;
		}
	}
private:
	std::map<std::string, int> vars;
};

typedef std::vector<std::string> program_t;

void load_file (std::string filename, program_t & program)
{
	std::ifstream input(filename);
	
	std::string line;
	while (!std::getline(input, line).fail()) {
		if (line[0] == '#') {
			load_file(line.substr(1), program);
		} else {
			program.push_back(line);
		}
	}
	
	input.close();
}

struct Parser {
public:
	Parser (std::string & line, size_t position) : line_(line), current_(position)
	{
		next();
	}
	
	int parse (const Scope & scope)
	{
		return add(scope);
	}
private:
	int add (const Scope & scope)
	{
		int value = mul(scope);
		while (current() == '+') {
			next();
			value += mul(scope);
		}
		return value;
	}
	
	int mul (const Scope & scope)
	{
		int value = val(scope);
		while (current() == '*') {
			next();
			value *= val(scope);
		}
		return value;
	}
	
	int val (const Scope & scope)
	{
		int sign = 1;
		int value = 0;
		if (current() == '-') {
			sign = -1;
			next();
		}
		if (isdigit(current())) {
			while (isdigit(current())) {
				value = 10 * value + (current() - '0');
				next();
			}
			return sign * value;
		}
		if (current() == '(') {
			next();
			value = add(scope);
			if (current() != ')') {
				throw std::exception();
			}
			next();
			return sign * value;
		}
		
		std::string name = "";
		while (isalnum(current())) {
			name += current();
			next();
		}
		return sign * scope.get(name);
	}
	
	char current () const
	{
		return line_[current_];
	}
	
	void next ()
	{
		current_ = current_ + 1;
		if (isspace(current())) {
			next();
		}
	}
	
	std::string line_;
	size_t current_;
};

int step (program_t & program, Scope & scope, size_t line_number)
{
	std::string line = program[line_number];
	size_t pos;
	
	if ((pos = line.find('?')) != std::string::npos) {
		std::string name = "";
		for (size_t i = 0; i < pos; i++) {
			if (isalpha(line[i])) {
				name += line[i];
			}
		}
		if (scope.get(name)) {
			size_t jump = 0;
			int sign = 1;
			for (size_t i = pos; i < line.length(); i++) {
				if (line[i] == '-') {
					sign = -sign;
				} else if (isdigit(line[i])) {
					jump = 10 * jump + (line[i] - '0');
				}
			}
			return line_number + sign * jump;
		}
	}
	if ((pos = line.find('=')) != std::string::npos) {
		std::string name = "";
		for (size_t i = 0; i < pos; i++) {
			if (isalpha(line[i])) {
				name += line[i];
			}
		}
		scope.set(name, Parser(line, pos).parse(scope));
	}
	return line_number + 1;
}

int main (int argc, char **argv) 
{
	program_t program;
	Scope scope;
	load_file("input.txt", program);
	size_t line = 0;
	size_t end = program.size();
	
	while ((line = step(program, scope, line)) != end) {
		// Do evil stuff
	}
	
	scope.print(std::cout);
	
	return 0;
}
