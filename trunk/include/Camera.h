#ifndef EXTENDEDCAMERA_H
#define EXTENDEDCAMERA_H

#include "OGREinclude.h"

#define CAM_MODE_FIXED			1
#define CAM_MODE_FIXED_CHASE	2
#define CAM_MODE_CHASE			3
#define CAM_MODE_FIRST_PERSON	4
#define CAM_MODE_X				5
#define CAM_MODE_Y				6
#define CAM_MODE_Z				7
#define CAM_MODE_EXPERIMENTAL	8


// Our extended camera class
class ExtendedCamera {
	protected:
		SceneNode *mTargetNode; // The camera target
		SceneNode *mCameraNode; // The camera itself
		Camera *mCamera; // Ogre camera

		SceneManager *mSceneMgr;
		String mName;

		bool mOwnCamera; // To know if the ogre camera binded has been created outside or inside of this class

		Real mTightness; // Determines the movement of the camera - 1 means tight movement, while 0 means no movement
	public:
// Methods ---------------------------------------------------------------------------------
	protected:
	public:
ExtendedCamera (String name, SceneManager *sceneMgr, Camera *camera);
		~ExtendedCamera ();
		void setTightness (Real tightness) {
			mTightness = tightness;
		}

		Real getTightness () {
			return mTightness;
		}

		Vector3 getCameraPosition () {
			return mCameraNode->getPosition ();
		}
		void instantUpdate (Vector3 cameraPosition, Vector3 targetPosition);
					void update (Real elapsedTime, Vector3 cameraPosition, Vector3 targetPosition);
					void update (Real elapsedTime, SceneNode *camera, SceneNode* target);

};

class EUCamera{
	protected:
		SceneNode *mTargetNode; // The camera target
		//This stuff is for the ChaseCam
		Real mAzimut;
		Real mHeight;
		Real mDistance;
		SceneNode *mIdealOrientationNode;
		SceneNode *mIdealPositionNode;
		Vector3 mVelocity; 
		SceneNode *mCameraNode; // The camera itself
		Real mSpringConstant;
		Real mDampingConstant;

		Camera *mCamera; // Ogre camera
		int mMode;

		SceneManager *mSceneMgr;
		String mName;

		bool mOwnCamera; // To know if the ogre camera binded has been created outside or inside of this class
	public:
		EUCamera(String name, SceneManager *sceneMgr, Camera *camera, SceneNode* target);
		~EUCamera(){}
		void setMode(int mode);
		void update (Real elapsedTime);
		void updateIdealPosition();
		void setAzimut(Real value)
		{
			mAzimut = value;
			updateIdealPosition();
		}
		void addAzimut(Real value)
		{
			mAzimut += value;
			updateIdealPosition();
		}
		void setHeight(Real value)
		{
			mHeight = value;
			updateIdealPosition();
		}
		void addHeight(Real value)
		{
			mHeight += value;
			updateIdealPosition();
		}
		void setDistance(Real value)
		{
			mDistance = value;
			updateIdealPosition();
		}
		void addDistance(Real value)
		{
			mDistance += value;
			updateIdealPosition();
		}
};
#endif //EXTENDEDCAMERA_H