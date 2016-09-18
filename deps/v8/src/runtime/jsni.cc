// JavaScript Native Interface Release License.
//
// Copyright (c) 2015-2016 Alibaba Group. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Alibaba Group nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//#include "jsni-internal.h"
#include "jsni-convert.h"
#include <assert.h>
//#include "src/v8.h"

#include "src/api.h"
#include "src/v8threads.h"
using namespace v8;
using namespace jsniinternal;

namespace jsniinternal {
/* ----------------------
   Implementation.
----------------------  */

// Internal function declaration.
void JsFunctionWrapper(const FunctionCallbackInfo<Value>& info);
void FakeNativeFunctionCallback(const FunctionCallbackInfo<Value> &info);


// PTR_NUM is the pointer number of struct v8_handle_scope.
#define PTR_NUM 4
struct v8_handle_scope {
  internal::Isolate* isolate_;
  internal::Object** prev_next_;
  internal::Object** prev_limit_;
  // Needed for escape jsvalue.
  internal::Object** escape_slot_;
};

void GetterStub(Local<Name> property,
                       const PropertyCallbackInfo<Value>& info);
void SetterStub(Local<Name> property,
                       Local<Value> value,
                       const PropertyCallbackInfo<void>& info);


// JsRuntime
JsRuntime::JsRuntime(RuntimeRef rt)
    : rt_(rt) {}

JsRuntime JsRuntime::GetCurrent() {
  return JsRuntime(Isolate::GetCurrent());
}


// JsContext
// JsContext::JsContext(v8::Context* context)
JsContext::JsContext(ContextRef context)
    : context_(context) {}

JsContext JsContext::GetCurrentContext(JsRuntime runtime) {
  Isolate* isolate = (Isolate*)runtime.rt_;
  Local<Context> cont = isolate->GetCurrentContext();
  return JsContext(*cont);
}

/*
JsContext JsContext::New(JsRuntime runtime) {
  return JsContext(Context::New(runtime.rt_));
}
*/

// JsValue
JsValue JsValue::Undefined() {
  return ToJSNIValue(v8::Undefined(Isolate::GetCurrent()));
}

JsValue JsValue::Null() {
  return ToJSNIValue(v8::Null(Isolate::GetCurrent()));
}

bool JsValue::IsEmpty() const {
    return val_ == NULL;
}

bool JsValue::IsUndefined() const {
  return ((Value*)val_)->IsUndefined();
}
bool JsValue::IsNull() const {
  return ((Value*)val_)->IsNull();
}
bool JsValue::IsString() const {
  return ((Value*)val_)->IsString();
}
bool JsValue::IsFunction() const {
  return ((Value*)val_)->IsFunction();
}
bool JsValue::IsArray() const {
  return ((Value*)val_)->IsArray();
}
bool JsValue::IsObject() const {
  return ((Value*)val_)->IsObject();
}
bool JsValue::IsNumber() const {
  return ((Value*)val_)->IsNumber();
}
bool JsValue::IsBoolean() const {
  return ((Value*)val_)->IsBoolean();
}
bool JsValue::IsTypedArray() const {
  return ((Value*)val_)->IsTypedArray();
}

JsObject JsValue::AsObject() const {
  // Isolate* isolate = Isolate::GetCurrent();
  // EscapableHandleScope scope(isolate);
  // Will crash if failed.
  if (((Value*)val_)->IsObject()) {
    return JsObject(val_);
  } else {
    assert(0);
  }
}

JsFunction JsValue::AsFunction() const {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  // Will crash if failed.
  if (((Value*)val_)->IsFunction()) {
    return JsFunction(val_);
  } else {
    assert(0);
  }
}

JsString JsValue::AsString() const {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  // Will crash if failed.
  if (((Value*)val_)->IsString()) {
    return JsString(val_);
  } else {
    assert(0);
  }
}

int JsValue::ToInt() const {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  // Will crash if failed.
  return ((Value*)val_)->Int32Value(context).FromJust();
}

double JsValue::ToDouble() const {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  // Will crash if failed.
  return ((Value*)val_)->NumberValue(context).FromJust();
}

bool JsValue::ToBool() const {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  // Will crash if failed.
  return ((Value*)val_)->BooleanValue(context).FromJust();
}

// Add static JsValue::New.
JsValue JsValue::NewInt(int val) {
  return JsValue(val);
}
JsValue JsValue::NewDou(double val) {
  return JsValue(val);
}
JsValue JsValue::NewBoo(bool val) {
  return JsValue(val);
}

JsValue::JsValue(int val) {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Integer> t = Integer::New(isolate, val);
  val_ = *(scope.Escape(t));
}

JsValue::JsValue(double val) {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Number> t = Number::New(isolate, val);
  val_ = *(scope.Escape(t));
}

JsValue::JsValue(bool val) {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Boolean> t = Boolean::New(isolate, val);
  val_ = *(scope.Escape(t));
}

JsValue::JsValue(JsLocal val)
    : val_(val) {}

JsValue::JsValue()
    : val_(0) {
  // Like Local<Value>(), this contruct an empty value.
  // Unlike does nothing in v0.1, because val_ is a type
  // of void*, should be initialized 0 here.
}

// JsNumber

JsNumber JsNumber::NewDou(double val){
  return JsNumber(val);
}
JsNumber JsNumber::NewInt(int32_t val){
  return JsNumber(val);
}
JsNumber JsNumber::NewUint(uint32_t val){
  return JsNumber(val);
}

JsNumber::JsNumber(JsLocal val)
    : JsValue(val) {}

JsNumber::JsNumber(double value) {
  Isolate* isolate = Isolate::GetCurrent();
  val_ = *(Number::New(isolate, value));
}

JsNumber::JsNumber(int32_t value) {
  Isolate* isolate = Isolate::GetCurrent();
  val_ = *(Number::New(isolate, value));
}

JsNumber::JsNumber(uint32_t value) {
  Isolate* isolate = Isolate::GetCurrent();
  val_ = *(Number::New(isolate, value));
}

// JsBoolean
JsBoolean JsBoolean::New(bool value) {
  return JsBoolean(value);
}

JsBoolean::JsBoolean(JsLocal val)
    : JsValue(val) {}

JsBoolean::JsBoolean(bool value) {
  Isolate* isolate = Isolate::GetCurrent();
  val_ = *(Boolean::New(isolate, value));
}

// JsObject
// JsObject::JsObject() {}

JsObject::JsObject(JsLocal val)
    : JsValue(val) {}

JsObject JsObject::New() {
  // New an object.
  Isolate* isolate = Isolate::GetCurrent();
  Local<Value> val = Object::New(isolate);
  return JsObject(*val);
}

// Use New... to avoid the ambiguity of val_ assignment.
JsObject JsObject::NewWithInternalField() {
  // Only support one internal Now.
  int count = 1;
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  // New a object. And set internal count.
  Local<ObjectTemplate> result = ObjectTemplate::New(isolate);
  result->SetInternalFieldCount(count);
  Local<Object> obj =
    result->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
  return JsObject(*(scope.Escape(obj)));
}

bool JsObject::HasInternalField() {
  // return val_.As<Object>()->InternalFieldCount() > 0;
  return Object::Cast((Value*)val_)->InternalFieldCount() > 0;
}

void JsObject::SetInternalField(void* field) {
  // Default 1 internal field.
  int index = 0;
  Isolate* isolate = Isolate::GetCurrent();
  Local<External> ext = External::New(isolate, field);
  Object::Cast((Value*)val_)->SetInternalField(index, ext);
}

void* JsObject::GetInternalField() {
  // Default 1 internal field.
  int index = 0;
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<External> ext = Local<External>::Cast(
    Object::Cast((Value*)val_)->GetInternalField(index));
  void* field = ext->Value();
  return field;
}

bool JsObject::HasProperty(const char* name) {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> obj = ((Value*)val_)->ToObject(context).ToLocalChecked();
  Maybe<bool> t =
    obj->Has(context,
             String::NewFromUtf8(isolate,
                                 name,
                                 NewStringType::kNormal).ToLocalChecked());
  return t.FromJust();
}

JsValue JsObject::GetProperty(const char* name) {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> obj = ((Value*)val_)->ToObject(context).ToLocalChecked();
  MaybeLocal<Value> p =
    obj->Get(context,
            String::NewFromUtf8(isolate,
                                name,
                                NewStringType::kNormal).ToLocalChecked());
  return ToJSNIValue(scope.Escape(p.ToLocalChecked()));
}

bool JsObject::SetProperty(const char* name, JsValue property) {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> obj = ((Value*)val_)->ToObject(context).ToLocalChecked();
  Maybe<bool> t =
    obj->Set(context,
             String::NewFromUtf8(isolate,
                                 name,
                                 NewStringType::kNormal).ToLocalChecked(),
                                 ToV8LocalValue(property));
  return t.FromJust();
}

bool JsObject::DeleteProperty(const char* name) {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> obj = ((Value*)val_)->ToObject(context).ToLocalChecked();
  Maybe<bool> t =
    obj->Delete(context,
                String::NewFromUtf8(isolate,
                                    name,
                                    NewStringType::kNormal).ToLocalChecked());
  return t.FromJust();
}

bool JsObject::SetAccessor(const char* name,
                 GetterCallback getter,
                 SetterCallback setter) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope handle_scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> recv = ((Value*)val_)->ToObject(context).ToLocalChecked();

