#ifndef _ENTITIES_MANAGER_
#define _ENTITIES_MANAGER_

#include "j1Module.h"
#include "BaseEntities.h"

class j1EntitiesManager : public j1Module
{
public:

	j1EntitiesManager();
	~j1EntitiesManager();

public:

	// Called before render is available
	bool Awake(pugi::xml_node& config_node);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	//Draw entities
	bool Draw()const;

	// Called before quitting
	bool CleanUp();

	//Handle Console Input ----------------------
	void Console_Command_Input(Command* command, Cvar* cvar, std::string* input);
	void Console_Cvar_Input(Cvar* cvar, Command* command_type, std::string* input);

private:

	//Lists of current game entities
	std::list<Unit*>		units;
	std::list<Resource*>	resources;
	std::list<Building*>	buildings;

	//Vector of predefined units
	std::vector<Unit>		units_defs;
	std::vector<Resource>	resources_defs;
	std::vector<Building>	buildings_defs;

	// Cvar that defines the console unit generator unit type
	Cvar* unit_cvar;

	//Methods to add entities definitions
	bool		AddUnitDefinition(const pugi::xml_node* unit_node);
	bool		AddResourceDefinition(const pugi::xml_node* resource_node);
	bool		AddBuildingDefinition(const pugi::xml_node* building_node);

	//Check if the entity civilizations string contains the chosen one
	bool		CivilizationCheck(char* civs_str, const char* chosen_civ);

public:

	//Functionality -------------------
	//Load all chosen civilization data
	bool		LoadCivilization(const char* folder);
	//Factory Methods -------
	Unit*		GenerateUnit(UNIT_TYPE type);
	Building*	GenerateBuilding(BUILDING_TYPE type);
	Resource*	GenerateResource(RESOURCE_TYPE type, uint id_index = 0);
};
#endif // _ENTITIES_MANAGER_
