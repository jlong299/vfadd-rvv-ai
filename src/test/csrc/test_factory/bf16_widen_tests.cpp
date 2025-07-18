#include "../include/test_factory.h"
#include "../include/fp_utils.h"
#include <vector>
#include <cstdio>

void add_bf16_widen_tests(std::vector<TestCase>& tests) {
    // -- BF16 widen 测试 --
    tests.push_back(TestCase(FADD_Operands_BF16_Widen{0x3f80, 0x4000}, ErrorType::Precise)); // 1.0 + 2.0 = 3.0
    tests.push_back(TestCase(FADD_Operands_BF16_Widen{0xbf80, 0x4000}, ErrorType::Precise)); // -1.0 + 2.0 = 1.0
    tests.push_back(TestCase(FADD_Operands_BF16_Widen{0x3f80, 0xbf80}, ErrorType::Precise)); // 1.0 + -1.0 = 0.0
    tests.push_back(TestCase(FADD_Operands_BF16_Widen{0x0000, 0x4000}, ErrorType::Precise)); // 0.0 + 2.0 = 2.0

    printf("\n---- Random tests for BF16 Widen ----\n");
    int num_random_tests_bf16_widen = 200;
    // ---- BF16 widen 任意值随机测试 ----
    for (int i = 0; i < num_random_tests_bf16_widen; ++i) {
        FADD_Operands_BF16_Widen ops = {gen_any_bf16(), gen_any_bf16()};
        tests.push_back(TestCase(ops, ErrorType::ULP));
    }
    // 更多不同范围的随机测试...
} 