  Maybe<bool> t =
    recv->SetAccessor(context,
                     (String::NewFromUtf8(isolate,
                              name,
                              NewStringType::kNormal).ToLocalChecked()),
                     GetterStub,
                     SetterStub);

  Local<ObjectTemplate> t_o = ObjectTemplate::New(isolate);
  t_o->SetInternalFieldCount(2);
  Local<Object> fake_obj = t_o->NewInstance(context).ToLocalChecked();
  // fake_obj->SetAlignedPointerInInternalField(0,
  //   reinterpret_cast<void*>(getter));
  // fake_obj->SetAlignedPointerInInternalField(1,
  //   reinterpret_cast<void*>(setter));
  Local<External> ext_get = External::New(isolate, (void*)getter);
  Local<External> ext_set = External::New(isolate, (void*)setter);
  fake_obj->SetInternalField(0, ext_get);
  fake_obj->SetInternalField(1, ext_set);
  // int n = strlen(name);
  // char R_name[n+20] = "_ACCESOR_";
  std::string acc = "_ACCESOR_";
  std::string rname = name;
  acc += rname;
  Local<String> fake_name = String::NewFromUtf8(isolate,
                            acc.c_str(), NewStringType::kNormal).ToLocalChecked();
  Maybe<bool> set = recv->Set(context, fake_name, fake_obj);
  return t.FromJust() && set.FromJust();
}

JsValue JsObject::GetPrototype() {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Object> o = ToV8LocalValue(*this).As<Object>();
  Local<Value> proto = o->GetPrototype();
  return JsValue(*(scope.Escape(proto)));
}

bool JsObject::SetPrototype(JsValue prototype) {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Value> val = ToV8LocalValue(*this);
  Local<Object> o = val.As<Object>();
  return o->SetPrototype(context,
          ToV8LocalValue(prototype)).FromJust();;
}

