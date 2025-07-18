#ifndef TEST_FACTORY_SPLIT_H
#define TEST_FACTORY_SPLIT_H

#include <vector>
#include "../include/test_factory.h"

// 声明各个精度的测试函数
void add_fp32_tests(std::vector<TestCase>& tests);
void add_fp16_tests(std::vector<TestCase>& tests);
void add_bf16_tests(std::vector<TestCase>& tests);
void add_fp16_widen_tests(std::vector<TestCase>& tests);
void add_bf16_widen_tests(std::vector<TestCase>& tests);

#endif // TEST_FACTORY_SPLIT_H 