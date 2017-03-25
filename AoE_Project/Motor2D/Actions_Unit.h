#ifndef __ACTION_UNIT_H__
#define __ACTION_UNIT_H__

#include "j1ActionManager.h"


//Move the character
class MoveUnitAction : public Action
{
public:
	MoveUnitAction(Unit* actor, int x, int y): Action(),
		 actor(actor), x_new(x), y_new(y), completed(false)
	{};


	virtual void execute()
	{
		//Set path so the unit starts moving
		App->entities_manager->SetUnitPath(actor, iPoint(x_new, y_new));
	};
	virtual bool IsDone()
	{
		return completed;
	}

private:
	Unit* actor;
	int x_new, y_new;
	bool completed;
};



#endif // !__ACTION_UNIT_H__

