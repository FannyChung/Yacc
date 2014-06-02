#include"symbol.h"
Symbol::Symbol()
{
	nullable = false;
}

Symbol::Symbol(string s, bool t) :content(s), isTermi(t), nullable(false){
}

Symbol::Symbol(string s, bool t, int l, char asso) : content(s), isTermi(t), level(l), associRule(asso), nullable(false){
}
Symbol::~Symbol()
{
}

void Symbol::setLevel(int l){
	level = l;
}

int Symbol::getLevel(){
	return level;
}

void Symbol::setAssociRule(char a){
	associRule = a;
}

char Symbol::getAssociRule(){
	return associRule;
}

string Symbol::getContent(){
	return content;
}

void Symbol::addToFirst(set<Symbol*> sbs){
	for each (Symbol* sb in sbs)
	{
		FIRST.insert(sb);
	}
}

void Symbol::setNullable(){
	nullable = true;
}

bool Symbol::isNullable(){
	return nullable;
}

set<Symbol*> Symbol::getFIRST(){
	return FIRST;
}

bool Symbol::isTerminate(){
	return isTermi;
}