// JsFunction
void JsFunctionWrapper(const FunctionCallbackInfo<Value>& info) {
  FakeNativeFunctionCallback(info);
}


JsFunction JsFunction::New(NativeFunctionCallback nativeFunc) {
  // return JsFunction(nativeFunc);
  // Create a fack function to keep opaque.
  // Note: V8 and jsni ABI is not the same, so we need the fake function to call
  // the real function.
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  Local<FunctionTemplate> temp =
    FunctionTemplate::New(
      isolate,
      JsFunctionWrapper,
      External::New(isolate, reinterpret_cast<void*>(nativeFunc))
      );
  Local<Function> function = temp->GetFunction(context).ToLocalChecked();
  JsLocal val = *(scope.Escape(function));
  return JsFunction(val);
}
JsFunction::JsFunction(JsLocal val)
    : JsObject(val) {}

JsFunction::JsFunction(NativeFunctionCallback nativeFunc) {
  // TODO(jiny). Use the JsFunction::New above.
  // Note: this construnctor is out of date. Do not use.
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  MaybeLocal<Function> func = Function::New(context,
    (FunctionCallback)nativeFunc);
  val_ = *(scope.Escape(func.ToLocalChecked()));
}

JsValue JsFunction::Call(JsValue recv, int argc,
             JsValue* argv) {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  // Local<Value> args[argc];
  // We do not need new args to pass. Just modify the primary.
  Local<Value>* new_argv = reinterpret_cast<Local<Value>*>(argv);
  for (int i = 0; i < argc; i++) {
    new_argv[i] = ToV8LocalValue(argv[i]);
  }
  MaybeLocal<Value> ret =
    Function::Cast((Value*)val_)->Call(context, ToV8LocalValue(recv),
                                argc, new_argv);
  return ToJSNIValue(scope.Escape(ret.ToLocalChecked()));
}

// JsString
JsString JsString::New(const char* src) {
  return JsString(src);
}
JsString::JsString(JsLocal val)
    : JsValue(val) {}

JsString::JsString(const char* src) {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<String> str = String::NewFromUtf8(isolate, src,
                      NewStringType::kNormal).ToLocalChecked();
  val_ = *(scope.Escape(str));
}

// Get utf8 value.
char* JsString::GetUtf8Chars() {
  if (val_ == nullptr) {
    return nullptr;
  }

  size_t length = JsString::Utf8Length();
  char * str = static_cast<char*> (malloc(length + 1));
  assert(str != nullptr);
  const int flags =
      String::NO_NULL_TERMINATION | String::REPLACE_INVALID_UTF8;
  /*size_t writeLength = */
  String::Cast((Value*)val_)->WriteUtf8(
                      str, length, 0, flags);
  str[length] = '\0';
  return str;
}


void JsString::ReleaseUtf8Chars(char* str) {
  free(str);
}

uint16_t* JsString::GetChars() {
  String* s = (String*)val_;
  if (s == nullptr) {
    return nullptr;
  }
  size_t length = JsString::Length();
  uint16_t* str =
    static_cast<uint16_t*>(malloc(sizeof(uint16_t) * (length + 1)));
  assert(str != nullptr);
  s->Write(str);
  str[length] = 0;
  return str;
}

void JsString::ReleaseChars(uint16_t* str) {
  free(str);
}

// Get length.
size_t JsString::Length() {
  return String::Cast((Value*)val_)->Length();
}

// Get utf8 length.
size_t JsString::Utf8Length() {
  return String::Cast((Value*)val_)->Utf8Length();
}

// Typed Array.
JsTypedArray JsTypedArray::New(JsTypedArrayType type, char* data, size_t length){
  return JsTypedArray(type, data, length);
}
JsTypedArray::JsTypedArray(JsLocal val)
    : JsObject(val) {}

JsTypedArray::JsTypedArray(JsTypedArrayType type,
                           char* data,
                           size_t length) {
  // Only uint8 supported yet.
  if (type != JsArrayTypeUint8) {
    assert(0 && "Only uint8 supported yet.");
  }
  // new an arraybuffer.
  // then new an unit8array.
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);
  Local<ArrayBuffer> abuf =
    ArrayBuffer::New(isolate, data, length,
    ArrayBufferCreationMode::kInternalized);
  Local<Uint8Array> uarr = Uint8Array::New(abuf, 0, length);
  val_ = *(scope.Escape(uarr));
}

JsTypedArrayType JsTypedArray::Type() {
  if (v8::TypedArray::Cast((Value*)val_)->IsUint8Array()) {
    return JsArrayTypeUint8;
  }
  return JsArrayTypeNone;
}

// In typed array, length is number of elements.
size_t JsTypedArray::Length() {
  // Check type.
  return v8::TypedArray::Cast((Value*)val_)->ByteLength();
}

void* JsTypedArray::Data() {
  ArrayBuffer::Contents ab_c =
    v8::TypedArray::Cast((Value*)val_)->Buffer()->GetContents();
  return static_cast<char *>(ab_c.Data())
    + v8::TypedArray::Cast((Value*)val_)->ByteOffset();
}


// JsGlobalRef
// TODO(Jin Yue): All internal reference should be modified
// in v8 internal. Do not use pointer cast.
JsGlobalRef::JsGlobalRef() :val_(0) {}

void JsGlobalRef::Set(JsValue ref) {
  // TODO(jiny).Check ref is null or not.
  Persistent<Value>* p = reinterpret_cast<Persistent<Value>*>(&val_);
  Isolate *isolate = Isolate::GetCurrent();
  p->Reset(isolate,
    *reinterpret_cast<Local<Value>*>(&(ref.val_)));
}

