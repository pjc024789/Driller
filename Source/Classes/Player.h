#include "cocos2d.h"

#include "EffectSprite/EffectSprite.h"

class Player : public EffectSprite {
public:
	static Player *create();

	void idle();
	void drillDown(bool recursive=false);

	int getCol();
	int getRow();
	int getComboCount();

private:
	bool initialize();
	void moveCamera();

private:
	bool isMovable = true;
	int depth = 0;
	int combo = 0;

	cocos2d::Sprite *prevCombo;
};