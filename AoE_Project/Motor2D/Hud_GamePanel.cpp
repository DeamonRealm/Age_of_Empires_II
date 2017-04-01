#include "Hud_GamePanel.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Menu.h"

#include "BaseEntities.h"

//UI
#include "UI_String.h"
#include "UI_Button.h"
#include "UI_Element.h"

// Constructor ================================================================
Game_Panel::Game_Panel() : wood(0), meat(0), gold(0), stone(0), population(0), max_population(5),
wood_width(0), meat_width(0), gold_width(0), stone_width(0), population_width(0)
{
	player_resources.reserve(MAX_GAME_RESOURCES);

	UI_String* resource_text;
	int i = 0;
	while (i < MAX_GAME_RESOURCES)
	{
		resource_text = (UI_String*)App->gui->GenerateUI_Element(STRING);
		resource_text->SetColor({ 255, 255, 255, 255 });
		resource_text->SetBoxPosition(73 + 77 * i, 5);
		player_resources.push_back(resource_text);
		i++;
	}

	App->gui->SetDefaultInputTarget((j1Module*)App->player);

	//Exit Menu 
	
	exit_menu_screen = App->gui->GenerateUI_Element(UI_TYPE::UNDEFINED);
	exit_menu_screen->SetBox({ 0,0,App->win->screen_surface->w, 23});
	exit_menu_screen->Activate();
	exit_menu_screen->SetInputTarget((j1Module*)App->player);
	exit_menu_screen->SetLayer(2);


	exit_menu_button = (UI_Button*)App->gui->GenerateUI_Element(UI_TYPE::BUTTON);
	exit_menu_button->SetBox({ 1306,3,50,17 });
	exit_menu_button->SetTexON({ 250,193, 50,17 }, ICONS);
	exit_menu_button->SetTexOFF({ 200,193, 50, 17 }, ICONS);
	exit_menu_button->SetTexOVER({ 200,193,50,17 }, ICONS);
	exit_menu_button->Activate();
	exit_menu_screen->AddChild(exit_menu_button);

	exit_menu_image = (UI_Image*)App->gui->GenerateUI_Element(UI_TYPE::IMG);
	exit_menu_image->SetBox({450,150,243,345});
	exit_menu_image->ChangeTextureId(TEXTURE_ID::CHAMPION_SKILL);
	exit_menu_image->ChangeTextureRect({ 0,0,243,345 });
	exit_menu_image->Desactivate();
	exit_menu_screen->AddChild(exit_menu_image);

	exit_to_main_menu = (UI_Button*)App->gui->GenerateUI_Element(UI_TYPE::BUTTON);
	exit_to_main_menu->SetBox({0, 0, 36, 36});
	exit_to_main_menu->SetTexOFF({396, 185,36,36}, CHAMPION_SKILL);
	exit_to_main_menu->SetTexOVER({ 396,185,36,36 }, CHAMPION_SKILL);
	exit_to_main_menu->SetTexON({ 391,250,36,36 }, CHAMPION_SKILL);
	exit_to_main_menu->Activate();
	exit_menu_image->AddChild(exit_to_main_menu);

}

// Destructor ==============================================
Game_Panel::~Game_Panel()
{
	CleanUp();
}

// FUNCTIONALITY ===========================================
bool Game_Panel::CleanUp()
{
	hud_game_text.clear();

	player_resources.clear();

	return true;
}

bool Game_Panel::PreUpdate()
{
	return true;
}

bool Game_Panel::PostUpdate()
{
	return true;
}

bool Game_Panel::Draw()
{
	for (int i = 0; i < MAX_GAME_RESOURCES; i++)
	{
		player_resources[i]->Draw(false);
	}

	return true;
}

