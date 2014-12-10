Compile test1:
==============
python workspace_tools/make.py -m NUCLEO_F401RE -t GCC_ARM -d /media/betzw/NUCLEO/ -n betzw_test_1
python workspace_tools/make.py -m NUCLEO_L053R8 -t GCC_ARM -d /media/betzw/NUCLEO/ -n betzw_test_1
python workspace_tools/make.py -m K64F -t GCC_ARM -d /media/betzw/MBED/ -n betzw_test_1

# Note: in order to enable debug in WFI compile with option '-DTPIU_DEBUG'


Compile mbed:
=============
python workspace_tools/build.py -m NUCLEO_F401RE -t GCC_ARM
python workspace_tools/build.py -m NUCLEO_L053R8 -t GCC_ARM
python workspace_tools/build.py -m K64F -t GCC_ARM


Mercurial:
==========
Library:
--------
git remote add mbed-lib "hg::http://wobetz:<pwd>@developer.mbed.org/users/wobetz/code/X_NUCLEO_IKC01A1"

git subtree split -P libraries/tests/peripherals/X_NUCLEO_IKC01A1 -b mbed-split
git checkout mbed-split

git push mbed-lib mbed-split:master
git pull mbed-lib master

git checkout betzw_wb
git subtree merge -P libraries/tests/peripherals/X_NUCLEO_IKC01A1 mbed-split
git branch -d mbed-split

Application:
------------
git remote add bm-app "hg::http://wobetz:<pwd>@developer.mbed.org/users/wobetz/code/Battery_Monitor"

git subtree split -P libraries/tests/betzw_tests/test1 -b bm-split
git checkout bm-split

git push bm-app bm-split:master
git pull bm-app master

git checkout betzw_wb
git subtree merge -P libraries/tests/betzw_tests/test1 bm-split
git branch -d bm-split
