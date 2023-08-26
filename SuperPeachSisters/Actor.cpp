#include "Actor.h"
#include "StudentWorld.h"
#include <string>
#include <cstdlib>
using namespace std;

//--------------------------Actor--------------------------
Actor::Actor(StudentWorld* w, int id, int x, int y, int dir, int depth, double size)
	: GraphObject(id, x, y, dir, depth, size) {
	m_world = w;
	m_alive = false;
	m_obstruct = true;
	m_damage = false;
	m_levelCompleted = 0;
}

Actor::~Actor() {
	return;
}

void Actor::bonk() {
	return;
}

bool Actor::overlap() {
	double px = this->getWorld()->getPeach()->getX();
	double py = this->getWorld()->getPeach()->getY();
	if (this->getX() == px && this->getY() == py) {
		return true;
	}
	return false;
}


StudentWorld* Actor::getWorld() {
	return m_world;
}

bool Actor::isAlive() {
	return m_alive;
}

void Actor::setAlive(bool a) {
	m_alive = a;
}

bool Actor::obstruct() {
	return m_obstruct;
}

void Actor::setObstruct(bool o) {
	m_obstruct = o;
}
bool Actor::damage() {
	return m_damage;
}
void Actor::setDamage(bool d) {
	m_damage = d;
}

int Actor::levelComplete() {
	return m_levelCompleted;
}
void Actor::setlevelComplete(int l) {
	m_levelCompleted = l;
}

//--------------------------Peach--------------------------
Peach::Peach(StudentWorld* w, int x, int y) 
	: Actor(w, IID_PEACH, x, y, 0, 0, 1.0) {
	this->setAlive(true);
	this->setObstruct(false);
	this->setDamage(true);
	m_hp = 1;
	m_jump = 0;
	m_tempInvis = 0;
	m_invis = 0;
	m_shootpower = false;
	m_jp = false;
}

Peach::~Peach() {
	return;
}

int Peach::getHp() {
	return m_hp;
}
void Peach::setHp(int a) {
	m_hp = a;
}
void Peach::setTempInvis(int n) {
	m_tempInvis = n;
}
int Peach::getTempInvis() {
	return m_tempInvis;
}
void Peach::setInvis(int n) {
	m_invis = n;
}
int Peach::getInvis() {
	return m_invis;
}
void Peach::setSP(bool n) {
	m_shootpower = n;
}
bool Peach::getSP() {
	return m_shootpower;
}
bool Peach::getJp() {
	return m_jp;
}
void Peach::setJp(bool n) {
	m_jp = n;
}
void Peach::bonk() {
	if (m_invis || m_tempInvis) return;
	else {
		m_hp--;
		m_tempInvis = 10;
		m_shootpower = false;
		m_jp = false;
		if (m_hp == 1) getWorld()->playSound(SOUND_PLAYER_HURT);
		else if (m_hp == 0) {
			setAlive(false);
			return;
		}
	}
	
}
void Peach::doSomething() {
	if (!isAlive()) return;
	if (m_invis) {          //Decrement Star Power (Invincibility)
		m_invis--;
	}
	if (m_tempInvis) {      //Decrement Temp invincibility
		m_tempInvis--;
	}
	int ch;
	int x = getX();
	int y = getY();

	if (getWorld()->overlapping(x,y) != nullptr) {  //Bonk other overlapping objects
		getWorld()->overlapping(x, y)->bonk();
		return;
	}
															
	if (m_jump > 0) {                               //Jumping
		if (getWorld()->isBlocking(x, y + SPRITE_HEIGHT+3) && getWorld()->overlapping(x, y + SPRITE_HEIGHT+3) != nullptr) { //Check if Peach can jump
			getWorld()->overlapping(x, y + SPRITE_HEIGHT+3)->bonk();
			m_jump = 0;
		}
		else if (getWorld()->isBlocking(x + SPRITE_WIDTH - 1, y + SPRITE_HEIGHT)                          //Check if Peach can "fit" in space above
			&& getWorld()->overlapping(x + SPRITE_WIDTH - 1, y + SPRITE_HEIGHT) != nullptr) {
			getWorld()->overlapping(x + SPRITE_WIDTH - 1, y + SPRITE_HEIGHT)->bonk();
			m_jump = 0;
		}
		else {
			this->moveTo(x , y + 4); //move up
			m_jump--;
		}
	}
	else {                           //Checking for falling
		bool block = false;
		for (int i = 0; i <= 3; i++) {
			if (getWorld()->isBlocking(x, y - i) || getWorld()->isBlocking(x+4, y - i)) {                 //Check if Peach is "fit" for the fall
				block = true;
				break;
			}
		}
		if (!block) {
			this->moveTo(x, y - 4);
		}
	}
	if (getWorld()->getKey(ch))                                            //User Input Updates
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:                                               //Left
			this->setDirection(180);
			if (getWorld()->isBlocking(x - 4, y)) {
				getWorld()->overlapping(x - 4, y)->bonk();
				return;
			}
			else this->moveTo(x - 4, y);
			break;
		case KEY_PRESS_RIGHT:                                               //Right
			this->setDirection(0);
			if (getWorld()->isBlocking(x + SPRITE_WIDTH + 3, y)) {
				getWorld()->overlapping(x + SPRITE_WIDTH + 3, y)->bonk();
				return;
			} 
			else this->moveTo(x + 4, y);
			break;
		case KEY_PRESS_UP:                                                   //Up
			if (getWorld()->isBlocking(x, y - 1)) {
				if (getJp()) m_jump = 12;
				else m_jump = 8;
				this->getWorld()->playSound(SOUND_PLAYER_JUMP);
			}
		}
	}
}

