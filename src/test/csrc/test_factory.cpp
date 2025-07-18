#include "include/test_factory.h"
#include "include/fp_utils.h"
#include <vector>
#include <cstdio>

std::vector<TestCase> create_all_tests() {
    std::vector<TestCase> tests;
  
    bool test_fp32 = true;
    bool test_fp16 = true;
    bool test_bf16 = true;
    bool test_fp16_widen = true;
    bool test_bf16_widen = true;
  
    if (test_fp32) {
        // -- FP32 单精度浮点数测试 --
        tests.push_back(TestCase(FADD_Operands_Hex{0xC0A00000, 0xC0E00000}, ErrorType::Precise)); // -5.0f + -7.0f = -12.0f
        tests.push_back(TestCase(FADD_Operands_Hex{0x3F800000, 0x40000000}, ErrorType::Precise)); // 1.0f + 2.0f = 3.0f
        tests.push_back(TestCase(FADD_Operands_Hex{0x40200000, 0x41200000}, ErrorType::Precise)); // 2.5f + 10.0f = 12.5f
        tests.push_back(TestCase(FADD_Operands_Hex{0x00000000, 0x42F6E666}, ErrorType::Precise)); // 0.0f + 123.45f = 123.45f
        tests.push_back(TestCase(FADD_Operands_Hex{0xC2F6E666, 0x00000000}, ErrorType::Precise)); // -123.45f + 0.0f = -123.45f
        tests.push_back(TestCase(FADD_Operands_Hex{0x42F60000, 0xC2860000}, ErrorType::Precise)); // 123.0f + -67.0f = 56.0f
        tests.push_back(TestCase(FADD_Operands_Hex{0x40A00000, 0x42F60000}, ErrorType::Precise)); // 5.0f + 123.0f = 128.0f
        tests.push_back(TestCase(FADD_Operands_Hex{0x40A00000, 0x40E00000}, ErrorType::Precise)); // 5.0f + 7.0f = 12.0f
        tests.push_back(TestCase(FADD_Operands_Hex{0xbf7f7861, 0x7bede2c6}, ErrorType::ULP));
        tests.push_back(TestCase(FADD_Operands_Hex{0x58800c00, 0x58800400}, ErrorType::RelativeError));
        tests.push_back(TestCase(FADD_Operands_Hex{0x816849E7, 0x00B6D8A2}, ErrorType::ULP));

        printf("\n---- Random tests for FP32 ----\n");
        int num_random_tests_32 = 200;
        // ---- FP32 任意值随机测试 ----
        for (int i = 0; i < num_random_tests_32; ++i) {
            FADD_Operands_Hex ops = {gen_any_fp32(), gen_any_fp32()};
            tests.push_back(TestCase(ops, ErrorType::RelativeError));
        }
        // ---- 进行不同指数范围的测试 ----
        // 小数范围测试：指数[-50, -10]
        for (int i = 0; i < num_random_tests_32; ++i) {
            FADD_Operands_Hex ops = {gen_random_fp32(-50, -10), gen_random_fp32(-50, -10)};
            tests.push_back(TestCase(ops, ErrorType::RelativeError));
        }
        // 中等数值范围测试：指数[-10, 10]
        for (int i = 0; i < num_random_tests_32; ++i) {
            FADD_Operands_Hex ops = {gen_random_fp32(-10, 10), gen_random_fp32(-10, 10)};
            tests.push_back(TestCase(ops, ErrorType::RelativeError));
        }
        // 大数范围测试：指数[10, 50]
        for (int i = 0; i < num_random_tests_32; ++i) {
            FADD_Operands_Hex ops = {gen_random_fp32(10, 50), gen_random_fp32(10, 50)};
            tests.push_back(TestCase(ops, ErrorType::RelativeError));
        }
        // 更多测试
        for (int i = 0; i < num_random_tests_32; ++i) {
            FADD_Operands_Hex ops = {gen_random_fp32(-126, 20), gen_random_fp32(-126, 20)};
            tests.push_back(TestCase(ops, ErrorType::RelativeError));
        }
        for (int i = 0; i < num_random_tests_32; ++i) {
            FADD_Operands_Hex ops = {gen_random_fp32(-126, 20), gen_random_fp32(-127, -126)};
            tests.push_back(TestCase(ops, ErrorType::RelativeError));
        }
        for (int i = 0; i < num_random_tests_32; ++i) {
            FADD_Operands_Hex ops = {gen_random_fp32(-127, -126), gen_random_fp32(-126, 20)};
            tests.push_back(TestCase(ops, ErrorType::RelativeError));
        }
        for (int i = 0; i < num_random_tests_32; ++i) {
            FADD_Operands_Hex ops = {gen_random_fp32(-127, 10), gen_random_fp32(-127, 10)};
            tests.push_back(TestCase(ops, ErrorType::RelativeError));
        }
    }

    if (test_fp16) {
        // -- FP16 并行双路半精度浮点数测试 --
        tests.push_back(TestCase(FADD_Operands_Hex_16{0x3c00, 0x4000}, FADD_Operands_Hex_16{0x4200, 0x3c00}, ErrorType::Precise)); // 1.0 + 2.0 = 3.0 | 3.0 + 1.0 = 4.0
        tests.push_back(TestCase(FADD_Operands_Hex_16{0xbc00, 0x4000}, FADD_Operands_Hex_16{0x3c00, 0xc000}, ErrorType::Precise)); // -1.0 + 2.0 = 1.0 | 1.0 + -2.0 = -1.0
        tests.push_back(TestCase(FADD_Operands_Hex_16{0x3c00, 0xbc00}, FADD_Operands_Hex_16{0x4400, 0x3800}, ErrorType::Precise)); // 1.0 + -1.0 = 0.0 | 4.0 + 0.5 = 4.5
        // 零值测试
        tests.push_back(TestCase(FADD_Operands_Hex_16{0x0000, 0x4000}, FADD_Operands_Hex_16{0x4000, 0x0000}, ErrorType::Precise)); // 0.0 + 2.0 = 2.0 | 2.0 + 0.0 = 2.0
        tests.push_back(TestCase(FADD_Operands_Hex_16{0x3c00, 0x0000}, FADD_Operands_Hex_16{0x0000, 0x4200}, ErrorType::Precise)); // 1.0 + 0.0 = 1.0 | 0.0 + 3.0 = 3.0
        // 无穷大测试
        tests.push_back(TestCase(FADD_Operands_Hex_16{0x7c00, 0x4000}, FADD_Operands_Hex_16{0x3c00, 0xfc00}, ErrorType::Precise)); // +inf + 2.0 = +inf | 1.0 + -inf = -inf
        tests.push_back(TestCase(FADD_Operands_Hex_16{0x3c00, 0x7c00}, FADD_Operands_Hex_16{0x7c00, 0x7c00}, ErrorType::Precise)); // 1.0 + +inf = +inf | +inf + inf = +inf
        // 非规格化数测试
        tests.push_back(TestCase(FADD_Operands_Hex_16{0x0001, 0x4000}, FADD_Operands_Hex_16{0x03ff, 0x3c00}, ErrorType::Precise)); // 最小非规格化数 + 2.0 | 最大非规格化数 + 1.0
        tests.push_back(TestCase(FADD_Operands_Hex_16{0x3c00, 0x0001}, FADD_Operands_Hex_16{0x4000, 0x03ff}, ErrorType::Precise)); // 1.0 + 最小非规格化数 | 2.0 + 最大非规格化数
        // 其他
        tests.push_back(TestCase(FADD_Operands_Hex_16{0x4d6f, 0x1ea8}, FADD_Operands_Hex_16{0x5455, 0xe39c}, ErrorType::Precise));
        tests.push_back(TestCase(FADD_Operands_Hex_16{0x668, 0x5b00}, FADD_Operands_Hex_16{0x8f63, 0x575}, ErrorType::Precise));

        printf("\n---- Random tests for FP16 ----\n");
        int num_random_tests_16 = 200;
        ErrorType errorType_fp16 = ErrorType::ULP;
        // ---- FP16 任意值随机测试 ----
        for (int i = 0; i < num_random_tests_16; ++i) {
            FADD_Operands_Hex_16 ops1 = {gen_any_fp16(), gen_any_fp16()};
            FADD_Operands_Hex_16 ops2 = {gen_any_fp16(), gen_any_fp16()};
            tests.push_back(TestCase(ops1, ops2, errorType_fp16));
        }
        // ---- 进行不同指数范围的FP16随机测试 ----
        // 小数范围测试：指数[-15, -5]
        for (int i = 0; i < num_random_tests_16; ++i) {
            FADD_Operands_Hex_16 ops1 = {gen_random_fp16(-15, -5), gen_random_fp16(-15, -5)};
            FADD_Operands_Hex_16 ops2 = {gen_random_fp16(-15, -5), gen_random_fp16(-15, -5)};
            tests.push_back(TestCase(ops1, ops2, errorType_fp16));
        }
        // 中等数值范围测试：指数[-5, 5]
        for (int i = 0; i < num_random_tests_16; ++i) {
            FADD_Operands_Hex_16 ops1 = {gen_random_fp16(-5, 5), gen_random_fp16(-5, 5)};
            FADD_Operands_Hex_16 ops2 = {gen_random_fp16(-5, 5), gen_random_fp16(-5, 5)};
            tests.push_back(TestCase(ops1, ops2, errorType_fp16));
        }
        // 大数范围测试：指数[5, 15]
        for (int i = 0; i < num_random_tests_16; ++i) {
            FADD_Operands_Hex_16 ops1 = {gen_random_fp16(5, 15), gen_random_fp16(5, 15)};
            FADD_Operands_Hex_16 ops2 = {gen_random_fp16(5, 15), gen_random_fp16(5, 15)};
            tests.push_back(TestCase(ops1, ops2, errorType_fp16));
        }
        // 更多测试
        for (int i = 0; i < num_random_tests_16; ++i) {
            FADD_Operands_Hex_16 ops1 = {gen_random_fp16(-15, 15), gen_random_fp16(-15, 15)};
            FADD_Operands_Hex_16 ops2 = {gen_random_fp16(-15, 15), gen_random_fp16(-15, 15)};
            tests.push_back(TestCase(ops1, ops2, errorType_fp16));
        }
        for (int i = 0; i < num_random_tests_16; ++i) {
            FADD_Operands_Hex_16 ops1 = {gen_random_fp16(-15, -14), gen_random_fp16(-15, 15)};
            FADD_Operands_Hex_16 ops2 = {gen_random_fp16(-15, 15), gen_random_fp16(-15, -14)};
            tests.push_back(TestCase(ops1, ops2, errorType_fp16));
        }
        for (int i = 0; i < num_random_tests_16; ++i) {
            FADD_Operands_Hex_16 ops1 = {gen_random_fp16(-15, 15), gen_random_fp16(-15, -14)};
            FADD_Operands_Hex_16 ops2 = {gen_random_fp16(-15, 15), gen_random_fp16(-15, -14)};
            tests.push_back(TestCase(ops1, ops2, errorType_fp16));
        }
        for (int i = 0; i < num_random_tests_16; ++i) {
            FADD_Operands_Hex_16 ops1 = {gen_random_fp16(-15, -14), gen_random_fp16(-15, -14)};
            FADD_Operands_Hex_16 ops2 = {gen_random_fp16(-15, -14), gen_random_fp16(-15, -14)};
            tests.push_back(TestCase(ops1, ops2, errorType_fp16));
        }
    }

    if (test_bf16) {
        // -- BF16 并行双路半精度浮点数测试 --
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x3f80, 0x4000}, FADD_Operands_Hex_BF16{0x4040, 0x3f80}, ErrorType::Precise)); // 1.0 + 2.0 = 3.0 | 3.0 + 1.0 = 4.0
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0xbf80, 0x4000}, FADD_Operands_Hex_BF16{0x3f80, 0xc000}, ErrorType::Precise)); // -1.0 + 2.0 = 1.0 | 1.0 + -2.0 = -1.0
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x3f80, 0xbf80}, FADD_Operands_Hex_BF16{0x4080, 0x3f00}, ErrorType::Precise)); // 1.0 + -1.0 = 0.0 | 4.0 + 0.5 = 4.5
        // 零值测试
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x0000, 0x4000}, FADD_Operands_Hex_BF16{0x4000, 0x0000}, ErrorType::Precise)); // 0.0 + 2.0 = 2.0 | 2.0 + 0.0 = 2.0
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x3f80, 0x0000}, FADD_Operands_Hex_BF16{0x0000, 0x4040}, ErrorType::Precise)); // 1.0 + 0.0 = 1.0 | 0.0 + 3.0 = 3.0
        // 无穷大测试
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x7f80, 0x4000}, FADD_Operands_Hex_BF16{0x3f80, 0xff80}, ErrorType::Precise)); // +inf + 2.0 = +inf | 1.0 + -inf = -inf
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x3f80, 0x7f80}, FADD_Operands_Hex_BF16{0x7f80, 0x7f80}, ErrorType::Precise)); // 1.0 + +inf = +inf | +inf + inf = +inf
        // 非规格化数测试（BF16的非规格化数非常小）
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x0001, 0x4000}, FADD_Operands_Hex_BF16{0x007f, 0x3f80}, ErrorType::Precise)); // 最小非规格化数 + 2.0 | 最大非规格化数 + 1.0
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x3f80, 0x0001}, FADD_Operands_Hex_BF16{0x4000, 0x007f}, ErrorType::Precise)); // 1.0 + 最小非规格化数 | 2.0 + 最大非规格化数
        // 边界值测试
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x7f7f, 0x3f80}, FADD_Operands_Hex_BF16{0x0080, 0x3f80}, ErrorType::ULP)); // 最大规格化数 + 1.0 | 最小规格化数 + 1.0
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0xff7f, 0x3f80}, FADD_Operands_Hex_BF16{0x8080, 0x3f80}, ErrorType::ULP)); // -最大规格化数 + 1.0 | -最小规格化数 + 1.0
        // 接近溢出的测试
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x7f00, 0x4000}, FADD_Operands_Hex_BF16{0x7e80, 0x4040}, ErrorType::ULP)); // 大数 + 2.0 | 大数 + 3.0
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x7f7f, 0x3f00}, FADD_Operands_Hex_BF16{0x7e80, 0x3f00}, ErrorType::ULP)); // 最大数 + 0.5 | 大数 + 0.5
        // 精度损失边界测试  
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x4000, 0x3e80}, FADD_Operands_Hex_BF16{0x4040, 0x3e00}, ErrorType::ULP)); // 2.0 + 小数 | 3.0 + 小数
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x5000, 0x3d80}, FADD_Operands_Hex_BF16{0x4c80, 0x3d00}, ErrorType::ULP)); // 大数 + 极小数 | 中数 + 极小数
        // 符号组合的复杂测试
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0xc000, 0xc000}, FADD_Operands_Hex_BF16{0xc040, 0xc000}, ErrorType::ULP)); // (-2.0) + (-2.0) = -4.0 | (-3.0) + (-2.0) = -5.0
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x4000, 0xc000}, FADD_Operands_Hex_BF16{0x4040, 0xbf80}, ErrorType::ULP)); // 2.0 + (-2.0) = 0.0 | 3.0 + (-1.0) = 2.0
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0xc000, 0x4000}, FADD_Operands_Hex_BF16{0xc040, 0x4000}, ErrorType::ULP)); // (-2.0) + 2.0 = 0.0 | (-3.0) + 2.0 = -1.0
        // 其他复杂测试用例
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x42a5, 0x3e12}, FADD_Operands_Hex_BF16{0x4567, 0x3d89}, ErrorType::ULP));
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x4012, 0x4234}, FADD_Operands_Hex_BF16{0x4156, 0x3e78}, ErrorType::RelativeError));
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x9a1d, 0x1fa1}, FADD_Operands_Hex_BF16{0xa174, 0xcafa}, ErrorType::ULP));
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x80e1, 0x80ed}, FADD_Operands_Hex_BF16{0x80cd, 0x806d}, ErrorType::ULP_or_RelativeError));
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0x80e1, 0x80ed}, FADD_Operands_Hex_BF16{0x80cd, 0x806d}, ErrorType::ULP));
        tests.push_back(TestCase(FADD_Operands_Hex_BF16{0xbf80, 0x0200}, FADD_Operands_Hex_BF16{0xbf80, 0x0200}, ErrorType::ULP));
    
        printf("\n---- Random tests for BF16 ----\n");
        int num_random_tests_bf16 = 200;
        
        // ---- BF16 任意值随机测试 ----
        for (int i = 0; i < num_random_tests_bf16; ++i) {
            FADD_Operands_Hex_BF16 ops1 = {gen_any_bf16(), gen_any_bf16()};
            FADD_Operands_Hex_BF16 ops2 = {gen_any_bf16(), gen_any_bf16()};
            tests.push_back(TestCase(ops1, ops2, ErrorType::ULP_or_RelativeError));
        }
        
        // ---- 进行不同指数范围的BF16随机测试 ----
        // 小数范围测试：指数[-50, -10]
        for (int i = 0; i < num_random_tests_bf16; ++i) {
            FADD_Operands_Hex_BF16 ops1 = {gen_random_bf16(-50, -10), gen_random_bf16(-50, -10)};
            FADD_Operands_Hex_BF16 ops2 = {gen_random_bf16(-50, -10), gen_random_bf16(-50, -10)};
            tests.push_back(TestCase(ops1, ops2, ErrorType::ULP_or_RelativeError));
        }
        // 中等数值范围测试：指数[-10, 10]
        for (int i = 0; i < num_random_tests_bf16; ++i) {
            FADD_Operands_Hex_BF16 ops1 = {gen_random_bf16(-10, 10), gen_random_bf16(-10, 10)};
            FADD_Operands_Hex_BF16 ops2 = {gen_random_bf16(-10, 10), gen_random_bf16(-10, 10)};
            tests.push_back(TestCase(ops1, ops2, ErrorType::ULP_or_RelativeError));
        }
        // 大数范围测试：指数[10, 50]
        for (int i = 0; i < num_random_tests_bf16; ++i) {
            FADD_Operands_Hex_BF16 ops1 = {gen_random_bf16(10, 50), gen_random_bf16(10, 50)};
            FADD_Operands_Hex_BF16 ops2 = {gen_random_bf16(10, 50), gen_random_bf16(10, 50)};
            tests.push_back(TestCase(ops1, ops2, ErrorType::ULP_or_RelativeError));
        }
        // 极端范围测试：指数[-126, 127]
        for (int i = 0; i < num_random_tests_bf16; ++i) {
            FADD_Operands_Hex_BF16 ops1 = {gen_random_bf16(-126, 127), gen_random_bf16(-126, 127)};
            FADD_Operands_Hex_BF16 ops2 = {gen_random_bf16(-126, 127), gen_random_bf16(-126, 127)};
            tests.push_back(TestCase(ops1, ops2, ErrorType::ULP_or_RelativeError));
        }
        // 非规格化数边界测试：指数[-126, -125]
        for (int i = 0; i < num_random_tests_bf16; ++i) {
            FADD_Operands_Hex_BF16 ops1 = {gen_random_bf16(-126, -125), gen_random_bf16(-126, 20)};
            FADD_Operands_Hex_BF16 ops2 = {gen_random_bf16(-126, 20), gen_random_bf16(-126, -125)};
            tests.push_back(TestCase(ops1, ops2, ErrorType::ULP_or_RelativeError));
        }
        // 混合精度范围测试
        for (int i = 0; i < num_random_tests_bf16; ++i) {
            FADD_Operands_Hex_BF16 ops1 = {gen_random_bf16(-126, 20), gen_random_bf16(-126, -125)};
            FADD_Operands_Hex_BF16 ops2 = {gen_random_bf16(-126, 20), gen_random_bf16(-126, -125)};
            tests.push_back(TestCase(ops1, ops2, ErrorType::ULP_or_RelativeError));
        }
        // 高精度范围测试
        for (int i = 0; i < num_random_tests_bf16; ++i) {
            FADD_Operands_Hex_BF16 ops1 = {gen_random_bf16(-126, -125), gen_random_bf16(-126, -125)};
            FADD_Operands_Hex_BF16 ops2 = {gen_random_bf16(-126, -125), gen_random_bf16(-126, -125)};
            tests.push_back(TestCase(ops1, ops2, ErrorType::ULP_or_RelativeError));
        }
        // 全范围混合测试
        for (int i = 0; i < num_random_tests_bf16; ++i) {
            FADD_Operands_Hex_BF16 ops1 = {gen_random_bf16(-127, 10), gen_random_bf16(-127, 10)};
            FADD_Operands_Hex_BF16 ops2 = {gen_random_bf16(-127, 10), gen_random_bf16(-127, 10)};
            tests.push_back(TestCase(ops1, ops2, ErrorType::ULP_or_RelativeError));
        }
        // 相对误差测试（较高精度要求）
        for (int i = 0; i < num_random_tests_bf16 / 5; ++i) {
            FADD_Operands_Hex_BF16 ops1 = {gen_random_bf16(-20, 20), gen_random_bf16(-20, 20)};
            FADD_Operands_Hex_BF16 ops2 = {gen_random_bf16(-20, 20), gen_random_bf16(-20, 20)};
            tests.push_back(TestCase(ops1, ops2, ErrorType::ULP_or_RelativeError));
        }
        // 极端范围测试：指数[-127, -126]
        for (int i = 0; i < num_random_tests_bf16; ++i) {
            FADD_Operands_Hex_BF16 ops1 = {gen_random_bf16(-127, -126), gen_random_bf16(-127, -126)};
            FADD_Operands_Hex_BF16 ops2 = {gen_random_bf16(-127, -126), gen_random_bf16(-127, -126)};
            tests.push_back(TestCase(ops1, ops2, ErrorType::ULP_or_RelativeError));
        }
    }

    if (test_fp16_widen) {
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

    if (test_bf16_widen) {
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

    return tests;
} 