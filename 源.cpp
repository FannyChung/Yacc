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


set<Symbol*> termiSyb;//�ս���ļ���
set<LRItem*, ProductionLess> prodSet;//����ʽ�ļ���
set<Symbol*, SymbolLess> Symbols;//���з��ż���
Symbol* startSymbol;//���в���ʽ��startSymbol
set<tuple<State, Symbol*, LRItem*>> R;//��Լӳ��
set<tuple<int, Symbol*, int> > E;//���бߵļ���
Graph T;//���нڵ�ļ���

int readFile(){
	set<string> stringOfSbls;//���з��ŵ�string
	set<Symbol*>nonTermiSyb;//���ս������

	cout << "Begin to open the infile!" << endl;
	if (!inFile.good()){					//good() ��ʾ�ļ�������û�ж�д����Ҳ���ǻ��ģ�Ҳû�н���.good() �� istream �ķ�����Ҳ�� ifstream �ķ���
		cout << "Fail to open the file !" << endl;
		return 0;
	}

	char ch = ' ';
	while (ch != '%')//����%{ǰ���ַ�
	{
		ch = inFile.get();
	}

	ch = inFile.get();
	if (ch != '{'){
		cout << "input should begin  %{  " << endl;
		return 0;
	}
	//----------------------------------------��ȡ����θ��Ƶ�����ļ�
	string in;//������ַ���
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
	//------------------------------------------------------------����token��start��
	//��%��ͷ�ķ��ź͹ؼ��֣������ǹ���εĸ�������һ�㶥��������д
	//%token��%left��%right��%nonassoc��%union��%type��%start
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
			if (in[2] == 'o'){//token,�����ս��
				inFile >> in;
				while (in[0] != '%')
				{
					tokenNums++;
					string mytoken(in);
					Symbol* mysb = new Symbol(mytoken, true);
					if (stringOfSbls.insert(mytoken).second){
						cout << "��ȡ�ս��: " << mytoken << endl;
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
		case 'l':case 'r':case 'n'://���ϡ��ҽ�ϡ����ɽ��
			inFile >> in;//����Symbol
			while (in[0] != '%')
			{
				char rule = type;//��Ϲ���
				Symbol* _newSymbol = new Symbol(in, true, precedence, rule);
				if (stringOfSbls.insert(in).second){
					cout << "��ȡ�ս��: " << in << endl;
					Symbols.insert(_newSymbol);
					termiSyb.insert(_newSymbol);
				}
				inFile >> in;
			}
			precedence++;
			break;
		case 'u'://union//Symbol������
			break;
		case 's'://start���忪ʼ��
			if (!startFlag){
				startFlag = true;
				inFile >> in;
				Symbol* _newSymbol = new Symbol(in, false);
				cout << "��ȡ�Ƿʷʷʷʷʷ��ս��: " << in << endl;
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
	cout << "�Զ������������������������������������������������������" << endl;
	//---------------------------------------------------��ȡ����ʽ����

	bool firstFlag = true;
	inFile >> in;//����ʽ�󲿵ķ��ս��
	while (in != "%%")
	{
		string leftPart = in;
		Symbol* tmpSymbol;
		if (stringOfSbls.insert(in).second)//�����ǰû�г��ֹ����������ս������
		{
			cout << "��ȡ���ս���� " << in << endl;
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

		if (firstFlag&&!startFlag){//���û��ָ��startFlag����ѵ�һ������ʽ������Ϊstart
			startSymbol = tmpSymbol;
		}
		firstFlag = false;

		inFile >> in;//����:
		if (in != ":"){
			cout << "there should be a : in a production" << endl;
			return 0;
		}
		do{
			inFile >> in;//�Ҳ�
			LRItem *_production = new LRItem(tmpSymbol);//����ʽ��LRItem��һ����,���Բ���ʽ������LRItem��ʾ
			while (in != "|"&&in != ";"&&in[0] != '{')
			{
				Symbol* tmpSymbol;
				if (stringOfSbls.insert(in).second)//�����ǰû�г��ֹ����������ս������
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
				_production->addSymbolToRight(tmpSymbol);//��ӵ�����ʽ�Ҳ�
				inFile >> in;
			}
			prodSet.insert(_production);
			if (in[0] == '{'){          //��������Ҫ����һֱ���� }
				_production->addAction(in);
				while (in[in.length() - 1] != '}')
				{
					inFile >> in;
					_production->addAction(in);
				}
				inFile >> in;//���룻��|
			}
		} while (in == "|");
		if (in != ";"){
			cout << "there should be a ; after production" << endl;
			return 0;
		}
		inFile >> in;
	}
	//------------------------------------------------------------------���򲿷�

	return 1;
}

bool nullAbleForEvery(int start, int end, vector<Symbol*> sbvec){//�ж��Ӽ���start��end�Ƿ�ȫ���ǿ�Ϊ�յģ�����ǣ�����true
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

void calculateFirst(){//����first
	for each (Symbol* sb in termiSyb)//��ÿ���ս��������first���Լ�
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
			vector<Symbol*> rgtP = pdct->getRight();//�ò���ʽ���Ҳ�
			Symbol* leftP = pdct->getLeft();//�ò���ʽ����
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

//--------------------------------------------------------------------------------------------------����LR��״̬ͼ
State Closure(State I){//��״̬�ڲ���հ�
	int oldSize = 0;
	do
	{
		oldSize = I.size();
		for each (LRItem* lr in I)
		{
			if (lr->getDotPosition() >= lr->getRight().size())
				continue;
			Symbol* X = lr->getSymbolAfterDot();

			if (!X->isTerminate()){//.������Ƿ��ս����A->a.Xb,z��
				vector<Symbol*> rightLR = lr->getRight();//����ʽ���Ҳ�
				set<Symbol*> firstbz;//FIRST(bz)
				bool continFlag = true;
				for (int i = lr->getDotPosition() + 1; i < rightLR.size(); i++)//����FIRST(bz)
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
					if (production->getLeft() == X){//���������ʽX->r
						for each (Symbol* w in firstbz)
						{
							State::iterator itr = I.find(production);
							if (itr == I.end()){//��������ڳ���Ԥ���֮�ⶼ��ͬ��LR��������LR
								LRItem* newLR = new LRItem(production, w);
								I.insert(newLR);
							}
							else//�ҵ�LR������Ԥ���
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

State GOTO(State I, Symbol* X){//GOTO�����״̬I�����ַ�X�����µ�״̬
	State J;
	for each (LRItem* lr in I)
	{
		if (lr->getDotPosition() >= lr->getRight().size())
			continue;
		if (lr->getSymbolAfterDot() == X){//��I�е�ÿ������(A->a.Xb,z)��LR
			LRItem* newLR = new LRItem(lr);//ͨ��һ��LR����µ�LR�����.���㣩����һλ
			J.insert(newLR);
		}
	}
	return Closure(J);
}

//����LR���LR״̬ͼ
void createLR(){
	//�½���ʼ���Ͳ���ʽ(��S'->S$  )
	Symbol* beforeStartSb = new Symbol("MYSTARTSYMBOL", false);//�½�����ʽ��
	vector<Symbol*> setSb;//����ʽ�Ҳ�
	setSb.push_back(startSymbol);
	Symbol* endSymbol = new Symbol("$", true);//�½�Ԥ���$�����Ұ�����FIRST����Ϊ�Լ���nullableΪfalse
	set<Symbol*> setOfpre;
	setOfpre.insert(endSymbol);
	endSymbol->addToFirst(setOfpre);

	//setSb.push_back(endSymbol);
	LRItem* zeroItem = new LRItem(beforeStartSb, setSb);//��0������ʽ,ʹ$��ΪԤ���
	zeroItem->addPreSymbol(endSymbol);

	State lrset;
	lrset.insert(zeroItem);
	State c = Closure(lrset);//�Կ�ʼ״̬��հ���Ϊ��һ��״̬

	T.push_back(c);//��һ��״̬����states����
	set<pair<int, Symbol*>> existGOTO;//�Ѿ����ڵĴ�Iͨ��X�ַ���ȥ�ı�

	int i = 0;
	for (int j = 0; j < T.size(); j++)//��T�е�ÿһ��״̬
	{
		State I = T[j];
		for each (LRItem* lr in I)//��״̬�е�ÿһ��LRItem
		{
			if (lr->getDotPosition() >= lr->getRight().size()){//���.�Ѿ��������������Լ����
				for each (Symbol* prS in lr->getPreSymbol())
				{
					R.insert(make_tuple(I,prS,lr));//���ӹ�Լ(I,z,A->a)
				}
				continue;
			}
			Symbol* X = lr->getSymbolAfterDot();//X�ǵ�֮��ķ���
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
			if (!existFlag){//�����ǰ������-----------------------������ΪState�����ָ��
				T.push_back(tem);
				i++;
			}
			if (E.insert(make_tuple(j, X,i)).second){//���I״̬��ǰû��X�ĳ��ߣ������ӳ��ߵ���Ӧ��״̬-----������ָ�������
			}
		}
	}
}


//-----------------------------------------------------------------------------------����������
void createTB(){
	for (int i = 0; i < T.size();i++)
	{
		State state = T[i];
		set<tuple<int, Symbol*, int>>::iterator itr=E.begin();
	}
}

int main(){
	//set<Symbol> termiSyb;//�ս������
	Symbol startSymbol;

	string inFilename = "translation.y";
	string outFilename = "yacc.yy.c";
	inFile.open(inFilename.c_str(), ios::in);
	outFile.open(outFilename.c_str(), ios::out);

	if (readFile() == 0)
	{
		cout << "Error input file!" << endl;
	}
	calculateFirst();//����First
	createLR();//����״̬ת��ͼ
	inFile.close();
	outFile.close();

}