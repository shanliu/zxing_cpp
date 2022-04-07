### zxing-cpp wrappers for php extension

> ZXing-C++,more detail see: https://github.com/nu-book/zxing-cpp

> fast, support multiple formats decode and encode

> support php >=7.0 

> API doc see : [zxing_cpp.php](./zxing_cpp.php)

> tutorial : [tools/dome.php](./tools/dome.php)

> windows dll : [dll/php_zxing_cpp_php8.*.dll](./dll)  `Tips:` Download according to the PHP version you use,only PHP > 8 and x64 has been compiled. other versions can be compiled according to the following documents 

#### build for linux or mac

```shell
cd zxing_cpp # go to your `shanliu/zxing_cpp` src dir
git clone --progress -v --depth 1 https://github.com/nothings/stb stb-src
git clone --progress -v --depth 1 https://github.com/nu-book/zxing-cpp zxing-cpp-src
cd zxing-cpp-src && cmake . && make && make install && cd .. # Maybe you set prefix??
phpize && ./configure --with-stb-dir=stb-src # ./configure --help see more config
make && make install
```

#### build for windows

> load build env and build

```
open [x64|x86] Native Tools Command Prompt for VS 2019 build env #need use >=vs2019
load PHP build env \your-php-sdk-binary-tools-dir\bin\phpsdk_setvars.bat 
```

```cmd
cd zxing_cpp # go to your zxing_cpp src dir and download `shanliu/zxing_cpp` to ext/zxing_cpp
win_dep.cmd #download lib
cd your_php_src # go to your php src dir
buildconf.bat
configure --disable-all --enable-cli --enable-zxing_cpp=shared 
nmake
```

