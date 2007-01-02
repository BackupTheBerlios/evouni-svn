//#include "FrameListener.h"
#include "EUClient.h"

ARPhysikListener::ARPhysikListener(RenderWindow* win, Camera* cam)
: ExampleFrameListener(win, cam)
{
	clearObjectList();
}

bool ARPhysikListener::addObject(Object* object)
{
	if(num_of_obj < MAX_OBJECTS)
	{
		obj[num_of_obj++] = object;
		return true;
	}
	return false;
}


bool ARPhysikListener::frameStarted(const FrameEvent& evt)
{
	for(int i = 0; i < num_of_obj; i++)
	{
		///////////////////////Can do here acceleration///////////////////
		//Position aktualisieren
		Vector3 pos = obj[i]->getMainNode()->getPosition() + obj[i]->getVelocity() * evt.timeSinceLastFrame;
		//out of bounds erkennung
		if(pos.x > MAX_X_RAND) pos.x -= MAX_X_RAND * 2;
		if(pos.x < -MAX_X_RAND) pos.x += MAX_X_RAND * 2;
		if(pos.y > MAX_Y_RAND) pos.y -= MAX_Y_RAND * 2;
		if(pos.y < -MAX_Y_RAND) pos.y += MAX_Y_RAND * 2;
		if(pos.z > MAX_Z_RAND) pos.z -= MAX_Z_RAND * 2;
		if(pos.z < -MAX_Z_RAND) pos.z += MAX_Z_RAND * 2;
		//neu setzen
		obj[i]->getMainNode()->setPosition(pos);
	}
	
	return true;
}

bool SampleListener::checkInput(const FrameEvent& evt)
{
	mInputDevice->capture ();

	if(toggler > 0) 
		toggler -= evt.timeSinceLastFrame;

	if (mChar) {
		mChar->update (evt.timeSinceLastFrame, mInputDevice);

		mEUCamera->update(evt.timeSinceLastFrame);
	}

#define CAM_MODE_FIXED			1
#define CAM_MODE_FIXED_CHASE	2
#define CAM_MODE_CHASE			3
#define CAM_MODE_FIRST_PERSON	4
#define CAM_MODE_X				5
#define CAM_MODE_Y				6
#define CAM_MODE_Z				7
#define CAM_MODE_EXPERIMENTAL	8


	if(mInputDevice->isKeyDown(KC_F1))
	{
		mEUCamera->setMode(CAM_MODE_FIXED);
	}
	if(mInputDevice->isKeyDown(KC_F2))
	{
		mEUCamera->setMode(CAM_MODE_FIXED_CHASE);
	}
	if(mInputDevice->isKeyDown(KC_F3))
	{
		mEUCamera->setMode(CAM_MODE_CHASE);
	}
	if(mInputDevice->isKeyDown(KC_F4))
	{
		mEUCamera->setMode(CAM_MODE_FIRST_PERSON);
	}
	if(mInputDevice->isKeyDown(KC_F5))
	{
		mEUCamera->setMode(CAM_MODE_X);
	}
	if(mInputDevice->isKeyDown(KC_F6))
	{
		mEUCamera->setMode(CAM_MODE_X);
	}
	if(mInputDevice->isKeyDown(KC_F7))
	{
		mEUCamera->setMode(CAM_MODE_Z);
	}

	if(mInputDevice->isKeyDown(KC_K))
	{
		mEUCamera->addHeight(evt.timeSinceLastFrame * 15);
	}
	if(mInputDevice->isKeyDown(KC_I))
	{
		mEUCamera->addHeight(evt.timeSinceLastFrame * -15);
	}
	if(mInputDevice->isKeyDown(KC_L))
	{
		mEUCamera->addAzimut(evt.timeSinceLastFrame * 15);
	}
	if(mInputDevice->isKeyDown(KC_J))
	{
		mEUCamera->addAzimut(evt.timeSinceLastFrame * -15);
	}
	if(mInputDevice->isKeyDown(KC_U))
	{
		mEUCamera->addDistance(evt.timeSinceLastFrame * -100);
	}
	if(mInputDevice->isKeyDown(KC_O))
	{
		mEUCamera->addDistance(evt.timeSinceLastFrame * 100);
	}


	if(toggler <= 0)
	{
		if (mInputDevice->isKeyDown (KC_1))
		{
			CPM->loadLevel("level 1");
			toggler = 1;
		}
		if (mInputDevice->isKeyDown (KC_2))
		{
			CPM->loadLevel("level 2");
			toggler = 1;
		}
		if (mInputDevice->isKeyDown (KC_3))
		{
			CPM->loadLevel("level 3");
			toggler = 1;
		}
		if (mInputDevice->isKeyDown (KC_4))
		{
			CPM->loadLevel("level 4");
			toggler = 1;
		}
		if (mInputDevice->isKeyDown (KC_0))
		{
			CPM->loadLevel("random");
			toggler = 1;
		}
        if (mInputDevice->isKeyDown(KC_F))
        {
            mStatsOn = !mStatsOn;
            showDebugOverlay(mStatsOn);
            toggler = 1;
        }
        if (mInputDevice->isKeyDown(KC_T))
        {
            switch(mFiltering)
            {
            case TFO_BILINEAR:
                mFiltering = TFO_TRILINEAR;
                mAniso = 1;
                break;
            case TFO_TRILINEAR:
                mFiltering = TFO_ANISOTROPIC;
                mAniso = 8;
                break;
            case TFO_ANISOTROPIC:
                mFiltering = TFO_BILINEAR;
                mAniso = 1;
                break;
            default:
                break;
            }
            MaterialManager::getSingleton().setDefaultTextureFiltering(mFiltering);
            MaterialManager::getSingleton().setDefaultAnisotropy(mAniso);


            showDebugOverlay(mStatsOn);

            toggler = 1;
        }
        if (mInputDevice->isKeyDown(KC_SYSRQ))
        {
			char tmp[20];
			sprintf(tmp, "screenshot_%d.png", ++mNumScreenShots);
            mWindow->writeContentsToFile(tmp);
            toggler = 0.5;
			mWindow->setDebugText(String("Wrote ") + tmp);
        }
		if (mInputDevice->isKeyDown(KC_R))
		{
			mSceneDetailIndex = (mSceneDetailIndex+1)%3 ;
			switch(mSceneDetailIndex) {
				case 0 : mCamera->setPolygonMode(PM_SOLID) ; break ;
				case 1 : mCamera->setPolygonMode(PM_WIREFRAME) ; break ;
				case 2 : mCamera->setPolygonMode(PM_POINTS) ; break ;
			}
			toggler = 0.5;
		}
	}

	// Exit if we press Esc
	if (mInputDevice->isKeyDown (KC_ESCAPE))
		return false;

	return true;
}

bool SampleListener::frameStarted(const FrameEvent& evt)
{
	if (!checkInput(evt)) return false;
	CPM->CheckColission(ogre);

	return true;
}

/**/

