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

#ifndef _JSNIINTERNAL_H
#define _JSNIINTERNAL_H
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define DEBUGJSNI 1

namespace jsniinternal {

typedef void* JsLocal;
typedef void* JsGlobal;
typedef void* ContextRef;
typedef void* RuntimeRef;
typedef void* JsScope;
typedef struct JSNINativeInterface JSNIEnv;

class JsValue;
class JsNumber;
class JsBoolean;
class JsObject;
class JsFunction;
class JsString;
class JsTypedArray;

class JsRuntime;
class JsContext;
class LocalRefScope;



class NativeCallbackInfo {
 public:
  // Arguments number.
  int Length() const;

  // Get the arguments.
  JsValue operator[](int i) const;
  // TT
  JsValue Get(int i) const;

  // Get the callee function.
  JsFunction Callee() const;

  // Get this object.
  JsObject This() const;
  JsObject Holder() const;
  bool IsConstructCall() const;
  JsValue Data() const;

  // Get runtime.
  JsRuntime GetRuntime() const;

  // Set return value.
  void SetReturnValue(JsValue ret) const;
};


class GCCallbackInfo {
 public:
  // Get the runtime.
  JsRuntime GetRuntime() const;

  // Get the pointer be set in Set(Weak)GCCallback.
  void* GetInfo() const;

  // Get the env.
  JSNIEnv* Env() const;
};

class PropertyInfo {
 public:
  JsObject This() const;
  JsObject Holder() const;
  JsValue Data() const;
  JsRuntime GetRuntime() const;

  // Get the env.
  JSNIEnv* Env() const;

  void SetReturnValue(JsValue ret) const;
  // This shouldn't be public, but the arm compiler needs it.
  static const int kArgsLength = 5;

  static const int kHolderIndex = 0;
  static const int kRuntimeIndex = 1;
  static const int kThisIndex = 2;
  static const int kReturnValueIndex = 3;
  static const int kDataIndex = 4;

  explicit PropertyInfo(void** values) : values_(values) {}
  void** values_;
};

typedef void (*GCCallback)(JSNIEnv* env, const GCCallbackInfo& info);
// Use env.
typedef void (*NativeFunctionCallback)(JSNIEnv* env, const NativeCallbackInfo& info);
typedef void (*GetterCallback)(JSNIEnv* env, JsValue *name, const PropertyInfo& info);
typedef void (*SetterCallback)(JSNIEnv* env, JsValue *name, JsValue val,
             const PropertyInfo& info);


class JsRuntime {
 public:
  explicit JsRuntime(RuntimeRef rt);

  // New a runtime.
  // Not Implemented.
  static JsRuntime New();

  // Dispose the runtime.
  // Not Implemented.
  void Dispose();

  static JsRuntime GetCurrent();

 private:
  RuntimeRef rt_;
  friend class JsContext;
  friend class LocalRefScope;
};

class JsContext {
 public:
  explicit JsContext(ContextRef context);

  static JsContext GetCurrentContext(JsRuntime runtime);

  ContextRef Get() const { return context_; }

  // Not need new yet.
  // static JsContext New(JsRuntime runtime);

 private:
  ContextRef context_;
};

// --- Static functions.
class JSNI {
 public:
  // Get the version of JSNI.
  static uint32_t GetVersion();

  // TODO. Should use 2 here.
  // Register native method.
  static bool RegisterMethod(JSNIEnv* env,
                             const JsValue recv,
                             const char* name,
                             NativeFunctionCallback callback);
/*
  // Register native method.
  static bool RegisterMethod2(const void* recv,
                           const char* name,
                           NativeFunctionCallback callback);
*/
  // This function is exported by loadable shared libs. Just declare here.
  // TODO(jiny): Maybe we should use void* instead of JsValue or JsObject.
  static int JSNI_Init(JSNIEnv* env, const JsValue recv);

  // This function is exported by loadable shared libs. Just declare here.
  // Not implemented.
  static int JSNI_UnInit(const JsValue recv, void* reserved);
};

// The superclass of all JavaScript values and objects.
class JsValue {
 public:
  static JsValue Undefined();
  static JsValue Null();

  bool IsEmpty() const;

  // Returns true if this value is the undefined value.
  bool IsUndefined() const;

  // Returns true if this value is the null value.
  bool IsNull() const;

  // Returns true if this value is a string.
  bool IsString() const;

  // Returns true if this value is a function.
  bool IsFunction() const;

  // Returns true if this value is an array.
  bool IsArray() const;

  // Returns true if this value is a object.
  bool IsObject() const;

  // Returns true if this value is a number.
  bool IsNumber() const;

