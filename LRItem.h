#pragma once
#include<iostream>
#include<string>
#include<set>
#include<vector>
#include"symbol.h"
using namespace std;

class LRItem
{
public:
	LRItem();
	LRItem(Symbol*);//用产生式左部新建LR项
	LRItem(Symbol*, vector<Symbol*>);
	LRItem(LRItem*,Symbol*);//通过一个LR项建立新的LR项，第二个参数为预测符,并且.位于第0位
	LRItem(LRItem*);//通过一个LR项建立新的LR项，并且.（点）后移一位
	void setLeft(Symbol*);//设置左部符号
	void addSymbolToRight(Symbol*);//向右部增加符号
	bool moveToNext();//.符号向前移动
	void addAction(string);
	vector<Symbol*> getRight();//获取Item右部
	Symbol* getLeft();//获取Item左部
	Symbol* getSymbolAfterDot();
	int getDotPosition();
	set<Symbol*> getPreSymbol();
	void addPreSymbol(Symbol*);
	//~LRItem();

private:
	Symbol *leftPart;
	vector<Symbol*> rightPart;
	set<Symbol*> preSymbol;
	int dotPosition;
	string action="";
};

class ProductionLess : public std::binary_function<LRItem, LRItem, bool>{//只比较产生式左部和右部
public:
	bool operator () (LRItem* lhs, LRItem* rhs) const
	{
		if (lhs->getLeft()!=rhs->getLeft())
			return lhs->getLeft() < rhs->getLeft();
		else
			return lhs->getRight() < rhs->getRight();
	}
};

class LRNoPreLess : public std::binary_function<LRItem, LRItem, bool>{//不考虑预测符的比较
public:
	bool operator () (LRItem* lhs, LRItem* rhs) const
	{
		if (lhs->getLeft() != rhs->getLeft())
			return lhs->getLeft() < rhs->getLeft();
		else if (lhs->getRight() != rhs->getRight())
			return lhs->getRight() < rhs->getRight();
		else
			return lhs->getDotPosition() < rhs->getDotPosition();
	}
};

class LRItemLess : public std::binary_function<LRItem, LRItem, bool>{//全部比较
public:
	bool operator () (LRItem* lhs, LRItem* rhs) const
	{
		if (lhs->getLeft() != rhs->getLeft())
			return lhs->getLeft() < rhs->getLeft();
		else if (lhs->getRight() != rhs->getRight())
			return lhs->getRight() < rhs->getRight();
		else if (lhs->getDotPosition() != rhs->getDotPosition())
			return lhs->getDotPosition() < rhs->getDotPosition();
		else
			return lhs->getPreSymbol() < lhs->getPreSymbol();
	}
};

/*
class StateLess : public std::binary_function<State, State, bool>{//比较两个state里的LRItem指针里的内容是否一样
public:
	bool operator () (State lhs, State rhs) const
	{
		for each (LRItem* lr in lhs)
		{
			for each(LRItem* rr in rhs)
				;
		}
		return true;
	}
};*/

typedef set<LRItem*, LRNoPreLess> State;
typedef vector<State> Graph;