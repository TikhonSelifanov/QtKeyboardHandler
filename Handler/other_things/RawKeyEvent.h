#ifndef NSAPPLICATION_NSKEYBOARD_CRAWKEYEVENT_H
#define NSAPPLICATION_NSKEYBOARD_CRAWKEYEVENT_H

#include "other_things/TimeApp.h"
#include "other_things/KeyID.h"
#include "other_things/KeyPosition.h"
#include "other_things/KeyTextData.h"
#include "other_things/KeyFlags.h"

#include <QString>

namespace NSApplication {
namespace NSKeyboard {

struct CKeyPressing {
  using CKeyFlags = NSKernel::CKeyFlags;
  CTime PressingTime;
  CKeyPosition KeyPosition;
  CKeyID KeyID;
  CLabelData KeyLabel;
  CKeyTextData KeyText;
  CKeyFlags Shifters;
};

struct CKeyReleasing {
  CTime ReleasingTime;
  CKeyPosition KeyPosition;
  CKeyID KeyID;
};

} // namespace NSKeyboard
} // namespace NSApplication

#endif // NSAPPLICATION_NSKEYBOARD_CRAWKEYEVENT_H