  // Returns true if this value is a boolean.
  bool IsBoolean() const;

  // Returns true if this value is a TypedArray.
  bool IsTypedArray() const;

  // These are quick path for primitive convertion.
  // Convert JsValue to native int.
  int ToInt() const;

  // Convert JsValue to native double.
  double ToDouble() const;

  // Convert JsValue to native bool.
  bool ToBool() const;

  // Cast as Object.
  JsObject AsObject() const;

  // Cast as Function.
  JsFunction AsFunction() const;

  // Cast as String.
  JsString AsString() const;

  // Add static JsValue::New.
  static JsValue NewInt(int val);
  static JsValue NewDou(double val);
  static JsValue NewBoo(bool val);

  // TODO. Delete these constructors in the future.
  // Now we still use them in the internal.cc.
  explicit JsValue(int val);

  explicit JsValue(double val);

  explicit JsValue(bool val);

  JsValue();

  JsLocal Get() { return val_; }

 private:
  JsLocal val_;
  explicit JsValue(JsLocal val);
  friend class JsGlobalRef;
  friend class JsNumber;
  friend class JsBoolean;
  friend class JsObject;
  friend class JsFunction;
  friend class JsString;
  friend class JsTypedArray;
  friend class LocalRefScope;
};

class JsNumber : public JsValue {
 public:
  JsNumber() {}
  explicit JsNumber(double value);
  explicit JsNumber(int32_t value);
  explicit JsNumber(uint32_t value);
  static JsNumber NewDou(double val);
  static JsNumber NewInt(int32_t val);
  static JsNumber NewUint(uint32_t val);
 private:
  explicit JsNumber(JsLocal val);
  friend class JsGlobalRef;
};


class JsBoolean : public JsValue {
 public:
  JsBoolean() {}
  explicit JsBoolean(bool value);
  static JsBoolean New(bool value);
 private:
  explicit JsBoolean(JsLocal val);
  friend class JsGlobalRef;
};


class JsObject : public JsValue {
 public:
  JsObject() {}

  // Create an object.
  // Use New to Create an Object.
  static JsObject New();

  // Create an object with an internal field.
  // Use New... to avoid the ambiguity of val_ assignment.
  static JsObject NewWithInternalField();

  // Returns true if this object has an internal field.
  bool HasInternalField();

  // Set a pointer field for this object.
  void SetInternalField(void* field);

  // Get the pointer field in this object.
  void* GetInternalField();

  // Return true if this object has the property name.
  bool HasProperty(const char* name);

  // Get the property.
  JsValue GetProperty(const char* name);

  // Set the property.
  bool SetProperty(const char* name, JsValue property);

  // Delete the property.
  bool DeleteProperty(const char* name);

  // Set the getter and setter function call back for
  // the object.
  bool SetAccessor(const char* name,
                   GetterCallback getter,
                   SetterCallback setter = 0);

  // Get the object's prototype.
  JsValue GetPrototype();

  // Set the object's prototype.
  bool SetPrototype(JsValue prototype);

 private:
  explicit JsObject(JsLocal val);
  friend class JsValue;
  friend class JsGlobalRef;
  friend class JsFunction;
  friend class JsTypedArray;
};

// JsObject has a constructor. So use JsValue?
class JsFunction : public JsObject {
 public:
  JsFunction() {}

  static JsFunction New(NativeFunctionCallback nativeFunc);
  explicit JsFunction(NativeFunctionCallback nativeFunc);

  // Call the function.
  // argc is the argument number, and
  // argv is the argument array or the pointer to the
  // first argument.
  JsValue Call(JsValue recv, int argc,
               JsValue* argv);
 private:
  explicit JsFunction(JsLocal val);
  friend class JsValue;
  friend class JsGlobalRef;
};

typedef enum _JsTypedArrayType {
  // Not a JsArrayType
  JsArrayTypeNone,
  // An int8 array.
  JsArrayTypeInt8,
  // An uint8 array.
  JsArrayTypeUint8,
  // An uint8 clamped array.
  JsArrayTypeUint8Clamped,
  // An int16 array.
  JsArrayTypeInt16,
  // An uint16 array.
  JsArrayTypeUint16,
  // An int32 array.
  JsArrayTypeInt32,
  // An uint32 array.
  JsArrayTypeUint32,
  // A float32 array.
  JsArrayTypeFloat32,
  // A float64 array.
  JsArrayTypeFloat64
} JsTypedArrayType;

class JsString : public JsValue {
 public:
  JsString() {}

  static JsString New(const char* src);
  explicit JsString(const char* src);

  // Get utf8 value. The chars are made copied.
  // Must call ReleaseUtf8Chars to free space.
  char* GetUtf8Chars();

