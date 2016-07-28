Compile mbed:
=============
python workspace_tools/build.py -t GCC_ARM -m NUCLEO_F401RE
python workspace_tools/build.py -t GCC_ARM -m NUCLEO_L053R8
python workspace_tools/build.py -t GCC_ARM -m LPC11U68
python workspace_tools/build.py -t GCC_ARM -m K64F

# Note: in order to enable debugging compile with option '-o debug-info'


Compile bm_app:
===============
python workspace_tools/make.py -t GCC_ARM -m NUCLEO_F401RE -d /media/betzw/NUCLEO/ -n betzw_test_1
python workspace_tools/make.py -t GCC_ARM -m NUCLEO_L053R8 -d /media/betzw/NUCLEO/ -n betzw_test_1
python workspace_tools/make.py -t GCC_ARM -m LPC11U68 -d /media/betzw/MBED/ -n betzw_test_1
python workspace_tools/make.py -t GCC_ARM -m K64F -d /media/betzw/MBED/ -n betzw_test_1

# Note: in order to enable debugging compile with option '-o debug-info'
#       and if you are using WFI compile also with '-DDBG_MCU'


Compile sr_app:
===============
python workspace_tools/make.py -t GCC_ARM -m NUCLEO_F401RE -d /media/betzw/NODE_F401RE/ -n betzw_test_2
python workspace_tools/make.py -t GCC_ARM -m NUCLEO_L053R8 -d /media/betzw/NUCLEO/ -n betzw_test_2
python workspace_tools/make.py -t GCC_ARM -m LPC11U68 -d /media/betzw/MBED/ -n betzw_test_2
python workspace_tools/make.py -t GCC_ARM -m K64F -d /media/betzw/MBED/ -n betzw_test_2

# Note: in order to enable debugging compile with option '-o debug-info'
#       and if you are using WFI compile also with '-DDBG_MCU'


Compile hw_iks01a1:
===================
python workspace_tools/make.py -t GCC_ARM -m NUCLEO_F401RE -d /media/betzw/NUCLEO/ -n betzw_test_3
python workspace_tools/make.py -t GCC_ARM -m NUCLEO_L053R8 -d /media/betzw/NUCLEO/ -n betzw_test_3
python workspace_tools/make.py -t GCC_ARM -m LPC11U68 -d /media/betzw/MBED/ -n betzw_test_3
python workspace_tools/make.py -t GCC_ARM -m K64F -d /media/betzw/MBED/ -n betzw_test_3

# Note: in order to enable debugging compile with option '-o debug-info'
#       and if you are using WFI compile also with '-DDBG_MCU'


Mercurial:
==========
Libraries:
~~~~~~~~~~
COMMON:
-------
{only oneshot: 
 git remote add common-lib "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST/code/X_NUCLEO_COMMON"
}

[git stash]

git subtree split -P libraries/tests/libs/X_NUCLEO_COMMON -b common-split
git checkout common-split

git pull common-lib master

git push common-lib common-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/libs/X_NUCLEO_COMMON common-split

git branch -d common-split

[git stash pop]

IKC01A1:
--------
{only oneshot: 
 git remote add ikc01a1-lib "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/X_NUCLEO_IKC01A1"
}

[git stash]

git subtree split -P libraries/tests/peripherals/X_NUCLEO_IKC01A1 -b ikc01a1-split
git checkout ikc01a1-split

git pull ikc01a1-lib master

git push ikc01a1-lib ikc01a1-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/peripherals/X_NUCLEO_IKC01A1 ikc01a1-split

git branch -d ikc01a1-split

[git stash pop]

IKS01A1:
--------
{only oneshot: 
 git remote add iks01a1-lib "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST/code/X_NUCLEO_IKS01A1"
}

[git stash]

git subtree split -P libraries/tests/peripherals/X_NUCLEO_IKS01A1 -b iks01a1-split
git checkout iks01a1-split

git pull iks01a1-lib master

git push iks01a1-lib iks01a1-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/peripherals/X_NUCLEO_IKS01A1 iks01a1-split

git branch -d iks01a1-split

[git stash pop]


Applications:
~~~~~~~~~~~~~
Battery-Monitor:
----------------
{only oneshot: 
 git remote add bm-app "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST-Expansion-SW-Team/code/Battery_Monitor"
}

[git stash]

git subtree split -P libraries/tests/betzw_tests/bm_app -b bm-split
git checkout bm-split

git pull bm-app master

git push bm-app bm-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/betzw_tests/bm_app bm-split

git branch -d bm-split

[git stash pop]

Hello World IKS01A1:
--------------------
{only oneshot: 
 git remote add hwiks01a1-app "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST/code/HelloWorld_IKS01A1"
}

[git stash]

git subtree split -P libraries/tests/betzw_tests/hw_iks01a1 -b hwiks01a1-split
git checkout hwiks01a1-split

git pull hwiks01a1-app master

git push hwiks01a1-app hwiks01a1-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/betzw_tests/hw_iks01a1 hwiks01a1-split

git branch -d hwiks01a1-split

[git stash pop]

Sensors-Reader:
---------------
{only oneshot: 
 git remote add sr-app "hg::http://wobetz:<pwd>@developer.mbed.org/teams/ST/code/Sensors_Reader"
}

[git stash]

git subtree split -P libraries/tests/betzw_tests/sr_app -b sr-split
git checkout sr-split

git pull sr-app master

git push sr-app sr-split:master

git checkout betzw_wb
git subtree merge -P libraries/tests/betzw_tests/sr_app sr-split

git branch -d sr-split

[git stash pop]
