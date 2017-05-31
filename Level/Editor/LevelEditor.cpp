#include "LevelEditor.h"

LevelEditor::LevelEditor()
	: key_press_delay_(), background_(glm::vec3(0), glm::vec2(Window::Instance().getWidth(), 300), glm::vec4(0.4f))
	, multi_select_box_(glm::vec3(0), glm::vec2(0), TextureManager::get("Textures/box.png"))
	, texture_name_("Texture : ", 100, 100, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1))
	, texture_name_value_("", 100, 100, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1))
	, position_("Position : ", 100, 100, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1))
	, position_value_("", 100, 100, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1))
	, rotation_("Rotation : ", 100, 100, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1))
	, rotation_value_("", 100, 100, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1)) 
{
	read_chars_ = false;
	grid_snap_ = true;
	render_collision_box_ = false;
	mouse_state_ = false;
	area_select_mouse_state_ = false;
	input_ = "";
	selected_object_ = SelectedObject::NOTHING;
}

void LevelEditor::save()
{
	std::ofstream level;
	level.open("level.txt");

	for (const auto& object : entities_) {
		level << "Object\n";
		level << "\ttexture " << object->getTexture()->getName() << "\n";
		level << "\tposition " << object->getX() << " " << object->getY() << "\n";
		level << "\tsize " << object->getWidth() << " " << object->getHeight() << "\n";
		level << "\tsolid " << object->isSolid() << "\n";
		level << "\tcbox " << object->getCollisionBox().x << " " << object->getCollisionBox().y 
			<< " " << object->getCollisionBox().width << " " << object->getCollisionBox().height << "\n";
	}

	level.close();
}

void LevelEditor::load()
{
	entities_.clear();
	selected_object_ = SelectedObject::NOTHING;
	std::string param_name;
	std::string texture_name;
	bool solid;
	float x;
	float y;
	float w;
	float h;

	std::ifstream level("level.txt");

	while (level >> param_name) {
		level >> param_name >> texture_name;
		level >> param_name >> x >> y;
		level >> param_name >> w >> h;

		std::unique_ptr<Entity> object = std::make_unique<Entity>(glm::vec3(x, y, 0), glm::vec2(w, h), TextureManager::get(texture_name));
		level >> param_name >> solid;
		object->setSolid(solid);
		level >> param_name >> x >> y >> w >> h;
		object->getCollisionBox().x = x;
		object->getCollisionBox().y = y;
		object->getCollisionBox().width = w;
		object->getCollisionBox().height = h;
		entities_.push_back(std::move(object));
	}

	std::cout << "objects : " << entities_.size() << "\n";

	level.close();
}

