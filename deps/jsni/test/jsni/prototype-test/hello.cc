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

// Define test v8.
#define TEST_V8 0
#if TEST_V8
void V8SetPro(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> ct = isolate->GetCurrentContext(env);
  Local<Object> a0 = args[0].As<Object>();
  Local<Object> a1 = args[1].As<Object>();
  a0->SetPrototype(envct, a1);
  args.GetReturnValue().Set(a0);
}

void V8GetPro(const FunctionCallbackInfo<Value>& args) {
  Local<Object> a0 = args[0].As<Object>();
  Local<Value> proto0 = a0->GetPrototype(env);
  args.GetReturnValue().Set(proto0);
}
#endif

void SetPro(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsValue a0 = info.Get(env, 0);
  JsValue a1 = info.Get(env, 1);
  a0.AsObject(env).SetPrototype(env, a1);
  info.SetReturnValue(env, a0);
}

void GetPro(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsValue a0 = info.Get(env, 0);
  JsValue proto = a0.AsObject(env).GetPrototype(env);
  info.SetReturnValue(env, proto);
}

extern "C" int JSNI_Init(JSNIEnv* env, JsValue exports) {
  JSNI::RegisterMethod(env, exports, "SetPro", SetPro);
  JSNI::RegisterMethod(env, exports, "GetPro", GetPro);
  return JSNI_VERSION_1_0;
}

