clear
echo --------------------------------------------------------------------------------
COLUMNS=$(tput cols)
title="DOOM BFA EDITION"
printf "%*s\n" $(((${#title}+$COLUMNS)/2)) "$title"
COLUMNS=$(tput cols)
title="UNIX MakeFile GENERATION FOR x86 - x64"
printf "%*s\n" $(((${#title}+$COLUMNS)/2)) "$title"
COLUMNS=$(tput cols)
title="DEBUG BUILD"
printf "%*s\n" $(((${#title}+$COLUMNS)/2)) "$title"
echo --------------------------------------------------------------------------------
cd ..
rm -rf buildDb > /dev/null 2>&1
mkdir buildDb > /dev/null 2>&1
cd buildDb
echo Generating files
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DSDL2=ON -DOpenGL_GL_PREFERENCE=GLVND ../neo > log.txt ||(
echo ERROR Generating files
cd ..
rmdir /Q /S buildDb
#read -rsp $'Press any key to continue...\n' -n1 key
)
#read -rsp $'Press any key to continue...\n' -n1 key
echo Files generated Successfully
