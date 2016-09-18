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

JsValue val;
JsGlobalRef globalStr;

void TestGlobal(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsString jsStr(env, "hello, world!");

  // Add to global.
  globalStr.Set(env, jsStr);
  
  JsValue local = globalStr.ToLocal(env);

  info.SetReturnValue(env, local);

  // Clear this global reference.
  globalStr.Clear(env);
  if (globalStr.IsEmpty(env)) {
    printf("OK. is empty.\n");
  }
  globalStr.Empty(env);
  if (globalStr.IsEmpty(env)) {
    printf("OK. is empty.\n");
  }

}

int numHandles(JsRuntime rt) {
  int n = v8::HandleScope::NumberOfHandles(ToV8Isolate(rt));
  // printf("%d\n", n);
  return n;
}

void Method(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsRuntime rt = info.GetRuntime(env);

  int n = numHandles(rt);
  const char* src = "hello, world!";
  // JsObject obj;
  // Local<Object> obj = Object::New(ToV8Isolate(rt));
  JsString jsStr(env, src);  
  // Check local handle number.
  assert(numHandles(rt) == n + 1 && "The local handle number isn't right.");

  val = jsStr;
  // Add to global.
  JsGlobalRef gpVal;
  gpVal.Set(env, val);
  
  JsValue local = gpVal.ToLocal(env);

  info.SetReturnValue(env, local);

  // Clear this global reference.
  gpVal.Clear(env);

}

void MethodAfterClear(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsRuntime rt = info.GetRuntime(env);

  int n = numHandles(rt);

  JsString jsStr(env, "hello, world!");
  
  // Check local handle number.
  assert(numHandles(rt) == n + 1);

  // Add to global.
  JsGlobalRef gpStr;
  gpStr.Set(env, jsStr);
  // Clear this global reference.  
  gpStr.Clear(env);


  // Force GC.
  (ToV8Isolate(rt))->RequestGarbageCollectionForTesting(Isolate::kFullGarbageCollection);
  // Check jsStr. gpStr should be invalid after gpStr release.
  while(!(ToV8Isolate(info.GetRuntime(env)))->IdleNotification(1000)) {};
  JsValue local = gpStr.ToLocal(env);

  info.SetReturnValue(env, local);

}

/*
void RefSaveTest(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsRuntime rt = info.GetRuntime(env);
  // Escape a value and then check if it is valid.
  {
    LocalRefScope ref_scope(rt);
    JsString check("hello, world!");
    JsValue save = ref_scope.SaveRef(envcheck);
  }
  // JsValue check = CheckHandle(rt);
  info.SetReturnValue(envcheck);
}
*/

extern "C" int JSNI_Init(JSNIEnv* env, JsValue exports) {
  JSNI::RegisterMethod(env, exports, "hello", Method);
  JSNI::RegisterMethod(env, exports, "helloRelease", MethodAfterClear);
  JSNI::RegisterMethod(env, exports, "TestGlobal", TestGlobal);
  // JSNI::RegisterMethod(env, exports, "RefSaveTest", RefSaveTest);

  return JSNI_VERSION_1_0;
}


