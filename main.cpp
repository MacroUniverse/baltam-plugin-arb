/*
 * ==========================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  main file for "arb plugin"
 *
 *       Version:  0.1
 *       Compiler:  g++
 *
 *         Author:  Hongyu Shi (@shi), shi@phys.ksu.edu.cn
 *   Organization:  Kansas State University
 *
 * ==========================================================================
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <complex>
#include "bex/bex.hpp"
#include "SLISC/hypergeom.h"
#include "SLISC/coulomb.h"
#include "bex/bex.import.hpp"

#define PLUGIN_NAME "arb"

using namespace baltam;

// double precision hypergeometric function, accurate to the last digit
// only supports 1F1 (real or complex params) for now
void hypergeom(int, bxArray*[], int, const bxArray*[]);
void gammaC(int, bxArray*[], int, const bxArray*[]);
void lgamma(int, bxArray*[], int, const bxArray*[]);
void coulombF(int, bxArray*[], int, const bxArray*[]);

static const char * hypergeom_help =
    "详见 Matlab 的 hypergeom 函数文档。";

static const char * gammaC_help =
    "复参数的 gamma 函数（也支持实数）。";

static const char * lgamma_help =
    "log(gammaC())";

static const char * coulombF_help =
    "第一类库伦函数";

BALTAM_BEX_DEFINE_FCN_VARS

int bxPluginInitLib(void * hdl){
    bex::bex_handle = hdl;
    BALTAM_BEX_IMPORT_FCN_FROM(hdl)
    return 0;
}

int bxPluginInit(int, const bxArray*[]){
    return 0;
}

int bxPluginFini(){ return 0; }

static bexfun_info_t flist[] = {
    {"hypergeom", hypergeom, hypergeom_help},
    {"gammaC", gammaC, gammaC_help},
    {"lgamma", lgamma, lgamma_help},
    {"coulombF", coulombF, coulombF_help},
    {"", nullptr, nullptr}
};

bexfun_info_t * bxPluginFunctions() {
    return flist;
}

void hypergeom(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    using namespace slisc;
    // bex::__bxPrintf("整数为 %ld， 浮点数为 %g， 字符串为 %s\n", 3, 3.1415926, "一些字符串");
    // bxPrintf("整数为 %ld， 浮点数为 %g， 字符串为 %s\n", 3, 3.1415926, "一些字符串");
    // 【不管用】 cout << "测试一下 cout 管不管用" << endl;

    if (nrhs != 3)
        bxErrMsgTxt("用法： hypergeom(a, b, z)， 其中 a, b 为标量， z 为实数或复数矩阵。");

    bool arg_comp[3]; bool has_comp = false;
    for (int i = 0; i < 3; ++i) {
        if (bxIsComplexDouble(prhs[i]))
            has_comp = arg_comp[i] = true;
        else if (bxIsRealDouble(prhs[i]))
            arg_comp[i] = false;
        else
            bxErrMsgTxt("参数必须是双精度实数或复数！");
    }
    if (bxGetM(prhs[0])*bxGetN(prhs[0]) != 1 || bxGetM(prhs[1])*bxGetN(prhs[1]) != 1)
        bxErrMsgTxt("用法： hypergeom(a, b, z)， 其中 a, b 为标量， z 为实数或复数矩阵。");

    if (nlhs > 1)
        bxErrMsgTxt("只允许 <= 1 个输出");
    baSize z_M = bxGetM(prhs[2]), z_N = bxGetN(prhs[2]);

    if (!has_comp) { // real args
        double a = *bxGetDoubles(prhs[0]);
        double b = *bxGetDoubles(prhs[1]);
        double *pz = bxGetDoubles(prhs[2]);
        

        plhs[0] = bxCreateDoubleMatrix(z_M, z_N, bxREAL);
        double *py = bxGetDoubles(plhs[0]);
        for (baSize i = 0; i < z_M*z_N; ++i)
            py[i] = arb_hypergeom1F1(a, b, pz[i]);
    }
    else { // complex args
        Comp a, b;
        if (arg_comp[0])
            a = *((Comp *)bxGetComplexDoubles(prhs[0]));
        else
            a = *bxGetDoubles(prhs[0]);

        if (arg_comp[1])
            b = *(Comp *)bxGetComplexDoubles(prhs[1]);
        else
            b = *bxGetDoubles(prhs[1]);

        plhs[0] = bxCreateDoubleMatrix(z_M, z_N, bxCOMPLEX);
        Comp *py = (Comp *)bxGetComplexDoubles(plhs[0]);

        if (arg_comp[2]) {
            Comp *pz = (Comp *)bxGetComplexDoubles(prhs[2]);
            for (baSize i = 0; i < z_M*z_N; ++i)
                py[i] = arb_hypergeom1F1(a, b, pz[i]);
        }
        else {
            double *pz = bxGetDoubles(prhs[2]);
            for (baSize i = 0; i < z_M*z_N; ++i)
                py[i] = arb_hypergeom1F1(a, b, pz[i]);
        }
    }
}

void gammaC(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    using namespace slisc;

    if (nrhs != 1)
        bxErrMsgTxt("用法： gammaC(z)， 其中 z 为实数或复数矩阵。");

    bool is_comp;
    if (bxIsComplexDouble(prhs[0]))
        is_comp = true;
    else if (bxIsRealDouble(prhs[0]))
        is_comp = false;
    else
        bxErrMsgTxt("参数必须是双精度实数或复数！");

    if (nlhs > 1)
        bxErrMsgTxt("只允许 <= 1 个输出");
    baSize z_M = bxGetM(prhs[0]), z_N = bxGetN(prhs[0]);

    if (!is_comp) { // real args
        double *pz = bxGetDoubles(prhs[0]);
        plhs[0] = bxCreateDoubleMatrix(z_M, z_N, bxREAL);
        double *py = bxGetDoubles(plhs[0]);
        for (baSize i = 0; i < z_M*z_N; ++i)
            py[i] = arb_gamma(pz[i]);
    }
    else { // complex args
        plhs[0] = bxCreateDoubleMatrix(z_M, z_N, bxCOMPLEX);
        Comp *pz = (Comp *)bxGetComplexDoubles(prhs[0]);
        Comp *py = (Comp *)bxGetComplexDoubles(plhs[0]);
        for (baSize i = 0; i < z_M*z_N; ++i) {
            if (pz[i].imag() == 0)
                py[i] = arb_gamma(pz[i].real());
            else
                py[i] = arb_gamma(pz[i]);
        }
    }
}

void lgamma(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    using namespace slisc;

    if (nrhs != 1)
        bxErrMsgTxt("用法： gammaC(z)， 其中 z 为实数或复数矩阵。");

    bool is_comp;
    if (bxIsComplexDouble(prhs[0]))
        is_comp = true;
    else if (bxIsRealDouble(prhs[0]))
        is_comp = false;
    else
        bxErrMsgTxt("参数必须是双精度实数或复数！");

    if (nlhs > 1)
        bxErrMsgTxt("只允许 <= 1 个输出");
    baSize z_M = bxGetM(prhs[0]), z_N = bxGetN(prhs[0]);

    if (!is_comp) { // real args
        double *pz = bxGetDoubles(prhs[0]);
        plhs[0] = bxCreateDoubleMatrix(z_M, z_N, bxREAL);
        double *py = bxGetDoubles(plhs[0]);
        for (baSize i = 0; i < z_M*z_N; ++i)
            py[i] = arb_lngamma(pz[i]);
    }
    else { // complex args
        plhs[0] = bxCreateDoubleMatrix(z_M, z_N, bxCOMPLEX);
        Comp *pz = (Comp *)bxGetComplexDoubles(prhs[0]);
        Comp *py = (Comp *)bxGetComplexDoubles(plhs[0]);
        for (baSize i = 0; i < z_M*z_N; ++i) {
            if (pz[i].imag() == 0)
                py[i] = arb_lngamma(pz[i].real());
            else
                py[i] = arb_lngamma(pz[i]);
        }
    }
}

void coulombF(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    using namespace slisc;
    if (nrhs != 3)
        bxErrMsgTxt("用法： coulombF(l, eta, z)， 其中 l, eta 为标量， z 为实数或复数矩阵。");

    bool arg_comp[3]; bool has_comp = false;
    for (int i = 0; i < 3; ++i) {
        if (bxIsComplexDouble(prhs[i]))
            has_comp = arg_comp[i] = true;
        else if (bxIsRealDouble(prhs[i]))
            arg_comp[i] = false;
        else
            bxErrMsgTxt("参数必须是双精度实数或复数！");
    }
    if (bxGetM(prhs[0])*bxGetN(prhs[0]) != 1 || bxGetM(prhs[1])*bxGetN(prhs[1]) != 1)
        bxErrMsgTxt("用法： coulombF(l, eta, z)， 其中 l, eta 为标量， z 为实数或复数矩阵。");

    if (nlhs > 1)
        bxErrMsgTxt("只允许 <= 1 个输出");
    baSize z_M = bxGetM(prhs[2]), z_N = bxGetN(prhs[2]);

    if (!has_comp) { // real args
        double l = *bxGetDoubles(prhs[0]);
        double eta = *bxGetDoubles(prhs[1]);
        double *pz = bxGetDoubles(prhs[2]);

        plhs[0] = bxCreateDoubleMatrix(z_M, z_N, bxREAL);
        double *py = bxGetDoubles(plhs[0]);
        for (baSize i = 0; i < z_M*z_N; ++i)
            py[i] = arb_coulombF(l, eta, pz[i]);
    }
    else { // complex args
        Comp l, eta;
        if (arg_comp[0])
            l = *((Comp *)bxGetComplexDoubles(prhs[0]));
        else
            l = *bxGetDoubles(prhs[0]);

        if (arg_comp[1])
            eta = *(Comp *)bxGetComplexDoubles(prhs[1]);
        else
            eta = *bxGetDoubles(prhs[1]);

        plhs[0] = bxCreateDoubleMatrix(z_M, z_N, bxCOMPLEX);
        Comp *py = (Comp *)bxGetComplexDoubles(plhs[0]);

        if (arg_comp[2]) {
            Comp *pz = (Comp *)bxGetComplexDoubles(prhs[2]);
            for (baSize i = 0; i < z_M*z_N; ++i)
                py[i] = acb_coulombF(l, eta, pz[i]);
        }
        else {
            double *pz = bxGetDoubles(prhs[2]);
            for (baSize i = 0; i < z_M*z_N; ++i)
                py[i] = acb_coulombF(l, eta, pz[i]);
        }
    }
}

// 用于直接编译成可执行文件进行调试， 而不是动态链接库
#if defined(ARB_BUILD_EXE)
int main(){
    using namespace slisc;
    vector<const bxArray*> in_args;
    vector<bxArray*> out_args;
    // 手动调用 init 函数
#if defined(__WIN32__) || defined(__CYGWIN__) || defined(__MINGW32__)
    void * handle = (void*)LoadLibrary("libbex.dll");
#else
    void * handle = dlopen("libbex.so", RTLD_NOW);
#endif

    bxPluginInitLib(handle);
    // 这里 addCXXClass 其实不会被执行，因为已经绕过了插件管理器
    bxPluginInit(0, NULL);
    cout << "bex 库已载入" << endl;

    // 从内核创建数据
    bxArray *a = bex::__bxCreateNumericMatrix(1, 1, bxDOUBLE_CLASS, bxREAL);
    bxArray *b = bex::__bxCreateNumericMatrix(1, 1, bxDOUBLE_CLASS, bxCOMPLEX);
    bxArray *z = bex::__bxCreateNumericMatrix(1, 2, bxDOUBLE_CLASS, bxCOMPLEX);

    // 修改数据
    *bex::__bxGetDoubles(a) = 1.6;
    Comp *b = (Comp *)bex::__bxGetDoubles(b);
    b = Comp(1.3, 0.6);
    Comp *z = (Comp *)bex::__bxGetDoubles(z);
    z[0] = Comp(1.3, 2.1);
    z[1] = Comp(-0.2, 2.4);

    // 调用 bv_create
    in_args.push_back(a); in_args.push_back(b);
    out_args.push_back(NULL);
    hypergeom(out_args.size(), out_args.data(), in_args.size(), in_args.data());
    
    // 释放内存
    bex::__bxDestroyArray(a);
    bex::__bxDestroyArray(b);
    bex::__bxDestroyArray(z);

#if defined(__WIN32__) || defined(__CYGWIN__) || defined(__MINGW32__)
    FreeLibrary((HMODULE)handle);
#else
    dlclose(handle);
#endif

    return 0;
}
#endif