void JsGlobalRef::Clear() {
  // Check empty.
  Persistent<Value>* p = reinterpret_cast<Persistent<Value>*>(&val_);
  p->Reset();
}

bool JsGlobalRef::IsEmpty() const{
  return val_ == NULL;
}

// Set the global reference to empty.
void JsGlobalRef::Empty() {
  val_ = 0;
}

JsValue JsGlobalRef::ToLocal() {
  Isolate *isolate = Isolate::GetCurrent();
  // New a local.
  Persistent<Value>* p = reinterpret_cast<Persistent<Value>*>(&val_);
  // Local<Value> local = p->Get(isolate);
  Local<Value> local = Local<Value>::New(isolate, *p);
  return JsValue(*local);
}

void FakeGCCallback(WeakCallbackInfo<void>& info) {
  Isolate* isolate = info.GetIsolate();
  JSNIEnvExt* env = reinterpret_cast<JSNIEnvExt*>(isolate->GetEnv());
  void** fake_arg = (void**)info.GetParameter();
  GCCallback callback = reinterpret_cast<GCCallback>(fake_arg[1]);

  typedef void (*FakeGCCallbackType)(JSNIEnv* , const WeakCallbackInfo<void>&);
  reinterpret_cast<FakeGCCallbackType>(callback)(env->JSNIEnv_, info);
  free(fake_arg);
  /*
  // Create a new real WeakCallbackInfo.
  void* internal[2];
  internal[0] = info.GetInternalField(0);
  internal[1] = info.GetInternalField(1);
  WeakCallbackInfo<void> new_info(isolate, fake_arg[0],
                         &internal, callback);
  callback(env->JSNIEnv_, new_info);
  */
}

void JsGlobalRef::SetWeakGCCallback(void* args, GCCallback callback) {
  // Use kParameter by default.
  Persistent<Value>* p = reinterpret_cast<Persistent<Value>*>(&val_);
  WeakCallbackType type = WeakCallbackType::kParameter;
  // Use face call back to pass env.
  // Store the callback in th arg.
  // TODO. Be more nice.
  // Or we can use external data pointer.
  void** fake_arg = (void**)malloc(2 * sizeof(void*));
  fake_arg[0] = args;
  fake_arg[1] = (void*) callback;
  p->SetWeak(
  (void*)fake_arg,
  (WeakCallbackInfo<void>::Callback)(void*)FakeGCCallback,
  type);
  /*
  p->SetWeak(
    args,
    (WeakCallbackInfo<void>::Callback)(void*)callback,
    type);
    */
}

// Do not allow copy. Like persistent. Or do we need it?.
// JsGlobalRef& operator=(JsGlobalRef& that) {
//   *(static_cast<Object* volatile*>(0)) = static_cast<Primitive*>(0);
//  return *this;
//}
#if 0
T JsGlobalRef::ToLocal() {
  Isolate *isolate;
  isolate = Isolate::GetCurrent();
  // New a local.
  // TODO(jiny): Use persistent to Local function.
  Local<Value> loc = Local<Value>::New(isolate, p);
  // Use Cast. Should check type.
  // Wrap it to our jsVal.
  // TODO(Jiny): Every JsValue should have Local<Value> constructor.
  T result(loc);
  // This works.
  // JsValue result(loc);
  // Cast to T.
  return result;
  // return * (static_cast <T*>(&result) );
}

void JsGlobalRef::SetWeakGCCallback(void* args, GCCallback callback) {
  // Use kParameter by default.
  WeakCallbackType type = WeakCallbackType::kParameter;
  p.SetWeak(
    args,
    (WeakCallbackInfo<void>::Callback)(void*)callback,
    type);
}
#endif

void JsException::ThrowError(const char* errmsg) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  isolate->ThrowException(v8::Exception::Error(
    v8::String::NewFromUtf8(
      isolate,
      errmsg,
      NewStringType::kNormal).ToLocalChecked()));
}

void JsException::ThrowTypeError(const char* errmsg) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  isolate->ThrowException(v8::Exception::TypeError(
    v8::String::NewFromUtf8(
      isolate,
      errmsg,
      NewStringType::kNormal).ToLocalChecked()));
}

void JsException::ThrowRangeError(const char* errmsg) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  isolate->ThrowException(v8::Exception::RangeError(
    v8::String::NewFromUtf8(
      isolate,
      errmsg,
      NewStringType::kNormal).ToLocalChecked()));
}

// JSNI
uint32_t JSNI::GetVersion() {
  return 0x00010000;
}

// A fake func to call native callback.
void FakeNativeFunctionCallback(const FunctionCallbackInfo<Value> &info) {
  NativeFunctionCallback nativeFunc =
    reinterpret_cast<NativeFunctionCallback>(info.Data().As<External>()->Value());

  JSNIEnvExt* env = reinterpret_cast<JSNIEnvExt*>(info.GetIsolate()->GetEnv());

  typedef void (*FakeCallback)(JSNIEnv* env, const FunctionCallbackInfo<Value> &info);
  // Don't bother here.
  // The real env is member of JSNIEnvExt. TODO. Should env here. Use function pointer
  // to call in jsni.h. Also should be env in node.cc dlopen.
  reinterpret_cast<FakeCallback>(nativeFunc)(env->JSNIEnv_, info);
}

