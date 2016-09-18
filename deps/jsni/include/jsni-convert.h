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

// Note:
// This is only for DEBUG. Will be removed.
// Do not use them in your jsni programs.

#include "v8.h"
#include "jsni.h"
using namespace jsni;
v8::Local<v8::Value> ToV8LocalValue(JsValue val) {
  JsLocal local = val.Get();
  return *reinterpret_cast<v8::Local<v8::Value>*>(&local);
}

v8::Isolate* ToV8Isolate(JsRuntime rt) {
  return *reinterpret_cast<v8::Isolate**>(&rt);
}

v8::Local<v8::Context> ToV8LocalContext(JsContext ct) {
  ContextRef cont = ct.Get();
  return *reinterpret_cast<v8::Local<v8::Context>*>(&cont);
}

JsValue ToJSNIValue(v8::Local<v8::Value> val) {
  v8::Value* get = *val;
  return *reinterpret_cast<JsValue*>(&get);
}

JsRuntime ToJSNIRuntime(v8::Isolate* isolate) {
  return *reinterpret_cast<JsRuntime*>(&isolate);
}

JsContext ToJSNIContext(v8::Local<v8::Context> ct) {
  v8::Context* cont = *ct;
  return *reinterpret_cast<JsContext*>(&cont);
}

