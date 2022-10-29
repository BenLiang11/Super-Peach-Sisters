# Super-Peach-Sisters

Project 3 - Report.txt - Ben Liang
1)------------------------------------------------------------

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
Use Stringstreams to load in appropriate level
Obtain the level file and load actors for each char in a 32x32 grid
return GWSTATUS_CONTINUE_GAME
	virtual int move();
Iterate through each actor in vector, ask actor to doSomething(), delete if not alive
If peach's hp is 0, return GWSTATUS_PLAYER_DIED;
If the status indicates Peach reached a Flag, return GWSTATUS_FINISHED_LEVEL;
If the status indicates Peach reached Mario, return GWSTATUS_PLAYER_WON;
Use a stringstream to construct a status line for the game and output it
return GWSTATUS_CONTINUE_GAME;
	virtual void cleanUp();
For each actor in vector, delete it
Delete all pointers
	bool isBlocking(int x, int y);
If a given (x,y) coordinate is in between the coordinates that a actor (that obstructs) "takes up": return true
	Peach* getPeach();
	Actor* overlapping(int x, int y);
Almost entire the same as isBlocking, but returns a pointer to the actual actor in the coordinate (x,y)
	void addGoodie(int x, int y);
View the text file,find Goodie at (x,y), load it
private:
	Peach* m_peach;
	std::vector <Actor*> m_actors;
};

class Actor : public GraphObject {
	public:
		Actor(StudentWorld* w, int id, int x, int y, int dir, int depth, double size);
		virtual ~Actor();
		virtual void doSomething() = 0;
//doSomething() is pure virtual because almost all actors have a unique doSomething()
		virtual void bonk();
//bonk() is not pure virtual because there are some that do not really need a bonk() function
do nothing
		StudentWorld* getWorld();

		bool overlap();
Return true if Peach overlaps with the actor

		bool isAlive();
		void setAlive(bool a);
Other classes need to know if Actor is Alive
		bool obstruct();
		void setObstruct(bool o);
Other classes need to know if Actor blocks their movement
		bool damage();
		void setDamage(bool d);
Other classes need to know if the Actor could be damange
		int levelComplete();
		void setlevelComplete(int l);
Student world needs to know if the level is completed
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
Decrement invincibility if needed
If Peach overlaps with other Actors, bonk it
If Peach is Jumping, check 4 units above her to see if she can continue jumping
	Check if Peach's body could fit in space above
	Fall 4 pixels
If Peach is falling her body is fully off the ledge, she will fall 4 pixels
View user input:
Left input: move 4 pixels left or bonk() Actor to it's left
Right input: move 4 pixels right or bonk() Actor to it's right
Up input: Set jump power to 12 if she has mushroom, else has power of 8
		virtual void bonk();
		int getHp();
		void setHp(int a);
Other classes need to know Peach's Hp
		void setTempInvis(int n);
		int getTempInvis();
Other classes need to know if Peach is Invincible temporarily
		void setInvis(int n);
		int getInvis();
Other classes need to know if Peach is Invincible 
(I decided to make temp and star invincibility different counters of ticks)
		void setSP(bool n);
		bool getSP();
Other classes need to know if Peach can shoot
		bool getJp();
		void setJp(bool n);
Other classes need to know if Peach has jump power
	private:
		int m_hp;
		int m_jump;
		int m_tempInvis;
		int m_invis;
		bool m_shootpower;
		bool m_jp;
};

//-------------------- Rest of the actors ----------------
void Block::bonk()
	If block is in a special block position and it is bonked, 
	it calls the addGoodie() function at (x,y)
class Pipe
	Did not implement a parent class for Pipe and Block because it seemed simplier to not have one since Pipe and Block essentially do nothing.
void Flag::doSomething()
	If Peach overlaps it, increase score by 1000 and indicate to StudentWorld that the level is completed
void Mario::doSomething()
	If Peach overlaps it, increase score by 1000 and indicate to StudentWorld that the game is completed
class Goodie
bool Goodie::overlapG()
	Checks if Goodie overlaps with peach and does some common functions between Goodies:
	(Goodie dies, plays Powerup sound)

void Goodie::moveG()
	If goodie could fall, it will move 2 pixels down
	switch(Goodie's direction):
		Right: move 2 pixels right or flip direction to left
		Left: move 2 pixels left or flip direction to right
void Flower::doSomething()
	If overlapG(), increase score 50, give Peach shootpower and Peach 1 Hp.
	Call moveG()
void Mushroom::doSomething()
	If overlapG(), increase score 75, give Peach jumppower and Peach 1 Hp.
	Call moveG()
void Star::doSomething()
	If overlapG(), increase score 100, give Peach invincibility for 150 ticks.
	Call moveG()

void Projectile::moveP()
If Projectile could fall, it will move 2 pixels down
	switch(Projectile's direction):
		Right: move 2 pixels right, Projectile dies or flip direction to left
		Left: move 2 pixels left, Projectile dies or flip direction to right
void PiranhaFireball::doSomething()
	If it overlaps with Peach, bonk() Peach, it dies
	moveP();
void PeachFireball::doSomething()
	If it overlaps an Actor that could be damaged(enemies), indicate to have them die in next tick
	moveP();
void PeachFireball::doSomething()
	If it overlaps an Actor that could be damaged(enemies), indicate to have them die in next tick
	moveP();

void Enemy::bonk()
	Check if the bonker is Peach with the overlap function and see if she has invincibility
		increase score by 100 and declare this enemy dead
void Enemy::moveE()
	switch(Projectile's direction):
		Right: move 1 pixel right 
			or flip direction to left if there is something blocking or it is about to fall off edge
		Left: move 1 pixel left
			or flip direction to right if there is something blocking or it is about to fall off edge
void Goomba::doSomething()
	If it overlaps with Peach, bonk her
	moveE();
	If Goomba is damaged, it dies and increases score 100
void Koopa::bonk()
	Check if the bonker is Peach with the overlap function and see if she has invincibility
		increase score by 100 and declare this enemy dead
	Add shell in the direction Koopa is facing
	Only koopa has overridden bonk() from enemy because it has to add a shell into the game
void Koopa::doSomething()
	If it overlaps with Peach, bonk her
	moveE();
Piranhas has a m_delay private member
void Piranha::doSomething()
	If Piranha is alive, open and close it's Jaws with increaseAnimationNumber();
	If it overlaps with Peach, bonk her
	Set the direction to Peach's direction if she is within 1.5 SPRITE_HEIGHTs of it's height
	If there is a shooting delay, decrement
	If there is none, shoot fireball

//Class Hierarchy
Actor
	Peach
	Block
	Pipe

	Flag
	Mario

	Projectile
		PiranhaFireball
		PeachFireball
		Shell
	Enemy
		Goomba
		Koopa
		Piranha
	Goodie
		Flower
		Mushroom
		Star
