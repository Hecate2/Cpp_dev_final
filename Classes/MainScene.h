/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "player.h"
#include "monster.h"
#include <vector>

class MainScene : public cocos2d::Scene
{
public:
	enum Direction { UP, DOWN, LEFT, RIGHT };
	//std::vector<Sprite> monster_group;
    virtual bool init() override;
	bool onContactBegin_bullet_barrel(cocos2d::PhysicsContact& contact);
    static cocos2d::Scene* scene();
	bool onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
<<<<<<< Updated upstream
    void menuCloseCallback(Ref* sender);
	void scheduleBlood(float delta);
	void addhp(float delta);
	void addBarrel();
	void addBox();
=======
	bool onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	//Direction Judgedirection(const Vec2& mon_pos);
    void menuCloseCallback(Ref* sender);
	void scheduleBlood(float delta);
	void addhp(float delta);
	void always_move(float delta);
	void addBarrel(const cocos2d::Vec2& s);
	void addBox();
	void change_weapon_animation(const std::string& weapon_name, bool out_of_bullet = false);
	void player_attack();
	//void mov_monsters(float delta);
>>>>>>> Stashed changes
	bool onContactBegin_player_box(cocos2d::PhysicsContact& contact);
    // implement the "static create()" method manually

	void monster_move(float dt);    //僵尸移动的调度器函数
	void monster_attack(float dt);  //僵尸攻击的调度器函数
	void addBigMonster(int birth_point);        //添加僵尸，参数为出生地
    CREATE_FUNC(MainScene);
private:
	player* _player;
<<<<<<< Updated upstream
=======
	int score=0;
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
	std::vector<BigMonster*> _BigMonster;
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
    std::vector<SmallMonster*> _SmallMonster;
	cocos2d::Label* current_score_label;
>>>>>>> Stashed changes
};

#endif // __HELLOWORLD_SCENE_H__
