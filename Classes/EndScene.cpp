#include "EndScene.h"
#include "MainScene.h"
#include "AppMacros.h"

USING_NS_CC;


Scene* End::scene()
{
	return End::create();
}

// on "init" you need to initialize your instance
bool End::init()
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

	auto bg = Sprite::create("end.png");
	if (bg)
	{
		bg->setPosition(Vec2(origin.x + CENTER_X, origin.y + CENTER_Y));

		bg->setScale(1.6f);
		this->addChild(bg);
	}

	auto beginbutton = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(End::menuCloseCallback, this));


	float x = origin.x + winSize.width / 2 - beginbutton->getContentSize().width / 2 + 5;
	float y = origin.y + winSize.height / 2 + beginbutton->getContentSize().height / 2 - 40;
	beginbutton->setPosition(Vec2(x, y));

	auto menu = Menu::create(beginbutton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}


void End::menuCloseCallback(Ref* sender)
{
	auto soundEffectID = AudioEngine::play2d("sound/enter_game.mp3", false);
	Director::getInstance()->replaceScene(TransitionSlideInT::create(2.0f, MainScene::scene()));
}

