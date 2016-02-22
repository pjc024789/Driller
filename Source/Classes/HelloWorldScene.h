#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class LightEffect;
class Player;
class Block;

class HelloWorld : public cocos2d::Layer
{
	friend Player;
public:
    static cocos2d::Scene* createScene();
	static HelloWorld *sharedInstance();

    virtual bool init();

    CREATE_FUNC(HelloWorld);

	Block *getBlock(int x, int y);

private:
	void buildBlocks();
	void update(float dt);

private :
	static HelloWorld *instance;

	cocos2d::Layer *parallax;

	LightEffect *light;
	cocos2d::Vec2 lightPos;

	Player *player;

	Block *blocks[5][100];
};

#endif // __HELLOWORLD_SCENE_H__
