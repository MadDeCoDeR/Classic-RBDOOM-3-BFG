cd ..
del /s /q build
mkdir build
cd build
cmake -G "Visual Studio 15 Win64" -T v141_xp -DCMAKE_INSTALL_PREFIX=../bin/windows10-64 -DWINDOWS10=ON ../neo
pause