bool JSNI::RegisterMethod(JSNIEnv* env, const JsValue recv,
                           const char* name,
                           NativeFunctionCallback callback) {
  Isolate* isolate = Isolate::GetCurrent();
  Local<Context> ctx = isolate->GetCurrentContext();
  HandleScope handle_scope(isolate);

#if 0
  Local<FunctionTemplate> t =
    FunctionTemplate::New(isolate,
                              (FunctionCallback)((void *)callback));
#endif

  // Create a facke function to keep opaque.
  Local<FunctionTemplate> t =
    FunctionTemplate::New(
      isolate,
      FakeNativeFunctionCallback,
      External::New(isolate, reinterpret_cast<void*>(callback))
      );

  Local<Function> fn = t->GetFunction(ctx).ToLocalChecked();
  Local<String> fn_name = v8::String::NewFromUtf8(isolate, name,
                                  NewStringType::kNormal).ToLocalChecked();
  fn->SetName(fn_name);

  return (ToV8LocalValue(recv))->ToObject(ctx).ToLocalChecked()
                               ->Set(ctx, fn_name, fn).FromJust();
}
/*
bool JSNI::RegisterMethod2(const void* recv,
                           const char* name,
                           NativeFunctionCallback callback) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::Local<v8::Context> ctx = isolate->GetCurrentContext();
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::FunctionTemplate> t =
    v8::FunctionTemplate::New(isolate,
                              (v8::FunctionCallback)((void *)callback));
  v8::Local<v8::Function> fn = t->GetFunction();
  v8::Local<v8::String> fn_name = v8::String::NewFromUtf8(isolate, name);
  fn->SetName(fn_name);

  (*reinterpret_cast<v8::Local<v8::Value>*>((void*)recv))
      ->ToObject(ctx).ToLocalChecked()->Set(fn_name, fn);
}
*/
void GetterStub(Local<Name> property,
                      const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> recv = info.This();
  Local<Object> holder = info.Holder();
  Local<Value> data = info.Data();
  void *values[PropertyInfo::kArgsLength];
  JsValue ret;

  values[PropertyInfo::kHolderIndex] = &holder;
  values[PropertyInfo::kRuntimeIndex] = isolate;
  // values[PropertyInfo::kThisIndex] = *recv;
  values[PropertyInfo::kThisIndex] = &recv;
  values[PropertyInfo::kReturnValueIndex] = &ret;
  values[PropertyInfo::kDataIndex] = &data;
  PropertyInfo args = PropertyInfo(reinterpret_cast<void **>(values));

  Local<String> f_name = property->ToString(context).ToLocalChecked();
  JsValue val_name = ToJSNIValue(f_name);
  Local<String> prefix =
    String::NewFromUtf8(isolate,
      "_ACCESOR_",
      NewStringType::kNormal).ToLocalChecked();
  Local<String> fake_name = String::Concat(prefix, f_name);
  Local<Object> fake_obj =
    recv->Get(context, fake_name).ToLocalChecked()
        ->ToObject(context).ToLocalChecked();
  void (*getter_func)(JSNIEnv* env, JsValue *name, const PropertyInfo& info);
  // func = reinterpret_cast<GetterCallback>(
  //   fake_obj->GetAlignedPointerFromInternalField(0));
  Local<External> ext = Local<External>::Cast(
    fake_obj->GetInternalField(0));
  getter_func = reinterpret_cast<GetterCallback>(
    ext->Value());

  // Pass env to get func.
  JSNIEnvExt* env = reinterpret_cast<JSNIEnvExt*>(isolate->GetEnv());
  (*getter_func)(env->JSNIEnv_, &val_name, args);
  info.GetReturnValue().Set(ToV8LocalValue(ret));
}

void SetterStub(Local<Name> property,
                      Local<Value> value,
                      const PropertyCallbackInfo<void>& info) {
  Isolate* isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> recv = info.This();
  Local<Object> holder = info.Holder();
  Local<Value> data = info.Data();
  void *values[PropertyInfo::kArgsLength];
  JsValue ret;

  values[PropertyInfo::kHolderIndex] = &holder;
  values[PropertyInfo::kRuntimeIndex] = isolate;
  values[PropertyInfo::kThisIndex] = &recv;
  values[PropertyInfo::kReturnValueIndex] = &ret;
  values[PropertyInfo::kDataIndex] = &data;

  PropertyInfo args = PropertyInfo(reinterpret_cast<void **>(values));

  JsValue val = ToJSNIValue(value);
  Local<String> f_name = property->ToString(context).ToLocalChecked();
  JsValue val_name = ToJSNIValue(f_name);
  Local<String> prefix =
    String::NewFromUtf8(isolate,
            "_ACCESOR_",
            NewStringType::kNormal).ToLocalChecked();
  Local<String> fake_name = String::Concat(prefix, f_name);
  Local<Object> fake_obj =
    recv->Get(context, fake_name).ToLocalChecked()
    ->ToObject(context).ToLocalChecked();
  // void (*func)(JsValue *name, JsValue val, const PropertyInfo& info);
  // func = reinterpret_cast<SetterCallback>(
  //   fake_obj->GetAlignedPointerFromInternalField(1));
  // (*func)(&val_name, val, args);
  void (*setter_func)(JSNIEnv* env, JsValue *name, JsValue val, const PropertyInfo& info);
  Local<External> ext = Local<External>::Cast(
    fake_obj->GetInternalField(1));
  setter_func = reinterpret_cast<SetterCallback>(
    ext->Value());
  // Pass env to get func.
  JSNIEnvExt* env = reinterpret_cast<JSNIEnvExt*>(isolate->GetEnv());
  (*setter_func)(env->JSNIEnv_, &val_name, val, args);
}


