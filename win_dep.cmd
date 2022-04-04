:: "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
:: D:\php-sdk-binary-tools-master\bin\phpsdk_setvars.bat
if exist ./zxing-cpp-src (echo "zxing is down") else (git.exe clone --progress -v --depth 1 "https://github.com/nu-book/zxing-cpp" "zxing-cpp-src")
if exist ./stb-src (echo "stb is down") else (git.exe clone --progress -v --depth 1 "https://github.com/nothings/stb" "stb-src")
cmake -S zxing-cpp-src -B zxing-cpp-build --install-prefix %~dp0%zxing-cpp-lib -DLINK_CPP_STATICALLY=Off
cmake --build zxing-cpp-build --config Release
cmake --install zxing-cpp-build




