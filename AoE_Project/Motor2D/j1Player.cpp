#include "j1Player.h"

#include "p2Log.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Animator.h"

#include "j1App.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1EntitiesManager.h"

#include "UI_Element.h"
#include "UI_Image.h"
#include "UI_String.h"


//Entity Profile Constructor =====================================================
Entity_Profile::Entity_Profile()
{
	element = nullptr;
	name = (UI_String*) App->gui->GenerateUI_Element(STRING);
	name->SetColor({ 0,0,0,255 });
	diplomacy = (UI_String*)App->gui->GenerateUI_Element(STRING);
	diplomacy->SetColor({ 0,0,0,255 });
}

//Entity Profile Destructor ======================================================
Entity_Profile::~Entity_Profile()
{
}

//Functionality ==================================================================
void Entity_Profile::SetEntity(Entity * entity_selected)
{
	element = entity_selected;
	name->SetString((char*)element->GetName());
	 
	switch (element->GetDiplomacy())
	{
	case NEUTRAL: diplomacy->SetString("Neutral");
		break;
	case ALLY: diplomacy->SetString("Player");
		break;
	case ENEMY: diplomacy->SetString("Enemy");
		break;
	}
}

void Entity_Profile::DrawProfile() const
{
	//Draw profile background
	App->render->DrawQuad({ 338 - App->render->camera.x, 628 - App->render->camera.y, 39, 41 }, 148, 148, 148);
	App->render->DrawQuad({ 340 - App->render->camera.x, 666 - App->render->camera.y, 36, 3 }, 255, 0, 0);

	//Draw icon
	App->render->Blit(App->gui->Get_UI_Texture(ICONS), 340 - App->render->camera.x, 630 - App->render->camera.y, &element->GetIcon());
	name->DrawAt(390, 630);
	diplomacy->DrawAt(390, 650);
	
}

//j1Player Constructor ============================================================
j1Player::j1Player()
{
	name = "player";
}

//j1Player Destructor ============================================================
j1Player::~j1Player()
{
}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;


	return ret;
}

bool j1Player::Start()
{
	game_hud = (UI_Image*) App->gui->GenerateUI_Element(IMG);
	game_hud->ChangeTextureId(HUD);
	game_hud->SetLayer(10);
	App->gui->PushScreen(game_hud);

	selection_rect = { 0,0,0,0 };
	Selected = new Entity_Profile();
	
	UI_Image*	profile;
	group_profile.reserve(max_selected_units);
	int i = 0;
	
	while (i < max_selected_units)
	{
		profile = (UI_Image*)App->gui->GenerateUI_Element(IMG);
		profile->Desactivate();
		profile->ChangeTextureId(ICONS);
		profile->SetBoxPosition(320, 630);
		group_profile.push_back(profile);
		i++;
	}

	double_clickon = false;

	return true;
}

bool j1Player::PreUpdate()
{
	int x, y;
	App->input->GetMousePosition(x, y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		selection_rect.x = x;
		selection_rect.y = y;
		Uint32 double_click_read = double_click.Read();

		Select_Entity();
		uint selected_size = selected_elements.size();

		if ((selected_size == 1 && double_click_read == 0)|| (selected_size == 1 && double_click_read >= 500))
		{
			double_click.Start();
		}
		else if (double_click_read > 0 && double_click_read < 500)
		{
			double_clickon = true;
			Select_Type();
		}
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		if (selected_elements.size() == 1 && selected_elements.begin()._Ptr->_Myval->GetEntityType() == UNIT)
		{
			App->entities_manager->SetUnitPath((Unit*)selected_elements.begin()._Ptr->_Myval, iPoint(x, y));
		}
	}

	//Generate a Militia unit in the mouse coordinates
	if(App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
	{
		Unit* new_unit = App->entities_manager->GenerateUnit(MILITIA);
		new_unit->SetPosition(x - App->render->camera.x, y - App->render->camera.y);
		new_unit->SetDiplomacy(ALLY);
		actual_population.push_back(new_unit);
	}
	//Generate a Arbalest unit in the mouse coordinates
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		Unit* new_unit = App->entities_manager->GenerateUnit(ARBALEST);
		new_unit->SetPosition(x - App->render->camera.x, y - App->render->camera.y);
		new_unit->SetDiplomacy(ALLY);
		actual_population.push_back(new_unit);
	}

	return true;
}

