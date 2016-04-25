cl /DUSRDLL /DWINDLL /LD /c hook.c main.c bwlmain.c sqlog.c util.c
rc rcdata.rc
link /OUT:Release/SuperQuitter.bwl /DLL bwlmain.obj hook.obj main.obj sqlog.obj util.obj rcdata.res Advapi32.lib User32.lib
