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
#include <jsni-convert.h>
#include <jsni.h>
#include <assert.h>
using namespace jsni;
using namespace v8;

void MethodA(JSNIEnv* env, const NativeCallbackInfo& info) {
  

  JsValue a = info.Get(env, 0);

  JsValue b = info.Get(env, 1);
  JsRuntime rt = info.GetRuntime(env);

  JsContext ct = JsContext::GetCurrentContext(env, rt);

  v8::Isolate* is = ToV8Isolate(rt);
  JsRuntime testruntime((RuntimeRef)is);
  v8::Local<Context> ctx = ToV8LocalContext(ct);
  JsContext testcontext((ContextRef)*ctx);
  
  /*JsRuntime rt_r = */ToJSNIRuntime(is);
  /*JsContext ct_r = */ToJSNIContext(ctx);

  v8::Local<Value> a_1 = ToV8LocalValue(a);
  v8::Local<Value> b_1 = ToV8LocalValue(b);

  int32_t a_r = a_1->ToInteger(ctx).ToLocalChecked()->Value();
  int32_t b_r = b_1->ToInteger(ctx).ToLocalChecked()->Value();
  int32_t r = a_r + b_r;

  v8::Local<v8::Value> r_r = v8::Local<Value>(Integer::New(is, r));
  JsValue ret = ToJSNIValue(r_r);
  info.SetReturnValue(env, ret);
}

extern "C" int JSNI_Init(JSNIEnv* env, JsValue exports) {

  JSNI::RegisterMethod(env, exports, "foo1", MethodA);
  return JSNI_VERSION_1_0;
}

