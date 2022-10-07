## 尝试做一个 arblib 的接口
* [官方页面](https://arblib.org/)
* 另外用到了作者的个人计算库 [SLISC0](https://github.com/MacroUniverse/SLISC0) 欢迎来 star

* (完成了 `hypergeom`) 先试试导入几个特殊函数， 例如 `gamma` 函数， `hypergeom`， 输出双精度类型就好。
## TODO
* 然后实现 `digits()` 函数， 重载 arf 类型的一些算符 `+, -, *, /` 等

## Ubuntu 使用说明
* 新建文件夹 `/opt/Baltamatica/plugins/arb`， 把 `main.so` 复制到里面。
* 把文件夹 `bin` 中所有文件都复制到 `/opt/Baltamatica/lib` 里面。

## Windows 使用说明
* 新建文件夹 `C:\baltamatica\plugins\arb`， 把 `main.dll` 复制到里面。
* 把文件夹 `windows\bin` 中所有文件都复制到 `C:\baltamatica\lib` 里面。

## Ubuntu 编译说明
* 把这个仓库 clone 到 `/opt/Baltamatica/bex/test/plugins/`。
* 目前在 Ubuntu 22.04 下编译测试， 直接运行 `./make.sh` 以及 `./install.sh`。

## Windows 编译说明
* 把这个仓库 clone 到 `C:\baltamatica\bex\test\plugins\`
* 双击打开群文件 `baltam_SDK/msys64-tmp` 中的 msys2.exe， 然后运行 `make-windows.sh` 以及 `./install-windows.sh`。
* 调试： 如果 baltam 加载 plugin 时出现错误 `The specified module could not be found.`， 说明缺少 `main.dll` 依赖的 `dll` 找不到。 在 msys2 里面用 `ldd main.dll` 可以查看所有依赖的其他 dll， 然后确认要么是 windows 的系统 dll， 要么复制到 `baltamatica\lib` 目录中。