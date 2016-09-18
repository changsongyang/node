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


#include <jsni.h>
#include <assert.h>
using namespace jsni;
class A
{
public:
    A(JSNIEnv* env, const NativeCallbackInfo& info) {

    }
    static void New(JSNIEnv* env, const NativeCallbackInfo& info) {
        A* a = new A(env, info);
        JsObject lobj = gobj.ToLocal(env).AsObject(env);
        JsObject newObj = lobj;
        assert(newObj.HasInternalField(env) == true);
        newObj.SetInternalField(env, (void*)a);
        info.SetReturnValue(env, newObj);
    }
    static void Binding(JSNIEnv* env, JsValue exports) {
        JsObject obj = JsObject::NewWithInternalField(env);
        obj.SetProperty(env, "func", JsFunction(env, myFunction));
        JsFunction tmp(env, myFunction);
        tmp.Call(env, JsValue::Null(env), 0, NULL);
        gobj.Set(env, obj);
        JSNI::RegisterMethod(env, exports, "A", New);
    }
    static void myFunction(JSNIEnv* env, const NativeCallbackInfo& info) {
        printf("%s\n", "OK");
    }
    static JsGlobalRef gobj;
};
JsGlobalRef A::gobj;

void Hello(JSNIEnv* env, const NativeCallbackInfo& info) {
    // Isolate* isolate = ToV8Isolate(info.GetRuntime(env));
    //(isolate)->RequestGarbageCollectionForTesting(Isolate::kFullGarbageCollection);
    printf("%s\n", "Hello.");
}

extern "C" int JSNI_Init(JSNIEnv* env, JsValue exports) {
  JSNI::RegisterMethod(env, exports, "hello", Hello);
  A::Binding(env, exports);
  return JSNI_VERSION_1_0;
}