void LevelEditor::update(float timeElapsed)
{
	area_objects_.clear();
	
	//if (selected_object_ != SelectedObject::MULTI_OBJECT_SELECT) 
	selected_objects_boxes_.clear();

	float mx = Window::Instance().getMouseWorldPosX();
	float my = Window::Instance().getMouseWorldPosY();

	if (Window::Instance().isKeyPressed(GLFW_KEY_C) && key_press_delay_.elapsed() > 0.1f) {
		render_collision_box_ = !render_collision_box_;
	}

	if (Window::Instance().isKeyPressed(GLFW_KEY_0) && key_press_delay_.elapsed() > 0.1f) {
		save();
		key_press_delay_.reset();
	}

	if (Window::Instance().isKeyPressed(GLFW_KEY_5) && key_press_delay_.elapsed() > 0.1f) {
		load();
		key_press_delay_.reset();
	}

	if (Window::Instance().isKeyPressed(GLFW_KEY_E)) {
		start_pos_.x = Window::Instance().getMouseWorldPosX();
		start_pos_.y = Window::Instance().getMouseWorldPosY();
		if (grid_snap_) {
			start_pos_.x = (int)(start_pos_.x) / 32 * 32;
			start_pos_.y = (int)(start_pos_.y) / 32 * 32;
		}
		selected_object_ = SelectedObject::NEW_OBJECT;
	}

	if (Window::Instance().isKeyPressed(GLFW_KEY_R)) {
		start_pos_.x = Window::Instance().getMouseWorldPosX();
		start_pos_.y = Window::Instance().getMouseWorldPosY();
		if (grid_snap_) {
			start_pos_.x = (int)(start_pos_.x) / 32 * 32;
			start_pos_.y = (int)(start_pos_.y) / 32 * 32;
		}
		selected_object_ = SelectedObject::AREA_PLACE;
	}

	if (!selectObject()) {
		areaSelect();
	}

	if (selected_object_ == SelectedObject::NEW_OBJECT) {
		current_object_ = std::make_unique<Entity>();
	}
	else {
		current_object_ = nullptr;
	}

	end_pos_.x = Window::Instance().getMouseWorldPosX();
	end_pos_.y = Window::Instance().getMouseWorldPosY();
	if (grid_snap_) {
		end_pos_.x = (int)(end_pos_.x) / 32 * 32;
		end_pos_.y = (int)(end_pos_.y) / 32 * 32;
	}

	switch (selected_object_)
	{
	case SelectedObject::NEW_OBJECT:
		current_object_->setPosition(start_pos_.x, start_pos_.y);
		current_object_->setSize(end_pos_ - start_pos_);
		current_object_->getCollisionBox().width = (end_pos_ - start_pos_).x;
		current_object_->getCollisionBox().height = (end_pos_ - start_pos_).y;
		current_object_->setTexture(TextureManager::get(input_));

		if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1)) {
			entities_.push_back(std::move(current_object_));
			std::cout << "size : " << entities_.size() << "\n";
			selected_object_ = SelectedObject::NOTHING;
		}

		updateMenu(current_object_.get());
		break;
	case SelectedObject::OLD_OBJECT:
		if (edit_object_) {
			float x = Window::Instance().getMouseWorldPosX() + offset_pos_.x;
			float y = Window::Instance().getMouseWorldPosY() + offset_pos_.y;
			if (grid_snap_) {
				x = (int)x / 32 * 32;
				y = (int)y / 32 * 32;
			}
			edit_object_->setPosition(x, y);
			updateMenu(edit_object_);
			if (Window::Instance().isKeyPressed(GLFW_KEY_LEFT_SHIFT) && Window::Instance().isKeyPressed(GLFW_KEY_V)) {
				edit_object_->setTexture(TextureManager::get(input_));
			}
		}

		selected_objects_boxes_.push_back(std::make_unique<Entity>(edit_object_->getPosition(), edit_object_->getSize(), TextureManager::get("Textures/box.png")));

		if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1) && key_press_delay_.elapsed() > 0.1f) {
			selected_object_ = SelectedObject::NOTHING;
			key_press_delay_.reset();
		}
		break;
	case SelectedObject::OLD_OBJECT_COLLISION_BOX:
	{
		if (edit_object_) {
			if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1) && !mouse_state_) {
				edit_object_->getCollisionBox().x = Window::Instance().getMouseWorldPosX();
				edit_object_->getCollisionBox().y = Window::Instance().getMouseWorldPosY();
				mouse_state_ = true;
			}

			if (mouse_state_) {
				int w = Window::Instance().getMouseWorldPosX() - edit_object_->getCollisionBox().x;
				int h = Window::Instance().getMouseWorldPosY() - edit_object_->getCollisionBox().y;
				edit_object_->getCollisionBox().width = w;
				edit_object_->getCollisionBox().height = h;
			}

			if (Window::Instance().isButtonReleased(GLFW_MOUSE_BUTTON_1) &&  mouse_state_) {
				mouse_state_ = false;
				selected_object_ = SelectedObject::NOTHING;
			}
		}
		break;
	}
	case SelectedObject::AREA_PLACE:
	{
		int x = (int)Window::Instance().getMouseWorldPosX() / 32 * 32;
		int y = (int)Window::Instance().getMouseWorldPosY() / 32 * 32;
		int sx = start_pos_.x;
		int sy = start_pos_.y;

		for (int i = std::min(sx, x); i < std::max(sx, x); i += 32) {
			for (int j = std::min(sy, y); j < std::max(sy, y); j += 32) {
				area_objects_.push_back(std::make_unique<Entity>(glm::vec3(i, j, 0), glm::vec2(32), TextureManager::get(input_)));
			}
		}

		if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1)) {
			while (!area_objects_.empty())
			{
				entities_.emplace_back(std::move(area_objects_.back()));
				area_objects_.pop_back();
			}
			selected_object_ = SelectedObject::NOTHING;
		}
		break;
	}
	case SelectedObject::MULTI_OBJECT_SELECT:
	{
		if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1)) {
			if (multi_select_box_.collide(BoundingBox(mx, my, 1, 1))) {
				std::cout << "selected\n";
			}
			else if (!area_select_mouse_state_) {
				selected_object_ = SelectedObject::NOTHING;
			}
		}
		break;
	}
	default:
		updateMenu(nullptr);
		break;
	};

}

