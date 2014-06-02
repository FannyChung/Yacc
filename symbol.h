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
	void addToFirst(set<Symbol*>);//��÷��ŵ�first����Symbol*����
	set<Symbol*> getFIRST();
	void setNullable();//��nullable����Ϊtrue
	bool isNullable();//��ȡ���Ƿ��ǿ�Ϊ�յ�
	bool isTerminate();
private:
	string content;
	bool isTermi;//�Ƿ����ս��
	char associRule;//��Ϲ���,0 ���ɽ��  1 ����  2 �ҽ��
	int level;//���ȼ�
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
