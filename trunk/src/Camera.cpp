#include "camera.h"

EUCamera::EUCamera(String name, SceneManager *sceneMgr, Camera *camera, SceneNode *target)
{
	// Basic member references setup
	mName = name;
	mSceneMgr = sceneMgr;

	// Create the camera's node structure
	mCameraNode = mSceneMgr->getRootSceneNode ()->createChildSceneNode (mName);
	mTargetNode = target;
	//mCameraNode->setAutoTracking (true, mTargetNode); // The camera will always look at the camera target
	//mCameraNode->setFixedYawAxis (true); // Needed because of auto tracking

	// Create our camera if it wasn't passed as a parameter
	if (camera == 0) {
		mCamera = mSceneMgr->createCamera (mName);
		mOwnCamera = true;
	}
	else {
		mCamera = camera;
		mOwnCamera = false;
	}
	// ... and attach the Ogre camera to the camera node
	mCameraNode->attachObject (mCamera);
	setMode(CAM_MODE_CHASE);

	mAzimut = 0;
	mHeight = -10;
	mDistance = 700;
	mIdealOrientationNode = mTargetNode->createChildSceneNode(mName + "_idealorientation");
	mIdealPositionNode = mIdealOrientationNode->createChildSceneNode(mName + "_idealposition");
	updateIdealPosition();
	mSpringConstant = 25/4;
	mDampingConstant = 5;
	mVelocity = Vector3(0,0,0);
	mCameraNode->setPosition( Vector3(0,0,200) );
	mCamera->setFOVy(Degree(60));
}

void EUCamera::update (Real elapsedTime)
{
//	mIdealSpherical.y = MathATan2( mCameraNode->getPosition().y - mTargetNode->getPosition().y,
//								   mCameraNode->getPosition().x - mTargetNode->getPosition().x);
	switch(mMode)
	{
	case CAM_MODE_X:
		{
			mCameraNode->setPosition(mTargetNode->getPosition() + Vector3(1000,0,0));
			mCameraNode->setOrientation(Quaternion(Math::Sqrt(0.5), 0, Math::Sqrt(0.5), 0));
		}
		break;
	case CAM_MODE_Y:
		{
			mCameraNode->setPosition(mTargetNode->getPosition() + Vector3(0,1000,0));
			mCameraNode->setOrientation(Quaternion(Math::Sqrt(0.5), -Math::Sqrt(0.5), 0, 0));
		}
		break;
	case CAM_MODE_Z:
		{
			Vector3 IdealPos = mTargetNode->getPosition() + Vector3(0,0,1000);
			mCameraNode->setPosition(IdealPos);
		}
		break;
	case CAM_MODE_CHASE:
		{
			Vector3 displacement = mCameraNode->getWorldPosition() - mIdealPositionNode->getWorldPosition();
			Vector3 springAcceleration = (-mSpringConstant * displacement) - ( mDampingConstant * mVelocity );
			//Vector3 spring = ( displacement * -mSpringConstant );
			//Vector3 damping = ( mVelocity * mDampingConstant );
			//Vector3 springAcceleration = spring - damping;
			mVelocity += springAcceleration * elapsedTime;
			mCameraNode->setPosition( mCameraNode->getPosition() + mVelocity * elapsedTime );

			mCameraNode->setOrientation(mIdealPositionNode->getWorldOrientation());
			mCameraNode->lookAt(mTargetNode->getPosition(), Node::TransformSpace::TS_WORLD);

		}
		break;
	case CAM_MODE_FIRST_PERSON:
		{
			mCameraNode->setPosition(mTargetNode->getPosition());
			Quaternion q = mTargetNode->getOrientation();
			q = q * Quaternion(Degree(180), Vector3::UNIT_Y);
			mCameraNode->setOrientation(q);
		}
		break;
	case CAM_MODE_FIXED_CHASE:
		{			
			mCameraNode->setPosition(mIdealPositionNode->getWorldPosition());
			mCameraNode->setOrientation(mIdealPositionNode->getWorldOrientation());
		}
		break;
	case CAM_MODE_FIXED:
		{
			mCameraNode->lookAt(mTargetNode->getPosition(), Node::TransformSpace::TS_WORLD);
		}
		break;
	case CAM_MODE_EXPERIMENTAL:
	default:
		{
			mCameraNode->setPosition(mIdealPositionNode->getWorldPosition());
			mCameraNode->setOrientation(mIdealPositionNode->getWorldOrientation());
		}
		break;
	}
}

void EUCamera::setMode(int mode)
{
	mMode = mode;
	if(mMode == CAM_MODE_FIRST_PERSON)
	{
		mTargetNode->setVisible(false);
	}
	else
	{
		mTargetNode->setVisible(true);
	}
}

void EUCamera::updateIdealPosition()
{
	Quaternion q;
	q = q * Quaternion(Degree(mAzimut+180), Vector3::UNIT_Y); //rotate by mAzimut on Y-Axis
	q = q * Quaternion(Degree(mHeight), Vector3::UNIT_X); //rotate by mHeight
	mIdealOrientationNode->setOrientation(q);
	Vector3 distance = Vector3(0,0,mDistance);
	mIdealPositionNode->setPosition(distance);

//	vec = mIdealPositionNode->getWorldPosition();
//	LogManager::getSingletonPtr()->logMessage("5: x: " + StringConverter::toString(vec.x) + " y: " + StringConverter::toString(vec.y) + " z: " + StringConverter::toString(vec.z));
/*
mIdealPositionNode->setPosition(mTargetNode->getPosition()); //set Position to Target
	Quaternion q = mTargetNode->getOrientation(); //getOrientation
	q = q * Quaternion(Degree(mAzimut), Vector3::UNIT_Y); //rotate by mAzimut on Y-Axis
	q = q * Quaternion(Degree(mHeight), Vector3::UNIT_X); //rotate by mHeight
	mIdealPositionNode->setOrientation(q);
	mIdealPositionNode->translate(Vector3(0,0,-mDistance));
*/
}