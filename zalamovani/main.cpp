#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#define LEN 10

class Printer {
public:
	Printer (std::ifstream & dict, std::ofstream & out): out_(out) 
	{
		std::string def;
		while (!dict.eof()) {
			dict >> def;
			std::string word = "";
			size_t pos = 0;
			wordData data;
			for (size_t i = 0; i < def.length(); ++i) {
				char ch = def[i];
				if (ch == '-') {
					data.push_back(pos);
				} else {
					word += ch;
					pos += 1;
				}
			}
			breaks.insert(std::make_pair(word, data));
		}
	}
	void printWord (std::string word, size_t from = 0) 
	{
		size_t max = LEN - cursor;
		size_t best = 0;
		std::string sub = word.substr(from);
		
		if (sub.length() <= max) {
			best = sub.length();
		} else {
			auto it = breaks.find(word);
			if (it != end(breaks)) {
				wordData data = it->second;
				for (size_t item: data) {
					size_t pos = item - from;
					if (pos <= max && pos > best) {
						best = pos;
					}
				}
			}
		}
		
		if (best == 0) {
			if (cursor == 0) {
				out_ << sub;
				cursor += sub.length();
			} else {
				out_ << std::endl;
				cursor = 0;
				printWord(word, from);
			}
		} else {
			out_ << sub.substr(0, best);
			cursor += best;
			if (best < sub.length()) {
				printWord(word, best);
			}
		}
	}
	void printSpace ()
	{
		cursor += 1;
		if (cursor < LEN) {
			out_ << ' ';
		} else {
			out_ << std::endl;
			cursor = 0;
		}
	}
private:
	typedef std::vector<size_t> wordData;
	std::ofstream & out_;
	std::map<std::string, wordData> breaks;
	size_t cursor = 0;
};

int main(int argc, char **argv) 
{
	std::ifstream input("input.txt");
	std::ifstream dict("dict.txt");
	std::ofstream output("output.txt");
	Printer printer(dict, output);
	dict.close();
	
	std::string word;
	while (!input.eof()) {
		input >> word;
		printer.printWord(word);
		printer.printSpace();
	}
	
	input.close();
	output.close();
	return 0;
}
