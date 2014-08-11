#pragma once

#include "cocos2d.h"

class PDrop : public cocos2d::Layer
{
public:
	PDrop();
	~PDrop();
	virtual bool init();
	virtual void update(float tm);
	CREATE_FUNC(PDrop);

private:
	void timeout(float tm);
	void onBack(cocos2d::Ref* pSender);
	void onLayerTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onMoveEnd();
	void onFloorActionEnd(cocos2d::Ref* pSender);
	void onFallEnd(cocos2d::Ref* pSender);
	void onRaiseEnd(cocos2d::Ref* pSender);

	void checkFloor();
	cocos2d::Sprite* createFloor(int x, int y);
	void doFall();
	void doRaise();

private:
	enum ZOrder
	{
		Z_Role,
		Z_Menu,
		Z_Floor,
		Z_Score,
		Z_GameOver,
	};
	static const int _maxFloor = 3;
	static const int _speed = 50;

private:
	cocos2d::Sprite* _role;
	cocos2d::Animate* _walk;
	cocos2d::Vector<cocos2d::Sprite*> _floors;
	cocos2d::SpriteBatchNode* _res;
	bool _isFalling;
	int _score;
};

/*
	各种坐标临时记录
	21,46,

	74,6
	68,5
*/
