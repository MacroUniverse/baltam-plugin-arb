## 尝试做一个 arblib 的接口
* [官方页面](https://arblib.org/)
* 另外用到了作者的个人计算库 [SLISC0](https://github.com/MacroUniverse/SLISC0) 欢迎来 star

* (完成了 hypergeom) 先试试导入几个特殊函数， 例如 gamma 函数， hypergeom， 输出双精度类型就好。
## TODO
* 然后实现 digits() 函数， 重载 arf 类型的一些算符 +, -, *, / 等

## 使用说明
目前使用以前需要把所有 .so 文件都复制/移动到 `/opt/Baltamatica/lib` 目录下面， 或者把 `/opt/Baltamatica/plugins/arb` 文件夹添加到环境变量 `LD_LIBRARY_PATH` 中

## Linux 编译说明
* 把这个仓库 clone 到 `baltamatica\bex\test\plugins\arb\.git`
* 目前在 Ubuntu 22.04 下编译测试， 把这个文件夹所有内容复制到新建文件夹 `/opt/Baltamatica/bex/test/plugins/arb/` 直接运行 `./make.sh` 即可编译 `main.so` 其他文件的来源都是 Ubuntu 22.04 的 `sudo apt install libflint-arb`， 用 `ldd main.so` 就可以查看所有依赖库的目录。

## Windows 编译说明
* 把这个仓库 clone 到 `baltamatica\bex\test\plugins\arb\.git`
* 双击打开群文件 `baltam_SDK/msys64-tmp` 中的 msys2.exe， 然后把这个文件夹所有内容复制到新建文件夹 `cd /c/baltamatica/bex/test/plugins/arb`， 然后运行 `make-windows.sh` 即可。
* 如果 baltam 加载 plugin 时出现错误 `The specified module could not be found.`， 就用在 msys2 里面用 `ldd main.dll` 查看所有依赖的其他 dll， 然后确认要么是 windows 的系统 dll， 要么复制到 `baltamatica\lib` 目录中。