bool j1Player::PostUpdate()
{
	game_hud->Draw(false);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && double_clickon == false)
	{
		Expand_SelectionRect();
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		if (double_clickon == true)
		{
			double_clickon = false;
		}
		else
		{
			Select_Group();
		}
		selection_rect = { 0,0,0,0 };
	}

	if (selected_elements.size() == 1) Selected->DrawProfile();
	else if (selected_elements.size() > 1) DrawGroup();

	return true;
}

bool j1Player::CleanUp()
{
	//Clear list and vectors
	actual_population.clear();
	selected_elements.clear();
	group_profile.clear();

	delete Selected;

	return true;
}

//Draw group_profiles
void j1Player::DrawGroup()
{
	SDL_Rect* box = nullptr;
	uint	  life = 0, max_life = 0;

	int i = selected_elements.size()-1;
	std::list<Entity*>::const_iterator item = selected_elements.end();
	while (item != selected_elements.begin())
	{
		item--;
		box = group_profile[i]->GetBox();
		//Draw profile background
		App->render->DrawQuad({ box->x - 2 - App->render->camera.x, box->y - 2 - App->render->camera.y, 39, 41 }, 148, 148, 148);
		App->render->DrawQuad({ box->x - App->render->camera.x, box->y +36 - App->render->camera.y, 36, 3 }, 255, 0, 0);

		//Draw life

		//Draw icon
		group_profile[i]->Draw(App->debug_mode);

		i--;
	}
}

bool j1Player::MoveSelectedUnits(int x, int y)
{
	return true;
}

//Check if unit is inside selection rect
bool j1Player::UnitisIn(int x, int y, int width, int height)
{
	if (selection_rect.w < 0)
	{
		selection_rect.x += selection_rect.w;
		selection_rect.w = -selection_rect.w;
	}
	if (selection_rect.h < 0)
	{
		selection_rect.y += selection_rect.h;
		selection_rect.h = -selection_rect.h;
	}
	int camera_x, camera_y;
	camera_x = App->render->camera.x;
	camera_y = App->render->camera.y;

	if (PointisIn(x,y)) return true;
	if (PointisIn(x + width, y + height)) return true;
	if (PointisIn(x + width,y)) return true;
	if (PointisIn(x, y + height)) return true;

	return false;
}

//Check if point is inside selection rect
bool j1Player::PointisIn(int x, int y)
{
	int camera_x, camera_y;
	camera_x = App->render->camera.x;
	camera_y = App->render->camera.y;

	if (selection_rect.x - camera_x < x && selection_rect.x - camera_x + selection_rect.w > x && selection_rect.y - camera_y < y && selection_rect.y - camera_y + selection_rect.h > y) return true;
	else return false;
}

//Select one unit
void j1Player::Select_Entity()
{
	int x, y;
	App->input->GetMousePosition(x, y);

	x -= App->render->camera.x;
	y -= App->render->camera.y;

	UnSelect_Entity();

	iPoint item_position;
	iPoint item_pivot;
	int rect_width = 0;
	int rect_height = 0;

	std::list<Unit*>::const_iterator item = actual_population.begin();
	while (item != actual_population.end())
	{
		rect_width = item._Ptr->_Myval->GetAnimation()->GetCurrentSprite()->GetFrame()->w;
		rect_height = item._Ptr->_Myval->GetAnimation()->GetCurrentSprite()->GetFrame()->h;
		
		fPoint pos = item._Ptr->_Myval->GetPosition();
		
		item_position = iPoint(pos.x, pos.y);
		item_pivot = { item._Ptr->_Myval->GetAnimation()->GetCurrentSprite()->GetXpivot(),item._Ptr->_Myval->GetAnimation()->GetCurrentSprite()->GetYpivot() };
		item_position -= item_pivot;

		if (x >= item_position.x && x <= item_position.x + rect_width && y >= item_position.y && y <= item_position.y + rect_height)
		{
			if (selected_elements.begin()._Ptr->_Myval != item._Ptr->_Myval)
			{
				if (selected_elements.size() == 1)
				{
					if (selected_elements.begin()._Ptr->_Myval->GetPosition().y <= item._Ptr->_Myval->GetPosition().y)
					{
						selected_elements.begin()._Ptr->_Myval->Deselect();
						selected_elements.clear();
					}
				}
				if(selected_elements.size() == 0)
				{
					selected_elements.push_back(item._Ptr->_Myval);
					LOG("Entity Selected: Type: %s", selected_elements.begin()._Ptr->_Myval->GetName());
					Selected->SetEntity(item._Ptr->_Myval);
					item._Ptr->_Myval->Select();
				}
			}			
		}
		else item._Ptr->_Myval->Deselect();
		item++;
	}

}

