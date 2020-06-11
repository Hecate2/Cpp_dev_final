#include "cocos2d.h"
#include "AppMacros.h"
#include <string>
//简易武器类
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
	//比较简短的函数为了方便直接写于头文件
	void set_direction(Direction d) {//设置当前方向
		Curr_Dir = d;
	}
	Direction get_direction() {//获得当前方向
		return Curr_Dir;
	}
	weapon get_weapon_attribute() {//获得当前武器属性
		return (*current_weapon);
	}
	bool Is_out_of_bullet() {//判断子弹是否耗尽
		return (current_weapon->num == 0);
	}
	bool Is_barrel() {//判断当前武器是否为油漆桶
		return current_weapon->weapon_name == "barrel";
	}
	void decrease_weapon_num() {//减小子弹数量
		if ((*current_weapon).weapon_name!="pistol")
			(*current_weapon).num--;
	}
	bool change_weapon(int weapon_option);//更换武器
	int get_hp() {//获得当前hp
		return hp;
	}
	void decrease_hp_barrel() {//在与油漆桶爆炸范围内的血量损失
		hp -= 20;
	}
	void add_hp_timely() {//每秒回血
		if (hp < 100)
			hp = hp + 3;
	}
	void renew_display_num() {//更新武器显示
		if (current_weapon->weapon_name!="pistol")
			current_weapon_label->setString(current_weapon->weapon_name + ":"+std::to_string(current_weapon->num));
	}
	void add_bullet();
	void player_attack();
	int hp = 100;
	void player_move(cocos2d::EventKeyboard::KeyCode keyCode);
	void Is_under_attack(int dir);
	virtual bool init() override;
private:
	Direction Curr_Dir = UP;//当前方向
	weapon* current_weapon;//当前武器
	cocos2d::Label* current_weapon_label;//当前武器显示
	std::vector<weapon> weapon_store;//武器仓库
	void init_weapon_system();//初始化武器库和显示
	void init_hp_bar();//初始化血条
};


