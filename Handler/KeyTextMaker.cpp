#include "KeyTextMaker.h"

namespace NSApplication {
namespace NSKeyboard {
namespace NSMacOS {
CKeyTextData CKeyTextMaker::get(const CVKCode VKCode) {
  if (CMacOSKeyboardAPI::isShifter(VKCode)) { return {{QChar(), QChar()}, 0}; }

  CInputSourcePtr CurrentKeyboardLayoutInputSource(
      CMacOSKeyboardAPI::copyCurrentKeyboardLayoutInputSource());
  if (!CurrentKeyboardLayoutInputSource.get()) {
    throw std::runtime_error("Failed to copy KeyboardLayoutInputSource");
  }

  if (CMacOSKeyboardAPI::isEqual(CurrentKeyboardLayoutInputSource.get(),
                                 KeyboardLayoutInputSource_.get())       == false) {
    DeadKeyState_ = 0;
  }
  KeyboardLayoutInputSource_ = std::move(CurrentKeyboardLayoutInputSource);

  const UCKeyboardLayout* KeyboardLayout =
      CMacOSKeyboardAPI::getCurrentKeyboardLayout(KeyboardLayoutInputSource_.get());
  if (KeyboardLayout == NULL) {
     throw std::runtime_error("Failed to get KeyboardLayout");
  }

  UniCharCount ActualStringLength =
      CMacOSKeyboardAPI::getUnicodeString(KeyboardLayout, VKCode, &DeadKeyState_, Text_);
  return getPrintableText(QString::fromUtf16(Text_, ActualStringLength));
}

CKeyTextData CKeyTextMaker::getPrintableText(QString&& Text) const {
  if (DeadKeyState_ != 0 && !Text.isEmpty()) {
    return {{Text.front(), QChar()}, 3};
  }
  if (Text.size() == 0 || CMacOSKeyboardAPI::getShifterState(::kVK_Command)) {
    return {{QChar(), QChar()}, 0};
  }
  if (Text.back().isPrint()) {
    return {{Text.front(), Text.back()}, 1};
  }
  return {{QChar(), QChar()}, 0};
}

} // namespace NSMacOS
} // namespace NSKeyboard
} // namespace NSApplication
