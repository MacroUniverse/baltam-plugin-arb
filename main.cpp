/*
 * ==========================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  main file for "arb plugin"
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Hongyu Shi (@addis), addis1@phys.ksu.edu.cn
 *   Organization:  Kansas State University
 *
 * ==========================================================================
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "bex/bex.hpp"
#include "SLISC/hypergeom.h"
#ifdef BV_USE_DYN_LOADER
#include "bex/bex.import.hpp"
#endif

#define PLUGIN_NAME "arb"

using namespace baltam;

// double precision hypergeometric function, accurate to the last digit
// only supports 1F1 (real or complex params) for now
void hypergeom(int, bxArray*[], int, const bxArray*[]);

static const char * hypergeom_help =
    "详见 Matlab 的 hypergeom 函数文档。";

#ifdef BV_USE_DYN_LOADER
BALTAM_BEX_DEFINE_FCN_VARS

int bxPluginInitLib(void * hdl){
    bex::bex_handle = hdl;
    BALTAM_BEX_IMPORT_FCN_FROM(hdl)
    return 0;
}
#endif

int bxPluginInit(int, const bxArray*[]){
    return 0;
}

int bxPluginFini(){ return 0; }

static bexfun_info_t flist[] = {
    {"hypergeom", hypergeom, hypergeom_help},
    {"", nullptr, nullptr}
};

bexfun_info_t * bxPluginFunctions(){
    return flist;
}


void hypergeom(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    using namespace std;
    bex::__bxPrintf("\n测试 hypergeom！\n");
    // 【不管用】 bxPrintf("测试一下 bxPrintf 管不管用");
    // 【不管用】 cout << "测试一下 cout 管不管用" << endl;
#ifndef BV_USE_DYN_LOADER
    bex::__bxErrMsgTxt("目前仅支持动态编译！");
#endif

    if (nrhs != 3) {
        bex::__bxErrMsgTxt("用法： y = hypergeom(a, b, c)");
        return;
    }

    double *pa = bxGetDoubles(prhs[0]);
    double *pb = bxGetDoubles(prhs[1]);
    double *pz = bxGetDoubles(prhs[2]);
    baSize z_M = bxGetM(prhs[2]), z_N = bxGetN(prhs[2]);

    double a = *pa, b = *pb, c = *pz;
    // bex::__bxPrintf(("a = " + to_string(a) + "\n").c_str());
    // bex::__bxPrintf(("b = " + to_string(b) + "\n").c_str());
    // bex::__bxPrintf(("c = " + to_string(c) + "\n").c_str());

    if (nlhs > 1) {
        bex::__bxErrMsgTxt("只允许 <= 1 个输出");
        return;
    }

    plhs[0] = bxCreateDoubleMatrix(z_M, z_N, bxREAL); // bxCOMPLEX
    double *py = bxGetDoubles(plhs[0]);
    for (baSize i = 0; i < z_M*z_N; ++i)
        py[i] = slisc::arb_hypergeom1F1(a, b, pz[i]);

    // bex::__bxPrintf("hypergeom 终止！\n");
    // bex::__bxErrMsgTxt("调试终止!");
}

#if defined(BV_USE_DYN_LOADER) && defined(BV_BUILD_EXE)
int main(){
    std::vector<const bxArray*> in_args;
    std::vector<bxArray*> out_args;
    // 手动调用 init 函数
#if defined(__WIN32__) || defined(__CYGWIN__) || defined(__MINGW32__)
    void * handle = (void*)LoadLibrary("libbex.dll");
#else
    void * handle = dlopen("libbex.so", RTLD_NOW);
#endif

    bxPluginInitLib(handle);
    // 这里 addCXXClass 其实不会被执行，因为已经绕过了插件管理器
    bxPluginInit(0, nullptr);
    std::cout << "bex 库已载入" << std::endl;

    // 从内核创建数据
    bxArray *A = bex::__bxCreateNumericMatrix(1, 1, bxINT_CLASS, bxREAL);
    bxArray *B = bex::__bxCreateNumericMatrix(1, 1, bxDOUBLE_CLASS, bxREAL);
    bxArray *C = bex::__bxCreateString("test");

    // 修改数据
    int *pA = bex::__bxGetInt32s(A);
    *pA = 42;
    double *pB = bex::__bxGetDoubles(B);
    *pB = 3.14;

    // 调用 bv_create
    in_args.push_back(A);
    in_args.push_back(B);
    in_args.push_back(C);
    out_args.push_back(nullptr);
    bv_create(out_args.size(), out_args.data(), in_args.size(), in_args.data());

    bxArray *V = out_args[0];

    // 调用 bv_size
    in_args.clear();
    out_args.clear();
    in_args.push_back(V);
    out_args.push_back(nullptr);
    bv_size(out_args.size(), out_args.data(), in_args.size(), in_args.data());

    bxArray *N = out_args[0];

    std::cout << "数组的大小是：" << static_cast<int>(*bex::__bxGetDoubles(N)) << std::endl;

    // 调用 bv_show
    in_args.clear();
    out_args.clear();
    in_args.push_back(V);
    bv_show(out_args.size(), out_args.data(), in_args.size(), in_args.data());

    // 调用 bv_at
    in_args.clear();
    out_args.clear();
    bxArray *ii = bex::__bxCreateNumericMatrix(1, 1, bxDOUBLE_CLASS, bxREAL);
    double *pii = bex::__bxGetDoubles(ii);
    *pii = 2;
    in_args.push_back(V);
    in_args.push_back(ii);
    out_args.push_back(nullptr);
    bv_at(out_args.size(), out_args.data(), in_args.size(), in_args.data());

    bxArray *ch = out_args[0];
    char *t = bex::__bxGetChars(ch);
    std::cout << t[0] << t[1] << t[2] << t[3] << std::endl;

    // 释放内存
    bex::__bxDestroyArray(A);
    bex::__bxDestroyArray(B);
    bex::__bxDestroyArray(C);
    bex::__bxDestroyArray(V);
    bex::__bxDestroyArray(N);
    bex::__bxDestroyArray(ii);
    bex::__bxDestroyArray(ch);

#if defined(__WIN32__) || defined(__CYGWIN__) || defined(__MINGW32__)
    FreeLibrary((HMODULE)handle);
#else
    dlclose(handle);
#endif

    return 0;
}
#endif
