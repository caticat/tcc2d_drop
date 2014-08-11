#pragma once

#include "cocos2d.h"

class PMenu : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
	void onQuit(cocos2d::Ref* pSender);
	void onNewGame(cocos2d::Ref* pSender);
	void onOption(cocos2d::Ref* pSender);
	void onBack();
    
    // implement the "static create()" method manually
    CREATE_FUNC(PMenu);

private:
	enum ZOrder
	{
		Z_Menu,
		Z_Option,
	};
};

