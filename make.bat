cl /DUSRDLL /DWINDLL /LD hook.cpp main.cpp bwlmain.c
link /OUT:SuperQuitter.bwl /DLL bwlmain.obj Advapi32.lib User32.lib
link /OUT:SuperQuitter.dll /DLL hook.obj main.obj rcdata.res Advapi32.lib