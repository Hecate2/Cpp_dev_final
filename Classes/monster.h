#include "cocos2d.h"
#include "AppMacros.h"
#include <string>

int getdirection(cocos2d::Vec2 diff);	//根据相对位置得到移动方向

class Monster :public cocos2d::Sprite {
public:
	virtual void move(int dir)=0;
	virtual void attack(cocos2d::Vec2 diff) =0;	//攻击函数
<<<<<<< Updated upstream
private:
	int hp = 100;
=======
	virtual void under_attack(cocos2d::Vec2 diff,int de_hp) = 0;	//被攻击函数
	virtual void death() = 0;
	int hp;
>>>>>>> Stashed changes
};

//Boss
class BigMonster :public Monster{
public:
	static BigMonster* BigMonster::create(const std::string& filename);
	void move(int dir);
	void attack(cocos2d::Vec2 diff);
<<<<<<< Updated upstream

=======
	void under_attack(cocos2d::Vec2 diff,int de_hp);
	void death();
	bool is_under_attack = false;
	int hp = 300;
>>>>>>> Stashed changes
};


//小怪
<<<<<<< Updated upstream
=======
class SmallMonster :public Monster {
public:
	static SmallMonster* SmallMonster::create(const std::string& filename);
	void move(int dir);
	void attack(cocos2d::Vec2 diff);
	void under_attack(cocos2d::Vec2 diff, int de_hp);
	void death();
	bool is_under_attack = false;
	int hp = 100;
};
>>>>>>> Stashed changes
