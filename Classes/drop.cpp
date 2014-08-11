#include "drop.h"
#include "menu.h"

USING_NS_CC;

PDrop::PDrop() : _role(NULL), _walk(NULL), _isFalling(false), _score(0)
{

}

PDrop::~PDrop()
{
	_walk->release();
	_res->release();
}

bool PDrop::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visableSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	auto backBtn = MenuItemFont::create("Back", CC_CALLBACK_1(PDrop::onBack, this));
	backBtn->setPosition(Point(origin.x + visableSize.width / 2, origin.y + visableSize.height-backBtn->getContentSize().height));
	auto menu = Menu::create(backBtn, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, Z_Menu);

	_res = SpriteBatchNode::create("role.png");
	_res->retain();

	//auto floor = createFloor(origin.x + visableSize.width / 2, origin.y + visableSize.height / 2 - _role->getContentSize().height / 2);
	auto floor = createFloor(origin.x + visableSize.width / 2, 0);
	this->addChild(floor, Z_Floor);

	_role = Sprite::createWithTexture(_res->getTexture(), Rect(0, 0, 21, 46));
	_role->setPosition(origin.x + visableSize.width / 2, origin.y + floor->getContentSize().height + _role->getContentSize().height / 2);
	this->addChild(_role, Z_Role);

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.2f);
	for (int i = 0; i < 3; ++i)
		animation->addSpriteFrameWithTexture(_res->getTexture(), Rect(21*i, 0, 21, 46));
	_walk = Animate::create(animation);
	_walk->retain();

	auto score = Label::create("Score:0","Marker Felt",25);
	score->setPosition(Point(origin.x+visableSize.width-score->getContentSize().width,origin.y+visableSize.height-score->getContentSize().height));
	this->addChild(score, Z_Score, Z_Score);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch* touch, Event* unused_event) { return true; };
	listener->onTouchCancelled = [](Touch *touch, Event *unused_event){};
	listener->onTouchMoved = [](Touch *touch, Event *unused_event){};
	listener->onTouchEnded = CC_CALLBACK_2(PDrop::onLayerTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->schedule(schedule_selector(PDrop::timeout), 1.0f);
	this->scheduleUpdate(); // 设置每帧调用

	return true;
}

void PDrop::update(float tm)
{
	Layer::update(tm);

	bool isFalling = true;
	auto roleRect = Rect(_role->getPositionX() - _role->getContentSize().width / 2, _role->getPositionY() - _role->getContentSize().height / 2, _role->getContentSize().width, _role->getContentSize().height);
	for (auto floor = _floors.begin(); floor != _floors.end(); ++ floor)
	{
		auto floorRect = Rect((*floor)->getPositionX() - (*floor)->getContentSize().width / 2,
			(*floor)->getPositionY() - (*floor)->getContentSize().height / 2,
			(*floor)->getContentSize().width,
			(*floor)->getContentSize().height);
		if (roleRect.intersectsRect(floorRect))
		{
			isFalling = false;
			break;
		}
	}
	if (_isFalling == isFalling)
		return;
	_role->stopAllActions();
	if (isFalling)
	{
		doFall();
	}
	else
	{
		doRaise();
	}
	_isFalling = isFalling;
}

void PDrop::timeout(float tm)
{
	_score += tm;
	auto score = (Label*)this->getChildByTag(Z_Score);
	char score_str[20];
	sprintf(score_str, "Score:%d", _score);
	score->setString(score_str);

	static int count = 0;
	if ((count++%3) == 0)
		this->checkFloor();
}

void PDrop::onBack(cocos2d::Ref* pSender)
{
	auto scene = Scene::create();
	scene->addChild(PMenu::create());
	Director::getInstance()->replaceScene(scene);
}

void PDrop::onLayerTouchEnded(Touch *touch, Event *unused_event)
{
	int mvLen = touch->getLocation().x - _role->getPositionX();
	_role->runAction(RepeatForever::create(_walk));
	auto onMoveEndFunc = CallFunc::create(CC_CALLBACK_0(PDrop::onMoveEnd,this));
	_role->runAction(Sequence::create(MoveBy::create(1.0f, Point(mvLen, 0)), onMoveEndFunc, NULL));
}

void PDrop::onMoveEnd()
{
	_role->stopAllActions();
	doFall();
}

void PDrop::onFloorActionEnd(cocos2d::Ref* pSender)
{
	auto floor = (Sprite*)pSender;
	_floors.eraseObject(floor);
	this->removeChild(floor);
}

void PDrop::onFallEnd(cocos2d::Ref* pSender)
{
	Size viewSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	this->unschedule(schedule_selector(PDrop::timeout));
	this->unscheduleUpdate();
	char gameOverStr[36];
	sprintf(gameOverStr, "Game Over!\nScore:%d", _score);
	auto gameOver = Label::create(gameOverStr, "Marker Felt", 25);
	gameOver->setPosition(Point(origin.x + viewSize.width / 2, origin.y + viewSize.height / 2));
	this->addChild(gameOver, Z_GameOver);
}

void PDrop::onRaiseEnd(cocos2d::Ref* pSender)
{
	auto roleRect = Rect(_role->getPositionX(), _role->getPositionY(), _role->getContentSize().width, _role->getContentSize().height);
	for (auto floor = _floors.begin(); floor != _floors.end(); ++floor)
	{
		auto floorRect = Rect((*floor)->getPositionX() - (*floor)->getContentSize().width / 2,
			(*floor)->getPositionY() - (*floor)->getContentSize().height / 2,
			(*floor)->getContentSize().width,
			(*floor)->getContentSize().height);
		if (roleRect.intersectsRect(floorRect))
		{
			floor = _floors.erase(floor);
		}
	}
}

void PDrop::checkFloor()
{
	if (_floors.size() > _maxFloor)
		return;
	
	Size viewSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	int x = origin.x+CCRANDOM_0_1()*viewSize.width;
	auto floor = createFloor(x, 0);
	this->addChild(floor, Z_Floor);
}

cocos2d::Sprite* PDrop::createFloor(int x, int y)
{
	Size viewSize = Director::getInstance()->getVisibleSize();
	auto floor = Sprite::createWithTexture(_res->getTexture(), Rect(74, 6, 68, 5));
	//floor->setScaleX(((CCRANDOM_0_1() * 100 - 50) + 100) / 100); // 这个缩放没有用……
	floor->setPosition(Point(x, y));
	auto floorAction = MoveTo::create((viewSize.height / _speed), Point(x, viewSize.height));
	auto floorActionEnd = CallFuncN::create(CC_CALLBACK_1(PDrop::onFloorActionEnd, this));
	floor->runAction(Sequence::create(floorAction, floorActionEnd, NULL));
	_floors.pushBack(floor);
	return floor;
}

void PDrop::doFall()
{
	Size viewSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	auto roleAction = MoveTo::create((viewSize.height / (_speed * 2)), Point(_role->getPositionX() + _role->getContentSize().width / 2, origin.y));
	auto roleActionEnd = CallFuncN::create(CC_CALLBACK_1(PDrop::onFallEnd, this));
	_role->runAction(Sequence::create(roleAction, roleActionEnd, NULL));
}

void PDrop::doRaise()
{
	Size viewSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	auto roleAction = MoveTo::create(((viewSize.height - (origin.y + _role->getPositionY())) / _speed), Point(_role->getPositionX() + _role->getContentSize().width / 2, origin.y + viewSize.height));
	auto roleActionEnd = CallFuncN::create(CC_CALLBACK_1(PDrop::onRaiseEnd, this));
	_role->runAction(Sequence::create(roleAction, roleActionEnd, NULL));
}
