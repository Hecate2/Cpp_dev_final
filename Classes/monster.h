#include "cocos2d.h"
#include "AppMacros.h"
#include <string>

int getdirection(cocos2d::Vec2 diff);	//根据相对位置得到移动方向

class Monster :public cocos2d::Sprite {
public:
	virtual void move(int dir)=0;
	virtual void attack(cocos2d::Vec2 diff) =0;	//攻击函数
private:
	int hp = 100;
};

//Boss
class BigMonster :public Monster{
public:
	static BigMonster* BigMonster::create(const std::string& filename);
	void move(int dir);
	void attack(cocos2d::Vec2 diff);

};


//小怪
