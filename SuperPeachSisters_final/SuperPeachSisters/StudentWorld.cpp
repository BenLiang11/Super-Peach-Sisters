#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::~StudentWorld() {
    this->cleanUp();
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_peach = new Peach(this, 0, 0);
}

int StudentWorld::init()
{   
    ostringstream l;
    l.setf(ios::fixed);
    l.precision(2);
    l << "level0" << getLevel() << ".txt";
    string level = l.str();

    //------------Load level file-------------
    Level lev(assetPath());
    string level_file = level;
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        return  GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_fail_bad_format)
        return  GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_success){
        Level::GridEntry ge;
        for (int i = 0; i < 32; i++) {
            for (int k = 0; k < 32; k++) {
                ge = lev.getContentsOf(i, k);
                switch (ge)
                {
                case Level::empty:
                    break;
                case Level::peach:
                    delete m_peach;
                    m_peach = new Peach(this, i * SPRITE_WIDTH, k * SPRITE_HEIGHT);
                    break;
                case Level::block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, false));
                    break;
                case Level::pipe:
                    m_actors.push_back(new Pipe(this, i * SPRITE_WIDTH, k * SPRITE_HEIGHT));
                    break;
                case Level::koopa:
                    m_actors.push_back(new Koopa(this, i * SPRITE_WIDTH, k * SPRITE_HEIGHT));
                    break;
                case Level::goomba:
                    m_actors.push_back(new Goomba(this, i * SPRITE_WIDTH, k * SPRITE_HEIGHT));
                    break;
                case Level::piranha:
                    m_actors.push_back(new Piranha(this, i * SPRITE_WIDTH, k * SPRITE_HEIGHT));
                    break;
                case Level::flag:
                    m_actors.push_back(new Flag(this, i * SPRITE_WIDTH, k * SPRITE_HEIGHT));
                    break;
                case Level::mario:
                    m_actors.push_back(new Mario(this, i * SPRITE_WIDTH, k * SPRITE_HEIGHT));
                    break;
                case Level::flower_goodie_block:
                case Level::mushroom_goodie_block:
                case Level::star_goodie_block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, true));
                    break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
    m_peach->doSomething();
    vector<Actor*>::iterator it;              //Allow all Actors to doSomething()
    it = m_actors.begin();
    int status = 0;
    while (it != m_actors.end()) {
        if ((*it)->isAlive()) {
            (*it)->doSomething();
            if ((*it)->levelComplete() != 0)
                status = (*it)->levelComplete();
            it++;
        }
        else {
            delete* it;
            it = m_actors.erase(it);
        }
    }
    if (m_peach->getHp() == 0) {               //Check if Peach is dead
        playSound(SOUND_PLAYER_DIE);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if (status == 1) {                         //Check if Peach finished level
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    if (status == 2) {                         //Check if Peach finished the game
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }

    // Update the game status line
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.precision(2);
    oss << "Lives: " << getLives() << "  Level: " << getLevel() << "  Points: " << getScore();
    if (m_peach->getInvis())
        oss << " StarPower!";
    else if (m_peach->getSP())
        oss << " ShootPower!";
    else if (m_peach->getJp())
        oss << " JumpPower!";
    string output = oss.str();
    setGameStatText(output);

    return GWSTATUS_CONTINUE_GAME; //Continue game
}

void StudentWorld::cleanUp()
{
    for (int i = 0; i < m_actors.size(); i++){
        delete (m_actors[i]);
    }
    m_actors.clear();

}

bool StudentWorld::isBlocking(int x, int y) { 
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        double tx = (*it)->getX();
        double ty = (*it)->getY();
        if (tx <= x && x <= (tx + SPRITE_WIDTH - 1) &&
            ty <= y && y <= (ty + SPRITE_HEIGHT - 1) &&
            (*it)->obstruct()) {
            return true;
        }
        it++;
    }
    return false;
}

Actor* StudentWorld::overlapping(int x, int y) {
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        double tx = (*it)->getX();
        double ty = (*it)->getY();
        if (tx <= x && x <= (tx + SPRITE_WIDTH - 1) &&
            ty <= y && y <= (ty + SPRITE_HEIGHT - 1) ) {
            return (*it);
        }
        it++;
    }
    return nullptr;
}

Peach* StudentWorld::getPeach() {
    return m_peach;
}

void StudentWorld::addGoodie(int x, int y) {
    Level lev(assetPath());
    ostringstream l;
    l.setf(ios::fixed);
    l.precision(2);
    l << "level0" << getLevel() << ".txt";
    string level = l.str();
    string level_file = level;
    lev.loadLevel(level_file);
    Level::GridEntry ge;
    ge = lev.getContentsOf(x / SPRITE_WIDTH, y / SPRITE_HEIGHT);
    switch (ge) {
        case Level::flower_goodie_block:
            m_actors.push_back(new Flower(this, x , y + 8));
            return;
        case Level::mushroom_goodie_block:
            m_actors.push_back(new Mushroom(this, x , y + 8));
            return;
        case Level::star_goodie_block:
            m_actors.push_back(new Star(this, x , y + 8));
            return;
    }
}