  // Release the utf8 string.
  static void ReleaseUtf8Chars(char* str);

  // Get 16-bits string. The chars are made copied.
  // Must call ReleaseChars to free space.
  uint16_t* GetChars();

  // Release 16-bits string.
  static void ReleaseChars(uint16_t* str);

  // Get 16-bits string length.
  size_t Length();

  // Get utf8 length.
  size_t Utf8Length();

 private:
  explicit JsString(JsLocal val);
  friend class JsGlobalRef;
  friend class JsValue;
};


// Only implement the uint8array.
// TODO(jiny): implement others.
class JsTypedArray : public JsObject {
 public:
  JsTypedArray() {}

  // Only uint8 array implemented now.
  static JsTypedArray New(JsTypedArrayType type, char* data, size_t length);
  JsTypedArray(JsTypedArrayType type, char* data, size_t length);

  // Return the specific type of TypedArray.
  JsTypedArrayType Type();

  // Get number of elements.
  size_t Length();

  // Get the pointer of the TypedArray.
  void *Data();
 private:
  explicit JsTypedArray(JsLocal val);
  friend class JsValue;
  friend class JsGlobalRef;
};


class JsGlobalRef {
 public:
  JsGlobalRef();

  // Set a value to global reference.
  void Set(const JsValue val);

  // Clear the global reference pointed to the value.
  void Clear();

  // Return true if the global reference is empty.
  bool IsEmpty() const;

  // Set the global reference to empty.
  void Empty();

  // Convert the global reference to a local value.
  JsValue ToLocal();

  // Not Implemented for now. Use SetWeakGCCallback.
  void SetGCCallback(void* args, GCCallback callback);

  // Set a callback when the global object is garbage collected.
  // Note: the time of gc is not guaranteed.
  void SetWeakGCCallback(void* args, GCCallback callback);

 private:
  JsGlobal val_;
};

// Note: It will create a handle internal.
class LocalRefScope {
 public:
  // No static New.
  void New(JsRuntime runtime);
  void Deconstruct();
  LocalRefScope(JsRuntime runtime);
  ~LocalRefScope();
  JsValue SaveRef(JsValue val);

  // Return 0 if success.
  // Not implemented.
  // TODO(Jiny). How to store scope_ in static methods?
  static int PushLocalScope(JsRuntime runtime);
  // Return a escope JsValue for the given val.
  // Not implemented.
  static JsValue PopLocalScope(JsValue val);
 private:
  // scope_ contains the information of local ref.
  JsScope scope_;
  // Should be different between jsni.h. not use jsni_env;
  JSNIEnv* jsni_env_;
};


// Exception.
class JsException {
 public:
  // Throw an exception for an error.
  static void ThrowError(const char* errmsg);

  // Throw an exception for a type error.
  static void ThrowTypeError(const char* errmsg);

  // Throw an exception for a range error.
  static void ThrowRangeError(const char* errmsg);
};

// Copy jsni.h JSNINativeInterface declare here.
// Table of interface method pointers.
struct JSNINativeInterface {
  //...reserved..
  uint32_t (*GetVersion)();
  bool (*RegisterMethod)(JSNIEnv*, const JsValue,
                         const char*,
                         NativeFunctionCallback);

  // JsValue
  JsValue (*Undefined)();
  JsValue (*Null)();
  bool (JsValue::*IsEmpty)() const;
  bool (JsValue::*IsUndefined)() const;
  bool (JsValue::*IsNull)() const;
  bool (JsValue::*IsString)() const;
  bool (JsValue::*IsFunction)() const;
  bool (JsValue::*IsArray)() const;
  bool (JsValue::*IsObject)() const;
  bool (JsValue::*IsNumber)() const;
  bool (JsValue::*IsBoolean)() const;
  bool (JsValue::*IsTypedArray)() const;
  int (JsValue::*ToInt)() const;
  double (JsValue::*ToDouble)() const;
  bool (JsValue::*ToBool)() const;
  JsObject (JsValue::*AsObject)() const;
  JsFunction (JsValue::*AsFunction)() const;
  JsString (JsValue::*AsString)() const;
  JsValue (*JsValueNewInt)(int);
  JsValue (*JsValueNewDou)(double);
  JsValue (*JsValueNewBoo)(bool);

  // JsNumber
  JsNumber (*JsNumberNewDou)(double);
  JsNumber (*JsNumberNewInt)(int32_t);
  JsNumber (*JsNumberNewUint)(uint32_t);

  // JsBoolean
  JsBoolean (*JsBooleanNew)(bool value);

