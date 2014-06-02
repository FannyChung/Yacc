#include"LRItem.h"

LRItem::LRItem()
{
}


LRItem::LRItem(Symbol *_asb) :leftPart(_asb){

}
LRItem::LRItem(Symbol *_l, vector<Symbol*> _r):leftPart(_l),rightPart(_r),dotPosition(0){

}
LRItem::LRItem(LRItem* other,Symbol* pre){
	leftPart = other->leftPart;
	rightPart = other->rightPart;
	dotPosition = 0;
	preSymbol.insert(pre);
}

LRItem::LRItem(LRItem* other):leftPart(other->leftPart),rightPart(other->rightPart),preSymbol(other->preSymbol),dotPosition(other->dotPosition+1){

}
void LRItem::addSymbolToRight(Symbol *a){
	rightPart.push_back(a);
}

void LRItem::addAction(string act) {
	action+= act;
}

vector<Symbol*> LRItem::getRight(){
	return rightPart;
}

Symbol* LRItem::getLeft(){
	return leftPart;
}

Symbol* LRItem::getSymbolAfterDot(){
	return rightPart[dotPosition];
}

int LRItem::getDotPosition(){
	return dotPosition;
}

set<Symbol*> LRItem::getPreSymbol(){
	return preSymbol;
}

void LRItem::addPreSymbol(Symbol* pre){
	preSymbol.insert(pre);
}