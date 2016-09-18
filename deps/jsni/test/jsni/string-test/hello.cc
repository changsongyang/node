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

  JsString jsStr(env, "hello, world!");
  info.SetReturnValue(env, jsStr);

  // Test IsString.
  assert(jsStr.IsString(env));

  // Test length.
  assert(jsStr.Utf8Length(env) == 13);
  // Length is also 13.
  assert(jsStr.Length(env) == 13);

  char *src;
  src = jsStr.GetUtf8Chars(env);
  assert(strcmp(src, "hello, world!") == 0);
  jsStr.ReleaseUtf8Chars(env, src);
  
  //TODO. Test getting the Unicode.
}

void TestUnicode(JSNIEnv* env, const NativeCallbackInfo& info) {
  JsString s = info.Get(env, 0).AsString(env);
  // Check Chinese string.
  assert(strcmp(s.GetUtf8Chars(env), "你好") == 0);
}

void TestUtf16(JSNIEnv* env, const NativeCallbackInfo& info) {
  // Get the value.
  JsString s = info.Get(env, 0).AsString(env);
  uint16_t* save = s.GetChars(env);
  uint16_t* str = save;
  // Get the length.
  size_t len = s.Length(env);
  // Check the length.
  assert(len == 2);
  // Check the underlying encoding.
  for (unsigned int i = 0; i < len + 1; ++i) {
    if (i == 0) {
      // "你"
      assert(*str == 0x4f60);
    }

    if (i == 1) {
      // "好"
      assert(*str == 0x597d);
    }

    if (i == 2) {
      assert(*str == 0x0);
    }
    str++;
  }
  JsString::ReleaseChars(env, save);
}


extern "C" int JSNI_Init(JSNIEnv* env, JsValue exports) {
  JSNI::RegisterMethod(env, exports, "hello", Method);
  JSNI::RegisterMethod(env, exports, "TestUnicode", TestUnicode);
  JSNI::RegisterMethod(env, exports, "TestUtf16", TestUtf16);
  return JSNI_VERSION_1_0;
}


