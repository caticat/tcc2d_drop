#include "option.h"
#include "menu.h"

USING_NS_CC;

bool POption::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto soundLb = MenuItemFont::create("Sound");
	soundLb->setEnabled(false);
	auto soundTog = MenuItemToggle::createWithCallback(CC_CALLBACK_1(POption::onSound,this),MenuItemFont::create("On"),MenuItemFont::create("Off"),NULL);
	auto backBtn = MenuItemFont::create("Back", CC_CALLBACK_1(POption::onBack, this));
	auto menu = Menu::create(soundLb, soundTog, backBtn, NULL);
	menu->alignItemsInColumns(2, 1, NULL);
	this->addChild(menu);

	return true;
}

void POption::onSound(cocos2d::Ref* pSender)
{
	auto soundTog = (MenuItemToggle*)pSender;
	//log("wahaha,%d", soundTog->getSelectedIndex()); // 索引从0开始
}

void POption::onBack(cocos2d::Ref* pSender)
{
	auto menu = (PMenu*)this->getParent();
	menu->onBack();
}
