#pragma once

#include <fstream>

#include "..\..\Graphics\Renderer.h"
#include "..\..\Entity\Entity.h"
#include "..\..\Graphics\label.h"

class LevelEditor
{
	enum SelectedObject
	{
		NEW_OBJECT,
		OLD_OBJECT,
		AREA_SELECT,
		NOTHING
	};

	std::vector<std::unique_ptr<Entity>> entities_;
	std::unique_ptr<Entity> current_object_;
	std::vector<std::unique_ptr<Entity>>  area_objects_;
	Entity* edit_object_;
	glm::vec2 start_pos_;
	glm::vec2 end_pos_;
	glm::vec2 offset_pos_;

	Sprite background_;

	// menu items
	Label texture_name_;
	Label texture_name_value_;
	Label position_;
	Label position_value_;
	Label rotation_;
	Label rotation_value_;

	Timer key_press_delay_;

	bool read_chars_;
	std::string input_;
	bool grid_snap_;

	SelectedObject selected_object_;

public:
	LevelEditor();

	void save();
	void load();

	void update(float timeElapsed);
	void render(Renderer& renderer);

private:

	void updateMenu(Entity* object);
	void renderMenu(Renderer& renderer);

	void selectObject();
};