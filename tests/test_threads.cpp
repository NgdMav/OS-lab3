#include "../include/thread_functions.h"
#include <gtest/gtest.h>
#include <vector>

// Тест функции MinMaxThread
TEST(MinMaxThreadTest, FindsCorrectMinMax) {
    int testArray[] = {5, 2, 8, 1, 9, 3};
    int size = 6;
    
    MinMaxData data = {testArray, size, 0, 0, 0, 0};
    
    DWORD result = MinMaxThread(&data);
    
    EXPECT_EQ(result, 0);
    EXPECT_EQ(data.minValue, 1);
    EXPECT_EQ(data.maxValue, 9);
    EXPECT_EQ(data.minIndex, 3);
    EXPECT_EQ(data.maxIndex, 4);
}

TEST(MinMaxThreadTest, SingleElement) {
    int testArray[] = {42};
    int size = 1;
    
    MinMaxData data = {testArray, size, 0, 0, 0, 0};
    
    DWORD result = MinMaxThread(&data);
    
    EXPECT_EQ(result, 0);
    EXPECT_EQ(data.minValue, 42);
    EXPECT_EQ(data.maxValue, 42);
    EXPECT_EQ(data.minIndex, 0);
    EXPECT_EQ(data.maxIndex, 0);
}

TEST(MinMaxThreadTest, AllSameElements) {
    int testArray[] = {5, 5, 5, 5, 5};
    int size = 5;
    
    MinMaxData data = {testArray, size, 0, 0, 0, 0};
    
    DWORD result = MinMaxThread(&data);
    
    EXPECT_EQ(result, 0);
    EXPECT_EQ(data.minValue, 5);
    EXPECT_EQ(data.maxValue, 5);
}

TEST(MinMaxThreadTest, NegativeNumbers) {
    int testArray[] = {-5, -2, -8, -1, -9};
    int size = 5;
    
    MinMaxData data = {testArray, size, 0, 0, 0, 0};
    
    DWORD result = MinMaxThread(&data);
    
    EXPECT_EQ(result, 0);
    EXPECT_EQ(data.minValue, -9);
    EXPECT_EQ(data.maxValue, -1);
}

TEST(MinMaxThreadTest, InvalidNullPointer) {
    DWORD result = MinMaxThread(nullptr);
    EXPECT_NE(result, 0);
}

TEST(MinMaxThreadTest, InvalidArrayPointer) {
    MinMaxData data = {nullptr, 5, 0, 0, 0, 0};
    DWORD result = MinMaxThread(&data);
    EXPECT_NE(result, 0);
}

// Тест функции AverageThread
TEST(AverageThreadTest, CalculatesCorrectAverage) {
    int testArray[] = {2, 4, 6, 8, 10};
    int size = 5;
    
    AverageData data = {testArray, size, 0.0};
    
    DWORD result = AverageThread(&data);
    
    EXPECT_EQ(result, 0);
    EXPECT_DOUBLE_EQ(data.averageValue, 6.0);
}

TEST(AverageThreadTest, SingleElement) {
    int testArray[] = {42};
    int size = 1;
    
    AverageData data = {testArray, size, 0.0};
    
    DWORD result = AverageThread(&data);
    
    EXPECT_EQ(result, 0);
    EXPECT_DOUBLE_EQ(data.averageValue, 42.0);
}

TEST(AverageThreadTest, NegativeNumbers) {
    int testArray[] = {-10, -20, -30};
    int size = 3;
    
    AverageData data = {testArray, size, 0.0};
    
    DWORD result = AverageThread(&data);
    
    EXPECT_EQ(result, 0);
    EXPECT_DOUBLE_EQ(data.averageValue, -20.0);
}

TEST(AverageThreadTest, MixedNumbers) {
    int testArray[] = {-5, 0, 5, 10};
    int size = 4;
    
    AverageData data = {testArray, size, 0.0};
    
    DWORD result = AverageThread(&data);
    
    EXPECT_EQ(result, 0);
    EXPECT_DOUBLE_EQ(data.averageValue, 2.5);
}

TEST(AverageThreadTest, InvalidNullPointer) {
    DWORD result = AverageThread(nullptr);
    EXPECT_NE(result, 0);
}

TEST(AverageThreadTest, InvalidArrayPointer) {
    AverageData data = {nullptr, 5, 0.0};
    DWORD result = AverageThread(&data);
    EXPECT_NE(result, 0);
}

// Тест функции ValidateArrayInput
TEST(ValidationTest, ValidSize) {
    EXPECT_TRUE(CheckArraySize(1));
    EXPECT_TRUE(CheckArraySize(10));
    EXPECT_TRUE(CheckArraySize(100));
    EXPECT_TRUE(CheckArraySize(1000));
}

TEST(ValidationTest, InvalidSize) {
    EXPECT_FALSE(CheckArraySize(0));
    EXPECT_FALSE(CheckArraySize(-1));
    EXPECT_FALSE(CheckArraySize(-100));
    EXPECT_FALSE(CheckArraySize(10001));
}

// Тест с реальными потоками
TEST(IntegrationTest, ThreadsWorkTogether) {
    int testArray[] = {10, 5, 15, 3, 20};
    int size = 5;
    
    MinMaxData minMaxData = {testArray, size, 0, 0, 0, 0};
    AverageData avgData = {testArray, size, 0.0};
    
    HANDLE hMinMax = CreateThread(nullptr, 0, MinMaxThread, &minMaxData, 0, nullptr);
    HANDLE hAverage = CreateThread(nullptr, 0, AverageThread, &avgData, 0, nullptr);
    
    ASSERT_NE(hMinMax, nullptr);
    ASSERT_NE(hAverage, nullptr);
    
    HANDLE handles[] = {hMinMax, hAverage};
    DWORD waitResult = WaitForMultipleObjects(2, handles, TRUE, INFINITE);
    
    EXPECT_NE(waitResult, WAIT_FAILED);
    
    // Проверка результатов
    EXPECT_EQ(minMaxData.minValue, 3);
    EXPECT_EQ(minMaxData.maxValue, 20);
    EXPECT_DOUBLE_EQ(avgData.averageValue, 10.6);
    
    CloseHandle(hAverage);
    CloseHandle(hMinMax);
}

// Главная функция для запуска тестов
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}