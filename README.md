# SuperQuitter

wLauncher plugin that invalidates the attack from a :skull:malicious UMS map that prevents exit in Starcraft.

:warning: The kind of attack was blocked as Starcraft was remastered.

It only works for Starcraft version 1.16.1.

## Feature
* Make the game client load the original `quit2mnu.bin`, although the UMS map try to modify it by using `scmloader`.

## How to build

* Install Microsoft Visual Studio
* Modify `PATH` environment variable to make system be able to search `cl`, `rc`, and `link`.
    * `cl`: Microsoft Visual C++ Compiler
    * `link`: Microsoft Visual C++ Linker
    * `rc`: [Resource Compiler](https://docs.microsoft.com/en-us/windows/win32/menurc/resource-compiler)
* Run `make.bat` through command line.

## How to use

* Put `SuperQuitter.bwl` on `wLauncher/plugins` folder.
* Run `wLauncher` and start `Starcraft I` with the plugin enabled.
* Press `Ctrl+q+q` to escape the game with a :skull:malicious UMS map.

## Thread
I first distributed SuperQuitter on [스타크래프트 에디터 아카데미](https://cafe.naver.com/edac/44727)

## License

This project is licensed under MIT - see the [LICENSE](LICENSE) file for details
