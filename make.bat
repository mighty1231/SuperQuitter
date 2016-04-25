cl /DUSRDLL /DWINDLL /LD hook.cpp main.cpp bwlmain.c
link /OUT:SuperQuitter.bwl /DLL bwlmain.obj hook.obj main.obj rcdata.res Advapi32.lib 
