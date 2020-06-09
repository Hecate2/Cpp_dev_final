#include "cocos2d.h"
#include "AppMacros.h"
#include <string>
class weapon {
public:
	weapon() {
	}
	weapon(int num_in, int speed_in, int Damage_in, int distance_in,int max_num_in,std::string weapon_name_in) {
		num = num_in;
		speed = speed_in;
		Damage = Damage_in;
		distance = distance_in;
		max_num = max_num_in;
		weapon_name = weapon_name_in;
	}
	int num = 100;
	int speed = 5;
	int Damage = 5;
	int distance = 200;
	int max_num = 100;
	bool enable = true;
	std::string weapon_name;
};
class player :public cocos2d::Sprite {
public:
	//player();
	enum Direction { UP, DOWN, LEFT, RIGHT };
	static player* player::create(const std::string& filename);
	void set_direction(Direction d) {
		Curr_Dir = d;
	}
	Direction get_direction() {
		return Curr_Dir;
	}
	weapon get_weapon_attribute() {
		return (*current_weapon);
	}
	bool Is_out_of_bullet() {
		return (current_weapon->num == 0);
	}
	bool Is_barrel() {
		return current_weapon->weapon_name == "barrel";
	}
	void decrease_weapon_num() {
		if ((*current_weapon).weapon_name!="pistol")
			(*current_weapon).num--;
	}
	bool change_weapon(int weapon_option) {
		if (!weapon_store[weapon_option].enable)
			return false;
		current_weapon = &(weapon_store[weapon_option]);
		if (weapon_option)
			current_weapon_label->setString(current_weapon->weapon_name + ":" + std::to_string(current_weapon->num));
		else
			current_weapon_label->setString(current_weapon->weapon_name);
		return true;
	}
	int get_hp() {
		return hp;
	}
	void decrease_hp_barrel() {
		hp -= 20;
	}
	void add_hp_timely() {
		if (hp<100)
			hp++;
	}
	void renew_display_num() {
		if (current_weapon->weapon_name!="pistol")
			current_weapon_label->setString(current_weapon->weapon_name + ":"+std::to_string(current_weapon->num));
	}
	void add_bullet_random() {
		for (int i = 1; i < 4; i++) {
			if (weapon_store[i].num < weapon_store[i].max_num) {
				weapon_store[i].num = weapon_store[i].max_num;
				break;
			}
		}
	}
	virtual bool init() override;
private:
	Direction Curr_Dir = UP;
	int hp = 100;
	weapon* current_weapon;
	cocos2d::Label* current_weapon_label;
	std::vector<weapon> weapon_store;
};