void Game_Panel::Handle_Input(UI_Element * ui_element, GUI_INPUT ui_input)
{
	switch (ui_input)
	{
	case UP_ARROW:
		break;
	case DOWN_ARROW:
		break;
	case LEFT_ARROW:
		break;
	case RIGHT_ARROW:
		break;
	case MOUSE_LEFT_BUTTON_DOWN:
	{
		if (ui_element == exit_menu_button)
		{
			if (exit_menu_image->GetActiveState()) exit_menu_image->Desactivate();
			else exit_menu_image->Activate();
		}
		
	}

	break;
	case MOUSE_LEFT_BUTTON_REPEAT:
		break;
	case MOUSE_LEFT_BUTTON_UP:
	{

		if (ui_element == exit_to_main_menu)
		{
			exit_menu_image->Desactivate();
			App->scene->Disable();
			App->player->Disable();
			App->menu->Enable();
		}
	}
		break;
	case MOUSE_RIGHT_BUTTON:
		break;
	case MOUSE_IN:
		break;
	case MOUSE_OUT:
		break;
	case SUPR:
		break;
	case BACKSPACE:
		break;
	case ENTER:
		break;
	case TAB:
		break;
	default:
		break;
	}
}

bool Game_Panel::AddResource(int amount, PLAYER_RESOURCES resource_type)
{
	bool ret = false;
	
	int height = 0;

	switch (resource_type)
	{
	case GP_WOOD:
		if (wood + amount >= 0)
		{
			wood += amount;
			player_resources[resource_type]->SetString(App->gui->SetStringFromInt(wood));
			App->font->CalcSize(player_resources[resource_type]->GetString(), wood_width, height);
			player_resources[resource_type]->SetBoxPosition(73 + 77 * resource_type - wood_width, 5);
			ret = true;
		}
		break;
	case GP_MEAT:
		if (meat + amount >= 0)
		{
			meat += amount;
			player_resources[resource_type]->SetString(App->gui->SetStringFromInt(meat));
			App->font->CalcSize(player_resources[resource_type]->GetString(), meat_width, height);
			player_resources[resource_type]->SetBoxPosition(73 + 77 * resource_type - meat_width, 5);
			ret = true;
		}
		break;
	case GP_GOLD:
		if (gold + amount >= 0)
		{
			gold += amount;
			player_resources[resource_type]->SetString(App->gui->SetStringFromInt(gold));
			App->font->CalcSize(player_resources[resource_type]->GetString(), gold_width, height);
			player_resources[resource_type]->SetBoxPosition(73 + 77 * resource_type - gold_width, 5);
			ret = true;
		}
		break;
	case GP_STONE:
		if (stone + amount >= 0)
		{
			stone += amount;
			player_resources[resource_type]->SetString(App->gui->SetStringFromInt(stone));
			App->font->CalcSize(player_resources[resource_type]->GetString(), stone_width, height);
			player_resources[resource_type]->SetBoxPosition(73 + 77 * resource_type - stone_width, 5);
			ret = true;
		}
		break;
	default:
		break;
	}
	return ret;
}

bool Game_Panel::IncressPopulation(int amount, bool increase_max)
{
	int height = 0;
	if (increase_max)
	{
		if (max_population + amount <= MAX_POPULATION) max_population += amount;
		else max_population = MAX_POPULATION;
		hud_game_text.clear();
		hud_game_text = App->gui->SetStringFromInt(population);
		hud_game_text = hud_game_text + "/" + App->gui->SetStringFromInt(max_population);
	}
	else
	{
		if (population + amount <= MAX_POPULATION && population + amount >= 0)
		{
			population += amount;
			hud_game_text.clear();
			hud_game_text = App->gui->SetStringFromInt(population);
			hud_game_text = hud_game_text + "/" + App->gui->SetStringFromInt(max_population);
		}
		else return false;
	}
	player_resources[4]->SetString((char*)hud_game_text.c_str());
	App->font->CalcSize(player_resources[4]->GetString(), population_width, height);
	player_resources[4]->SetBoxPosition(381 - population_width, 5);

	return true;
}

bool Game_Panel::CheckPopulation() const
{
	if (population == max_population) return false;
	else return true;
}

UI_Element * Game_Panel::GetExitMenu()
{
	return exit_menu_screen;
}

