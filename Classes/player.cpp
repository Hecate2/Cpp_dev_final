#include "cocos2d.h"
#include "player.h"


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
