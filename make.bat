cl /DUSRDLL /DWINDLL /LD hook.cpp main.cpp /link /OUT:SuperQuit.dll /DLL rcdata.res User32.lib Advapi32.lib Version.lib
