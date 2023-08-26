#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;
class Goodie;

class Actor : public GraphObject {
	public:
		Actor(StudentWorld* w, int id, int x, int y, int dir, int depth, double size);
		virtual ~Actor();
		virtual void doSomething() = 0;
		virtual void bonk();
		StudentWorld* getWorld();
		bool overlap();
		bool isAlive();
		void setAlive(bool a);
		bool obstruct();
		void setObstruct(bool o);
		bool damage();
		void setDamage(bool d);
		int levelComplete();
		void setlevelComplete(int l);

	private:
		bool m_alive;
		bool m_obstruct;
		bool m_damage;
		int m_levelCompleted;
		StudentWorld* m_world;
};

class Peach : public Actor {
	public:
		Peach(StudentWorld* w, int x, int y);
		virtual ~Peach();
		virtual void doSomething();
		virtual void bonk();
		int getHp();
		void setHp(int a);
		void setTempInvis(int n);
		int getTempInvis();
		void setInvis(int n);
		int getInvis();
		void setSP(bool n);
		bool getSP();
		bool getJp();
		void setJp(bool n);
	private:
		int m_hp;
		int m_jump;
		int m_tempInvis;
		int m_invis;
		bool m_shootpower;
		bool m_jp;
};

class Block : public Actor {
public:
	Block(StudentWorld* w, int x, int y, bool g); //Make this the only way to make block
	virtual ~Block();
	virtual void bonk();
	virtual void doSomething();
private:
	bool m_goodie;
};

class Pipe : public Actor {
public:
	Pipe(StudentWorld* w, int x, int y);
	virtual ~Pipe();
	virtual void doSomething();
};

class Flag : public Actor {
public:
	Flag(StudentWorld* w, int x, int y);
	virtual void doSomething();
};

class Mario : public Actor {
public:
	Mario(StudentWorld* w, int x, int y);
	virtual void doSomething();
};

//--------------------------Projectile--------------------------
class Projectile : public Actor {
	public:
		Projectile(StudentWorld* w, int id, int x, int y, int dir);
		virtual void doSomething();
		virtual void bonk();
		void moveP();
};

class PiranhaFireball : public Projectile {
	public:
		PiranhaFireball(StudentWorld* w, int x, int y, int dir);
		virtual void doSomething();
		
};

class PeachFireball : public Projectile {
	public:
		PeachFireball(StudentWorld* w, int x, int y, int dir);
		virtual void doSomething();
};

class Shell : public Projectile {
	public:
		Shell(StudentWorld* w, int x, int y, int dir);
		virtual void doSomething();
};





//--------------------------Enemy--------------------------
class Enemy : public Actor {
public:
	Enemy(StudentWorld* w, int id, int x, int y);
	virtual void doSomething();
	virtual void bonk();
	void moveE();
};

class Goomba : public Enemy {
	public:
		Goomba(StudentWorld* w, int x, int y);
		virtual void doSomething();
};

class Koopa : public Enemy {
	public:
		Koopa(StudentWorld* w, int x, int y);
		virtual void doSomething();
		virtual void bonk();
};

class Piranha : public Enemy {
	public:
		Piranha(StudentWorld* w, int x, int y);
		virtual void doSomething();
	private:
		int m_delay;
};

//--------------------------Goodie--------------------------
class Goodie : public Actor {
public:
	Goodie(StudentWorld* w, int id, int x, int y, int dir, int depth, double size);
	virtual void doSomething() = 0;
	virtual void bonk();
	bool overlapG();
	void moveG();
private:
	
};

class Flower : public Goodie {
public:
	Flower(StudentWorld* w, int x, int y);
	virtual void doSomething();
private:

};

class Mushroom : public Goodie {
public:
	Mushroom(StudentWorld* w, int x, int y);
	virtual void doSomething();
private:

};

class Star : public Goodie {
public:
	Star(StudentWorld* w, int x, int y);
	virtual void doSomething();
private:

};







#endif // ACTOR_H_