JsRuntime GCCallbackInfo::GetRuntime() const {
  WeakCallbackInfo<void>* info =
    reinterpret_cast<WeakCallbackInfo<void>*>((void*)this);
  return JsRuntime(info->GetIsolate());
}

void* GCCallbackInfo::GetInfo() const {
  WeakCallbackInfo<void>* info =
    reinterpret_cast<WeakCallbackInfo<void>*>((void*)this);
  // It is fake arg. Get the real
  void** fake_arg = (void**) info->GetParameter();
  return fake_arg[0];
}

JSNIEnv* GCCallbackInfo::Env() const {
  WeakCallbackInfo<void>* info =
    reinterpret_cast<WeakCallbackInfo<void>*>((void*)this);
  Isolate* isolate = info->GetIsolate();
  JSNIEnvExt* env = reinterpret_cast<JSNIEnvExt*>(isolate->GetEnv());
  return env->JSNIEnv_;
}

int NativeCallbackInfo::Length() const {
  FunctionCallbackInfo<Value>* args =
    reinterpret_cast<FunctionCallbackInfo<Value>*>((void*)this);
  return args->Length();
}

JsValue NativeCallbackInfo::Get(int i) const {
  FunctionCallbackInfo<Value>* args =
    reinterpret_cast<FunctionCallbackInfo<Value>*>((void*)this);
  return ToJSNIValue((*args)[i]);
}

JsValue NativeCallbackInfo::operator[](int i) const {
  FunctionCallbackInfo<Value>* args =
    reinterpret_cast<FunctionCallbackInfo<Value>*>((void*)this);
  return ToJSNIValue((*args)[i]);
}

JsFunction NativeCallbackInfo::Callee() const {
  FunctionCallbackInfo<Value>* args =
    reinterpret_cast<FunctionCallbackInfo<Value>*>((void*)this);
  return ToJSNIValue(args->Callee()).AsFunction();
}

JsObject NativeCallbackInfo::This() const {
  FunctionCallbackInfo<Value>* args =
    reinterpret_cast<FunctionCallbackInfo<Value>*>((void*)this);
  return ToJSNIValue(args->This()).AsObject();
}

JsObject NativeCallbackInfo::Holder() const {
  FunctionCallbackInfo<Value>* args =
    reinterpret_cast<FunctionCallbackInfo<Value>*>((void*)this);
  return ToJSNIValue(args->Holder()).AsObject();
}

bool NativeCallbackInfo::IsConstructCall() const {
  FunctionCallbackInfo<Value>* args =
    reinterpret_cast<FunctionCallbackInfo<Value>*>((void*)this);
  return args->IsConstructCall();
}

JsValue NativeCallbackInfo::Data() const {
  FunctionCallbackInfo<Value>* args =
    reinterpret_cast<FunctionCallbackInfo<Value>*>((void*)this);
  return ToJSNIValue(args->Data());
}

JsRuntime NativeCallbackInfo::GetRuntime() const {
  FunctionCallbackInfo<Value>* args =
    reinterpret_cast<FunctionCallbackInfo<Value>*>((void*)this);
  return JsRuntime(args->GetIsolate());
}

void NativeCallbackInfo::SetReturnValue(JsValue ret) const {
  FunctionCallbackInfo<Value>* args =
    reinterpret_cast<FunctionCallbackInfo<Value>*>((void*)this);
  args->GetReturnValue().Set((ToV8LocalValue(ret)));
}


JsObject PropertyInfo::This() const {
  // values store pointer already, do not get the address again.
  return *(reinterpret_cast<JsObject*>(values_[kThisIndex]));
  // return JsObject((JsLocal)values_[kThisIndex]);
}


JsObject PropertyInfo::Holder() const {
  return *(reinterpret_cast<JsObject*>(values_[kHolderIndex]));
}


JsValue PropertyInfo::Data() const {
  return *(reinterpret_cast<JsValue*>(values_[kDataIndex]));
}


JsRuntime PropertyInfo::GetRuntime() const {
  return JsRuntime(values_[kRuntimeIndex]);
}

JSNIEnv* PropertyInfo::Env() const {
  Isolate* isolate = (Isolate*)values_[kRuntimeIndex];
  JSNIEnvExt* env = reinterpret_cast<JSNIEnvExt*>(isolate->GetEnv());
  return env->JSNIEnv_;
}

void PropertyInfo::SetReturnValue(JsValue ret) const {
  *(reinterpret_cast<JsValue*>(values_[kReturnValueIndex])) = ret;
}


// LocalRefScope
void LocalRefScope::New(JsRuntime runtime) {
  // Do not use constructor to call.
  // It will deconstructor automaticly.
  // return LocalRefScope(runtime);
  // assert malloc success.
  v8_handle_scope * save_scope =
    static_cast<v8_handle_scope *>(malloc(PTR_NUM * sizeof(void*)));
  scope_ = save_scope;
  // assert runtime is not null.
  Isolate* isolate = (Isolate*)runtime.rt_;
  i::Isolate* internal_isolate = reinterpret_cast<i::Isolate*>(isolate);

  // Create an escape handle first. So we have a slot to escape before scope initialize.
  // Needed for escaping jsvalue.
  i::Object* value =
    reinterpret_cast<i::Object*>(internal_isolate->heap()->the_hole_value());
  save_scope->escape_slot_ = i::HandleScope::CreateHandle(
                             internal_isolate,
                             value);

  // We do not want to check the correct usage of the Locker class all over the
  // place, so we do it only here: Without a HandleScope, an embedder can do
  // almost nothing, so it is enough to check in this central place.
  // We make an exception if the serializer is enabled, which means that the
  // Isolate is exclusively used to create a snapshot.
  Utils::ApiCheck(
      !v8::Locker::IsActive() ||
          internal_isolate->thread_manager()->IsLockedByCurrentThread() ||
          internal_isolate->serializer_enabled(),
      "HandleScope::HandleScope",
      "Entering the V8 API without proper locking in place");

  i::HandleScopeData* current = internal_isolate->handle_scope_data();
  save_scope->isolate_ = internal_isolate;
  save_scope->prev_next_ = current->next;
  save_scope->prev_limit_ = current->limit;

  current->level++;
}

