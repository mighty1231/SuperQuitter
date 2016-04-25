cl /DUSRDLL /DWINDLL /LD hook.cpp main.cpp bwlmain.c
rc rcdata.rc
link /OUT:Release/SuperQuitter.bwl /DLL bwlmain.obj hook.obj main.obj rcdata.res Advapi32.lib 
