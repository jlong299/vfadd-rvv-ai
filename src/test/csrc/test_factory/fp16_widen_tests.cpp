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
    // 正常范围测试
    for (int i = 0; i < num_random_tests_fp16_widen; ++i) {
        FADD_Operands_FP16_Widen ops = {gen_random_fp16(-10, 10), gen_random_fp16(-10, 10)};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    // 更多不同范围的随机测试...
} 