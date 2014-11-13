Compile test1:
==============
python workspace_tools/make.py -m NUCLEO_F401RE -t GCC_ARM -d /media/betzw/NUCLEO/ -n betzw_test_1
python workspace_tools/make.py -m NUCLEO_L053R8 -t GCC_ARM -d /media/betzw/NUCLEO/ -n betzw_test_1

# Note: in order to enable debug in WFI compile with option '-DTPIU_DEBUG'


Compile mbed:
=============
python workspace_tools/build.py -m NUCLEO_F401RE -t GCC_ARM
python workspace_tools/build.py -m NUCLEO_L053R8 -t GCC_ARM


Mercurial:
==========
git remote add mbed "hg::http://wobetz:<pwd>@developer.mbed.org/users/wobetz/code/X-CUBE-BFUELG1"
git subtree split -P libraries/tests/peripherals/X-CUBE-BFUELG1 -b split-lib
git push mbed split-lib:master
git branch -D split-lib
