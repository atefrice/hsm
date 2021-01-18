/*

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
