#include "../include/test_factory.h"
#include "../include/fp_utils.h"
#include <vector>
#include <cstdio>

void add_fp16_widen_tests(std::vector<TestCase>& tests) {
    // -- FP16 widen 测试 --
    tests.push_back(TestCase(FADD_Operands_FP16_Widen{0x3c00, 0x4000}, ErrorType::Precise)); // 1.0 + 2.0 = 3.0
    tests.push_back(TestCase(FADD_Operands_FP16_Widen{0xbc00, 0x4000}, ErrorType::Precise)); // -1.0 + 2.0 = 1.0
    tests.push_back(TestCase(FADD_Operands_FP16_Widen{0x3c00, 0xbc00}, ErrorType::Precise)); // 1.0 + -1.0 = 0.0
    tests.push_back(TestCase(FADD_Operands_FP16_Widen{0x0000, 0x4000}, ErrorType::Precise)); // 0.0 + 2.0 = 2.0
  
    printf("\n---- Random tests for FP16 Widen ----\n");
    int num_random_tests_fp16_widen = 200;
    // ---- FP16 widen 任意值随机测试 ----
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_any_fp16(), gen_any_fp16()};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    // 更多不同范围的随机测试...
    // 正常范围测试
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(-10, 10), gen_random_fp16(-10, 10)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    // 小数范围测试 - FP16指数范围
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(-15, -5), gen_random_fp16(-15, -5)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    // 大数范围测试 - FP16指数范围  
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(5, 15), gen_random_fp16(5, 15)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    // 混合指数范围测试
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(-15, 15), gen_random_fp16(-15, 15)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    // 非规格化数边界测试 - FP16
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(-15, -14), gen_random_fp16(-15, 15)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(-15, 15), gen_random_fp16(-15, -14)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    // 极端范围测试 - 接近FP16溢出
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(14, 15), gen_random_fp16(14, 15)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    // 极端下溢测试 - 接近FP16下溢
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(-15, -14), gen_random_fp16(-15, -14)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    // 高精度FP32 c值测试
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(-5, 5), gen_random_fp16(-5, 5)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(-5, 5), gen_random_fp16(-5, 5)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    // 全范围随机测试 - 最全面的测试
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(-15, 15), gen_random_fp16(-15, 15)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    // 特殊组合测试 - 一个操作数极大，另一个极小
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(10, 15), gen_random_fp16(-15, -10)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(-15, -10), gen_random_fp16(10, 15)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
} 