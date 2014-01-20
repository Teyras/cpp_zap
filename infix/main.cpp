#include <iostream>
#include <fstream>
#include <memory>
#include <cctype>

typedef long long num_t;

class AbstractNode {
public:
	~AbstractNode () {};
	virtual num_t eval (num_t x) = 0;
protected:
	num_t m_;
};

typedef std::unique_ptr<AbstractNode> Tree;

class ConstNode: public AbstractNode {
public:
	ConstNode (num_t val, num_t m): val_(val % m)
	{

	}
	num_t eval (num_t x)
	{
		return val_;
	}
private:
	num_t val_;
};

class VarNode: public AbstractNode {
public:
	num_t eval (num_t x)
	{
		return x;
	}
};

class AbstractBinaryNode: public AbstractNode {
protected:
	Tree left_;
	Tree right_;
};

class AddNode: public AbstractBinaryNode {
public:
	AddNode (Tree left, Tree right, num_t m)
	{
		left_ = std::move(left);
		right_ = std::move(right);
		m_ = m;
	}
	num_t eval (num_t x)
	{
		return (left_->eval(x) + right_->eval(x)) % m_;
	}
};

class MulNode: public AbstractBinaryNode {
public:
	MulNode (Tree left, Tree right, num_t m)
	{
		left_ = std::move(left);
		right_ = std::move(right);
		m_ = m;
	}
	num_t eval (num_t x)
	{
		return (left_->eval(x) * right_->eval(x)) % m_;
	}
};


struct Parser {
public:
	Parser (std::istream & in, num_t m): in_(in), m_(m)
	{
		next();
	}
	Tree parse ()
	{
		return add();
	}
private:
	Tree add ()
	{
		Tree left = mul();

		while (current() == '+') {
			next();
			Tree right = mul();
			left = Tree(new AddNode(std::move(left), std::move(right), m_));
		}

		return left;
	}
	Tree mul ()
	{
		Tree left = val();

		while (current() == '*') {
			next();
			Tree right = val();
			left = Tree(new MulNode(std::move(left), std::move(right), m_));
		}

		return left;
	}
	Tree val ()
	{
		if (current() == '(') {
			next();
			Tree tree = add();
			if (current() != ')') {
				throw std::exception();
			}
			next();
			return tree;
		}
		if (current() == 'x') {
			next();
			return Tree(new VarNode());
		}
		int sign = 1;
		if (current() == '-') {
			sign = -1;
			next();
		}
		if (isdigit(current())) {
			num_t value = 0;
			while (isdigit(current())) {
				value *= 10;
				value += current() - '0';
				next();
			}
			return Tree(new ConstNode(sign * value, m_));
		}
		throw new std::bad_exception();
	}
	void next ()
	{
		in_.get(c_);
		if (c_ == ' ') {
			next();
		}
	}
	char current ()
	{
		return c_;
	}
	std::istream & in_;
	char c_;
	num_t m_;
};

int main (int argc, char ** argv)
{
	num_t m = 1000;
	std::cin >> m;
	std::ifstream input("input.txt");
	Tree tree = Parser(input, m).parse();

	/*for (num_t i = 0; i < 10; i++) {
		std::cout << tree->eval(i) << std::endl;
	}*/

	for (num_t i = 0; i < m; i++) {
		if (tree->eval(i) == 0) {
			std::cout << i << std::endl;
		}
	}

	return 0;
}