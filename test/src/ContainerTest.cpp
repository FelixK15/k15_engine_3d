#include "UnitTest++.h"

#include "K15_Container.h"
#include <list>
#include <vector>
#include <hash_map>
#include <stack>

using namespace K15_EngineV2;

SUITE(ListTests)
{
	struct TestStruct {int VALUE;int VALUE2;bool VALUE3;};

	TEST(Consistency)
	{
		List<TestStruct*> lsTestList;

		for(int i = 0;i < 20000;++i){
			lsTestList.PushBack(K15_NEW TestStruct());
		}

		for(ListNode<TestStruct*> *pNode = lsTestList.GetFirstElement();pNode;pNode = pNode->GetNext()->GetNext()){
			K15_DELETE pNode->GetElement();
			lsTestList.Erase(pNode);
		}

		CHECK_EQUAL(10000,lsTestList.Size());

		for(ListNode<TestStruct*> *pNode = lsTestList.GetFirstElement();pNode;pNode = pNode->GetNext()){
			K15_DELETE pNode->GetElement();
		}

		lsTestList.Clear();

		CHECK(lsTestList.Size() == 0);
	}

	TEST(SpeedComparedToSTL)
	{
		List<TestStruct*> lsTestList;
		std::list<TestStruct*> lsSTLTestList;

		double dList = 0.0;
		double dSTLList = 0.0;
		double dStart = 0.0;

		//Creation
		dStart = g_pSystem->TimeSinceStart();
		for(int i = 0;i < 20000;++i){
			lsTestList.PushBack(K15_NEW TestStruct());
		}
		dList = g_pSystem->TimeSinceStart() - dStart;

		dStart = g_pSystem->TimeSinceStart();
		for(int i =0;i < 20000;++i){
			lsSTLTestList.push_back(K15_NEW TestStruct());
		}
		dSTLList = g_pSystem->TimeSinceStart() - dStart;

		printf("K15 List - Creation of 20000 elements : %f ms\nSTL List - Creation of 20000 elements : %f ms\n",dList,dSTLList);
		CHECK(dList < dSTLList);

		//Iteration
		dStart = g_pSystem->TimeSinceStart();
		for(ListNode<TestStruct*> *pNode = lsTestList.GetFirstElement();pNode;pNode = pNode->GetNext()){
			K15_DELETE pNode->GetElement();
		}
		dList = g_pSystem->TimeSinceStart() - dStart;

		dStart = g_pSystem->TimeSinceStart();
		for(std::list<TestStruct*>::iterator iter = lsSTLTestList.begin();iter != lsSTLTestList.end();++iter){
			K15_DELETE (*iter);
		}
		dSTLList = g_pSystem->TimeSinceStart() - dStart;

		printf("K15 List - Iterating over 20000 elements : %f ms\nSTL List - Iterating over 20000 elements : %f ms\n",dList,dSTLList);
		CHECK(dList < dSTLList);

		//Clear
		dStart = g_pSystem->TimeSinceStart();
		lsTestList.Clear();
		dList = g_pSystem->TimeSinceStart() - dStart;

		dStart = g_pSystem->TimeSinceStart();
		lsSTLTestList.clear();
		dSTLList = g_pSystem->TimeSinceStart() - dStart;

		printf("K15 List - Clearing : %f ms\nSTL List - Clearing : %f ms\n",dList,dSTLList);
		CHECK(dList < dSTLList);
	}
}

SUITE(TreeTests)
{
	TEST(Conistency)
	{	
		NTree<int,20> trTestTree;

		trTestTree.Insert(20,NULL);

		CHECK_EQUAL(*(trTestTree.GetRoot()->Value),20);

		trTestTree.Insert(30,trTestTree.GetRoot());
		trTestTree.Insert(40,trTestTree.GetRoot());

		CHECK_EQUAL(trTestTree.GetRoot()->AmountChildren,2);

		NTreeNode<int,20> *pRoot = trTestTree.GetRoot();
		CHECK_EQUAL((*pRoot->Children[0]->Value),30);
		CHECK_EQUAL((*pRoot->Children[1]->Value),40);

		trTestTree.Clear();

		CHECK(trTestTree.Size() == 0);
		CHECK(!trTestTree.GetRoot());
	}
}
