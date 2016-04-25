cl /DUSRDLL /DWINDLL /LD /c hook.c main.c bwlmain.c sqlog.c
rc rcdata.rc
link /OUT:Release/SuperQuitter.bwl /DLL bwlmain.obj hook.obj main.obj sqlog.obj rcdata.res Advapi32.lib User32.lib
