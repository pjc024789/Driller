#include "cocos2d.h"

class IntroLayer : public cocos2d::Sprite {
public:
	static IntroLayer *create();

	void dismiss();

private:
	bool initialize();

private:
};