//--------------------------Block--------------------------

Block::Block(StudentWorld* w, int x, int y, bool g) : Actor(w, IID_BLOCK, x, y, 0, 2, 1) {
	this->setAlive(true);
	m_goodie = g;
}

Block::~Block() {
	return;
}

void Block::bonk() {
	if (m_goodie == false)
		this->getWorld()->playSound(SOUND_PLAYER_BONK);
	else {
		this->getWorld()->playSound(SOUND_POWERUP_APPEARS);
		this->getWorld()->addGoodie(getX(), getY());
		m_goodie = false;
	}

}

void Block::doSomething() {
	return;
}


//--------------------------Pipe--------------------------
Pipe::Pipe(StudentWorld* w, int x, int y) : Actor(w, IID_PIPE, x, y, 0, 2, 1) {
	this->setAlive(true);
}

Pipe::~Pipe() {
	return;
}

void Pipe::doSomething() {
	return;
}

//--------------------------Flag--------------------------
Flag::Flag(StudentWorld* w, int x, int y) : Actor(w, IID_FLAG, x, y, 0, 1, 1) {
	this->setAlive(true);
	this->setObstruct(false);
}

void Flag::doSomething() {
	if (!isAlive()) return;
	if (overlap()) {
		this->getWorld()->increaseScore(1000);
		this->setAlive(false);
		setlevelComplete(1);
		return;
	}
}

//--------------------------Mario--------------------------
Mario::Mario(StudentWorld* w, int x, int y) : Actor(w, IID_MARIO, x, y, 0, 1, 1) {
	this->setAlive(true);
	this->setObstruct(false);
}

void Mario::doSomething() {
	if (!isAlive()) return;
	if (overlap()) {
		this->getWorld()->increaseScore(1000);
		this->setAlive(false);
		setlevelComplete(2);
	}
}

//----------------------------------------------------------
//--------------------------Goodie--------------------------
//----------------------------------------------------------

Goodie::Goodie(StudentWorld* w, int id, int x, int y, int dir, int depth, double size)
	: Actor(w, id, x, y, dir, depth, size) {
	this->setAlive(true);
	this->setObstruct(false);
}

void Goodie::bonk() {
	return;
}

