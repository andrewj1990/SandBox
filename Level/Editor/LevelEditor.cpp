#include "LevelEditor.h"

LevelEditor::LevelEditor()
	: key_press_delay_(), background_(glm::vec3(0), glm::vec2(Window::Instance().getWidth(), 300), glm::vec4(0.4f))
	, texture_name_("Texture : ", 100, 100, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1))
	, texture_name_value_("", 100, 100, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1))
	, position_("Position : ", 100, 100, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1))
	, position_value_("", 100, 100, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1))
	, rotation_("Rotation : ", 100, 100, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1))
	, rotation_value_("", 100, 100, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1)) 
{
	read_chars_ = false;
	grid_snap_ = true;
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
	}

	level.close();
}

void LevelEditor::load()
{
	entities_.clear();
	selected_object_ = SelectedObject::NOTHING;
	std::string param_name;
	std::string texture_name;
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
		entities_.push_back(std::move(object));
	}

	std::cout << "objects : " << entities_.size() << "\n";

	level.close();
}

void LevelEditor::update(float timeElapsed)
{
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

	selectObject();

	if (selected_object_ == SelectedObject::NEW_OBJECT) {
		current_object_ = std::make_unique<Entity>();
	}
	else {
		current_object_ = nullptr;
	}

	end_pos_.x = Window::Instance().getMouseWorldPosX();
	end_pos_.y = Window::Instance().getMouseWorldPosY();
	if (grid_snap_) {
		glm::vec2 factor = end_pos_ - start_pos_;
		factor = factor / factor;
		end_pos_.x = (int)(end_pos_.x) / 32 * 32 + (factor.x) * 32;
		end_pos_.y = (int)(end_pos_.y) / 32 * 32 + (factor.y) * 32;
	}

	switch (selected_object_)
	{
	case SelectedObject::NEW_OBJECT:
		current_object_->setPosition(start_pos_.x, start_pos_.y);
		current_object_->setSize(end_pos_ - start_pos_);
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

		if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1) && key_press_delay_.elapsed() > 0.1f) {
			selected_object_ = SelectedObject::NOTHING;
			key_press_delay_.reset();
		}
		break;
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

void LevelEditor::selectObject()
{
	if (selected_object_ == SelectedObject::OLD_OBJECT) return;

	if (Window::Instance().isKeyPressed(GLFW_KEY_LEFT_CONTROL) && Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1) && key_press_delay_.elapsed() > 0.1f) {
		float x = Window::Instance().getMouseWorldPosX();
		float y = Window::Instance().getMouseWorldPosY();

		BoundingBox mouseBox(x, y, 5, 5);

		for (auto& object : entities_) {
			if (object->collide(mouseBox)) {
				selected_object_ = SelectedObject::OLD_OBJECT;
				std::unique_ptr<Entity> copy = std::make_unique<Entity>();
				copy->setTexture(TextureManager::get(object->getTexture()->getName()));
				copy->setSize(object->getSize());
				entities_.push_back(std::move(copy));
				edit_object_ = entities_.back().get();
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
				break;
			}
		}
		key_press_delay_.reset();
	}
}
