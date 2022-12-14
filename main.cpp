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
#include "SLISC/arb_extension.h"
#include "bex/bex.import.hpp"

#define PLUGIN_NAME "arb"

using namespace baltam;
using namespace slisc;

// double precision hypergeometric function, accurate to the last digit
// only supports 1F1 (real or complex params) for now
void hypergeom(int, bxArray*[], int, const bxArray*[]);
void gammaC(int, bxArray*[], int, const bxArray*[]);
void lgamma(int, bxArray*[], int, const bxArray*[]);
void coulombF(int, bxArray*[], int, const bxArray*[]);

void BigInt_create(int, bxArray*[], int, const bxArray*[]);
void BigInt_add(int, bxArray*[], int, const bxArray*[]);
void BigInt_sub(int, bxArray*[], int, const bxArray*[]);
void BigInt_mul(int, bxArray*[], int, const bxArray*[]);
void BigInt_div(int, bxArray*[], int, const bxArray*[]);

void BigFloat_create(int, bxArray*[], int, const bxArray*[]);
void digits(int, bxArray*[], int, const bxArray*[]);
void BigFloat_add(int, bxArray*[], int, const bxArray*[]);
void BigFloat_sub(int, bxArray*[], int, const bxArray*[]);
void BigFloat_mul(int, bxArray*[], int, const bxArray*[]);
void BigFloat_div(int, bxArray*[], int, const bxArray*[]);

static const char * hypergeom_help =
    "详见 Matlab 的 hypergeom 函数文档。";

static const char * gammaC_help =
    "复参数的 gamma 函数（也支持实数）。";

static const char * lgamma_help =
    "log(gammaC())";

static const char * coulombF_help =
    "第一类库伦函数";

static const char * BigInt_create_help =
    "生成一个大整数";

static const char * BigInt_add_help =
    "大整数相加";

static const char * BigInt_sub_help =
    "大整数相减";

static const char * BigInt_mul_help =
    "大整数相乘";

static const char * BigInt_div_help =
    "大整数相除";

static const char * BigFloat_create_help =
    "生成任意精度浮点数";

static const char * digits_help =
    "设置或查看 BigFloat 运算的有效数字位数";

static const char * BigFloat_add_help =
    "任意精度浮点数相加";

static const char * BigFloat_sub_help =
    "任意精度浮点数相减";

static const char * BigFloat_mul_help =
    "任意精度浮点数相乘";

static const char * BigFloat_div_help =
    "任意精度浮点数相除";

BALTAM_BEX_DEFINE_FCN_VARS

int bxPluginInitLib(void * hdl){
    bex::bex_handle = hdl;
    BALTAM_BEX_IMPORT_FCN_FROM(hdl)
    return 0;
}

struct BigInt : public extern_obj_base {
    Bint data;
    BALTAM_LOCAL static int ID;
    BigInt() = default;
    BigInt(Llong_I n): data(n) {}
    BigInt(Doub_I x): data(Llong(round(x))) {}
    BigInt(Str_I s): data(s) {}
    extern_obj_base *dup() const override;
    ~BigInt() override;
    // 可选：转化为字符串的实现
    string to_string() const override;
    // 自定义类型名字
    string classname() const override;
};

int BigInt::ID = 0;

extern_obj_base *BigInt::dup() const {
    BigInt *ret = new BigInt(*this);
    ret->data = data;
    return ret;
}

BigInt::~BigInt() {}

string BigInt::to_string() const { return ::to_string(data); }

string BigInt::classname() const { return "BigInt"; }

struct BigFloat : public extern_obj_base {
    Breal data;
    BALTAM_LOCAL static int ID;
    BigFloat() = default;
    BigFloat(Doub_I x): data(x) {}
    BigFloat(Str_I s): data(s) {}
    extern_obj_base *dup() const override;
    ~BigFloat() override;
    // 可选：转化为字符串的实现
    string to_string() const override;
    // 自定义类型名字
    string classname() const override;
};

int BigFloat::ID = 0;

extern_obj_base *BigFloat::dup() const {
    BigFloat *ret = new BigFloat(*this);
    ret->data = data;
    return ret;
}

