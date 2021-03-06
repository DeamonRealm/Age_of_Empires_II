#ifndef __ACTION_BUILDING_H__
#define __ACTION_BUILDING_H__

#include "j1ActionManager.h"
#include "j1EntitiesManager.h"
#include "BaseEntities.h"
#include "j1App.h"
#include "j1SoundManager.h"
#include "Actions_Unit.h"
#include "j1Player.h"
#include "j1AI.h"

#include "Hud_MinimapPanel.h"

enum RESEARCH_TECH
{
	NO_TECH,

	//Archery Tech
	A_ARCHER_UP,
	A_CROSSBOW_UP,
	A_SKIMISHER_UP,
	A_C_ARCHER_UP,
	A_THUMBRING_UP,

	//Barrak Tech
	B_MILITIA_UP,
	B_MAN_AT_ARMS_UP,
	B_LONGSWORDSMAN_UP,
	B_TWO_HANDED_UP,
	B_SPEARMAN_UP,

	//Blacksmith Tech
	BS_PADDED_AA,
	BS_LEATHER_AA,
	BS_RING_AA,
	BS_FLETCHING,
	BS_BODKINARROW,
	BS_BRACER,
	BS_FORGING,
	BS_IRONCASTING,
	BS_BLASTFURNACE,
	BS_SCALE_BA,
	BS_CHAIN_BA,
	BS_PLATE_BA,
	BS_SCALE_MAIL_ARMOR,
	BS_CHAIN_MAIL_ARMOR,
	BS_PLATE_MAIL_ARMOR,

	// Stable
	S_BLOODLINES,
	S_KNIGHT_UP,
	S_CAVALLIER_UP,
	S_HUSBANDRY,
	S_SCOUTC_UP,

	// Seige Workshop

	// TownCenter Tech
	TC_FEUDAL,
	TC_CASTLE,
	TC_IMPERIAL,
	TC_LOOM,
	TC_PATROL,
	TC_TOWNWATCH,
	TC_WHEELBARROW,
	TC_HANDCART,

	// Castle Tech
	// Mill Tecnology
	M_HORSECOLLAR,
	M_HEAVYPLOW,
	M_CROPROTATION,

	// Lumber Camp Tecnology
	LC_DOUBLEBIT_AXE,
	LC_BOW_SAW,
	LC_TWOMAN_SAW,

	// Mining Camp Tecnology
	MC_GOLD_MINING,
	MC_GOLD_SHAFT,
	MC_STONE_MINING,
	MC_STONE_SHAFT

	// Mk Market

	// Monk
	// University
};

class SpawnUnitAction : public Action
{
public:

	SpawnUnitAction(ActionWorker* worker, ProductiveBuilding* actor, UNIT_TYPE new_type, DIPLOMACY diplomacy, uint runned_time = 0) : Action(actor, TASK_B_SPAWN_UNITS), u_type(new_type), diplomacy(diplomacy), worker(worker)
	{
		new_unit = App->entities_manager->GenerateUnit(u_type, diplomacy, false);
		if (runned_time > new_unit->GetTrainTime())time = 0;
		else time = new_unit->GetTrainTime() - runned_time;
	}

	~SpawnUnitAction()
	{
		if (new_unit != nullptr)
		{
			delete new_unit;
		}
	}

	bool Activation()
	{
		timer.Start();
		return true;
	}


	bool Execute()
	{
		if (timer.Read() >= time)
		{
			iPoint spawn_point = ((ProductiveBuilding*)actor)->GetSpawnPoint();

			int x = ((ProductiveBuilding*)actor)->GetSpawnPoint().x + actor->GetPosition().x;
			int y = ((ProductiveBuilding*)actor)->GetSpawnPoint().y + actor->GetPosition().y;
			iPoint position = new_unit->Spawn(iPoint(x,y));
			App->entities_manager->AddUnit(new_unit);
			new_unit->SetPosition((float)position.x, (float)position.y);
			App->animator->UnitPlay(new_unit);
			if (diplomacy == ALLY)
			{
				App->sound->PlayFXAudio(SOUND_TYPE::VILLAGER_CREATED_SOUND);
				if (u_type == WARRIOR_CHMP || u_type == WIZARD_CHMP || u_type == ARCHER_CHMP)
				{
					App->player->minimap_panel->SetHeroShortcut(new_unit, u_type);
				}
			}

			//Add an autoattack passive action
			new_unit->AddAction(App->action_manager->AutoAttackAction(new_unit), TASK_CHANNELS::PASSIVE);


			//Add it to the AI units list if it is an enemy
			if (new_unit->GetDiplomacy() == ENEMY)
				App->AI->enemy_units.push_back(new_unit);

			new_unit = nullptr;
			return true;
		}

		return false;
	}

	//Get Methods -----------
	DIPLOMACY GetUnitDiplomacy()const
	{
		return diplomacy;
	}

	UNIT_TYPE GetUnitType()const
	{
		return u_type;
	}

	uint GetTime()const
	{
		return timer.Read();
	}

	//Set Methods -----------
	void SetUnitType(UNIT_TYPE new_type)
	{
		u_type = new_type;
		delete new_unit;
		new_unit = App->entities_manager->GenerateUnit(u_type, diplomacy, false);
		if (new_unit == nullptr)
		{
			LOG("ups SetUnitType() doesnt work");
		}
	}

	void SetPausedTime(uint time)
	{
		this->time += time;
	}

	SDL_Rect GetIcon()
	{
		return new_unit->GetIcon();
	}

private:

	ActionWorker*	worker = nullptr;
	Unit*			new_unit = nullptr;
	uint			time = 0;
	j1Timer			timer;
	UNIT_TYPE		u_type = UNIT_TYPE::NO_UNIT;
	DIPLOMACY		diplomacy = DIPLOMACY::NEUTRAL;

};

class ResearchTecAction : public Action
{
public:

	ResearchTecAction(ActionWorker*	worker, RESEARCH_TECH type, uint r_time, DIPLOMACY diplomacy = ALLY) : Action(nullptr, TASK_B_RESEARCH), r_type(type), diplomacy(diplomacy), research_time(r_time), worker(worker){};
	~ResearchTecAction() {};

	bool Activation()
	{
		timer.Start();
		return true;
	}

	bool Execute()
	{
		if (research_time <= timer.Read())
		{
			App->entities_manager->UpgradeEntity(r_type, diplomacy);
			if (diplomacy == ALLY)
			{
				App->player->UpgradeCivilization(r_type);
				App->sound->PlayFXAudio(BLACKSMITH_SOUND);
			}
			else App->AI->UpgradeCivilization(r_type);
			
			return true;
		}

		return false;
	}

	//Get Methods -----------
	RESEARCH_TECH GetResearchType()const
	{
		return r_type;
	}

	DIPLOMACY GetDiplomacy()const
	{
		return diplomacy;
	}

	uint GetTime()const
	{
		return timer.Read();
	}

	uint GetResearchTime()const
	{
		return research_time;
	}

	//Set Methods -----------
	void SetPausedTime(uint time)
	{
		this->research_time += time;
	}

private:

	ActionWorker*	worker = nullptr;
	uint			research_time = 0;
	j1Timer			timer;
	RESEARCH_TECH	r_type = RESEARCH_TECH::NO_TECH;
	DIPLOMACY		diplomacy = DIPLOMACY::NEUTRAL;
};


#endif // __ACTION_BUILDING_H__
