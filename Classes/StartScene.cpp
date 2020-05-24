#include "StartScene.h"
#include "MainScene.h"
#include "AppMacros.h"

USING_NS_CC;


Scene* Start::scene()
{
	return Start::create();
}

// on "init" you need to initialize your instance
bool Start::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	

	auto winSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto background = DrawNode::create();
	background->drawSolidRect(origin, winSize, cocos2d::Color4F(0.6, 0.6, 0.6, 1.0));
	this->addChild(background);

	auto bg = Sprite::create("startbg.png");
	if (bg)
	{
		bg->setPosition(Vec2(origin.x+236,origin.y+150));

		bg->setScale(1.5f );
		this->addChild(bg);
	}

	auto beginbutton = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(Start::menuCloseCallback, this));

	
	float x = origin.x + winSize.width/2 - beginbutton->getContentSize().width / 2+5;
	float y = origin.y + winSize.height/2+beginbutton->getContentSize().height / 2-40;
	beginbutton->setPosition(Vec2(x, y));

	auto menu = Menu::create(beginbutton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	
	return true;
}


void Start::menuCloseCallback(Ref* sender)
{
	Director::getInstance()->replaceScene(TransitionSlideInT::create(2.0f, MainScene::scene()));
}

