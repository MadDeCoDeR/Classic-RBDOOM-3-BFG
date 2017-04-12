cd ..
del /s /q build
mkdir build
cd build
cmake -G "Visual Studio 14 Win64" -T v140_xp -DCMAKE_INSTALL_PREFIX=../bin/windows7-64 ../neo
pause