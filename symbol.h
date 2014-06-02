#pragma once
#include <string>
#include<vector>
#include<set>
#include<functional>
using namespace std;

class Symbol
{
public:
	Symbol();
	Symbol(string, bool);
	Symbol(string, bool, int, char);
	~Symbol();
	/*bool operator <(const Symbol *&b)const{
		return (this->content < b->content);
	}*/
	void setLevel(int);
	int getLevel();
	void setAssociRule(char);
	char getAssociRule();
	string getContent();
	void addToFirst(set<Symbol*>);//向该符号的first加入Symbol*集合
	set<Symbol*> getFIRST();
	void setNullable();//将nullable设置为true
	bool isNullable();//获取它是否是可为空的
	bool isTerminate();
private:
	string content;
	bool isTermi;//是否是终结符
	char associRule;//结合规则,0 不可结合  1 左结合  2 右结合
	int level;//优先级
	bool nullable;
	set<Symbol*> FIRST;
};

class SymbolLess : public std::binary_function<Symbol, Symbol, bool>{
public:
	bool operator () (Symbol* lhs, Symbol* rhs) const
	{
		return lhs->getContent()< rhs->getContent();
	}
};
