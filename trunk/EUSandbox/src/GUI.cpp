#include "GUI.h"

CEGUI::MouseButton convertOgreButtonToCegui(int buttonID)
{
   switch (buttonID)
   {
   case MouseEvent::BUTTON0_MASK:
       return CEGUI::LeftButton;
   case MouseEvent::BUTTON1_MASK:
       return CEGUI::RightButton;
   case MouseEvent::BUTTON2_MASK:
       return CEGUI::MiddleButton;
   case MouseEvent::BUTTON3_MASK:
       return CEGUI::X1Button;
   default:
       return CEGUI::LeftButton;
   }
}
