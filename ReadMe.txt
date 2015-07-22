Compile mbed:
=============
python workspace_tools/build.py -t GCC_ARM -m NUCLEO_F401RE
python workspace_tools/build.py -t GCC_ARM -m NUCLEO_L053R8
python workspace_tools/build.py -t GCC_ARM -m LPC11U68
python workspace_tools/build.py -t GCC_ARM -m K64F

# Note: in order to enable debugging compile with option '-o debug-info'


Compile test1:
==============
python workspace_tools/make.py -t GCC_ARM -m NUCLEO_F401RE -d /media/betzw/NUCLEO/ -n betzw_test_1
python workspace_tools/make.py -t GCC_ARM -m NUCLEO_L053R8 -d /media/betzw/NUCLEO/ -n betzw_test_1
python workspace_tools/make.py -t GCC_ARM -m LPC11U68 -d /media/betzw/MBED/ -n betzw_test_1
python workspace_tools/make.py -t GCC_ARM -m K64F -d /media/betzw/MBED/ -n betzw_test_1

# Note: in order to enable debugging compile with option '-o debug-info'
#       and if you are using WFI compile also with '-DDBG_MCU'


Mercurial:
==========
Libraries:
~~~~~~~~~~
COMMON:
-------
git remote add common-lib "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/X_NUCLEO_COMMON"

git subtree split -P libraries/tests/libs/X_NUCLEO_COMMON -b common-split
git checkout common-split
git pull common-lib master

git push common-lib common-split:master

git checkout ble_wb
git subtree merge -P libraries/tests/libs/X_NUCLEO_COMMON common-split

git branch -d common-split

IDB0XA1:
--------
git remote add idb0xa1-lib "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/X_NUCLEO_IDB0XA1"

git subtree split -P libraries/tests/peripherals/X_NUCLEO_IDB0XA1 -b idb0xa1-split
git checkout idb0xa1-split
git pull idb0xa1-lib master

git push idb0xa1-lib idb0xa1-split:master

git checkout ble_wb
git subtree merge -P libraries/tests/peripherals/X_NUCLEO_IDB0XA1 idb0xa1-split

git branch -d idb0xa1-split

Applications:
~~~~~~~~~~~~~
Sensors-Reader:
---------------
git remote add sr-app "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/Sensors_Reader"

git subtree split -P libraries/tests/betzw_tests/test2 -b sr-split
git checkout sr-split
git pull sr-app master

git push sr-app sr-split:master

git checkout ble_wb
git subtree merge -P libraries/tests/betzw_tests/test2 sr-split

git branch -d sr-split
