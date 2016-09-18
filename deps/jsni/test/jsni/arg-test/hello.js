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

const addon = nativeLoad('./build/Release/addon');
// Create various typeof values. Read and write test.
// Value.
var value;

// Number
var number = 1;

// Bool
var bool = true;

// Object
var object = {
  name: "abc"
};

// Function
function func() {
  console.log("OK")
};

// String
var string = "abc"

// Uint8Array typed array.
var buffer = new ArrayBuffer(16);
var uint8View = new Uint8Array(buffer);
for (var i = 0; i < uint8View.length; i++) {
  uint8View[i] = i;
}

// Test them.
// Test Read.
console.assert(typeof addon.TestValue(value) == "undefined");
console.assert(addon.TestNumber(number) == 1);
console.assert(addon.TestBool(bool) == true);
console.assert(addon.TestObject(object).name == "abc");
console.log("Next line should print \"OK\".");addon.TestFunction(func)();
console.assert(addon.TestString(string) == "abc");
var check_view = addon.TestArray(uint8View);
for (var i = 0; i < check_view.length; i++) {
 console.assert(check_view[i] == i);
}

// Test Write.
// We can only write to object and array type values.
addon.TestWrite(object, uint8View);
console.assert(object.number  == 88);
for (var i = 0; i < check_view.length; i++) {
  console.assert(check_view[i] == i * 2);
}

process.exit();
