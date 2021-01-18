// inner_transition.cpp

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
	myOwner.UpdateStateMachine();

	cout << "------------------Set Move = true" << endl;
	myOwner.SetMove(true);
	myOwner.UpdateStateMachine();

	cout << "-------------------Set Move = false" << endl;
	myOwner.SetMove(false);
	myOwner.UpdateStateMachine();
}