void LevelEditor::render(Renderer& renderer)
{
	renderer.render(background_);

	renderer.begin();

	for (auto& entity : entities_) {
		entity->submit(renderer);
	}

	for (auto& entity : area_objects_) {
		entity->submit(renderer);
	}

	if (render_collision_box_) {
		for (auto& entity : entities_) {
			auto& cb = entity->getCollisionBox();
			renderer.debugSubmit(cb, TextureManager::get("Textures/collision_box.png"));
		}
	}

	for (auto& selectBox : selected_objects_boxes_) {
		selectBox->submit(renderer);
	}

	if (selected_object_ == SelectedObject::NEW_OBJECT) current_object_->submit(renderer);

	renderer.end();
	renderer.flush();

	renderMenu(renderer);

}

void LevelEditor::updateMenu(Entity* object)
{
	background_.setPosition(Window::Instance().getCamera().getPosition().x, Window::Instance().getCamera().getPosition().y);

	int bg_pos_x = background_.getX();
	int bg_pos_y = background_.getY();

	texture_name_.setPosition(glm::vec3(bg_pos_x + 50, bg_pos_y + background_.getSize().y - 50, 0));
	texture_name_value_.setPosition(glm::vec3(bg_pos_x + 200, bg_pos_y + background_.getSize().y - 50, 0));
	position_.setPosition(glm::vec3(bg_pos_x + 50, bg_pos_y + background_.getSize().y - 100, 0));
	position_value_.setPosition(glm::vec3(bg_pos_x + 200, bg_pos_y + background_.getSize().y - 100, 0));
	rotation_.setPosition(glm::vec3(bg_pos_x + 50, bg_pos_y + background_.getSize().y - 150, 0));
	rotation_value_.setPosition(glm::vec3(bg_pos_x + 200, bg_pos_y + background_.getSize().y - 150, 0));

	if (Window::Instance().isKeyPressed(GLFW_KEY_LEFT_SHIFT) && Window::Instance().isKeyPressed(GLFW_KEY_V)) {
		read_chars_ = true;
		input_ = "";
		std::cout << "texture path : ";
		std::cin >> input_;
		input_ = "Textures/" + input_ + ".png";
	}

	if (object) {
		texture_name_value_.text = object->getTexture()->getName();
		position_value_.text = std::to_string(object->getX()) + ", " + std::to_string(object->getY());
		rotation_value_.text = std::to_string(object->getAngle());
	}
}

void LevelEditor::renderMenu(Renderer & renderer)
{
	renderer.render(texture_name_);
	renderer.render(texture_name_value_);
	renderer.render(position_);
	renderer.render(position_value_);
	renderer.render(rotation_);
	renderer.render(rotation_value_);
}

