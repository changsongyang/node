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
#include <jsni-convert.h>
using namespace jsni;
using namespace v8;

int numHandles(JsRuntime rt) {
  int n = v8::HandleScope::NumberOfHandles(ToV8Isolate(rt));
  return n;
}

int pnumHandles(JsRuntime rt) {
  int n = v8::HandleScope::NumberOfHandles(ToV8Isolate(rt));
  printf("n = %d\n", n);
  return n;
}


void Method(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsRuntime rt = info.GetRuntime(env);

  int n;

  // Check String.
  n = numHandles(rt);
  const char* src = "hello, world!";
  JsString jsStr(env, src);  
  JsString st;  
  assert(numHandles(rt) == n + 1 && "The local handle number isn't right.");
  
  // Check Object.
  n = numHandles(rt);
  JsObject ae;
  Local<Object> a; //Empty.
  // a->Set(envtt, tt); //Empty set will crash.
  Local<Value> ac;
  assert(ae.IsEmpty(env));
  assert(a.IsEmpty());
  assert(ac.IsEmpty());
  assert(n == numHandles(rt));

  // Check Number.
  n = numHandles(rt);
  JsNumber b(env, 1);
  JsNumber bt;
  assert(n + 1 == numHandles(rt));

  // Check JsBoolean.
  n = numHandles(rt);
  JsBoolean c(env, true);
  JsBoolean ct;
  Local<Boolean> cc = Boolean::New(ToV8Isolate(rt), true);
  cc.IsEmpty();
  // Boolean, null, undefined is rooted slot and
  // have no local handles.
  assert(n == numHandles(rt));


  // Check Function.
  n = numHandles(rt);
  JsFunction d(env, Method);
  JsFunction dt;
  assert(n + 1 == numHandles(rt));

  // Check TypedArray.
  n = numHandles(rt);
  uint8_t t[] = {1, 2, 3};
  uint8_t * newarr = static_cast<uint8_t*> (malloc(3));
  memcpy(newarr, t, sizeof t);
  JsTypedArray e(env, JsArrayTypeUint8,(char *)newarr, 3);
  JsTypedArray et;
  assert(n + 1 == numHandles(rt));

  // Check jsValue.
  n = numHandles(rt);
  JsValue f;
  //Local<Value> f;
  assert(numHandles(rt) == n);

}



extern "C" int JSNI_Init(JSNIEnv* env, JsValue exports) {
  JSNI::RegisterMethod(env, exports, "hello", Method);
  return JSNI_VERSION_1_0;
}


