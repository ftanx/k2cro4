// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_H_
#define UI_BASE_IME_INPUT_METHOD_H_

#include <string>

#include "base/basictypes.h"
#include "base/event_types.h"
#include "base/i18n/rtl.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/base/ui_export.h"

namespace ui {

namespace internal {
class InputMethodDelegate;
}  // namespace internal

class KeyEvent;
class TextInputClient;

// An interface implemented by an object that encapsulates a native input method
// service provided by the underlying operating system, and acts as a "system
// wide" input method for all Chrome windows. A class that implements this
// interface should behave as follows:
// - Receives a keyboard event directly from a message dispatcher for the
//   system through the InputMethod::DispatchKeyEvent API, and forwards it to
//   an underlying input method for the OS.
// - The input method should handle the key event either of the following ways:
//   1) Send the original key down event to the focused window, which is e.g.
//      a NativeWidgetAura (NWA) or a RenderWidgetHostViewAura (RWHVA), using
//      internal::InputMethodDelegate::DispatchKeyEventPostIME API, then send
//      a Char event using TextInputClient::InsertChar API to a text input
//      client, which is, again, e.g. NWA or RWHVA, and then send the original
//      key up event to the same window.
//   2) Send VKEY_PROCESSKEY event to the window using the DispatchKeyEvent API,
//      then update IME status (e.g. composition text) using TextInputClient,
//      and then send the original key up event to the window.
// - Keeps track of the focused TextInputClient to see which client can call
//   APIs, OnTextInputTypeChanged, OnCaretBoundsChanged, and CancelComposition,
//   that change the state of the input method.
// In Aura environment, aura::RootWindowHost creates an instance of
// ui::InputMethod and owns it.
class InputMethod {
 public:
  virtual ~InputMethod() {}

  // Sets the delegate used by this InputMethod instance. It should only be
  // called by an object which manages the whole UI.
  virtual void SetDelegate(internal::InputMethodDelegate* delegate) = 0;

  // Initializes the InputMethod object. Pass true if the system toplevel window
  // already has keyboard focus.
  virtual void Init(bool focused) = 0;

  // Called when the top-level system window gets keyboard focus.
  virtual void OnFocus() = 0;

  // Called when the top-level system window loses keyboard focus.
  virtual void OnBlur() = 0;

  // Sets the text input client which receives text input events such as
  // SetCompositionText(). |client| can be NULL. A gfx::NativeWindow which
  // implementes TextInputClient interface, e.g. NWA and RWHVA, should register
  // itself by calling the method when it is focused, and unregister itself by
  // calling the metho with NULL when it is unfocused.
  virtual void SetFocusedTextInputClient(TextInputClient* client) = 0;

  // Gets the current text input client. Returns NULL when no client is set.
  virtual TextInputClient* GetTextInputClient() const = 0;

  // Dispatches a key event to the input method. The key event will be
  // dispatched back to the caller via
  // ui::InputMethodDelegate::DispatchKeyEventPostIME(), once it's processed by
  // the input method. It should only be called by a message dispatcher.
  virtual void DispatchKeyEvent(const base::NativeEvent& native_key_event) = 0;

  // TODO(yusukes): Add DispatchFabricatedKeyEvent to support virtual keyboards.
  virtual void DispatchFabricatedKeyEvent(const ui::KeyEvent& event) = 0;

  // Called by the focused client whenever its text input type is changed.
  // Before calling this method, the focused client must confirm or clear
  // existing composition text and call InputMethod::CancelComposition() when
  // necessary. Otherwise unexpected behavior may happen. This method has no
  // effect if the client is not the focused client.
  virtual void OnTextInputTypeChanged(const TextInputClient* client) = 0;

  // Called by the focused client whenever its caret bounds is changed.
  // This method has no effect if the client is not the focused client.
  virtual void OnCaretBoundsChanged(const TextInputClient* client) = 0;

  // Called by the focused client to ask the input method cancel the ongoing
  // composition session. This method has no effect if the client is not the
  // focused client.
  virtual void CancelComposition(const TextInputClient* client) = 0;

  // Returns the locale of current keyboard layout or input method, as a BCP-47
  // tag, or an empty string if the input method cannot provide it.
  virtual std::string GetInputLocale() = 0;

  // Returns the text direction of current keyboard layout or input method, or
  // base::i18n::UNKNOWN_DIRECTION if the input method cannot provide it.
  virtual base::i18n::TextDirection GetInputTextDirection() = 0;

  // Checks if the input method is active, i.e. if it's ready for processing
  // keyboard event and generate composition or text result.
  // If the input method is inactive, then it's not necessary to inform it the
  // changes of caret bounds and text input type.
  // Note: character results may still be generated and sent to the text input
  // client by calling TextInputClient::InsertChar(), even if the input method
  // is not active.
  virtual bool IsActive() = 0;

  // Gets the text input type of the focused text input client. Returns
  // ui::TEXT_INPUT_TYPE_NONE if there is no focused client.
  virtual TextInputType GetTextInputType() const = 0;

  // Checks if the focused text input client supports inline composition.
  virtual bool CanComposeInline() const = 0;
};

}  // namespace ui

#endif  // UI_BASE_IME_INPUT_METHOD_H_
