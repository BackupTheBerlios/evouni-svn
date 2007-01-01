#ifndef CHARACTER_H
#define CHARACTER_H

#include "OGREinclude.h"

// Generic Character class
class Object {
// Attributes ------------------------------------------------------------------------------
	protected:
		SceneManager *mSceneMgr;
		SceneNode *mMainNode; // Main character node
		Vector3 mVelocity;
		Vector3 mAcceleration;
		Entity *mEntity; // Character entity
		String mName;
	public:
// Methods ---------------------------------------------------------------------------------
	protected:
	public:
		Object(String name, SceneManager* scenemgr, SceneNode* node, Vector3 velocity, Vector3 acceleration)
		{
			mName = name;
			mMainNode = node;
			mVelocity = velocity;
			mAcceleration = acceleration;
			mSceneMgr = scenemgr; 
		}
		Object(String name, SceneManager* sceneMgr, Vector3 position, Vector3 velocity, Vector3 acceleration)
		{
			mSceneMgr = sceneMgr;
			mName = name;
			mMainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(mName,position);
			mVelocity = velocity;
			mAcceleration = acceleration;
		}
		// Updates the character (movement...)
		virtual void update (Real elapsedTime, InputReader *input) = 0;
		// The three methods below returns the two camera-related nodes, 
		// and the current position of the character (for the 1st person camera)
		SceneNode *getMainNode()
		{
			return mMainNode;
		}
		Vector3 getWorldPosition () {
			return mMainNode->getWorldPosition ();
		}
		Vector3 getVelocity()
		{
			return mVelocity;
		}
		Vector3 getAcceleration()
		{
			return mAcceleration;
		}
		void setAcceleration(Vector3 a)
		{
			mAcceleration = a;
		}
		void setVelocity(Vector3 vel)
		{
			mVelocity = vel;
		}

};

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
		float mSpeed;
		float mAcceleration;
		ParticleSystem *pThrusters;
		SceneManager *mSceneMgr;
public:
// Methods ---------------------------------------------------------------------------------
	protected:
	public:
	OgreCharacter(String name, SceneManager *sceneMgr, Vector3 position = Vector3(0,0,0), Vector3 velocity = Vector3(0,0,0), Vector3 acceleration = Vector3(0,0,0) );
		~OgreCharacter () {
			mMainNode->detachAllObjects ();
			delete mEntity;
			mMainNode->removeAndDestroyAllChildren ();
			mSceneMgr->destroySceneNode (mName);
		}

		void update (Real elapsedTime, InputReader *input) {
			// Handle movement
			if (input->isKeyDown (KC_UP)) {
				mAcceleration += 0.01;				
			}
			if (input->isKeyDown (KC_DOWN)) {
				mAcceleration -= 0.01;				
			}
			if(mAcceleration!=0){
				if (input->isKeyDown (KC_W)) {
					mMainNode->pitch (Radian (elapsedTime));
				}
				if (input->isKeyDown (KC_S)) {
					mMainNode->pitch (Radian (-elapsedTime));
				}
				if (input->isKeyDown (KC_Q)) {
					mMainNode->yaw (Radian (elapsedTime));
				}
				if (input->isKeyDown (KC_E)) {
					mMainNode->yaw (Radian (-elapsedTime));
				}
				if (input->isKeyDown (KC_A)) {
					mMainNode->roll (Radian (-elapsedTime));
				}
				if (input->isKeyDown (KC_D)) {
					mMainNode->roll (Radian (elapsedTime));
				}
			}
			if (input->isKeyDown (KC_SPACE))
			{
				mAcceleration = 0;
			}
			if(mAcceleration<-0.1) {
				mAcceleration=-0.1;
			}
			if(mAcceleration>0.1) {
				mAcceleration=0.1;
			}
			pThrusters->setDefaultDimensions(mAcceleration*200, mAcceleration*200);
			if(mAcceleration!=0){
				mMainNode->yaw( Degree(-70 * input->getMouseRelativeX() * elapsedTime) );
				//mCamNode->getChild( 0 )->pitch( Degree(-mRotate * mInputDevice->getMouseRelativeY()) );
				mMainNode->pitch( Degree(70* input->getMouseRelativeY() * elapsedTime) );
			}	
			mSpeed+=mAcceleration*elapsedTime;
			mMainNode->translate (mMainNode->getOrientation () * Vector3 (0, 0, mSpeed));
		}

		// Change visibility - Useful for 1st person view ;)
		void setVisible (bool visible) {
			mMainNode->setVisible (visible);
		}
};

#endif //CHARACTER_H