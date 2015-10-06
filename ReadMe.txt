Compile mbed:
=============
python workspace_tools/build.py -t GCC_ARM -m NUCLEO_F401RE
python workspace_tools/build.py -t GCC_ARM -m NUCLEO_L053R8
python workspace_tools/build.py -t GCC_ARM -m LPC11U68
python workspace_tools/build.py -t GCC_ARM -m K64F

# Note: in order to enable debugging compile with option '-o debug-info'


Compile ble_tests:
==================
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
IDB0XA1:
--------
{only oneshot: 
 git remote add idb0xa1-lib "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST/code/X_NUCLEO_IDB0XA1"
}

[git stash]

git subtree split -P libraries/tests/peripherals/X_NUCLEO_IDB0XA1 -b idb0xa1-split
git checkout idb0xa1-split

git pull idb0xa1-lib master

git push idb0xa1-lib idb0xa1-split:master

git checkout ble_wb
git subtree merge -P libraries/tests/peripherals/X_NUCLEO_IDB0XA1 idb0xa1-split

git branch -d idb0xa1-split

[git stash pop]


Applications:
~~~~~~~~~~~~~
BLE-TESTS:
----------
{only oneshot: 
 git remote add ble-app "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/X-MBED-BLE1"
}

[git stash]

git subtree split -P libraries/tests/betzw_tests/ble_tests -b ble-split
git checkout ble-split

git pull ble-app master

git push ble-app ble-split:master

git checkout ble_wb
git subtree merge -P libraries/tests/betzw_tests/ble_tests ble-split

git branch -d ble-split

[git stash pop]
