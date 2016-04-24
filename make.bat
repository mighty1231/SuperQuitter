cl /DUSRDLL /DWINDLL /LD hook.cpp main.cpp Plugin.cpp
rc rcdata.rc
link /OUT:plugin.dll /DLL hook.obj rcdata.res main.obj Plugin.obj User32.lib Advapi32.lib Version.lib 
