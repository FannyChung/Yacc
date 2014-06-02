#define  _CRT_SECURE_NO_WARNINGS
#include"symbol.h"
#include"LRItem.h"
//#include"Compare.h"
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include<tuple>
#include<map>
#include <vector>
#include <algorithm>
using namespace std;


#define MAXREADLEN 256
ifstream inFile;
ofstream outFile;


set<Symbol*> termiSyb;//终结符的集合
set<LRItem*, ProductionLess> prodSet;//产生式的集合
set<Symbol*, SymbolLess> Symbols;//所有符号集合
Symbol* startSymbol;//所有产生式的startSymbol
set<tuple<State, Symbol*, LRItem*>> R;//规约映射
set<tuple<int, Symbol*, int> > E;//所有边的集合
Graph T;//所有节点的集合

int readFile(){
	set<string> stringOfSbls;//所有符号的string
	set<Symbol*>nonTermiSyb;//非终结符集合

	cout << "Begin to open the infile!" << endl;
	if (!inFile.good()){					//good() 表示文件正常，没有读写错误，也不是坏的，也没有结束.good() 是 istream 的方法，也是 ifstream 的方法
		cout << "Fail to open the file !" << endl;
		return 0;
	}

	char ch = ' ';
	while (ch != '%')//忽略%{前的字符
	{
		ch = inFile.get();
	}

	ch = inFile.get();
	if (ch != '{'){
		cout << "input should begin  %{  " << endl;
		return 0;
	}
	//----------------------------------------读取定义段复制到输出文件
	string in;//读入的字符串
	getline(inFile, in);
	do{
		outFile << in << endl;
		cout << in << endl;
		getline(inFile, in);
	} while (in.length() < 2 || (in[0] != '%'&&in[1] != '}'));


	if (inFile.eof()){
		cout << "erro input missing  %} " << endl;
		return 0;
	}
	cout << "The definition segment finished!" << endl;
	//------------------------------------------------------------处理token、start等
	//以%开头的符号和关键字，或者是规则段的各个规则一般顶着行首来写
	//%token，%left，%right，%nonassoc，%union，%type，%start
	bool startFlag = false;
	inFile >> in;
	int tokenNums = 0;
	int precedence = 0;
	while (in[0] == '%'){
		char type = in[1];
		if (type == '%')
			break;
		switch (type)
		{
		case 't':
			if (in[2] == 'o'){//token,定义终结符
				inFile >> in;
				while (in[0] != '%')
				{
					tokenNums++;
					string mytoken(in);
					Symbol* mysb = new Symbol(mytoken, true);
					if (stringOfSbls.insert(mytoken).second){
						cout << "获取终结符: " << mytoken << endl;
						Symbols.insert(mysb);
						termiSyb.insert(mysb);
					}
					inFile >> in;
				}
				precedence++;
			}
			else if (in[2] == 'y')//type
			{

			}
			else
			{
				cout << "wrong input after %" << endl;
				return 0;
			}
			break;
		case 'l':case 'r':case 'n'://左结合、右结合、不可结合
			inFile >> in;//读入Symbol
			while (in[0] != '%')
			{
				char rule = type;//结合规则
				Symbol* _newSymbol = new Symbol(in, true, precedence, rule);
				if (stringOfSbls.insert(in).second){
					cout << "获取终结符: " << in << endl;
					Symbols.insert(_newSymbol);
					termiSyb.insert(_newSymbol);
				}
				inFile >> in;
			}
			precedence++;
			break;
		case 'u'://union//Symbol的属性
			break;
		case 's'://start定义开始符
			if (!startFlag){
				startFlag = true;
				inFile >> in;
				Symbol* _newSymbol = new Symbol(in, false);
				cout << "获取非肥肥肥肥肥肥终结符: " << in << endl;
				nonTermiSyb.insert(_newSymbol);
				Symbols.insert(_newSymbol);
				startSymbol = _newSymbol;
				stringOfSbls.insert(in);
			}
			else
			{
				cout << "only one start Symbol is permited in the program. so we just ignore others you defined except the first one" << endl;
			}
			inFile >> in;
			break;
		default:
			cout << "error input!" << endl;
			return 0;
			break;
		}
	}
	cout << "自定义结束————————————————————————" << endl;
	//---------------------------------------------------读取产生式规则

	bool firstFlag = true;
	inFile >> in;//产生式左部的非终结符
	while (in != "%%")
	{
		string leftPart = in;
		Symbol* tmpSymbol;
		if (stringOfSbls.insert(in).second)//如果以前没有出现过，则插入非终结符集合
		{
			cout << "获取非终结符： " << in << endl;
			tmpSymbol = new Symbol(in, false);
			nonTermiSyb.insert(tmpSymbol);
			Symbols.insert(tmpSymbol);
		}
		else
		{
			Symbol* aS = new Symbol(in, false);
			set<Symbol*>::iterator irt = Symbols.find(aS);
			tmpSymbol = (*irt);
		}

		if (firstFlag&&!startFlag){//如果没有指定startFlag，则把第一个产生式的左部作为start
			startSymbol = tmpSymbol;
		}
		firstFlag = false;

		inFile >> in;//读入:
		if (in != ":"){
			cout << "there should be a : in a production" << endl;
			return 0;
		}
		do{
			inFile >> in;//右部
			LRItem *_production = new LRItem(tmpSymbol);//产生式是LRItem的一部分,所以产生式可以用LRItem表示
			while (in != "|"&&in != ";"&&in[0] != '{')
			{
				Symbol* tmpSymbol;
				if (stringOfSbls.insert(in).second)//如果以前没有出现过，则插入非终结符集合
				{
					tmpSymbol = new Symbol(in, false);
					nonTermiSyb.insert(tmpSymbol);
					Symbols.insert(tmpSymbol);
				}
				else
				{
					Symbol* aS = new Symbol(in, false);
					set<Symbol*>::iterator irt = Symbols.find(aS);
					tmpSymbol = (*irt);
				}
				_production->addSymbolToRight(tmpSymbol);//添加到产生式右部
				inFile >> in;
			}
			prodSet.insert(_production);
			if (in[0] == '{'){          //读到语义要求，则一直读到 }
				_production->addAction(in);
				while (in[in.length() - 1] != '}')
				{
					inFile >> in;
					_production->addAction(in);
				}
				inFile >> in;//读入；或|
			}
		} while (in == "|");
		if (in != ";"){
			cout << "there should be a ; after production" << endl;
			return 0;
		}
		inFile >> in;
	}
	//------------------------------------------------------------------程序部分

	return 1;
}

