/*
flat： add 的时候 1 enter, 
	   while 而后 1 transition ,  
	   		1 exit
			   2 enter
	即，同 depth 时 只维护一个 state， 一个pop, 另一个才能 push, 直到  no transition

HSM:
	按照 depth stack 来维护状态， 每次 Process transition 都会 遍历该stack all depth 的 transition
	进行执行，最后会形成一个 stack,
	当 stack 中间的 状态变化时， 从 depth 、depth-1 进行 pop, 即此时 exit,
	而后 push 对应的 新变化的中间state (即此时enter)

	详细可看源码---也是很容易理解

	注意：
		inner entry 可理解为 默认子状态， 当 inner most状态转换时，还是会先进入到 inner entry, 
		而后， depth-1 pop , new depth push,----> 再重新执行下 0----> depth的 transition 


!!!!!!!!!!!!!!!!!!!
Alive ...... 
Alive ...... 
Locomotion ...... 
Alive ...... 
Locomotion ...... 
Stand ...... 
+++++++++++++++++++
Alive ...... 
Locomotion ...... 
Stand ...... 
-------------------
Alive ...... 
Locomotion ...... 
Stand ...... 
Alive ...... 
Locomotion ...... 
Move ...... 
@@@@@@@@@@@@@@@@@@@
Alive ...... 
Dead ...... 


*/

// inner_entry_transition.cpp

#include <iostream>
#include "hsm.h"

using namespace std;
using namespace hsm;

class MyOwner
{
public:
	MyOwner();
	void UpdateStateMachine();

	void Die() { mDead = true; }

	void SetMove(bool enable) { mMove = enable; }

	void init();
private:
	bool IsDead() const { return mDead; } // Stub
	bool PressedMove() const { return mMove; } // Stub

	bool mMove;
	bool mDead;

	friend struct MyStates;
	StateMachine mStateMachine;
};

struct MyStates
{
	struct BaseState : StateWithOwner<MyOwner>
	{
	};

	struct Alive : BaseState
	{
		virtual Transition GetTransition()
		{
			cout << "Alive ...... " << endl;
			if (Owner().IsDead())
				return SiblingTransition<Dead>();

			return InnerEntryTransition<Locomotion>();
		}
	};

	struct Dead : BaseState
	{
		virtual Transition GetTransition()
		{
			cout << "Dead ...... " << endl;
			return NoTransition();
		}
	};

	struct Locomotion : BaseState
	{
		virtual Transition GetTransition()
		{
			cout << "Locomotion ...... " << endl;
			return InnerEntryTransition<Stand>();
		}
	};

	struct Stand : BaseState
	{
		virtual Transition GetTransition()
		{
			cout << "Stand ...... " << endl;
			if (Owner().PressedMove())
				return SiblingTransition<Move>();

			return NoTransition();
		}
	};

	struct Move : BaseState
	{
		virtual Transition GetTransition()
		{
			cout << "Move ...... " << endl;
			if (!Owner().PressedMove())
				return SiblingTransition<Stand>();

			return NoTransition();
		}
	};
};

MyOwner::MyOwner()
	: mDead(false)
	, mMove(false)
{
	//mStateMachine.Initialize<MyStates::Alive>(this);
	//mStateMachine.SetDebugInfo("TestHsm", TraceLevel::Basic);
}

void MyOwner::init()
{
	mStateMachine.Initialize<MyStates::Alive>(this);
	mStateMachine.SetDebugInfo("TestHsm", TraceLevel::Basic);
}

void MyOwner::UpdateStateMachine()
{
	mStateMachine.ProcessStateTransitions();
	mStateMachine.UpdateStates();
}

int main()
{
	MyOwner myOwner;
	cout << "???????????????????" << endl;
	myOwner.init();
	cout << "!!!!!!!!!!!!!!!!!!!" << endl;
	myOwner.UpdateStateMachine();
	cout << "+++++++++++++++++++" << endl;

	myOwner.UpdateStateMachine();
	cout << "-------------------" << endl;

	myOwner.SetMove(true);
	myOwner.UpdateStateMachine();

	cout << "@@@@@@@@@@@@@@@@@@@" << endl;
	myOwner.Die();
	myOwner.UpdateStateMachine();


}