void LocalRefScope::Deconstruct() {
  v8_handle_scope * save_scope =
    static_cast<v8_handle_scope *>(scope_);

  internal::Isolate* isolate = save_scope->isolate_;
  internal::Object** prev_next =  save_scope->prev_next_;
  internal::Object** prev_limit = save_scope->prev_limit_;

  i::HandleScopeData* current = isolate->handle_scope_data();


  std::swap(current->next, prev_next);
  current->level--;
  if (current->limit != prev_limit) {
    current->limit = prev_limit;
    i::HandleScope::DeleteExtensions(isolate);
#if 0
#ifdef ENABLE_HANDLE_ZAPPING
    ZapRange(current->next, prev_limit);
  } else {
    ZapRange(current->next, prev_next);
#endif
#endif
  }

  free(scope_);
}

LocalRefScope::LocalRefScope(JsRuntime runtime) {
  // TODO(Jiny). Use LocalRefScope::New(JsRuntime runtime).
  // assert malloc success.
  v8_handle_scope * save_scope =
    static_cast<v8_handle_scope *>(malloc(PTR_NUM * sizeof(void*)));
  scope_ = save_scope;
  // assert runtime is not null.
  Isolate* isolate = (Isolate*)runtime.rt_;
  i::Isolate* internal_isolate = reinterpret_cast<i::Isolate*>(isolate);

  // Create an escape handle first. So we have a slot to escape before scope initialize.
  // Needed for escaping jsvalue.
  i::Object* value =
    reinterpret_cast<i::Object*>(internal_isolate->heap()->the_hole_value());
  save_scope->escape_slot_ = i::HandleScope::CreateHandle(
                             internal_isolate,
                             value);

  // We do not want to check the correct usage of the Locker class all over the
  // place, so we do it only here: Without a HandleScope, an embedder can do
  // almost nothing, so it is enough to check in this central place.
  // We make an exception if the serializer is enabled, which means that the
  // Isolate is exclusively used to create a snapshot.
  Utils::ApiCheck(
      !v8::Locker::IsActive() ||
          internal_isolate->thread_manager()->IsLockedByCurrentThread() ||
          internal_isolate->serializer_enabled(),
      "HandleScope::HandleScope",
      "Entering the V8 API without proper locking in place");

  i::HandleScopeData* current = internal_isolate->handle_scope_data();
  save_scope->isolate_ = internal_isolate;
  save_scope->prev_next_ = current->next;
  save_scope->prev_limit_ = current->limit;

  current->level++;
}

LocalRefScope::~LocalRefScope() {
  v8_handle_scope * save_scope =
    static_cast<v8_handle_scope *>(scope_);

  internal::Isolate* isolate = save_scope->isolate_;
  internal::Object** prev_next =  save_scope->prev_next_;
  internal::Object** prev_limit = save_scope->prev_limit_;

  i::HandleScopeData* current = isolate->handle_scope_data();


  std::swap(current->next, prev_next);
  current->level--;
  if (current->limit != prev_limit) {
    current->limit = prev_limit;
    i::HandleScope::DeleteExtensions(isolate);
#if 0
#ifdef ENABLE_HANDLE_ZAPPING
    ZapRange(current->next, prev_limit);
  } else {
    ZapRange(current->next, prev_next);
#endif
#endif
  }

  free(scope_);
}

// Save a JsValue when LocalRefScope is deconstructed.
JsValue LocalRefScope::SaveRef(JsValue val) {
  v8_handle_scope * save_scope =
    static_cast<v8_handle_scope *>(scope_);
  i::Object** escape_value = reinterpret_cast<i::Object**>(val.val_);

  i::Heap* heap = reinterpret_cast<i::Isolate*>(save_scope->isolate_)->heap();
  Utils::ApiCheck(*(save_scope->escape_slot_) == heap->the_hole_value(),
                  "EscapeableHandleScope::Escape",
                  "Escape value set twice");
  if (escape_value == NULL) {
    *(save_scope->escape_slot_) = heap->undefined_value();
    return JsValue(nullptr);
  }
  *(save_scope->escape_slot_) = *escape_value;
  return JsValue((save_scope->escape_slot_));
}
/*
// Return 0 if success.
int LocalRefScope::PushLocalScope(JsRuntime runtime) {
  // assert malloc success.
  v8_handle_scope * save_scope =
    static_cast<v8_handle_scope *>(malloc(PTR_NUM * sizeof(void*)));
  scope_ = save_scope;
  // assert runtime is not null.
  Isolate* isolate = (Isolate*)runtime.rt_;
  i::Isolate* internal_isolate = reinterpret_cast<i::Isolate*>(isolate);
  // We do not want to check the correct usage of the Locker class all over the
  // place, so we do it only here: Without a HandleScope, an embedder can do
  // almost nothing, so it is enough to check in this central place.
  // We make an exception if the serializer is enabled, which means that the
  // Isolate is exclusively used to create a snapshot.
  Utils::ApiCheck(
      !v8::Locker::IsActive() ||
          internal_isolate->thread_manager()->IsLockedByCurrentThread() ||
          internal_isolate->serializer_enabled(),
      "HandleScope::HandleScope",
      "Entering the V8 API without proper locking in place");

  i::HandleScopeData* current = internal_isolate->handle_scope_data();
  save_scope->isolate_ = internal_isolate;
  save_scope->prev_next_ = current->next;
  save_scope->prev_limit_ = current->limit;
  // Needed for escaping jsvalue.
  save_scope->escape_slot_ = i::HandleScope::CreateHandle(
                             isolate,
                             isolate->heap()->the_hole_value());
  current->level++;
}

// Return a escope JsValue for the given val.
JsValue LocalRefScope::PopLocalScope(JsValue val) {
  v8_handle_scope * save_scope =
    static_cast<v8_handle_scope *>(scope_);

  internal::Isolate* isolate = save_scope->isolate_;
  internal::Object** prev_next =  save_scope->prev_next_;
  internal::Object** prev_limit = save_scope->prev_limit_;

  i::HandleScopeData* current = isolate->handle_scope_data();

  std::swap(current->next, prev_next);
  current->level--;
  if (current->limit != prev_limit) {
    current->limit = prev_limit;
    i::HandleScope::DeleteExtensions(isolate);
#if 0
#ifdef ENABLE_HANDLE_ZAPPING
    ZapRange(current->next, prev_limit);
  } else {
    ZapRange(current->next, prev_next);
#endif
#endif
  }

  // Save jsvalue.
  JsValue saved = SaveRef(val);

  free(scope_);

  return saved;
}
*/

