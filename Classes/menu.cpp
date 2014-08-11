#include "menu.h"
#include "option.h"
#include "drop.h"

USING_NS_CC;

Scene* PMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = PMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PMenu::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	auto newGameBtn = MenuItemFont::create("New Game", CC_CALLBACK_1(PMenu::onNewGame, this));
	auto optionBtn = MenuItemFont::create("Option", CC_CALLBACK_1(PMenu::onOption, this));
	auto quitBtn = MenuItemFont::create("Quit", CC_CALLBACK_1(PMenu::onQuit, this));

	auto menu = Menu::create(newGameBtn, optionBtn, quitBtn, NULL);
	menu->alignItemsVertically();
	this->addChild(menu, Z_Menu, Z_Menu);

    return true;
}

void PMenu::onQuit(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void PMenu::onNewGame( cocos2d::Ref* pSender )
{
	auto scene = Scene::create();
	auto drop = PDrop::create();
	scene->addChild(drop);
	Director::getInstance()->replaceScene(scene);
}

void PMenu::onOption(cocos2d::Ref* pSender)
{
	auto option = POption::create();
	this->addChild(option, Z_Option, Z_Option);
	auto menu = (Menu*)this->getChildByTag(Z_Menu);
	menu->setVisible(false);
}

void PMenu::onBack()
{
	auto menu = (Menu*)this->getChildByTag(Z_Menu);
	menu->setVisible(true);
	this->removeChildByTag(Z_Option); // 会调用析构函数
}
