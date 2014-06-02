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
	LRItem(Symbol*);//�ò���ʽ���½�LR��
	LRItem(Symbol*, vector<Symbol*>);
	LRItem(LRItem*,Symbol*);//ͨ��һ��LR����µ�LR��ڶ�������ΪԤ���,����.λ�ڵ�0λ
	LRItem(LRItem*);//ͨ��һ��LR����µ�LR�����.���㣩����һλ
	void setLeft(Symbol*);//�����󲿷���
	void addSymbolToRight(Symbol*);//���Ҳ����ӷ���
	bool moveToNext();//.������ǰ�ƶ�
	void addAction(string);
	vector<Symbol*> getRight();//��ȡItem�Ҳ�
	Symbol* getLeft();//��ȡItem��
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

class ProductionLess : public std::binary_function<LRItem, LRItem, bool>{//ֻ�Ƚϲ���ʽ�󲿺��Ҳ�
public:
	bool operator () (LRItem* lhs, LRItem* rhs) const
	{
		if (lhs->getLeft()!=rhs->getLeft())
			return lhs->getLeft() < rhs->getLeft();
		else
			return lhs->getRight() < rhs->getRight();
	}
};

class LRNoPreLess : public std::binary_function<LRItem, LRItem, bool>{//������Ԥ����ıȽ�
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

class LRItemLess : public std::binary_function<LRItem, LRItem, bool>{//ȫ���Ƚ�
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
class StateLess : public std::binary_function<State, State, bool>{//�Ƚ�����state���LRItemָ����������Ƿ�һ��
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