bool Goodie::overlapG() {

	if (overlap()) {
		this->setAlive(false);
		this->getWorld()->playSound(SOUND_PLAYER_POWERUP);
		return true;
	}
}

void Goodie::moveG() {
	double x = getX();
	double y = getY();
	if (!getWorld()->isBlocking(x, y - 2)) {
		this->moveTo(x, y - 2);
		return;
	}
	switch (this->getDirection()) {
	case 0:
		if (getWorld()->isBlocking(x + SPRITE_WIDTH + 1, y)) {
			this->setDirection(180);
			break;
		}
		this->moveTo(x + 2, y);
		break;
	case 180:
		if (getWorld()->isBlocking(x - 2, y)) {
			this->setDirection(0);
			break;
		}
		this->moveTo(x - 2, y);
		break;
	}
}
//--------------------------Flower--------------------------
Flower::Flower(StudentWorld* w, int x, int y) : Goodie(w, IID_FLOWER, x, y, 0, 1, 1) {

}

void Flower::doSomething() {
	if (overlapG()) {
		getWorld()->increaseScore(50);
		getWorld()->getPeach()->setSP(true);
		getWorld()->getPeach()->setHp(2);
	}
	moveG();
	return;
}

//--------------------------Mushroom--------------------------
Mushroom::Mushroom(StudentWorld* w, int x, int y) : Goodie(w, IID_MUSHROOM, x, y, 0, 1, 1) {

}

void Mushroom::doSomething() {
	if (overlapG()) {
		getWorld()->increaseScore(75);
		getWorld()->getPeach()->setJp(true);
		getWorld()->getPeach()->setHp(2);
	}
	moveG();
	return;
}

//--------------------------Star--------------------------
Star::Star(StudentWorld* w, int x, int y) : Goodie(w, IID_STAR, x, y, 0, 1, 1) {

}

void Star::doSomething() {
	if (overlapG()) {
		getWorld()->increaseScore(100);
		getWorld()->getPeach()->setInvis(150);
		return;
	}
	moveG();
	return;
}

//----------------------------------------------------------
//------------------------Projectile------------------------
//----------------------------------------------------------
Projectile::Projectile(StudentWorld* w, int id, int x, int y, int dir) : Actor(w, id, x, y, dir, 1, 1) {
	this->setAlive(true);
	this->setObstruct(false);
}
void Projectile::doSomething() {
	
}
void Projectile::bonk() {
	return;
}

void Projectile::moveP() { //Move Projectile left or right
	double x = getX();
	double y = getY();
	if (!getWorld()->isBlocking(x, y - 2)) {
		this->moveTo(x, y - 2);
	}
	switch (this->getDirection()) {
	case 0:
		if (getWorld()->isBlocking(x + SPRITE_WIDTH + 1, y)) {
			this->setAlive(false);
			break;
		}
		this->moveTo(x + SPRITE_WIDTH + 1, y);
		break;
	case 180:
		if (getWorld()->isBlocking(x - 2, y)) {
			this->setAlive(false);
			break;
		}
		this->moveTo(x - 2, y);
		break;
	}
}

PiranhaFireball::PiranhaFireball(StudentWorld* w, int x, int y, int dir) : Projectile(w, IID_PIRANHA_FIRE, x, y, dir) {

}

void PiranhaFireball::doSomething() {
	if (overlap()) {                                  //Check if fireball hits Peach
		getWorld()->getPeach()->bonk();
		setAlive(false);
		return;
	}
	moveP();                                          //Move
}

PeachFireball::PeachFireball(StudentWorld* w, int x, int y, int dir) : Projectile(w, IID_PEACH_FIRE, x, y, dir) {

}

void PeachFireball::doSomething() {
	if (getWorld()->overlapping(getX(),getY())->damage()){ //Check if fireball hits Enemy
		getWorld()->overlapping(getX(), getY())->setDamage(false);
		setAlive(false);
		return;
	}
	moveP();
}

