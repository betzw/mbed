Compile test1:
==============
python workspace_tools/make.py -m NUCLEO_F401RE -t GCC_ARM -d /media/betzw/NUCLEO/ -n betzw_test_1
python workspace_tools/make.py -m NUCLEO_L053R8 -t GCC_ARM -d /media/betzw/NUCLEO/ -n betzw_test_1
python workspace_tools/make.py -m LPC11U68 -t GCC_ARM -d /media/betzw/MBED/ -n betzw_test_1
python workspace_tools/make.py -m K64F -t GCC_ARM -d /media/betzw/MBED/ -n betzw_test_1

# Note: in order to enable debugging compile with option '-o debug-info'
#       and if you are using WFI compile also with '-DDBG_MCU'


Compile mbed:
=============
python workspace_tools/build.py -m NUCLEO_F401RE -t GCC_ARM
python workspace_tools/build.py -m NUCLEO_L053R8 -t GCC_ARM
python workspace_tools/build.py -m LPC11U68 -t GCC_ARM
python workspace_tools/build.py -m K64F -t GCC_ARM

# Note: in order to enable debugging compile with option '-o debug-info'


Mercurial:
==========
Library:
--------
git remote add mbed-lib "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/X_NUCLEO_IKC01A1"

git subtree split -P libraries/tests/peripherals/X_NUCLEO_IKC01A1 -b mbed-split
git checkout mbed-split

git pull mbed-lib master
git push mbed-lib mbed-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/peripherals/X_NUCLEO_IKC01A1 mbed-split
git branch -d mbed-split

Application:
------------
git remote add bm-app "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/Battery_Monitor"

git subtree split -P libraries/tests/betzw_tests/test1 -b bm-split
git checkout bm-split

git pull bm-app master
git push bm-app bm-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/betzw_tests/test1 bm-split
git branch -d bm-split

DevI2C:
-------
git remote add devi2c-class "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/DevI2C"

git subtree split -P libraries/tests/libs/X_NUCLEO_COMMON/DevI2C -b devi2c-split
git checkout devi2c-split

git pull devi2c-class master
git push devi2c-class devi2c-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/libs/X_NUCLEO_COMMON/DevI2C devi2c-split
git branch -d devi2c-split