  // JsObject
  JsObject (*JsObjectNew)();
  JsObject (*JsObjectNewWithInternalField)();
  bool (JsObject::*HasInternalField)();
  void (JsObject::*SetInternalField)(void*);
  void* (JsObject::*GetInternalField)();
  bool (JsObject::*HasProperty)(const char*);
  JsValue (JsObject::*GetProperty)(const char*);
  bool (JsObject::*SetProperty)(const char*, JsValue);
  bool (JsObject::*DeleteProperty)(const char*);
  bool (JsObject::*SetAccessor)(const char*,
                   GetterCallback,
                   SetterCallback);
  JsValue (JsObject::*GetPrototype)();
  bool (JsObject::*SetPrototype)(JsValue);

  // JsFunction
  JsFunction (*JsFunctionNew)(NativeFunctionCallback);
  JsValue (JsFunction::*Call)(JsValue, int, JsValue*);

  // JsString
  JsString (*JsStringNew)(const char*); // static
  char* (JsString::*GetUtf8Chars)();
  void (*ReleaseUtf8Chars)(char*); // static
  uint16_t* (JsString::*GetChars)();
  void (*ReleaseChars)(uint16_t*); // static
  size_t (JsString::*JsStringLength)();
  size_t (JsString::*JsStringUtf8Length)();

  // JsTypedArray
  JsTypedArray (*JsTypedArrayNew)(JsTypedArrayType, char*, size_t);
  JsTypedArrayType (JsTypedArray::*Type)();
  size_t (JsTypedArray::*JsTypedArrayLength)();
  void* (JsTypedArray::*JsTypedArrayData)();

  // JsGlobalRef
  void (JsGlobalRef::*JsGlobalRefSet)(const JsValue);
  void (JsGlobalRef::*JsGlobalRefClear)();
  bool (JsGlobalRef::*JsGlobalRefIsEmpty)() const;
  void (JsGlobalRef::*JsGlobalRefEmpty)();
  JsValue (JsGlobalRef::*JsGlobalRefToLocal)();
  void (JsGlobalRef::*SetGCCallback)(void*, GCCallback);
  void (JsGlobalRef::*SetWeakGCCallback)(void*, GCCallback);

  // LocalRefScope
  void (LocalRefScope::*LocalRefScopeNew)(JsRuntime runtime);
  void (LocalRefScope::*LocalRefScopeDeconstruct)();
  JsValue (LocalRefScope::*SaveRef)(JsValue);

  // JsException
  void (*ThrowError)(const char*);
  void (*ThrowTypeError)(const char*);
  void (*ThrowRangeError)(const char*);

  // NativeCallbackInfo
  int (NativeCallbackInfo::*NativeCallbackInfoLength)() const;
  JsValue (NativeCallbackInfo::*NativeCallbackInfoGet)(int) const;
  JsFunction (NativeCallbackInfo::*NativeCallbackInfoCallee)() const;
  JsObject (NativeCallbackInfo::*NativeCallbackInfoThis)() const;
  JsObject (NativeCallbackInfo::*NativeCallbackInfoHolder)() const;
  bool (NativeCallbackInfo::*NativeCallbackInfoIsConstructCall)() const;
  JsValue (NativeCallbackInfo::*NativeCallbackInfoData)() const;
  JsRuntime (NativeCallbackInfo::*NativeCallbackInfoGetRuntime)() const;
  void (NativeCallbackInfo::*NativeCallbackInfoSetReturnValue)(JsValue) const;

  // GCCallbackInfo
  JsRuntime (GCCallbackInfo::*GCCallbackInfoGetRuntime)() const;
  void* (GCCallbackInfo::*GCCallbackInfoGetInfo)() const;
  JSNIEnv* (GCCallbackInfo::*GCCallbackInfoEnv)() const;

  // PropertyInfo
  JsObject (PropertyInfo::*PropertyInfoThis)() const;
  JsObject (PropertyInfo::*PropertyInfoHolder)() const;
  JsValue (PropertyInfo::*PropertyInfoData)() const;
  JsRuntime (PropertyInfo::*PropertyInfoGetRuntime)() const;
  JSNIEnv* (PropertyInfo::*PropertyInfoEnv)() const;
  void (PropertyInfo::*PropertyInfoSetReturnValue)(JsValue) const;

  // JsRuntime
  JsRuntime (*JsRuntimeGetCurrent)();

  // JsContext
  JsContext (*GetCurrentContext)(JsRuntime);
  // ..Many many function pointer.
};




// Should be in jsni.h also ?? Wrap isolate.
struct JSNIEnvExt {
  JSNIEnvExt(void* isolate);

  JSNINativeInterface* JSNIEnv_;
  void* isolate_;

};

}  // namespace jsniinternal

#endif
