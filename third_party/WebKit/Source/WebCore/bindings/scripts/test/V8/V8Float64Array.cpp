/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "config.h"
#include "V8Float64Array.h"

#include "BindingState.h"
#include "ContextFeatures.h"
#include "ExceptionCode.h"
#include "Frame.h"
#include "RuntimeEnabledFeatures.h"
#include "V8ArrayBufferView.h"
#include "V8ArrayBufferViewCustom.h"
#include "V8Binding.h"
#include "V8DOMWrapper.h"
#include "V8Float32Array.h"
#include "V8Int32Array.h"
#include <wtf/Float32Array.h>
#include <wtf/Float64Array.h>
#include <wtf/GetPtr.h>
#include <wtf/Int32Array.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>
#include <wtf/UnusedParam.h>

namespace WebCore {

WrapperTypeInfo V8Float64Array::info = { V8Float64Array::GetTemplate, V8Float64Array::derefObject, 0, 0, V8Float64Array::installPerContextPrototypeProperties, &V8ArrayBufferView::info, WrapperTypeObjectPrototype };

namespace Float64ArrayV8Internal {

template <typename T> void V8_USE(T) { }

static v8::Handle<v8::Value> fooCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.Float64Array.foo");
    if (args.Length() < 1)
        return throwNotEnoughArgumentsError(args.GetIsolate());
    Float64Array* imp = V8Float64Array::toNative(args.Holder());
    EXCEPTION_BLOCK(Float32Array*, array, V8Float32Array::HasInstance(MAYBE_MISSING_PARAMETER(args, 0, DefaultIsUndefined)) ? V8Float32Array::toNative(v8::Handle<v8::Object>::Cast(MAYBE_MISSING_PARAMETER(args, 0, DefaultIsUndefined))) : 0);
    return toV8(imp->foo(array), args.Holder(), args.GetIsolate());
}

static v8::Handle<v8::Value> setCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.Float64Array.set");
    return setWebGLArrayHelper<Float64Array, V8Float64Array>(args);
}

} // namespace Float64ArrayV8Internal

v8::Handle<v8::Object> wrap(Float64Array* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl);
    v8::Handle<v8::Object> wrapper = V8Float64Array::createWrapper(impl, creationContext, isolate);
    if (!wrapper.IsEmpty())
        wrapper->SetIndexedPropertiesToExternalArrayData(impl->baseAddress(), v8::kExternalDoubleArray, impl->length());
    return wrapper;
}

static const V8DOMConfiguration::BatchedCallback V8Float64ArrayCallbacks[] = {
    {"set", Float64ArrayV8Internal::setCallback},
};

v8::Handle<v8::Value> V8Float64Array::constructorCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.Float64Array.Contructor");
    return constructWebGLArray<Float64Array, V8Float64Array, double>(args, &info, v8::kExternalDoubleArray);
}

static v8::Persistent<v8::FunctionTemplate> ConfigureV8Float64ArrayTemplate(v8::Persistent<v8::FunctionTemplate> desc)
{
    desc->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = V8DOMConfiguration::configureTemplate(desc, "Float64Array", V8ArrayBufferView::GetTemplate(), V8Float64Array::internalFieldCount,
        0, 0,
        V8Float64ArrayCallbacks, WTF_ARRAY_LENGTH(V8Float64ArrayCallbacks));
    UNUSED_PARAM(defaultSignature); // In some cases, it will not be used.
    desc->SetCallHandler(V8Float64Array::constructorCallback);
    v8::Local<v8::ObjectTemplate> instance = desc->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> proto = desc->PrototypeTemplate();
    UNUSED_PARAM(instance); // In some cases, it will not be used.
    UNUSED_PARAM(proto); // In some cases, it will not be used.
    

    // Custom Signature 'foo'
    const int fooArgc = 1;
    v8::Handle<v8::FunctionTemplate> fooArgv[fooArgc] = { V8Float32Array::GetRawTemplate() };
    v8::Handle<v8::Signature> fooSignature = v8::Signature::New(desc, fooArgc, fooArgv);
    proto->Set(v8::String::NewSymbol("foo"), v8::FunctionTemplate::New(Float64ArrayV8Internal::fooCallback, v8Undefined(), fooSignature));

    // Custom toString template
    desc->Set(v8::String::NewSymbol("toString"), V8PerIsolateData::current()->toStringTemplate());
    return desc;
}

v8::Persistent<v8::FunctionTemplate> V8Float64Array::GetRawTemplate()
{
    V8PerIsolateData* data = V8PerIsolateData::current();
    V8PerIsolateData::TemplateMap::iterator result = data->rawTemplateMap().find(&info);
    if (result != data->rawTemplateMap().end())
        return result->value;

    v8::HandleScope handleScope;
    v8::Persistent<v8::FunctionTemplate> templ = createRawTemplate();
    data->rawTemplateMap().add(&info, templ);
    return templ;
}

v8::Persistent<v8::FunctionTemplate> V8Float64Array::GetTemplate()
{
    V8PerIsolateData* data = V8PerIsolateData::current();
    V8PerIsolateData::TemplateMap::iterator result = data->templateMap().find(&info);
    if (result != data->templateMap().end())
        return result->value;

    v8::HandleScope handleScope;
    v8::Persistent<v8::FunctionTemplate> templ =
        ConfigureV8Float64ArrayTemplate(GetRawTemplate());
    data->templateMap().add(&info, templ);
    return templ;
}

bool V8Float64Array::HasInstance(v8::Handle<v8::Value> value)
{
    return GetRawTemplate()->HasInstance(value);
}


v8::Handle<v8::Object> V8Float64Array::createWrapper(PassRefPtr<Float64Array> impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl.get());
    ASSERT(DOMDataStore::current(isolate)->get(impl.get()).IsEmpty());
    ASSERT(static_cast<void*>(static_cast<ArrayBufferView*>(impl.get())) == static_cast<void*>(impl.get()));

    v8::Handle<v8::Object> wrapper = V8DOMWrapper::instantiateV8Object(creationContext, &info, impl.get());
    if (UNLIKELY(wrapper.IsEmpty()))
        return wrapper;

    installPerContextProperties(wrapper, impl.get());
    v8::Persistent<v8::Object> wrapperHandle = V8DOMWrapper::createDOMWrapper(impl, &info, wrapper, isolate);
    if (!hasDependentLifetime)
        wrapperHandle.MarkIndependent();
    return wrapper;
}

void V8Float64Array::derefObject(void* object)
{
    static_cast<Float64Array*>(object)->deref();
}

} // namespace WebCore