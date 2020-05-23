#include "cocos2d.h"
class weapon {
public:
	int num = 100;
	int speed = 5;
	int Damage = 5;
	int distance = 200;
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
		return current_weapon;
	}
	void decrease_weapon_num() {
		current_weapon.num--;
	}
	void change_UZI() {
		current_weapon.speed = 10;
	}
private:
	Direction Curr_Dir = UP;
	int hp = 100;
	weapon current_weapon;

};
