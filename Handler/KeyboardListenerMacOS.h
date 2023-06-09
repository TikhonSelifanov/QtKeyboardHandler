#ifndef NSAPPLICATION_NSKEYBOARD_CKEYBOARDLISTENERMACOS_H
#define NSAPPLICATION_NSKEYBOARD_CKEYBOARDLISTENERMACOS_H

#include "MacOSKeyboardAPI.h"
#include "other_things/AnyKeyboardKiller.h"
#include "KeyTextMaker.h"
#include "ShifterInfo.h"
#include "other_things/KeyFlags.h"
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>

#include <QObject>

#include <future>

namespace NSApplication {
namespace NSKeyboard {

struct CKeyPressing;
struct CKeyReleasing;

class CKeyboardHandler;

namespace NSMacOS {

class CKeyboardListenerMacImpl : public QObject {
  Q_OBJECT

  friend class CKiller;

  using CVKCode = CMacOSKeyboardAPI::CVKCode;
  using CEventTapUPtr = CMacOSKeyboardAPI::CEventTapUPtr;
  using CMessagePortUPtr = CMacOSKeyboardAPI::CMessagePortUPtr;
  using CType = NSKernel::CKeyFlagsEnum::CType;

public:
  using CAnyKillerPromise = std::promise<CAnyKeyboardKiller>;
  CKeyboardListenerMacImpl(CAnyKillerPromise, CKeyboardHandler*);
  ~CKeyboardListenerMacImpl();

Q_SIGNALS:
  void KeyPressing(const CKeyPressing&);
  void KeyReleasing(const CKeyReleasing&);

private:
  CFMachPortRef createEventTap();
  void addEventTapToRunLoop();
  static CGEventRef callbackEventTap(CGEventTapProxy, CGEventType, CGEventRef, void*);

  CFMessagePortRef createMessagePort();
  void addMessagePortToRunLoop();
  static CFDataRef callbackMessagePort(CFMessagePortRef, SInt32, CFDataRef, void*);

  static CLabelData getKeyLabel(CKeyTextData&);
  static CKeyboardListenerMacImpl* getListener(void*);
  static CType getFlag(const CVKCode VKCode);
  static bool isDeadKey(CKeyTextData& keyTextData);

  CEventTapUPtr EventTap_;
  CMessagePortUPtr MessagePort_;
  CShifterInfo ShifterInfo_;
  CKeyTextMaker KeyTextMaker_;
};

// The object provides a way to shut down the listener
class CKiller {
public:
  CKiller() = default;
  CKiller(CFMessagePortRef);

  void stopListener() const;

private:
  CFMessagePortRef KillerPort_;
};

} // namespace NSMacOS
} // namespace NSKeyboard
} // namespace NSApplication

#endif // NSAPPLICATION_NSKEYBOARD_CKEYBOARDLISTENERMACOS_H
