#include "cocos2d.h"
#include "Pacman.h"
using std::vector;
using std::string;

class MapPacman {
public:
    cocos2d::TMXTiledMap *_tileMap;
    Pacman *pacman;
    cocos2d::TMXLayer *_walls;
    cocos2d::TMXLayer *_food;
    int lvl = 0;
};

class MapBuilder : public cocos2d::Layer {
public:
    void addMap() {
        string str_map = "map/level";
        std::stringstream oss_convert;
        oss_convert << sceneMap.lvl;
        str_map += oss_convert.str();
        str_map += ".tmx";
        sceneMap._tileMap = cocos2d::TMXTiledMap::create(str_map);
    }
    void addFood() {
        sceneMap._food = sceneMap._tileMap->layerNamed("Food");
    }
    void addWalls() {
        sceneMap._walls = sceneMap._tileMap->layerNamed("Walls");
    }
    void addPacman() {
        sceneMap.pacman = new Pacman();
        sceneMap.pacman->createPacman();
    }
    void setLvl(int level) {
        sceneMap.lvl = level;
    }
    MapPacman getMap() {
        return sceneMap;
    }
protected:
    MapPacman sceneMap;
};

class DirectorMap {
public:
    MapPacman createMap(MapBuilder &builder) {
        builder.addMap();
        builder.addPacman();
        builder.addFood();
        builder.addWalls();
        return (builder.getMap());
    }
};
