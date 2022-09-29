## 尝试做一个 arblib 的接口
* [官方页面](https://arblib.org/)
* 另外用到了作者的个人计算库 [SLISC0](https://github.com/MacroUniverse/SLISC0) 欢迎来 star


* (完成了 hypergeom) 先试试导入几个特殊函数， 例如 gamma 函数， hypergeom， 输出双精度类型就好。
## TODO
* 然后实现 digits() 函数， 重载 arf 类型的一些算符 +, -, *, / 等

## 使用说明
目前使用以前需要把所有 .so 文件都复制/移动到 `/opt/Baltamatica/lib` 目录下面， 或者把 `/opt/Baltamatica/plugins/arb` 文件夹添加到环境变量 `LD_LIBRARY_PATH` 中

## 编译说明
* 目前在 Ubuntu 22.04 下编译测试， 直接运行 `./make.sh` 即可编译 `main.so` 其他文件的来源都是 Ubuntu 22.04 的 `sudo apt install libflint-arb`， 用 `ldd main.so` 就可以查看所有依赖库的目录。
