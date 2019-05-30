#include <node.h>
#include <v8.h>
#include <iostream>
#include <string.h>

extern "C"
{
#include "bsdiff/bsdiff.h"
}

using namespace std;
using namespace v8;

char *string2Char(Local<String> str)
{
  String::Utf8Value utfValue(str);
  string a1 = string(*utfValue);
  char *temp = new char[a1.length()];
  strcpy(temp, a1.c_str());
  return temp;
};

void bsdiff(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() < 4)
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }
  char *a = string2Char(Local<String>::Cast(args[0]));
  char *b = string2Char(Local<String>::Cast(args[1]));
  char *c = string2Char(Local<String>::Cast(args[2]));
  // cout << a << endl;
  int ret = diff(a, b, c);
  cout << "diff done:" << ret << endl;
  delete[] a;
  delete[] b;
  delete[] c;

  Local<Function> callback = Local<Function>::Cast(args[3]);
  if (ret == 0)
  {
    Local<Value> argv[1] = {
        String::NewFromUtf8(isolate, "patch succeed!")};
    // 调用js端传进来的回调函数，参数: 当前上下文，参数个数，结果数组
    callback->Call(isolate->GetCurrentContext()->Global(), 1, argv);
  }
  else
  {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "collected error message")));
  }
}

// 相当于在 exports 对象中添加 { diff: bsdiff }
void init(Handle<Object> exports)
{
  NODE_SET_METHOD(exports, "diff", bsdiff);
}

// 将 export 对象暴露出去
// 原型 `NODE_MODULE(module_name, Initialize)`
NODE_MODULE(bsdiff, init);