bool nullAbleForEvery(int start, int end, vector<Symbol*> sbvec){//判断子集从start到end是否全部是可为空的，如果是，返回true
	bool flag = true;
	for (int i = start; i <= end; i++)
	{
		bool tmp = sbvec[i]->isNullable();
		if (!tmp){
			flag = false;
			break;
		}
	}
	return flag;
}

void calculateFirst(){//计算first
	for each (Symbol* sb in termiSyb)//对每个终结符，它的first是自己
	{
		set<Symbol*> ss;
		ss.insert(sb);
		sb->addToFirst(ss);
	}
	bool changeFlag;
	do
	{
		changeFlag = false;
		for each (LRItem* pdct in prodSet)
		{
			vector<Symbol*> rgtP = pdct->getRight();//该产生式的右部
			Symbol* leftP = pdct->getLeft();//该产生式的左部
			if (rgtP.empty() && !leftP->isNullable()){
				leftP->setNullable();
				changeFlag = true;
				continue;
			}
			int k = rgtP.size();
			if (nullAbleForEvery(0, k - 1, rgtP)){
				if (!leftP->isNullable()){
					changeFlag = true;
					leftP->setNullable();
				}
			}
			for (int i = 1; i <= k - 1; i++)
			{
				if (nullAbleForEvery(0, i - 1, rgtP)){
					int s1 = leftP->getFIRST().size();
					leftP->addToFirst(rgtP[i]->getFIRST());
					if (s1 < leftP->getFIRST().size())
						changeFlag = true;
				}
			}
		}
	} while (changeFlag);
}