Shell::Shell(StudentWorld* w, int x, int y, int dir) : Projectile(w, IID_SHELL, x, y, dir) {
	
}
void Shell::doSomething() {
	if (getWorld()->overlapping(getX(), getY())->damage()) { //Check if Shell hits Enemy
		getWorld()->overlapping(getX(), getY())->setDamage(false);
		setAlive(false);
		return;
	}
	moveP();
}

//----------------------------------------------------------
//--------------------------Enemy---------------------------
//----------------------------------------------------------

Enemy::Enemy(StudentWorld* w, int id, int x, int y)
	: Actor(w, id, x, y, 0, 0, 1) {
	this->setAlive(true);
	this->setObstruct(false);
	this->setDamage(true);
}

void Enemy::doSomething() {

}

void Enemy::bonk() {
	if (this->getWorld()->getPeach()->getInvis() && overlap()) {
		this->getWorld()->playSound(SOUND_PLAYER_KICK);
		getWorld()->increaseScore(100);
		this->setAlive(false);
	}
}

void Enemy::moveE() {
	double x = getX();
	double y = getY();
	bool can_move = false;
	switch (this->getDirection()) {
	case 0:
		if (getWorld()->isBlocking(x + SPRITE_WIDTH, y) || !getWorld()->isBlocking(x + SPRITE_WIDTH, y - 1)) {
			this->setDirection(180);
			break;
		}
		can_move = true;
		break;
	case 180:
		if (getWorld()->isBlocking(x - 1, y) || !getWorld()->isBlocking(x - 1, y - 1)) {
			this->setDirection(0);
			break;
		}
		can_move = true;
		break;
	}
	if (can_move) {
		switch (this->getDirection()) {
		case 0:
			this->moveTo(x + 1, y);
			return;
		case 180:
			this->moveTo(x - 1, y);
			return;
		}
	}
}

//--------------------------Goomba--------------------------
Goomba::Goomba(StudentWorld* w, int x, int y) 
	: Enemy(w, IID_GOOMBA, x, y) {
	this->setAlive(true);
}

void Goomba::doSomething() {
	if (!isAlive()) return;
	if (overlap()) {                          //Check for Peach overlap
		this->getWorld()->getPeach()->bonk();
		return;
	}
	moveE();
	if (!damage()) {						  //Check if Goomba has been damaged
		getWorld()->increaseScore(100);
		setAlive(false);
	}
}

//--------------------------Koopa--------------------------
Koopa::Koopa(StudentWorld* w, int x, int y) 
	: Enemy(w, IID_KOOPA, x, y) {

}
void Koopa::bonk() {
	if (this->getWorld()->getPeach()->getInvis() && overlap()) {
		this->getWorld()->playSound(SOUND_PLAYER_KICK);
		getWorld()->increaseScore(100);
		this->setAlive(false);
		//add shell
	}
}

void Koopa::doSomething() {
	if (!isAlive()) return;
	if (overlap()) { //Check for Peach overlap
		this->getWorld()->getPeach()->bonk();
		return;
	}
	moveE();
}


//--------------------------Piranha--------------------------
Piranha::Piranha(StudentWorld* w, int x, int y) 
	: Enemy(w, IID_PIRANHA, x, y) {

	m_delay = 0;
}

void Piranha::doSomething() {
	if (!isAlive()) return;
	this->increaseAnimationNumber();
	if (overlap()) {                                 //Check for Peach overlap
		this->getWorld()->getPeach()->bonk();
		return;
	}
	double px = this->getWorld()->getPeach()->getX();
	double py = this->getWorld()->getPeach()->getY();
	if ((getY()-(1.5 * SPRITE_HEIGHT)) <= py 
		&& py <= (getY() + (1.5 * SPRITE_HEIGHT))) { //Peach is within 1.5 height lengths of Piranha
		if (px < getX()) setDirection(180);
		else setDirection(0);
	}
	else return;
	if (m_delay) {                                   //Decrease fire delay if more than 0
		m_delay--;
		return;
	}
	else {
		//fire
		return;
	}
	
}