BigFloat::~BigFloat() {}

string BigFloat::to_string() const { return ::to_string(data, 32); }

string BigFloat::classname() const { return "BigFloat"; }

int bxPluginInit(int, const bxArray*[]) {
    int BigInt_sid = bxAddCXXClass<BigInt>(PLUGIN_NAME, bex::__bxAddCXXClass_impl);
    bxRegisterBinaryOperator(PLUGIN_NAME, "+", BigInt_sid, BigInt_sid, BigInt_add);
    bxRegisterBinaryOperator(PLUGIN_NAME, "-", BigInt_sid, BigInt_sid, BigInt_sub);
    bxRegisterBinaryOperator(PLUGIN_NAME, "*", BigInt_sid, BigInt_sid, BigInt_mul);
    bxRegisterBinaryOperator(PLUGIN_NAME, "/", BigInt_sid, BigInt_sid, BigInt_div);

    int BigFloat_sid = bxAddCXXClass<BigFloat>(PLUGIN_NAME, bex::__bxAddCXXClass_impl);
    bxRegisterBinaryOperator(PLUGIN_NAME, "+", BigFloat_sid, BigFloat_sid, BigFloat_add);
    bxRegisterBinaryOperator(PLUGIN_NAME, "-", BigFloat_sid, BigFloat_sid, BigFloat_sub);
    bxRegisterBinaryOperator(PLUGIN_NAME, "*", BigFloat_sid, BigFloat_sid, BigFloat_mul);
    bxRegisterBinaryOperator(PLUGIN_NAME, "/", BigFloat_sid, BigFloat_sid, BigFloat_div);
    return 0;
}

int bxPluginFini() { flint_cleanup(); return 0; }

static bexfun_info_t flist[] = {
    {"hypergeom", hypergeom, hypergeom_help},
    {"gammaC", gammaC, gammaC_help},
    {"lgamma", lgamma, lgamma_help},
    {"coulombF", coulombF, coulombF_help},

    {"BigInt", BigInt_create, BigInt_create_help},
    {"BigInt_add", BigInt_add, BigInt_add_help},
    {"BigInt_sub", BigInt_sub, BigInt_sub_help},
    {"BigInt_mul", BigInt_mul, BigInt_mul_help},
    {"BigInt_div", BigInt_div, BigInt_div_help},

    {"BigFloat", BigFloat_create, BigFloat_create_help},
    {"digits", digits, digits_help},
    {"BigFloat_add", BigFloat_add, BigFloat_add_help},
    {"BigFloat_sub", BigFloat_sub, BigFloat_sub_help},
    {"BigFloat_mul", BigFloat_mul, BigFloat_mul_help},
    {"BigFloat_div", BigFloat_div, BigFloat_div_help},

    {"", nullptr, nullptr}
};

bexfun_info_t * bxPluginFunctions() { return flist; }

void BigInt_create(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    if (nlhs > 1 || nrhs != 1)
        bxErrMsgTxt("用法： BigInt(整数或字符串表示的整数)");
    BigInt *ret;
    if (bxIsRealDouble(prhs[0]))
        ret = bxNewCXXObject<BigInt>(*bxGetDoubles(prhs[0]));
    else if (bxIsString(prhs[0]))
        ret = bxNewCXXObject<BigInt>(bxGetStringDataPr(prhs[0]));
    plhs[0] = bex::__bxCreateExtObj_v(ret);
}

void BigInt_add(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    if (nlhs > 1 || nrhs != 2)
        bxErrMsgTxt("用法： BigInt_add(大整数, 大整数)");
    
    BigInt *px = bxGetExtObj<BigInt>(prhs[0], bex::__bxGetExtObj_impl);
    BigInt *py = bxGetExtObj<BigInt>(prhs[1], bex::__bxGetExtObj_impl);
    BigInt *ret = bxNewCXXObject<BigInt>();
    add(ret->data, px->data, py->data);
    plhs[0] = bex::__bxCreateExtObj_v(ret);
}