bool LevelEditor::selectObject()
{
	bool ret = false; 
	if (selected_object_ == SelectedObject::OLD_OBJECT) return ret;
	if (selected_object_ != SelectedObject::NOTHING) return ret;

	if (Window::Instance().isKeyPressed(GLFW_KEY_LEFT_SHIFT) && Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1) && key_press_delay_.elapsed() > 0.1f) {
		float x = Window::Instance().getMouseWorldPosX();
		float y = Window::Instance().getMouseWorldPosY();

		BoundingBox mouseBox(x, y, 1, 1);

		for (auto& object : entities_) {
			if (object->collide(mouseBox)) {
				selected_object_ = SelectedObject::OLD_OBJECT_COLLISION_BOX;
				edit_object_ = object.get();
				offset_pos_.x = object->getX() - x;
				offset_pos_.y = object->getY() - y;
				ret = true;
				break;
			}
		}
		key_press_delay_.reset();
	}
	else if (Window::Instance().isKeyPressed(GLFW_KEY_LEFT_CONTROL) && Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1) && key_press_delay_.elapsed() > 0.1f) {
		float x = Window::Instance().getMouseWorldPosX();
		float y = Window::Instance().getMouseWorldPosY();

		BoundingBox mouseBox(x, y, 1, 1);

		for (auto& object : entities_) {
			if (object->collide(mouseBox)) {
				selected_object_ = SelectedObject::OLD_OBJECT;
				std::unique_ptr<Entity> copy = std::make_unique<Entity>(*object);
				entities_.push_back(std::move(copy));
				edit_object_ = entities_.back().get();
				ret = true;
				break;
			}
		}
		key_press_delay_.reset();
	}
	else if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1) && key_press_delay_.elapsed() > 0.1f) {
		float x = Window::Instance().getMouseWorldPosX();
		float y = Window::Instance().getMouseWorldPosY();

		BoundingBox mouseBox(x, y, 5, 5);

		for (auto& object : entities_) {
			if (object->collide(mouseBox)) {
				selected_object_ = SelectedObject::OLD_OBJECT;
				edit_object_ = object.get();
				offset_pos_.x = object->getX() - x;
				offset_pos_.y = object->getY() - y;
				ret = true;
				break;
			}
		}
		key_press_delay_.reset();
	}

	return ret;
}

void LevelEditor::areaSelect()
{
	if (selected_object_ == SelectedObject::NOTHING) {
		if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1) && !area_select_mouse_state_) {
			multi_select_box_.setPosition(Window::Instance().getMouseWorldPosX(), Window::Instance().getMouseWorldPosY());
			area_select_mouse_state_ = true;

			selected_object_ = SelectedObject::MULTI_OBJECT_SELECT;
		}
	}

	if (selected_object_ == SelectedObject::MULTI_OBJECT_SELECT) {
		int w = 0;
		int h = 0;
		if (area_select_mouse_state_) {
			w = Window::Instance().getMouseWorldPosX() - multi_select_box_.getX();
			h = Window::Instance().getMouseWorldPosY() - multi_select_box_.getY();
		}
		else {
			w = multi_select_end_pos_.x - multi_select_box_.getX();
			h = multi_select_end_pos_.y - multi_select_box_.getY();
		}

		multi_select_box_.setSize(glm::vec2(w, h));

		std::unique_ptr<Entity> select_box = std::make_unique<Entity>(glm::vec3(multi_select_box_.getX(), multi_select_box_.getY(), 0), glm::vec2(w, h), TextureManager::get("Textures/box.png"));

		for (auto& object : entities_) {
			if (object->collide(*select_box)) {
				selected_objects_boxes_.push_back(std::make_unique<Entity>(object->getPosition(), object->getSize(), TextureManager::get("Textures/box.png")));
			}
		}

		if (Window::Instance().isButtonReleased(GLFW_MOUSE_BUTTON_1) && area_select_mouse_state_) {
			multi_select_end_pos_.x = Window::Instance().getMouseWorldPosX();
			multi_select_end_pos_.y = Window::Instance().getMouseWorldPosY();
			area_select_mouse_state_ = false;

			return;
		}

		if (area_select_mouse_state_) selected_objects_boxes_.push_back(std::move(select_box));
	}

}
