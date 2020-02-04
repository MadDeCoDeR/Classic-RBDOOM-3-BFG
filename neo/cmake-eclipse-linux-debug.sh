clear
echo --------------------------------------------------------------------------------
COLUMNS=$(tput cols) 
title="DOOM BFA EDITION" 
printf "%*s\n" $(((${#title}+$COLUMNS)/2)) "$title"
COLUMNS=$(tput cols) 
title="ECLIPSE UNIX MakeFile GENERATION FOR x86 - x64" 
printf "%*s\n" $(((${#title}+$COLUMNS)/2)) "$title" 
COLUMNS=$(tput cols) 
title="DEBUG BUILD" 
printf "%*s\n" $(((${#title}+$COLUMNS)/2)) "$title"
echo --------------------------------------------------------------------------------
cd ..
rm -rf build > /dev/null 2>&1
mkdir build > /dev/null 2>&1
cd build 
echo Generating files
cmake -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DSDL2=ON ../neo > /dev/null 2>&1 ||(
echo ERROR Generating files
cd ..
rmdir /Q /S build
read -rsp $'Press any key to continue...\n' -n1 key
)
read -rsp $'Press any key to continue...\n' -n1 key
