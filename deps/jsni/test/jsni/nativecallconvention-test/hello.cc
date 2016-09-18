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

using namespace jsni;

void Method(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsString jsStr = JsString(env, "hello, world!");
  info.SetReturnValue(env, jsStr);

}

void length(JSNIEnv* env, const NativeCallbackInfo& info) {
  info.SetReturnValue(env, JsValue(env, info.Length(env)));
}

void args(JSNIEnv* env, const NativeCallbackInfo& info) {
  int x = info.Get(env, 0).ToInt(env)+info.Get(env, 1).ToInt(env)+info.Get(env, 2).ToInt(env);
  info.SetReturnValue(env, JsValue(env, x));
}

void callee(JSNIEnv* env, const NativeCallbackInfo& info) {
  info.SetReturnValue(env, info.Callee(env));
}

void This(JSNIEnv* env, const NativeCallbackInfo& info) {
  info.SetReturnValue(env, info.This(env));
  // Check misc.
  info.This(env).HasInternalField(env);
  info.This(env).GetInternalField(env);
}

void holder(JSNIEnv* env, const NativeCallbackInfo& info) {
  info.SetReturnValue(env, info.Holder(env));
}

#if 1
void isConstruct(JSNIEnv* env, const NativeCallbackInfo& info) {
  info.SetReturnValue(env, JsValue(env, info.IsConstructCall(env)));
}

void data(JSNIEnv* env, const NativeCallbackInfo& info) {
  info.SetReturnValue(env, info.Data(env));
}
#endif

extern "C" int JSNI_Init(JSNIEnv* env, JsValue exports) {
  JSNI::RegisterMethod(env, exports, "hello", Method);
  JSNI::RegisterMethod(env, exports, "length", length);
  JSNI::RegisterMethod(env, exports, "args", args);
  JSNI::RegisterMethod(env, exports, "callee", callee);
  JSNI::RegisterMethod(env, exports, "this", This);
  JSNI::RegisterMethod(env, exports, "holder", holder);
  JSNI::RegisterMethod(env, exports, "isConstruct", isConstruct);
  JSNI::RegisterMethod(env, exports, "data", data);
  return JSNI_VERSION_1_0;
}


