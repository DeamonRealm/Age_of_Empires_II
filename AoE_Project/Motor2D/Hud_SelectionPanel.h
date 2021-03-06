#ifndef HUD_SELECTION_PANEL
#define HUD_SELECTION_PANEL

#include "p2Defs.h"
#include <vector>
#include <list>

#include "j1Timer.h"

#include "SDL\include\SDL_rect.h"

#include "Quadtree.h"

//Units/Bukdings Actions
#include "j1ActionManager.h"

#define OFFSET_Y 10
#define OFFSET_X 10
#define PROFILE_QUEUE_SIZE 15
#define REFRESH_RATE 200

class UI_String;
class UI_Image;

class Entity;
class Unit;
class Building;
class Resource;
class Particle;

enum ENTITY_TYPE;
enum GUI_INPUT;


enum SELECT_TYPE
{
	SINGLE,
	GROUP,
	DOUBLECLICK,
	ADD
};

enum MOVE_CAMERA
{
	C_DONT_MOVE,
	C_MOVE_RIGHT,
	C_MOVE_LEFT,
	C_MOVE_DOWN,
	C_MOVE_UP
};

class Entity_Profile
{
public:
	Entity_Profile();
	~Entity_Profile();

	void SetEntity(Entity* entity_selected);
	Entity* GetEntity();

	void Reset();

	void DrawProfile() const;
	void DrawQueue() const;
	void UpdateStats();
	void UpdateQueue();

private:

	std::string		profile_text;

	Entity*			element = nullptr;
	ENTITY_TYPE		e_type = (ENTITY_TYPE)0;
	SDL_Rect		background = {0,0,0,0};

	//General
	UI_String*		name = nullptr;
	//UI_String*		civilization;
	UI_String*		diplomacy = nullptr;
	bool			isenemy = false;

	int				life_update = 0;
	int				m_life = 0;
	UI_String*		life = nullptr;

	//Units and Offensive Buildings
	uint			u_attack = 0;
	UI_String*		attack = nullptr;
	uint			u_deffence = 0;
	UI_String*		deffence = nullptr;

	uint			u_range = 0;
	UI_String*		range = nullptr;

	//Buildings
	uint			u_capacity = 0;
	uint			m_capacity = 0;
	UI_String*		capacity = nullptr;
	bool			got_queue = false;
	int				previous_queue_size = 0;
	int				production_queue_size = 0;
	std::vector<UI_Image*>  production_queue;

	//Buff stats
	int				attack_up = 0;
	int				deffence_up = 0;
};


class Selection_Panel
{
public:

	Selection_Panel();
	~Selection_Panel();

public:
	// Called before all Updates
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Handle Input
	void Handle_Input(GUI_INPUT newevent);

	// GUI Handle Input
	void Handle_Input(UI_Element* target, GUI_INPUT input);

	// Disable/Enable Module
	void Enable();
	void Disable();
	void Reset();

	bool Load(pugi::xml_node &data);

	bool Save(pugi::xml_node &data) const;

	// Draw Selection Panel
	bool Draw();
	void DrawGroup();
	void DrawLife();

	// Coordinates Methods
	bool PointisInViewport(int x = 0, int y = 0) ;

	// Move Methods
	void MoveSelectedToPoint(int x, int y);

	// Select Methods
	void Select(SELECT_TYPE type);
	void Select(Unit* unit);
	void UpdateSelected();
	bool UpdateGroup();

	// Clear Selected Elements List
	void UnSelect_Entity();

	void Expand_SelectionRect();
	Entity*	GetUpperEntity(int x, int y);
	 
	// Selection Panel (Group Functions)
	void SetGroupProfile();

	Entity* GetSelected() const;
	uint	GetSelectedSize() const;

	void    GetSelectedType(DIPLOMACY &d_type, ENTITY_TYPE &e_type, UNIT_TYPE &u_type, BUILDING_TYPE &b_type);
	void	GetChampionsSelected(Unit* &warrior, Unit* &wizard, Unit* &archer);
	bool    GetSelectedIsEntity();
	void	ResetSelectedType(SELECT_TYPE select_type);
	void	ResetSelectedType();
	void	CheckSelectedType();
	void	RemoveDeadSelectedUnits(Unit*);

	UI_Element*	GetViewport();
	bool	GetInViewport()const;
	bool	WindowsMove(MOVE_CAMERA must_move = C_DONT_MOVE);

	//		Minimap Alert
	void	SetMinimapAlert(iPoint pos);
	void	SetMinimapAlertPosition();

	//		Champions Lvl Up Alert
	void	SetChampionsLvlUp(UNIT_TYPE champ_type);
	void	SetChampionsLvlUpPosition();
	void	UpdateChampionsLvlUpState();
	void	DrawChampionsUp();

private:

	// SelectionPanel Viewport
	UI_Element*		viewport = nullptr;
	bool			inviewport = false;

	// Mouse_pos
	int mouse_x = 0;
	int mouse_y = 0;

	// Group selection
	uint			max_selected_units = 60;
	uint			max_row_units = 16;
	uint			row_size = 608;

	j1Timer			double_click;
	bool			double_clickon = false;

	bool			expand = false;
	SDL_Rect		selection_rect = {0,0,0,0};
	const SDL_Rect	map_viewport;


	std::list<Entity*>		selected_elements;
	std::vector<UI_Image*>  group_profile;

	std::vector<Unit*>		unit_quad_selection;
	std::vector<Building*>  building_quad_selection;
	std::vector<Resource*>	resource_quad_selection;

	Entity_Profile*		Selected = nullptr;
	
	// Used for mouse detection
	Entity*			UpperEntity = nullptr;
	j1Timer			refresh_upperentity;

	// Selected Types;
	DIPLOMACY		selected_diplomacy = (DIPLOMACY)0;
	ENTITY_TYPE		selected_entity_type = (ENTITY_TYPE)0;
	UNIT_TYPE		selected_unit_type = (UNIT_TYPE)0;
	BUILDING_TYPE   selected_building_type = (BUILDING_TYPE)0;
	bool			champions_selected = false;

	//Action inputs
	Action* action_command = nullptr;
	
	//UI Particles
	Particle* arrows_particle = nullptr;
	Particle* warrior_lvl_up = nullptr;
	Particle* wizard_lvl_up = nullptr;
	Particle* hunter_lvl_up = nullptr;
	bool	  champions_up = false;
	iPoint	  champions_up_position = {0,0};
	std::list<Particle*> champions_lvlup_order;

	Particle* map_alert_particle = nullptr;
	iPoint	  map_alert_pos = { 0,0 };

private:

	void SetMoveArrows();

};

#endif // !HUD_SELECTION_PANEL

