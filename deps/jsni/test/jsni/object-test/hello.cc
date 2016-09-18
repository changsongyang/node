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
//
using namespace jsni;

void CheckSetNativeFunc(JSNIEnv* env, const NativeCallbackInfo& info) {
  info.This(env);
  info.This(env).HasInternalField(env);
}

void CheckSet(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsObject obj = JsObject::New(env);
  assert(obj.IsObject(env));
  obj.SetProperty(env, "func", JsValue(env, 1));
  assert(obj.HasProperty(env, "func"));
  JsValue p = obj.GetProperty(env, "func");
  assert(1 == p.ToInt(env));

  // Check set func.
  JsFunction set_func(env, CheckSetNativeFunc);
  obj.SetProperty(env, "CheckSetNativeFunc", set_func);

  obj.SetProperty(env, "f", info.Get(env, 0));

  info.SetReturnValue(env, obj);
}

void CheckDelete(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsObject obj = JsObject::New(env);
  assert(obj.IsObject(env));
  obj.SetProperty(env, "func", JsValue(env, 1));
  assert(obj.HasProperty(env, "func"));
  obj.DeleteProperty(env, "func");
  assert(obj.HasProperty(env, "func") == false);
}

extern "C" int JSNI_Init(JSNIEnv* env, JsValue exports) {
  JSNI::RegisterMethod(env, exports, "CheckSet", CheckSet);
  JSNI::RegisterMethod(env, exports, "CheckDelete", CheckDelete);
  return JSNI_VERSION_1_0;
}


