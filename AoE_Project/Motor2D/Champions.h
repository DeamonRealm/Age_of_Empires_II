#ifndef _CHAMPIONS_
#define _CHAMPIONS_

#include "Units.h"
class PassiveBuff;

///Class Champion -------------------------------
//Base class that define the champions bases
class Champion : public Unit
{
public:

	Champion();
	Champion(const Champion& copy);
	~Champion();

protected:

	//Hero level
	uint	level = 1;
	
	//Buff area
	Circle			buff_area;
	PassiveBuff*	buff_to_apply = nullptr;

	//Stats bonus for level
	uint	attack_for_level = 0;
	uint	range_for_level = 0;
	float	defense_for_level = 0;
	float	armor_for_level = 0;
	float	speed_for_level = 0;
	uint	view_area_for_level = 0;

	//List of all units buffed by the hero
	std::list<Unit*>	buffed_units;

public:

	//Functionality ---------
	//Actions -----
	virtual void Hability_A();
	virtual void Hability_B();

	//Set Methods -
	void	SetPosition(float x, float y);
	void	SetBuffArea(const Circle& area);
	void	SetBuffToApply(const PassiveBuff* buff);
	void	SetLevel(uint lvl);
	void	SetAttackForLevel(uint atk_for_lvl);
	void	SetRangeForLevel(uint rng_for_lvl);
	void	SetDefenseForLevel(float def_for_lvl);
	void	SetArmorForLevel(float arm_for_lvl);
	void	SetSpeedForLevel(float spd_for_lvl);
	void	SetViewAreaForLevel(uint view_for_level);

	//Get Methods -
	Circle			GetBuffArea()const;
	PassiveBuff*	GetBuffToApply()const;
	uint			GetLevel()const;
	uint			GetAttackForLevel()const;
	uint			GetRangeForLevel()const;
	float			GetDefenseForLevel()const;
	float			GetArmorForLevel()const;
	float			GetSpeedForLevel()const;
	uint			GetViewAreaForLevel()const;

};
/// ---------------------------------------------


/// Class Warrior -------------------------------
// Class that defines the warrior champion states
class Warrior : public Champion
{
public:

	Warrior();
	Warrior(const Warrior& copy);
	~Warrior();

private:

	/* extra data */
	Triangle			special_attack_area;

public:

	//Functionality ---------
	//Draw --------
	bool	Draw(bool debug);
	//Actions -----
	void	Hability_A();
	void	CalculateSpecialAttackArea(const iPoint& base);

	//Set Methods -
	void	SetPosition(float x, float y);
	void	SetSpecialAttackArea(const Triangle& tri);


};
/// ---------------------------------------------
#endif // _CHAMPIONS_

