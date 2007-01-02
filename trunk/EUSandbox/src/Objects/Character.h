#ifndef CHARACTER_H
#define CHARACTER_H

#include "Objects/Object.h"

class Character : public Object
{
public:
	Character(String name, SceneManager* scenemgr, Vector3 position, Vector3 velocity,Vector3 acceleration)
	:Object(name, scenemgr, position, velocity, acceleration)
	{}
	void update (Real elapsedTime, InputReader *input)
	{
	}
};

// Specialization of the Character class - Our dear Ogre :D
class Asteroid : public Character
{
public:
	Asteroid(String name, SceneManager* sceneMgr, Vector3 position = Vector3(0,0,0), Vector3 velocity = Vector3(0,0,0), Vector3 acceleration = Vector3(0,0,0));
};

class OgreCharacter : public Character {
// Attributes ------------------------------------------------------------------------------
	protected:
		String mName;
		Vector3 mSpeed;
		float mAcceleration;
		ParticleSystem *pThrusters;
		SceneManager *mSceneMgr;
public:
// Methods ---------------------------------------------------------------------------------
	protected:
	public:
	OgreCharacter(String name, SceneManager *sceneMgr, Vector3 position = Vector3(0,0,0), Vector3 velocity = Vector3(0,0,0), Vector3 acceleration = Vector3(0,0,0) );
		~OgreCharacter();

		void update (Real elapsedTime, InputReader *input);

		// Change visibility - Useful for 1st person view ;)
		void setVisible (bool visible) {
			mMainNode->setVisible (visible);
		}
};

#endif //CHARACTER_H