void BigInt_sub(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    if (nlhs > 1 || nrhs != 2)
        bxErrMsgTxt("用法： BigInt_sub(大整数, 大整数)");
    
    BigInt *px = bxGetExtObj<BigInt>(prhs[0], bex::__bxGetExtObj_impl);
    BigInt *py = bxGetExtObj<BigInt>(prhs[1], bex::__bxGetExtObj_impl);
    BigInt *ret = bxNewCXXObject<BigInt>();
    sub(ret->data, px->data, py->data);
    plhs[0] = bex::__bxCreateExtObj_v(ret);
}

void BigInt_mul(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    if (nlhs > 1 || nrhs != 2)
        bxErrMsgTxt("用法： BigInt_mul(大整数, 大整数)");
    
    BigInt *px = bxGetExtObj<BigInt>(prhs[0], bex::__bxGetExtObj_impl);
    BigInt *py = bxGetExtObj<BigInt>(prhs[1], bex::__bxGetExtObj_impl);
    BigInt *ret = bxNewCXXObject<BigInt>();
    mul(ret->data, px->data, py->data);
    plhs[0] = bex::__bxCreateExtObj_v(ret);
}

void BigInt_div(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    if (nlhs > 1 || nrhs != 2)
        bxErrMsgTxt("用法： BigInt_div(大整数, 大整数)");
    
    BigInt *px = bxGetExtObj<BigInt>(prhs[0], bex::__bxGetExtObj_impl);
    BigInt *py = bxGetExtObj<BigInt>(prhs[1], bex::__bxGetExtObj_impl);
    BigInt *ret = bxNewCXXObject<BigInt>();
    div(ret->data, px->data, py->data);
    plhs[0] = bex::__bxCreateExtObj_v(ret);
}

void BigFloat_create(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    if (nlhs > 1 || (nrhs != 1 && nrhs != 2))
        bxErrMsgTxt("用法： BigFloat(浮点数或字符串表示的浮点数)");
    BigFloat * ret;
    if (bxIsRealDouble(prhs[0])) {
        ret = bxNewCXXObject<BigFloat>(*bxGetDoubles(prhs[0]));
    }
    else if (bxIsString(prhs[0])) {
        ret = bxNewCXXObject<BigFloat>(bxGetStringDataPr(prhs[0]));
    }
    plhs[0] = bex::__bxCreateExtObj_v(ret);
}

void digits(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    if (nlhs > 1 || (nrhs != 0 && nrhs != 1))
        bxErrMsgTxt("用法： 旧有效位数 = BigFloat(新有效位数-可省略)");

    double old_digi = arb_digits();

    if (nrhs == 1) {
        if (!bxIsRealDouble(prhs[0]))
            bxErrMsgTxt("输入必须是双精度实数");
        if (bxGetM(prhs[0]) != 1 || bxGetN(prhs[0]) != 1)
            bxErrMsgTxt("不允许输入数组");
        double *px = bxGetDoubles(prhs[0]);
        arb_digits(*px);
    }

    plhs[0] = bxCreateDoubleMatrix(1, 1, bxREAL);
    double *py = bxGetDoubles(plhs[0]);
    *py = old_digi;
}

void BigFloat_add(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    if (nlhs > 1 || nrhs != 2)
        bxErrMsgTxt("用法： BigFloat_add(大整数, 大整数)");
    
    BigFloat *px = bxGetExtObj<BigFloat>(prhs[0], bex::__bxGetExtObj_impl);
    BigFloat *py = bxGetExtObj<BigFloat>(prhs[1], bex::__bxGetExtObj_impl);
    BigFloat *ret = bxNewCXXObject<BigFloat>();
    add(ret->data, px->data, py->data);
    plhs[0] = bex::__bxCreateExtObj_v(ret);
}

void BigFloat_sub(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    if (nlhs > 1 || nrhs != 2)
        bxErrMsgTxt("用法： BigFloat_sub(大整数, 大整数)");
    
    BigFloat *px = bxGetExtObj<BigFloat>(prhs[0], bex::__bxGetExtObj_impl);
    BigFloat *py = bxGetExtObj<BigFloat>(prhs[1], bex::__bxGetExtObj_impl);
    BigFloat *ret = bxNewCXXObject<BigFloat>();
    sub(ret->data, px->data, py->data);
    plhs[0] = bex::__bxCreateExtObj_v(ret);
}