//--------------------------------------------------------------------------------------------------构造LR的状态图
State Closure(State I){//在状态内部求闭包
	int oldSize = 0;
	do
	{
		oldSize = I.size();
		for each (LRItem* lr in I)
		{
			if (lr->getDotPosition() >= lr->getRight().size())
				continue;
			Symbol* X = lr->getSymbolAfterDot();

			if (!X->isTerminate()){//.后面的是非终结符（A->a.Xb,z）
				vector<Symbol*> rightLR = lr->getRight();//产生式的右部
				set<Symbol*> firstbz;//FIRST(bz)
				bool continFlag = true;
				for (int i = lr->getDotPosition() + 1; i < rightLR.size(); i++)//计算FIRST(bz)
				{
					set<Symbol*> firsts = rightLR[i]->getFIRST();
					firstbz.insert(firsts.begin(), firsts.end());
					if (!rightLR[i]->isNullable()){
						continFlag = false;
						break;
					}
				}
				if (continFlag){
					set<Symbol*> pre = lr->getPreSymbol();
					for each (Symbol* preSb in pre)
					{
						set<Symbol*> firsts = preSb->getFIRST();
						firstbz.insert(firsts.begin(), firsts.end());
						if (!preSb->isNullable()){
							break;
						}
					}
				}
				for each (LRItem* production in prodSet)
				{
					if (production->getLeft() == X){//对任意产生式X->r
						for each (Symbol* w in firstbz)
						{
							State::iterator itr = I.find(production);
							if (itr == I.end()){//如果不存在除了预测符之外都相同的LR，则增加LR
								LRItem* newLR = new LRItem(production, w);
								I.insert(newLR);
							}
							else//找到LR，增加预测符
							{
								(*itr)->addPreSymbol(w);
							}
						}
					}
				}
			}
		}
	} while (oldSize < I.size());
	return I;
}

State GOTO(State I, Symbol* X){//GOTO，求从状态I经过字符X到达新的状态
	State J;
	for each (LRItem* lr in I)
	{
		if (lr->getDotPosition() >= lr->getRight().size())
			continue;
		if (lr->getSymbolAfterDot() == X){//对I中的每个形如(A->a.Xb,z)的LR
			LRItem* newLR = new LRItem(lr);//通过一个LR项建立新的LR项，并且.（点）后移一位
			J.insert(newLR);
		}
	}
	return Closure(J);
}

//构造LR项和LR状态图
void createLR(){
	//新建开始符和产生式(如S'->S$  )
	Symbol* beforeStartSb = new Symbol("MYSTARTSYMBOL", false);//新建产生式左部
	vector<Symbol*> setSb;//产生式右部
	setSb.push_back(startSymbol);
	Symbol* endSymbol = new Symbol("$", true);//新建预测符$，并且把它的FIRST设置为自己，nullable为false
	set<Symbol*> setOfpre;
	setOfpre.insert(endSymbol);
	endSymbol->addToFirst(setOfpre);

	//setSb.push_back(endSymbol);
	LRItem* zeroItem = new LRItem(beforeStartSb, setSb);//第0个产生式,使$成为预测符
	zeroItem->addPreSymbol(endSymbol);

	State lrset;
	lrset.insert(zeroItem);
	State c = Closure(lrset);//以开始状态求闭包作为第一个状态

	T.push_back(c);//第一个状态加入states集合
	set<pair<int, Symbol*>> existGOTO;//已经存在的从I通过X字符出去的边

	int i = 0;
	for (int j = 0; j < T.size(); j++)//对T中的每一个状态
	{
		State I = T[j];
		for each (LRItem* lr in I)//对状态中的每一个LRItem
		{
			if (lr->getDotPosition() >= lr->getRight().size()){//如果.已经到了最后，则加入规约集合
				for each (Symbol* prS in lr->getPreSymbol())
				{
					R.insert(make_tuple(I,prS,lr));//增加规约(I,z,A->a)
				}
				continue;
			}
			Symbol* X = lr->getSymbolAfterDot();//X是点之后的符号
			State tem = GOTO(I, X);
			bool existFlag = true;
			for each (LRItem* var in tem)
			{
				for each (State st in T)
				{
					existFlag = true;
					for each (LRItem* lr in st)
					{
					}
					if (existFlag)
						break;
				}
			}
			if (!existFlag){//如果以前不存在-----------------------出错，因为State里的是指针
				T.push_back(tem);
				i++;
			}
			if (E.insert(make_tuple(j, X,i)).second){//如果I状态以前没有X的出边，则增加出边到对应的状态-----第三个指针出问题
			}
		}
	}
}


//-----------------------------------------------------------------------------------建立分析表
void createTB(){
	for (int i = 0; i < T.size();i++)
	{
		State state = T[i];
		set<tuple<int, Symbol*, int>>::iterator itr=E.begin();
	}
}

int main(){
	//set<Symbol> termiSyb;//终结符集合
	Symbol startSymbol;

	string inFilename = "translation.y";
	string outFilename = "yacc.yy.c";
	inFile.open(inFilename.c_str(), ios::in);
	outFile.open(outFilename.c_str(), ios::out);

	if (readFile() == 0)
	{
		cout << "Error input file!" << endl;
	}
	calculateFirst();//计算First
	createLR();//构造状态转换图
	inFile.close();
	outFile.close();

}