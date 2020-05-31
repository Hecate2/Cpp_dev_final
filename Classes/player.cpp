#include "cocos2d.h"
#include "player.h"
#define BLOODBAR_TAG 1

player* player::create(const std::string& filename)
{
	player* pSprite = new (std::nothrow) player();
	if (pSprite && pSprite->initWithFile(filename))
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return nullptr;
}
bool player::init() {
	//initialize weapon store
	weapon_store.resize(4);
	weapon_store[0] = weapon(100, 5, 5, 200,100, "pistol");
	weapon_store[1] = weapon(100, 10, 5, 200, 100,"UZI");
	weapon_store[2] = weapon(20, 5, 20, 150, 20, "shotgun");
	weapon_store[3] = weapon(10, 0, 0, 0, 10, "barrel");
	//initialze current weapon
	current_weapon = &(weapon_store[0]);
	current_weapon_label = cocos2d::Label::createWithSystemFont("pistol", "Arial", 15);
	current_weapon_label->setPosition(cocos2d::Vec2(80, 180));
	this->addChild(current_weapon_label);
	//initialize hp bar
	auto sprite = Sprite::create("bar.png");   
	sprite->setPosition(cocos2d::Vec2(this->getPosition().x-20, 150)); 
	this->addChild(sprite);            
	auto sprBlood = Sprite::create("blood.png");  //create blood bar
	cocos2d::ProgressTimer* progress = cocos2d::ProgressTimer::create(sprBlood); //create progress
	progress->setType(cocos2d::ProgressTimer::Type::BAR);        //settype bar
	progress->setPosition(cocos2d::Vec2(this->getPosition().x-20, 150));
	//decrease from right to left
	progress->setMidpoint(cocos2d::Point(0, 0.5));    
	progress->setBarChangeRate(cocos2d::Point(1, 0));
	progress->setTag(BLOODBAR_TAG);       //make a tag
	this->addChild(progress);
	return true;
}