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


Compile test2:
==============
python workspace_tools/make.py -t GCC_ARM -m NUCLEO_F401RE -d /media/betzw/NUCLEO/ -n betzw_test_2
python workspace_tools/make.py -t GCC_ARM -m NUCLEO_L053R8 -d /media/betzw/NUCLEO/ -n betzw_test_2
python workspace_tools/make.py -t GCC_ARM -m LPC11U68 -d /media/betzw/MBED/ -n betzw_test_2
python workspace_tools/make.py -t GCC_ARM -m K64F -d /media/betzw/MBED/ -n betzw_test_2

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

git checkout betzw_wb
git subtree merge -P libraries/tests/libs/X_NUCLEO_COMMON common-split

git branch -d common-split

IKC01A1:
--------
git remote add ikc01a1-lib "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/X_NUCLEO_IKC01A1"

git subtree split -P libraries/tests/peripherals/X_NUCLEO_IKC01A1 -b ikc01a1-split
git checkout ikc01a1-split
git pull ikc01a1-lib master

git push ikc01a1-lib ikc01a1-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/peripherals/X_NUCLEO_IKC01A1 ikc01a1-split

git branch -d ikc01a1-split

IKS01A1:
--------
git remote add iks01a1-lib "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/X_NUCLEO_IKS01A1"

git subtree split -P libraries/tests/peripherals/X_NUCLEO_IKS01A1 -b iks01a1-split
git checkout iks01a1-split
git pull iks01a1-lib master

git push iks01a1-lib iks01a1-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/peripherals/X_NUCLEO_IKS01A1 iks01a1-split

git branch -d iks01a1-split

Applications:
~~~~~~~~~~~~~
Battery-Monitor:
----------------
git remote add bm-app "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/Battery_Monitor"

git subtree split -P libraries/tests/betzw_tests/test1 -b bm-split
git checkout bm-split
git pull bm-app master

git push bm-app bm-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/betzw_tests/test1 bm-split

git branch -d bm-split

Sensors-Reader:
---------------
git remote add sr-app "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/Sensors_Reader"

git subtree split -P libraries/tests/betzw_tests/test2 -b sr-split
git checkout sr-split
git pull sr-app master

git push sr-app sr-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/betzw_tests/test2 sr-split

git branch -d sr-split
