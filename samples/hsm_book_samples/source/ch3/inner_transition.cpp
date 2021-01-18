// inner_transition.cpp
/*
	与inner enter 区别在于, inner 那一层状态切换时， 不用再执行到inner entry这个default的
	transition func

	一个是 inner(此处), 一个是 Sibling



	输出：
	HSM_1_TestHsm: Init    : N8MyStates5AliveE
	Alive ...... 
	HSM_1_TestHsm:  Entry   : N8MyStates10LocomotionE
	Alive ...... 
	Locomotion ...... 
	HSM_1_TestHsm:   Inner   : N8MyStates5StandE
	Alive ...... 
	Locomotion ...... 
	Stand ...... 
	------------------Set Move = true
	Alive ...... 
	Locomotion ...... 
	HSM_2_TestHsm:   Pop     : N8MyStates5StandE
	HSM_1_TestHsm:   Inner   : N8MyStates4MoveE
	Alive ...... 
	Locomotion ...... 
	Move ...... 
	-------------------Set Move = false
	Alive ...... 
	Locomotion ...... 
	HSM_2_TestHsm:   Pop     : N8MyStates4MoveE
	HSM_1_TestHsm:   Inner   : N8MyStates5StandE
	Alive ...... 
	Locomotion ...... 
	Stand ......

*/


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

private:
	bool IsDead() const { return mDead; }
	bool PressedMove() const { return mMove; }

	bool mDead;
	bool mMove;

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
			if (Owner().PressedMove())
				return InnerTransition<Move>();
			else
				return InnerTransition<Stand>();
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
	mStateMachine.Initialize<MyStates::Alive>(this);
	mStateMachine.SetDebugInfo("TestHsm", TraceLevel::Diagnostic);
}

void MyOwner::UpdateStateMachine()
{
	mStateMachine.ProcessStateTransitions();
	mStateMachine.UpdateStates();
}

int main()
{
	MyOwner myOwner;
	myOwner.UpdateStateMachine();

	cout << "------------------Set Move = true" << endl;
	myOwner.SetMove(true);
	myOwner.UpdateStateMachine();

	cout << "-------------------Set Move = false" << endl;
	myOwner.SetMove(false);
	myOwner.UpdateStateMachine();
}
