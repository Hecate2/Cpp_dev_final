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
#include "AudioEngine.h"  //音效引擎
#include <vector>

void attack_sound(std::string gun_name);

class MainScene : public cocos2d::Scene
{
public:
	enum Direction { UP, DOWN, LEFT, RIGHT };
	//std::vector<Sprite> monster_group;
    virtual bool init() override;
    static cocos2d::Scene* scene();
	bool onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	bool onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	//Direction Judgedirection(const Vec2& mon_pos);
    void menuCloseCallback(Ref* sender);
	void scheduleBlood(float delta);
	void addhp(float delta);
	void always_move(float delta);
	void always_attack_normal(float delta);
	void always_attack_uzi(float delta);
	void addBarrel(const cocos2d::Vec2& s);
	void addBox(const cocos2d::Vec2& pos);
	void change_weapon_animation(const std::string& weapon_name, bool out_of_bullet = false);
	void judge_and_attack();
	//void mov_monsters(float delta);
	bool onContactBegin_player_box(cocos2d::PhysicsContact& contact);
    bool onContactBegin_bullet_barrel(cocos2d::PhysicsContact& contact);
    bool onContactBegin_player_fireball(cocos2d::PhysicsContact& contact);
    bool onContactBegin_bullet_monster(cocos2d::PhysicsContact& contact);
	void level_up();
    // implement the "static create()" method manually
	void monster_move(float dt);    //用于僵尸移动调度器
	void monster_attack(float dt);  //用于僵尸攻击调度器
    void monster_death(float dt);  //用于僵尸死亡调度器
    void border(float dt);          //边界控制
	void addBigMonster(int birth_point);        //增加一个boss
    void addSmallMonster(int birth_point);        //增加一个boss
    void set_z_oder(float dt);                          //对所有项目进行渲染顺序的调整，解决覆盖问题
    int get_z_odre(cocos2d::Node* spr);

    CREATE_FUNC(MainScene);
private:
	player* _player;
	int score=0;
	int current_level = 0;
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
	std::vector<BigMonster*> _BigMonster;
    std::vector<SmallMonster*> _SmallMonster;
	cocos2d::Label* current_score_label;
	cocos2d::Label* current_level_label;
};

#endif // __HELLOWORLD_SCENE_H__
