#pragma once


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

