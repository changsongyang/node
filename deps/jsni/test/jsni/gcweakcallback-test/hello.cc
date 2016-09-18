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

int a = 1990;

void callback(JSNIEnv* env, const GCCallbackInfo& info) {
  // TODO
  #if 1
  JsRuntime r = info.GetRuntime(env);
  JsRuntime cr = JsRuntime::GetCurrent(env);
  assert(ToV8Isolate(r) == ToV8Isolate(cr));
  void* i = info.GetInfo(env);
  int aa = *(int*)i;
  assert(aa = 1990);
  #endif
  printf("OK callback!\n");
}

void Method(JSNIEnv* env, const NativeCallbackInfo& info) {
  Isolate* isolate = ToV8Isolate(info.GetRuntime(env));
  Local<Context> cxt = isolate->GetCurrentContext();
  const char* src = "function loadIC(obj) {"
                "  return obj.name;"
                "}"
                " (function() {"
                "   var proto = {'name' : 'weak'};"
                "   var obj = Object.create(proto);"
                "   loadIC(obj);"
                "   loadIC(obj);"
                "   loadIC(obj);"
                "   return proto;"
                " })();";

  JsGlobalRef garbage;
  v8::Local<String> source = String::NewFromUtf8(isolate, src);

  {
    v8::HandleScope scope(isolate);
    Local<Value> tmp = v8::Script::Compile(source)->Run()
                                 ->ToObject(cxt)
                                 .ToLocalChecked();
    garbage.Set(env, ToJSNIValue(tmp).AsObject(env));
  }
  garbage.SetWeakGCCallback(env, &a, &callback);
  (isolate)->RequestGarbageCollectionForTesting(Isolate::kFullGarbageCollection);

}

extern "C" int JSNI_Init(JSNIEnv* env, JsValue exports) {
  JSNI::RegisterMethod(env, exports, "hello", Method);
  return JSNI_VERSION_1_0;
}


