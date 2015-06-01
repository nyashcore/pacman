#include "cocos2d.h"

class Pacman : public cocos2d::Sprite{
public:
    cocos2d::Sprite *pacman;
    void createPacman();
    cocos2d::Sprite *getPacman();
};
