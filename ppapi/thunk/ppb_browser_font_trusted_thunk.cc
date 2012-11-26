// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/thunk/thunk.h"
#include "ppapi/thunk/enter.h"
#include "ppapi/thunk/ppb_browser_font_trusted_api.h"
#include "ppapi/thunk/resource_creation_api.h"

namespace ppapi {
namespace thunk {

namespace {

typedef EnterResource<PPB_BrowserFont_Trusted_API> EnterBrowserFont;

PP_Var GetFontFamilies(PP_Instance instance) {
  EnterInstance enter(instance);
  if (enter.failed())
    return PP_MakeUndefined();
  return enter.functions()->GetFontFamilies(instance);
}

PP_Resource Create(PP_Instance instance,
                   const PP_BrowserFont_Trusted_Description* description) {
  EnterResourceCreation enter(instance);
  if (enter.failed())
    return 0;
  return enter.functions()->CreateBrowserFont(instance, description);
}

PP_Bool IsBrowserFont(PP_Resource resource) {
  EnterBrowserFont enter(resource, false);
  return enter.succeeded() ? PP_TRUE : PP_FALSE;
}

PP_Bool Describe(PP_Resource font_id,
                 PP_BrowserFont_Trusted_Description* description,
                 PP_BrowserFont_Trusted_Metrics* metrics) {
  EnterBrowserFont enter(font_id, true);
  if (enter.failed())
    return PP_FALSE;
  return enter.object()->Describe(description, metrics);
}

PP_Bool DrawTextAt(PP_Resource font_id,
                   PP_Resource image_data,
                   const PP_BrowserFont_Trusted_TextRun* text,
                   const PP_Point* position,
                   uint32_t color,
                   const PP_Rect* clip,
                   PP_Bool image_data_is_opaque) {
  EnterBrowserFont enter(font_id, true);
  if (enter.failed())
    return PP_FALSE;
  return enter.object()->DrawTextAt(image_data, text, position, color, clip,
                                    image_data_is_opaque);
}

int32_t MeasureText(PP_Resource font_id,
                    const PP_BrowserFont_Trusted_TextRun* text) {
  EnterBrowserFont enter(font_id, true);
  if (enter.failed())
    return -1;
  return enter.object()->MeasureText(text);
}

uint32_t CharacterOffsetForPixel(PP_Resource font_id,
                                 const PP_BrowserFont_Trusted_TextRun* text,
                                 int32_t pixel_position) {
  EnterBrowserFont enter(font_id, true);
  if (enter.failed())
    return -1;
  return enter.object()->CharacterOffsetForPixel(text, pixel_position);
}

int32_t PixelOffsetForCharacter(PP_Resource font_id,
                                const PP_BrowserFont_Trusted_TextRun* text,
                                uint32_t char_offset) {
  EnterBrowserFont enter(font_id, true);
  if (enter.failed())
    return -1;
  return enter.object()->PixelOffsetForCharacter(text, char_offset);
}

const PPB_BrowserFont_Trusted_1_0 g_ppb_browser_font_trusted_thunk = {
  &GetFontFamilies,
  &Create,
  &IsBrowserFont,
  &Describe,
  &DrawTextAt,
  &MeasureText,
  &CharacterOffsetForPixel,
  &PixelOffsetForCharacter
};

}  // namespace

const PPB_BrowserFont_Trusted_1_0* GetPPB_BrowserFont_Trusted_1_0_Thunk() {
  return &g_ppb_browser_font_trusted_thunk;
}

}  // namespace thunk
}  // namespace ppapi
