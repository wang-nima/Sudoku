#pragma once

#include "cocos2d.h"
#include <string>
using namespace cocos2d;
using namespace std;

class numberSprite : public Sprite
{
public:
	numberSprite();
	~numberSprite();
	static numberSprite* create(const string &filename);
	// make public intentionally, no need to write code for getter and setter
	bool inBoard;
	int num;
};

