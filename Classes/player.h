#include "cocos2d.h"
#include "AppMacros.h"
#include <string>
//����������
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
	//�Ƚϼ�̵ĺ���Ϊ�˷���ֱ��д��ͷ�ļ�
	void set_direction(Direction d) {//���õ�ǰ����
		Curr_Dir = d;
	}
	Direction get_direction() {//��õ�ǰ����
		return Curr_Dir;
	}
	weapon get_weapon_attribute() {//��õ�ǰ��������
		return (*current_weapon);
	}
	bool Is_out_of_bullet() {//�ж��ӵ��Ƿ�ľ�
		return (current_weapon->num == 0);
	}
	bool Is_barrel() {//�жϵ�ǰ�����Ƿ�Ϊ����Ͱ
		return current_weapon->weapon_name == "barrel";
	}
	void decrease_weapon_num() {//��С�ӵ�����
		if ((*current_weapon).weapon_name!="pistol")
			(*current_weapon).num--;
	}
	bool change_weapon(int weapon_option);//��������
	int get_hp() {//��õ�ǰhp
		return hp;
	}
	void decrease_hp_barrel() {//��������Ͱ��ը��Χ�ڵ�Ѫ����ʧ
		hp -= 20;
	}
	void add_hp_timely() {//ÿ���Ѫ
		if (hp < 100)
			hp = hp + 3;
	}
	void renew_display_num() {//����������ʾ
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
	Direction Curr_Dir = UP;//��ǰ����
	weapon* current_weapon;//��ǰ����
	cocos2d::Label* current_weapon_label;//��ǰ������ʾ
	std::vector<weapon> weapon_store;//�����ֿ�
	void init_weapon_system();//��ʼ�����������ʾ
	void init_hp_bar();//��ʼ��Ѫ��
};


