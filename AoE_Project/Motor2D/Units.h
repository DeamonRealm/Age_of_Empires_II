#ifndef __UNITS_H__
#define __UNITS_H__

#include "BaseEntities.h"

enum PLAYER_RESOURCES;

///Class Villager -----------
class Villager : public Unit
{
public:

	Villager();
	Villager(const Villager& copy);
	~Villager();
	
private:

	//Item used id
	ITEM_TYPE item_type = NO_ITEM;

	//Resource related data
	PLAYER_RESOURCES resource_collected_type;
	uint resources_capacity = 0;
	uint current_resources = 0;
	uint recollect_capacity = 0;
	uint recollect_rate = 0;

public:

	//Functionality ----
	//Actions -----
	bool Die();

	//Get Methods -
	ITEM_TYPE			GetItemType()const;
	PLAYER_RESOURCES	GetResourceCollectedType()const;
	uint				GetCurrentResources()const;

	//Set Methods -
	void SetItemType(ITEM_TYPE type);
	void SetResourceCollectedType(PLAYER_RESOURCES type);
	void SetResourcesCapacity(uint value);
	void SetRecollectCapacity(uint value);
	void SetRecollectRate(uint value);
	void SetCurrentResources(uint value);

	//Actions -----
	bool Recollect(Resource** target);
	bool SaveResources(Building** save_point);
	void CheckRecollectResource(RESOURCE_TYPE type);
	void CheckCarryResource();
	void ResetResourcesData();
};
/// -------------------------
#endif // !__UNITS_H__

