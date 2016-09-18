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

JsString checkv8(JSNIEnv* env, Isolate* isolate) {
  v8::HandleScope handle_scope(isolate);
  JsString jsStr(env, "hello, world!"); 
  return jsStr;
}

void Method(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsRuntime rt = info.GetRuntime(env);

  //TryScope try_scope(ToV8Isolate(rt));
  v8::Isolate* isolate = ToV8Isolate(rt);
  int n = 0;
  numHandles(rt);
  {
    v8::HandleScope handle_scope(isolate);
    JsString jsStr(env, "hello, world!");    
  }
  numHandles(rt);

  // Check escape.
  n = numHandles(rt);
  {
    v8::EscapableHandleScope handle_scope(isolate);
    Local<String> str = v8::String::NewFromUtf8(isolate, "value1");
    handle_scope.Escape(str);        
  }
  assert(numHandles(rt) == n + 1);

  // Check valid.
  info.SetReturnValue(env, checkv8(env, isolate));
}

void HandleScopeTest(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsRuntime rt = info.GetRuntime(env);

  
  int n;
  n = numHandles(rt);
  {
    JsString jsStr(env, "hello, world!");
  }
  assert(numHandles(rt) == n + 1);

  n = numHandles(rt);
  {
    LocalRefScope ref_scope(env, rt);
    JsString jsStr(env, "hello, world!");
  }

  // Check jsStr is valid or not.
  assert(numHandles(rt) == n + 1);

}

void HandleSaveTest(JSNIEnv* env, const NativeCallbackInfo& info) {

  JsRuntime rt = info.GetRuntime(env);
  // Escape a value and then check if it is valid.
  int n;
  n = numHandles(rt);
  JsValue save;
  {
    LocalRefScope ref_scope(env, rt);
    JsString check(env, "hello, world!");
    save = ref_scope.SaveRef(env, check);
  }
  assert(numHandles(rt) == n + 1);
  info.SetReturnValue(env, save);
}

extern "C" int JSNI_Init(JSNIEnv* env, JsValue exports) {
  JSNI::RegisterMethod(env, exports, "hello", Method);
  JSNI::RegisterMethod(env, exports, "HandleScopeTest", HandleScopeTest);
  JSNI::RegisterMethod(env, exports, "HandleSaveTest", HandleSaveTest);
  return JSNI_VERSION_1_0;
}


