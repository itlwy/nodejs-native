var test = require('./build/Release/test');
test.hello('hello', function(data) {
  console.log(data);
});