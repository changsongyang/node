// hello.cc
#include <node.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Persistent;
using namespace v8;

int a = 1;

void AGet(Local<Name> property,
                      const PropertyCallbackInfo<Value>& info) {


}

void ASet(Local<Name> property,
                      Local<Value> value,
                      const PropertyCallbackInfo<void>& info) {
  info.This()->InternalFieldCount();
}


void SetAcc(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  Local<Context> context = isolate->GetCurrentContext();

  Local<Object> val = args[0]->ToObject();
  char* name = "xxx";
  val->SetAccessor(context,
                     (String::NewFromUtf8(isolate,
                              name,
                              NewStringType::kNormal).ToLocalChecked()),
                     AGet,
                     ASet);
}


void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "SetAcc", SetAcc);
}

NODE_MODULE(addon, init)

