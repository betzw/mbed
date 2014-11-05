Compile test1:
==============
python workspace_tools/make.py -m NUCLEO_F401RE -t GCC_ARM -d /media/betzw/NUCLEO/ -n betzw_test_1
python workspace_tools/make.py -m NUCLEO_L053R8 -t GCC_ARM -d /media/betzw/NUCLEO/ -n betzw_test_1

# Note: in order to enable debug in WFI compile with option '-DTPIU_DEBUG'


Compile mbed:
=============
python workspace_tools/build.py -m NUCLEO_F401RE -t GCC_ARM
python workspace_tools/build.py -m NUCLEO_L053R8 -t GCC_ARM
