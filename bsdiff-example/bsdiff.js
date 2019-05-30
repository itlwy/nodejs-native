var bsdiff = require('./build/Release/bsdiff');
var path = require('path');
var oldApk = path.join(__dirname, "./folder/file_old.txt");
var newApkPath = path.join(__dirname, "./folder/file_new.txt");
var apkPatchPath = path.join(__dirname, "./folder/file.patch");

bsdiff.diff(oldApk, newApkPath, apkPatchPath, function (data) {
  console.log('生成差分包成功，文件:' + apkPatchPath);
});