JSNINativeInterface gJSNINativeInterface = {
  JSNI::GetVersion,
  JSNI::RegisterMethod,
  &JsValue::Undefined,
  &JsValue::Null,
  &JsValue::IsEmpty,
  &JsValue::IsUndefined,
  &JsValue::IsNull,
  &JsValue::IsString,
  &JsValue::IsFunction,
  &JsValue::IsArray,
  &JsValue::IsObject,
  &JsValue::IsNumber,
  &JsValue::IsBoolean,
  &JsValue::IsTypedArray,
  &JsValue::ToInt,
  &JsValue::ToDouble,
  &JsValue::ToBool,
  &JsValue::AsObject,
  &JsValue::AsFunction,
  &JsValue::AsString,
  &JsValue::NewInt,
  &JsValue::NewDou,
  &JsValue::NewBoo,

    // JsNumber
  &JsNumber::NewDou,
  &JsNumber::NewInt,
  &JsNumber::NewUint,

    // JsBoolean
  &JsBoolean::New,

    // JsObject
  &JsObject::New,
  &JsObject::NewWithInternalField,
  &JsObject::HasInternalField,
  &JsObject::SetInternalField,
  &JsObject::GetInternalField,
  &JsObject::HasProperty,
  &JsObject::GetProperty,
  &JsObject::SetProperty,
  &JsObject::DeleteProperty,
  &JsObject::SetAccessor,
  &JsObject::GetPrototype,
  &JsObject::SetPrototype,

    // JsFunction
  &JsFunction::New,
  &JsFunction::Call,

  // JsString
  &JsString::New,
  &JsString::GetUtf8Chars,
  &JsString::ReleaseUtf8Chars,
  &JsString::GetChars,
  &JsString::ReleaseChars,
  &JsString::Length,
  &JsString::Utf8Length,

  // JsTypedArray
  &JsTypedArray::New,
  &JsTypedArray::Type,
  &JsTypedArray::Length,
  &JsTypedArray::Data,

  // JsGlobalRef
  &JsGlobalRef::Set,
  &JsGlobalRef::Clear,
  &JsGlobalRef::IsEmpty,
  &JsGlobalRef::Empty,
  &JsGlobalRef::ToLocal,
  // For &JsGlobalRef::SetGCCallback,
  nullptr,
  &JsGlobalRef::SetWeakGCCallback,

  // LocalRefScope
  &LocalRefScope::New,
  &LocalRefScope::Deconstruct,
  &LocalRefScope::SaveRef,

  // JsException
  &JsException::ThrowError,
  &JsException::ThrowTypeError,
  &JsException::ThrowRangeError,


  // NativeCallbackInfo
  &NativeCallbackInfo::Length,
  &NativeCallbackInfo::Get,
  &NativeCallbackInfo::Callee,
  &NativeCallbackInfo::This,
  &NativeCallbackInfo::Holder,
  &NativeCallbackInfo::IsConstructCall,
  &NativeCallbackInfo::Data,
  &NativeCallbackInfo::GetRuntime,
  &NativeCallbackInfo::SetReturnValue,

  // GCCallbackInfo
  &GCCallbackInfo::GetRuntime,
  &GCCallbackInfo::GetInfo,
  &GCCallbackInfo::Env,

  // PropertyInfo
  &PropertyInfo::This,
  &PropertyInfo::Holder,
  &PropertyInfo::Data,
  &PropertyInfo::GetRuntime,
  &PropertyInfo::Env,
  &PropertyInfo::SetReturnValue,

  // JsRuntime
  JsRuntime::GetCurrent,

  // JsContext
  JsContext::GetCurrentContext,
};

JSNIEnvExt::JSNIEnvExt(void* isolate) {
  #if 0
  printf("JSNIEnvExt:: gJSNINativeInterface ptr is %p\n", &gJSNINativeInterface);
  printf("JSNIEnvExt:: JSNI::GetVersion ptr is %p\n", JSNI::GetVersion);
  #endif
  JSNIEnv_ = &gJSNINativeInterface;
  isolate_ = isolate;
}

}  // namespace jsni