void BigFloat_mul(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    if (nlhs > 1 || nrhs != 2)
        bxErrMsgTxt("用法： BigFloat_mul(大整数, 大整数)");
    
    BigFloat *px = bxGetExtObj<BigFloat>(prhs[0], bex::__bxGetExtObj_impl);
    BigFloat *py = bxGetExtObj<BigFloat>(prhs[1], bex::__bxGetExtObj_impl);
    BigFloat * ret = bxNewCXXObject<BigFloat>();
    mul(ret->data, px->data, py->data);
    plhs[0] = bex::__bxCreateExtObj_v(ret);
}

void BigFloat_div(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    if (nlhs > 1 || nrhs != 2)
        bxErrMsgTxt("用法： BigFloat_div(大整数, 大整数)");
    
    BigFloat *px = bxGetExtObj<BigFloat>(prhs[0], bex::__bxGetExtObj_impl);
    BigFloat *py = bxGetExtObj<BigFloat>(prhs[1], bex::__bxGetExtObj_impl);
    BigFloat *ret = bxNewCXXObject<BigFloat>();
    div(ret->data, px->data, py->data);
    plhs[0] = bex::__bxCreateExtObj_v(ret);
}

void hypergeom(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
    
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
#pragma omp parallel for
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
#pragma omp parallel for
            for (baSize i = 0; i < z_M*z_N; ++i)
                py[i] = arb_hypergeom1F1(a, b, pz[i]);
        }
        else {
            double *pz = bxGetDoubles(prhs[2]);
#pragma omp parallel for
            for (baSize i = 0; i < z_M*z_N; ++i)
                py[i] = arb_hypergeom1F1(a, b, pz[i]);
        }
    }
}

void gammaC(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
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
#pragma omp parallel for
        for (baSize i = 0; i < z_M*z_N; ++i)
            py[i] = arb_gamma(pz[i]);
    }
    else { // complex args
        plhs[0] = bxCreateDoubleMatrix(z_M, z_N, bxCOMPLEX);
        Comp *pz = (Comp *)bxGetComplexDoubles(prhs[0]);
        Comp *py = (Comp *)bxGetComplexDoubles(plhs[0]);
#pragma omp parallel for
        for (baSize i = 0; i < z_M*z_N; ++i) {
            if (pz[i].imag() == 0)
                py[i] = arb_gamma(pz[i].real());
            else
                py[i] = arb_gamma(pz[i]);
        }
    }
}

void lgamma(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
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
#pragma omp parallel for
        for (baSize i = 0; i < z_M*z_N; ++i)
            py[i] = arb_lngamma(pz[i]);
    }
    else { // complex args
        plhs[0] = bxCreateDoubleMatrix(z_M, z_N, bxCOMPLEX);
        Comp *pz = (Comp *)bxGetComplexDoubles(prhs[0]);
        Comp *py = (Comp *)bxGetComplexDoubles(plhs[0]);
#pragma omp parallel for
        for (baSize i = 0; i < z_M*z_N; ++i) {
            if (pz[i].imag() == 0)
                py[i] = arb_lngamma(pz[i].real());
            else
                py[i] = arb_lngamma(pz[i]);
        }
    }
}

void coulombF(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[]) {
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
#pragma omp parallel for
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
#pragma omp parallel for
            for (baSize i = 0; i < z_M*z_N; ++i)
                py[i] = acb_coulombF(l, eta, pz[i]);
        }
        else {
            double *pz = bxGetDoubles(prhs[2]);
#pragma omp parallel for
            for (baSize i = 0; i < z_M*z_N; ++i)
                py[i] = acb_coulombF(l, eta, pz[i]);
        }
    }
}

// 用于直接编译成可执行文件进行调试， 而不是动态链接库
#if defined(ARB_BUILD_EXE)
int main(){
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