//Select group of units (only Allys)
void j1Player::Select_Group()
{
	if (selection_rect.w == 0 || selection_rect.h == 0) return;
	UnSelect_Entity();

	int x = 0, y = 0;
	int width = 0, height = 0;

	std::list<Unit*>::const_iterator item = actual_population.begin();
	while (item != actual_population.end())
	{
		if (item._Ptr->_Myval->GetDiplomacy() == ALLY && selected_elements.size() < 60)
		{
			x = item._Ptr->_Myval->GetPosition().x;
			x -= item._Ptr->_Myval->GetAnimation()->GetCurrentSprite()->GetXpivot();
			y = item._Ptr->_Myval->GetPosition().y;
			y -= item._Ptr->_Myval->GetAnimation()->GetCurrentSprite()->GetYpivot();
			width = item._Ptr->_Myval->GetSelectionRect()->w;
			height = item._Ptr->_Myval->GetSelectionRect()->h;
			
			if (UnitisIn(x, y, width, height))
			{
				selected_elements.push_back(item._Ptr->_Myval);
				item._Ptr->_Myval->Select();
			}
		}
		item++;
	}
	if (selected_elements.size() == 1) Selected->SetEntity(selected_elements.begin()._Ptr->_Myval);
	else
	{
		max_row_units = 16;
		SetGroupProfile();
	}
	LOG("Selected: %i", selected_elements.size());
}

void j1Player::Select_Type()
{
	if (selected_elements.begin()._Ptr->_Myval->GetEntityType() != UNIT) return;
	uint width = 0, height = 0;
	App->win->GetWindowSize(width, height);

	selection_rect = { 0, 32, (int)width, 560 };

	Unit* selected = (Unit*)selected_elements.begin()._Ptr->_Myval;
	UNIT_TYPE type = selected->GetUnitType();
	
	UnSelect_Entity();
	
	int x = 0, y = 0;
	
	std::list<Unit*>::const_iterator item = actual_population.begin();
	while (item != actual_population.end())
	{
		if (item._Ptr->_Myval->GetUnitType() == type && item._Ptr->_Myval->GetDiplomacy() == ALLY && selected_elements.size() < 60)
		{
			x = item._Ptr->_Myval->GetPosition().x;
			x -= item._Ptr->_Myval->GetAnimation()->GetCurrentSprite()->GetXpivot();
			y = item._Ptr->_Myval->GetPosition().y;
			y -= item._Ptr->_Myval->GetAnimation()->GetCurrentSprite()->GetYpivot();
			width = item._Ptr->_Myval->GetSelectionRect()->w;
			height = item._Ptr->_Myval->GetSelectionRect()->h;

			if (UnitisIn(x, y, width, height))
			{
				selected_elements.push_back(item._Ptr->_Myval);
				item._Ptr->_Myval->Select();
			}
		}
		item++;
	}
	if (selected_elements.size() == 1) Selected->SetEntity(selected_elements.begin()._Ptr->_Myval);
	else
	{
		max_row_units = 16;
		SetGroupProfile();
	}
}

void j1Player::UnSelect_Entity()
{
	std::list<Unit*>::const_iterator item = actual_population.begin();
	while (item != actual_population.end())
	{
		item._Ptr->_Myval->Deselect();
		item++;
	}
	selected_elements.clear();
}

//Expand Selection Rect
void j1Player::Expand_SelectionRect()
{
	App->input->GetMousePosition(selection_rect.w, selection_rect.h);

	selection_rect.w -= selection_rect.x;
	selection_rect.h -= selection_rect.y;
	
	App->render->DrawQuad({ selection_rect.x - App->render->camera.x, selection_rect.y - App->render->camera.y, selection_rect.w, selection_rect.h }, 255, 255, 255, 255, false);
}

void j1Player::SetGroupProfile()
{
	int i = 0;
	int j = 0;

	std::list<Entity*>::const_iterator item = selected_elements.begin();
	while (item != selected_elements.end())
	{
		if (i%max_row_units == 0 && i != 0) j++;
		if (j == 3) {
			max_row_units++;
			SetGroupProfile();
			return;
		} 
		group_profile[i]->ChangeTextureRect(item._Ptr->_Myval->GetIcon());
		group_profile[i]->SetBoxPosition(340 + (i%max_row_units) * (row_size/max_row_units), 630 + j * 42);
		item++;
		i++;
	}
}

