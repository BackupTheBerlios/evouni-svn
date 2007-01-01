#ifndef SAMPLELISTENER_H
#define SAMPLELISTENER_H

#include "ExampleFrameListener.h"
#include "camera.h"
#include "Character.h"

#define MAX_OBJECTS 20

class ARPhysikListener : public ExampleFrameListener
{
protected:
	Object* obj[MAX_OBJECTS];
	int num_of_obj;
public:
	ARPhysikListener(RenderWindow* win, Camera* cam);
	bool addObject(Object* object);
	void clearObjectList()
	{
		num_of_obj = 0;
	}
	bool frameStarted(const FrameEvent& evt);
};

class SampleListener : public ExampleFrameListener
{
protected:
	// References to the main character and the camera
	Character *mChar;
	EUCamera *mEUCamera;

	// Camera mode - Now supports 1st person, 3rd person (chasing) and 3rd person (fixed)
	unsigned int mMode;
	float toggler;

public:
	SampleListener(RenderWindow* win, Camera* cam)
	: ExampleFrameListener(win, cam)
	{
		mChar = 0;
		mEUCamera = 0;
		mMode = 0;
		toggler = 0;
	}

	void setCharacter (Character *character) {
		mChar = character;
	}

	void setExtendedCamera (EUCamera *cam) {
		mEUCamera = cam;
	}

	bool checkInput(const FrameEvent& evt);
	bool frameStarted(const FrameEvent& evt);
};

#endif //SAMPLELISTENER_H