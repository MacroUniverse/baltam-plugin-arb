## 尝试给北太天元 Baltam 做一个 arblib 的接口
* [北太天元官网](http://www.baltamatica.com/product/NCS.html)
* arblib 是一个任意精度计算库， 用 ball arithmetic （类似于 interval arithmetic）追踪误差传递
* Mathematica 中把函数值精确到任意位小数的功能就是通过 arblib 实现的（至少一部分）。
* [arblib 的官方页面](https://arblib.org/)
* 另外用到了作者的个人计算库 [SLISC0](https://github.com/MacroUniverse/SLISC0) 欢迎来 star :)

## 已完成
* `test.m` 中对所有实现的函数进行测试。
* 超几何函数 `hypergeom(a,b,z)`： 输入输出双精度实数或者复数。 使用说明见[这里](https://www.mathworks.com/help/symbolic/hypergeom.html)， 但目前 `a,b` 不支持向量， `z` 支持向量/矩阵。 例子： `hypergeom(1.1+0.1i, 2.2, [2.4-0.1i, 1.5])`： 这相当于分别使用 `hypergeom(1.1+0.1i, 2.2, 2.4-0.1i)` 和 `hypergeom(1.1+0.1i, 2.2, 1.5)`。
* Γ 函数 `gammaC(z)`： 输入输出双精度实数或者复数矩阵。
* Γ 函数的对数 `lgamma(z)`： 输入输出双精度实数或者复数矩阵。
* 第一类库仑函数 `coulombF(l, eta, z)`： 其中 `l`, `eta` 是实或复标量， `z` 是实或复矩阵。
* 大整数 `a = BigInt(双精度整数或字符串)` 生成大整数类型， 四则运算如 `a + b`, `a * b`

## TODO
* 实现其他特殊函数， 以及任意精度版本
* 然后实现 `digits()` 函数， 重载 arf 类型的一些算符 `+, -, *, /` 等

## Ubuntu 使用说明
* 新建文件夹 `/opt/Baltamatica/plugins/arb`， 把 `main.so` 复制到里面。
* 把文件夹 `bin` 中所有文件都复制到 `/opt/Baltamatica/lib` 里面。
* 在帮助菜单中选择 `plugin`， 刷新插件， 然后勾选 `arb`， 应用即可。

## Windows 使用说明
* 新建文件夹 `C:\baltamatica\plugins\arb`， 把 `main.dll` 复制到里面。
* 把文件夹 `windows\bin-arb4win` 中所有文件都复制到 `C:\baltamatica\lib` 里面。 如果有问题就把 `windows\bin-mingw64` 中的所有文件也复制过去（选择覆盖）。
* 在帮助菜单中选择 `plugin`， 刷新插件， 然后勾选 `arb`， 应用即可。

## Ubuntu 编译说明
* 把这个仓库 clone 到任意位置。
* 目前在 Ubuntu 22.04 下编译测试， 直接运行 `./make.sh` 以及 `./install.sh`。

## Windows 编译说明
* 把这个仓库 clone 到任意位置。
* 双击打开群文件 `baltam_SDK/msys64-tmp` 中的 `mingw64.exe`， 然后运行 `./make-windows.sh` 以及 `./install-windows.sh`。
* 调试： 如果 baltam 加载 plugin 时出现错误 `The specified module could not be found.`， 说明缺少 `main.dll` 依赖的 `dll` 找不到。 在 msys2 里面用 `ldd main.dll` 可以查看所有依赖的其他 dll， 然后确认要么是 windows 的系统 dll， 要么复制到 `baltamatica\lib` 目录中。
