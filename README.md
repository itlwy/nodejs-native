# nodejs-native
Nodejs native(c++) example list. if it helps you,please star to encourge,thank you！

## 环境配置

1. 你得有node环境，此处默认已有

2. 编译工具"node-gyp"安装:

   ```javascript
   npm install node-gyp -g	
   ```

   具体安装时可能会需要一些其他依赖，详见其 [github](https://github.com/nodejs/node-gyp)主页，一般直接敲命令即可



## 边缘试探

​	先来个"hello world"开胃菜体验下js调用c++和c++回调，文件主要包括3部分：

### 文件

- 配置文件

  在项目根目录下创建binding.gyp配置文件，内容格式如下：

  ```json
  {
    "targets": [
      {
        "target_name": "test",
        "sources": [ "test.cc" ]
      }
    ]
  }
  ```

  此处是极简配置哦

- cc文件

  ```c++
  #include <node.h>
  #include <v8.h>
  
  using namespace v8;
  
  // 参数是从js端传进来的
  void hello(const FunctionCallbackInfo<Value> &args)
  {
    // 使用 HandleScope 来管理生命周期
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  
    // 判断参数长度和格式，不符合就抛出异常
    if (args.Length() < 2 || !args[0]->IsString())
    {
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Wrong arguments")));
      return;
    }
  
    // callback, 使用Cast方法来转换
    Local<Function> callback = Local<Function>::Cast(args[1]);
    // 返回给js端的结果数组
    Local<Value> argv[1] = {
        String::Concat(Local<String>::Cast(args[0]), String::NewFromUtf8(isolate, " world"))};
    // 调用js端传进来的回调函数，参数: 当前上下文，参数个数，结果数组
    callback->Call(isolate->GetCurrentContext()->Global(), 1, argv);
  }
  
  // 相当于在 exports 对象中添加 { hello: hello }
  void init(Handle<Object> exports)
  {
    NODE_SET_METHOD(exports, "hello", hello);
  }
  
  // 将 export 对象暴露出去
  // 原型 `NODE_MODULE(module_name, Initialize)`
  NODE_MODULE(test, init);
  ```

- js文件

  js文件首先加载c++的模块，然后调用，如下：

  ```javascript
  var test = require('./build/Release/test');
  test.hello('hello', function(data) {
    console.log(data);
  });
  ```

### 编译和运行

```
node-gyp configure
node-gyp build
node test.js
```

​	好了，此时应该可以看到控制台输出"hello world"了。但是，一般我们都是手头上有一个现成的库需要去调用，可不是写个.cc源文件输出"hello world"可以完事的，咱看下一步。代码见 [github](https://github.com/itlwy/nodejs-native/tree/master/hello-world)

## 入门一

​	此处以调用bsdiff算法为例(本人用来做apk的增量更新)，生成差分包，目录结构如下：

- bsdiff

  存放bsdiff源码目录

- bzip2

  bsdiff需要用到的依赖

- folder

  放差分的示例文件

- binding.gyp

  node-gyp编译配置文件

- bsdiff.cc

  native原生入口文件

- bsdiff.js

功能：

- [x] 生成差分包
- [ ] 合并差分包

内容比较简单，主要比一章的多个调用外部库，主要展示调用方式，详见源码 [github](https://github.com/itlwy/nodejs-native/tree/master/bsdiff-example) 。

此算法的封装有现成的npm库，见：[bsdiff-node](https://github.com/Dezeiraud/bsdiff-nodejs)   ，这里就不做深入封装了。

​	