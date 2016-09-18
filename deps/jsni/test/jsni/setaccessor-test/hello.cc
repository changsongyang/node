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

// hello.cc
#include <jsni.h>
#include <assert.h>
// #include <node.h>
#include <jsni-convert.h>
using namespace jsni;
using namespace v8;

static int a;

void GetInt(JSNIEnv* env, JsValue *name, const PropertyInfo& info) {
  // info.SetReturnValue(env, JsValue(env, a));
  info.SetReturnValue(env, JsValue(env, a));
  // Check misc.
  info.This(env).HasInternalField(env);
  info.Holder(env).HasInternalField(env);
  info.Data(env).IsEmpty(env);
  JsRuntime r = info.GetRuntime(env);
  JsRuntime cr = JsRuntime::GetCurrent(env);
  assert(ToV8Isolate(r) == ToV8Isolate(cr));
}

void SetInt(JSNIEnv* env, JsValue *name, JsValue val, const PropertyInfo& info) {
  int x = val.ToInt(env);
  // Use a trick to check set/get. Add x with 1. Then check x + 1;
  a = x + 1;
  // Check misc.
  info.This(env).HasInternalField(env);
  info.Holder(env).HasInternalField(env);
  info.Data(env).IsEmpty(env);
  JsRuntime r = info.GetRuntime(env);
  JsRuntime cr = JsRuntime::GetCurrent(env);
  assert(ToV8Isolate(r) == ToV8Isolate(cr));
}


void SetAcc(JSNIEnv* env, const NativeCallbackInfo& info) {
  // info.This(env).HasInternalField(env);
  JsValue val = info.Get(env, 0);
  val.AsObject(env).SetAccessor(env, "xxx", GetInt, SetInt);
}

extern "C" int JSNI_Init(JSNIEnv* env, JsValue exports) {
  JSNI::RegisterMethod(env, exports, "SetAcc", SetAcc);
  return JSNI_VERSION_1_0;
}


