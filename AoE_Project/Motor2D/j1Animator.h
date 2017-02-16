#ifndef _J1ANIMATOR_H_
#define _J1ANIMATOR_H_

#include "j1Module.h"
#include "j1Timer.h"
#include "p2Point.h"

struct SDL_Rect;
struct SDL_Texture;

enum UNIT_TYPE
{
	NO_UNIT = 0,
	MILITIA
};
enum ACTION_TYPE
{
	NO_ACTION = 0,
	ATTATCK,
	DIE,
	DISAPEAR,
	IDLE,
	WALK
};
enum DIRECTION_TYPE
{
	NO_DIRECTION = 0,
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST
};

///Animation Class ------------------------------
//Class that contains the animation basic necessary data
class Animation
{
public:

	Animation();

	~Animation();

private:

	//Vectors that storage the frames rect & pivot
	std::vector<SDL_Rect>	frames;
	std::vector<iPoint>		pivots;
	//Id of the animation enum type
	uint					enum_id = 0;
	//Current frame calculated by the timer
	float					current_frame = 0;
	bool					loop = true;
	int						loops = 0;
	//Animation update rate
	uint					speed = 300;
	j1Timer					timer;

public:

	//Set Methods -----------
	void	SetLoop(bool loop_state);
	void	SetSpeed(uint new_speed);
	void	SetId(uint id);

	//Get Methods -----------
	bool			GetLoop()const;
	uint			GetSpeed()const;
	const SDL_Rect&	GetCurrentFrame();
	const iPoint&	GetCurrentPivot()const;
	uint			GetId()const;

	//Add New frame with pivot defined
	void AddFrame(const SDL_Rect& rect, const iPoint& point);

};
/// ---------------------------------------------

/// Animation Block Class -----------------------
//Block that contains a vector of animations 
class Animation_Block
{
public:

	Animation_Block();

	~Animation_Block();

private:

	//Vector of other animation blocks
	std::vector<Animation_Block*>	childs;
	//Enum id of this block
	uint							enum_id = 0;
	//Pointer to a vector of animations 
	Animation*		animation;

public:

	//Delete all contained blocks data
	void	ClearAnimationBlocks();

	//Set Methods -----------
	void	SetId(uint id);

	//Get Methods -----------
	uint				GetId()const;
	Animation*			GetAnimation()const;
	Animation_Block*	GetBlock(int index)const;
	
	//Add Methods -----------
	void	SetAnimation(const Animation* new_animation);
	void	AddAnimationBlock(Animation_Block* new_animation_block);

};
/// ---------------------------------------------


//Animator Module -------------------------------
class j1Animator : public j1Module
{
public:

	j1Animator();
	~j1Animator();

public:

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:

	/// Just test
	//Test Animation
	Animation* test;
	Animation* test_1;
	Animation* test_2;
	Animation* test_3;
	Animation* test_4;

	SDL_Texture* test_texture;
	/// -----------


	std::vector<Animation_Block*> animation_blocks;

	//Methods that transform strings to enums (used when loading data from xml)
	UNIT_TYPE		Str_to_UnitEnum(const std::string* str)const;
	ACTION_TYPE		Str_to_ActionEnum(const std::string* str)const;
	DIRECTION_TYPE	Str_to_DirectionEnum(const std::string* str)const;

public:

	//bool Play(/* Unit* */)const;
};
#endif // !_J1ANIMATOR_H_