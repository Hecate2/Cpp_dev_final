#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"

class Start : public cocos2d::Scene
{
public:
	virtual bool init() override;

	static cocos2d::Scene* scene();

	// a selector callback
	void menuCloseCallback(Ref* sender);

	// implement the "static create()" method manually
	CREATE_FUNC(Start);
private:

};

#endif // __HELLOWORLD_SCENE_H__
