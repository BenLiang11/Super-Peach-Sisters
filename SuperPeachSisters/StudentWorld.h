#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <vector>
#include <string>

class Actor;
class Peach;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool isBlocking(int x, int y);
	Peach* getPeach();
	Actor* overlapping(int x, int y);
	void addGoodie(int x, int y);
private:
	Peach* m_peach;
	std::vector <Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
