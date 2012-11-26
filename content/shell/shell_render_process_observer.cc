// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/shell/shell_render_process_observer.h"

#include "base/command_line.h"
#include "content/public/renderer/render_view.h"
#include "content/public/renderer/render_thread.h"
#include "content/shell/shell_messages.h"
#include "content/shell/shell_switches.h"
#include "content/shell/webkit_test_runner_bindings.h"
#include "webkit/glue/webkit_glue.h"
#include "webkit/support/gc_extension.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebTestingSupport.h"
#include "third_party/WebKit/Tools/DumpRenderTree/chromium/TestRunner/public/WebTestInterfaces.h"

using WebKit::WebFrame;
using WebKit::WebTestingSupport;
using WebTestRunner::WebTestDelegate;
using WebTestRunner::WebTestInterfaces;

namespace content {

namespace {
ShellRenderProcessObserver* g_instance = NULL;
}

// static
ShellRenderProcessObserver* ShellRenderProcessObserver::GetInstance() {
  return g_instance;
}

ShellRenderProcessObserver::ShellRenderProcessObserver()
    : test_delegate_(NULL) {
  CHECK(!g_instance);
  g_instance = this;
  RenderThread::Get()->AddObserver(this);
}

ShellRenderProcessObserver::~ShellRenderProcessObserver() {
  CHECK(g_instance == this);
  g_instance = NULL;
}

void ShellRenderProcessObserver::SetMainWindow(
    RenderView* view,
    WebTestDelegate* delegate) {
  if (view == NULL) {
    if (delegate == test_delegate_) {
      test_interfaces_->setDelegate(NULL);
      test_interfaces_->setWebView(NULL);
      test_delegate_ = NULL;
    }
  } else {
    test_interfaces_->setDelegate(delegate);
    test_interfaces_->setWebView(view->GetWebView());
    test_delegate_ = delegate;
  }
}

void ShellRenderProcessObserver::BindTestRunnersToWindow(WebFrame* frame) {
  WebTestingSupport::injectInternalsObject(frame);
  test_interfaces_->bindTo(frame);
}

void ShellRenderProcessObserver::WebKitInitialized() {
  if (!CommandLine::ForCurrentProcess()->HasSwitch(switches::kDumpRenderTree))
    return;

  // To implement a catch-all for not yet implemented controller properties.
  webkit_glue::SetJavaScriptFlags(" --harmony_proxies");
  RenderThread::Get()->RegisterExtension(new WebKitTestRunnerBindings());

  // We always expose GC to layout tests.
  webkit_glue::SetJavaScriptFlags(" --expose-gc");
  RenderThread::Get()->RegisterExtension(extensions_v8::GCExtension::Get());

  test_interfaces_.reset(new WebTestInterfaces);
}

bool ShellRenderProcessObserver::OnControlMessageReceived(
    const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(ShellRenderProcessObserver, message)
    IPC_MESSAGE_HANDLER(ShellViewMsg_ResetAll, OnResetAll)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  return handled;
}

void ShellRenderProcessObserver::OnResetAll() {
  test_interfaces_->resetAll();
  // We don't reset the WebTestingSupport objects, as we don't reuse WebViews.
}

}  // namespace content
