#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
//#include <stdlib.h>
//#include <crtdbg.h>


#include <cassert>
#include <assert.h>
#include <iostream>
using std::cout;
using std::cin;

#include "VG.h"
#include "SharedPtr.h"

struct C {
	float value;
	C(float value) :value(value) {};
};

void Test() {
	//Constructor test:	

	SharedPtr<C> sp11;
	assert(!sp11);
	SharedPtr<C> p15(nullptr);
	assert(!p15);
	SharedPtr<C> sp12(new C(12));
	assert(sp12);
	SharedPtr<C> sp13(sp11);
	assert(!sp13);

	assert(sp12.unique());
	SharedPtr<C> sp14(sp12);
	assert(sp14);
	assert(!sp12.unique());


	//-	Destructor test
	//-	Assignment from another SharedPtr
	sp14 = sp12;
	assert(sp14);

	sp14 = sp14;
	assert(sp14);

	//-	== and < operator test:

	SharedPtr<C> sp31(new C(31));
	assert(sp11 == nullptr);
	assert(sp11 < sp12);
	assert(!(sp12 < sp11));
	assert(sp14 == sp12);
	assert(!(sp14 == sp31));  
	assert((sp14 < sp31) || (sp31 < sp14));

	//get, * and -> test:

	SharedPtr<C> sp41(new C(41));
	SharedPtr<C> sp42(new C(42));
	assert((sp41->value) == (sp41.get()->value));
	assert((sp41->value) != (sp42.get()->value));
	assert(&(*sp41) == (sp41.get()));

	//move constructor test
	SharedPtr<C> sp51(std::move(sp41));
	assert(sp51->value == 41);
	assert(!sp41);

	sp51.reset();
	assert(!sp51);
}


int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::locale::global(std::locale("swedish"));

	Test();
	cin.get();
}
