#pragma once

#include "cocos2d.h"

class POption : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(POption);

private:
	void onSound(cocos2d::Ref* pSender);
	void onBack(cocos2d::Ref* pSender);
};
