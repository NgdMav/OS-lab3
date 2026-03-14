#include "../include/marker_thread.h"
#include <gtest/gtest.h>
#include <vector>

TEST(ValidationTest, ValidSize) {
    EXPECT_TRUE(CheckSize(1));
    EXPECT_TRUE(CheckSize(10));
    EXPECT_TRUE(CheckSize(100));
    EXPECT_TRUE(CheckSize(1000));
}

TEST(ValidationTest, InvalidSize) {
    EXPECT_FALSE(CheckSize(0));
    EXPECT_FALSE(CheckSize(-1));
    EXPECT_FALSE(CheckSize(-100));
    EXPECT_FALSE(CheckSize(10001));
}

TEST(PrintArrayTest, ValidSmallArray) {
    int32_t testArray[] = {1, 2, 3, 0, 5};
    EXPECT_NO_THROW(PrintArray(testArray, 5, "Test Array"));
}
 
TEST(PrintArrayTest, ValidLargeArray) {
    int32_t testArray[100];
    for (int32_t i = 0; i < 100; i++) {
        testArray[i] = i % 10;
    }
    EXPECT_NO_THROW(PrintArray(testArray, 100, "Large Array"));
}
 
TEST(PrintArrayTest, ArrayWithAllZeros) {
    int32_t testArray[10] = {0};
    EXPECT_NO_THROW(PrintArray(testArray, 10, "Zero Array"));
}
 
TEST(PrintArrayTest, ArrayWithAllSameValues) {
    int32_t testArray[] = {5, 5, 5, 5, 5, 5, 5, 5};
    EXPECT_NO_THROW(PrintArray(testArray, 8, "Same Values"));
}
 
TEST(PrintArrayTest, InvalidNullPointer) {
    EXPECT_NO_THROW(PrintArray(nullptr, 5, "Test"));
}
 
TEST(PrintArrayTest, InvalidZeroSize) {
    int32_t testArray[] = {1, 2, 3};
    EXPECT_NO_THROW(PrintArray(testArray, 0, "Test"));
}
 
TEST(PrintArrayTest, InvalidNegativeSize) {
    int32_t testArray[] = {1, 2, 3};
    EXPECT_NO_THROW(PrintArray(testArray, -1, "Test"));
}


TEST(CriticalSectionTest, BasicInitializeAndDelete) {
    CRITICAL_SECTION cs;
    EXPECT_NO_THROW(InitializeCriticalSection(&cs));
    EXPECT_NO_THROW(DeleteCriticalSection(&cs));
}
 
TEST(CriticalSectionTest, EnterAndLeave) {
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    
    EXPECT_NO_THROW(EnterCriticalSection(&cs));
    EXPECT_NO_THROW(LeaveCriticalSection(&cs));
    
    DeleteCriticalSection(&cs);
}
 
TEST(CriticalSectionTest, MultipleEnterLeave) {
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    
    for (int32_t i = 0; i < 100; i++) {
        EXPECT_NO_THROW(EnterCriticalSection(&cs));
        EXPECT_NO_THROW(LeaveCriticalSection(&cs));
    }
    
    DeleteCriticalSection(&cs);
}
 
TEST(CriticalSectionTest, ProtectsSharedData) {
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    
    int32_t sharedValue = 0;
    
    EnterCriticalSection(&cs);
    sharedValue = 42;
    EXPECT_EQ(sharedValue, 42);
    LeaveCriticalSection(&cs);
    
    EnterCriticalSection(&cs);
    sharedValue = 100;
    EXPECT_EQ(sharedValue, 100);
    LeaveCriticalSection(&cs);
    
    DeleteCriticalSection(&cs);
}
 
TEST(CriticalSectionTest, ReentrantLocking) {
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    
    EnterCriticalSection(&cs);
    EnterCriticalSection(&cs);
    EnterCriticalSection(&cs);
    
    LeaveCriticalSection(&cs);
    LeaveCriticalSection(&cs);
    LeaveCriticalSection(&cs);
    
    DeleteCriticalSection(&cs);
}

TEST(EventTest, CreateAndClose) {
    HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);
    ASSERT_NE(event, nullptr);
    EXPECT_NO_THROW(CloseHandle(event));
}
 
TEST(EventTest, InitialStateNonsignaled) {
    HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);
    ASSERT_NE(event, nullptr);
    
    DWORD result = WaitForSingleObject(event, 0);
    EXPECT_EQ(result, WAIT_TIMEOUT);
    
    CloseHandle(event);
}
 
TEST(EventTest, InitialStateSignaled) {
    HANDLE event = CreateEvent(NULL, TRUE, TRUE, NULL);
    ASSERT_NE(event, nullptr);
    
    DWORD result = WaitForSingleObject(event, 0);
    EXPECT_EQ(result, WAIT_OBJECT_0);
    
    CloseHandle(event);
}
 
TEST(EventTest, SetEventMakesSignaled) {
    HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);
    ASSERT_NE(event, nullptr);
    
    SetEvent(event);
    
    DWORD result = WaitForSingleObject(event, 0);
    EXPECT_EQ(result, WAIT_OBJECT_0);
    
    CloseHandle(event);
}
 
TEST(EventTest, ResetEventMakesNonsignaled) {
    HANDLE event = CreateEvent(NULL, TRUE, TRUE, NULL);
    ASSERT_NE(event, nullptr);
    
    ResetEvent(event);
    
    DWORD result = WaitForSingleObject(event, 0);
    EXPECT_EQ(result, WAIT_TIMEOUT);
    
    CloseHandle(event);
}
 
TEST(EventTest, ManualResetStaysSignaled) {
    HANDLE manualEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    ASSERT_NE(manualEvent, nullptr);
    
    SetEvent(manualEvent);
    
    EXPECT_EQ(WaitForSingleObject(manualEvent, 0), WAIT_OBJECT_0);
    EXPECT_EQ(WaitForSingleObject(manualEvent, 0), WAIT_OBJECT_0);
    EXPECT_EQ(WaitForSingleObject(manualEvent, 0), WAIT_OBJECT_0);
    
    CloseHandle(manualEvent);
}
 
TEST(EventTest, AutoResetBecomesNonsignaled) {
    HANDLE autoEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    ASSERT_NE(autoEvent, nullptr);
    
    SetEvent(autoEvent);
    
    EXPECT_EQ(WaitForSingleObject(autoEvent, 0), WAIT_OBJECT_0);
    EXPECT_EQ(WaitForSingleObject(autoEvent, 0), WAIT_TIMEOUT);
    
    CloseHandle(autoEvent);
}
 
TEST(EventTest, MultipleSetReset) {
    HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);
    ASSERT_NE(event, nullptr);
    
    for (int32_t i = 0; i < 10; i++) {
        SetEvent(event);
        EXPECT_EQ(WaitForSingleObject(event, 0), WAIT_OBJECT_0);
        
        ResetEvent(event);
        EXPECT_EQ(WaitForSingleObject(event, 0), WAIT_TIMEOUT);
    }
    
    CloseHandle(event);
}

TEST(MultipleEventsTest, WaitAllNonsignaled) {
    HANDLE event1 = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE event2 = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE event3 = CreateEvent(NULL, TRUE, FALSE, NULL);
    
    ASSERT_NE(event1, nullptr);
    ASSERT_NE(event2, nullptr);
    ASSERT_NE(event3, nullptr);
    
    HANDLE events[] = {event1, event2, event3};
    
    DWORD result = WaitForMultipleObjects(3, events, TRUE, 0);
    EXPECT_EQ(result, WAIT_TIMEOUT);
    
    CloseHandle(event1);
    CloseHandle(event2);
    CloseHandle(event3);
}
 
TEST(MultipleEventsTest, WaitAllSignaled) {
    HANDLE event1 = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE event2 = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE event3 = CreateEvent(NULL, TRUE, FALSE, NULL);
    
    ASSERT_NE(event1, nullptr);
    ASSERT_NE(event2, nullptr);
    ASSERT_NE(event3, nullptr);
    
    SetEvent(event1);
    SetEvent(event2);
    SetEvent(event3);
    
    HANDLE events[] = {event1, event2, event3};
    
    DWORD result = WaitForMultipleObjects(3, events, TRUE, 0);
    EXPECT_EQ(result, WAIT_OBJECT_0);
    
    CloseHandle(event1);
    CloseHandle(event2);
    CloseHandle(event3);
}
 
TEST(MultipleEventsTest, WaitAnyOneSignaled) {
    HANDLE event1 = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE event2 = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE event3 = CreateEvent(NULL, TRUE, FALSE, NULL);
    
    ASSERT_NE(event1, nullptr);
    ASSERT_NE(event2, nullptr);
    ASSERT_NE(event3, nullptr);
    
    SetEvent(event2);
    
    HANDLE events[] = {event1, event2, event3};
    
    DWORD result = WaitForMultipleObjects(3, events, FALSE, 0);
    EXPECT_EQ(result, WAIT_OBJECT_0 + 1);
    
    CloseHandle(event1);
    CloseHandle(event2);
    CloseHandle(event3);
}
 
TEST(MultipleEventsTest, WaitAllPartiallySignaled) {
    HANDLE event1 = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE event2 = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE event3 = CreateEvent(NULL, TRUE, FALSE, NULL);
    
    ASSERT_NE(event1, nullptr);
    ASSERT_NE(event2, nullptr);
    ASSERT_NE(event3, nullptr);
    
    SetEvent(event1);
    SetEvent(event2);
    
    HANDLE events[] = {event1, event2, event3};
    
    DWORD result = WaitForMultipleObjects(3, events, TRUE, 0);
    EXPECT_EQ(result, WAIT_TIMEOUT);
    
    CloseHandle(event1);
    CloseHandle(event2);
    CloseHandle(event3);
}

TEST(MarkerThreadTest, ThreadCreation) {
    const int32_t arraySize = 10;
    int32_t array[arraySize] = {0};
    
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    
    HANDLE startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE cannotContinue = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE continueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE terminateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    
    ASSERT_NE(startEvent, nullptr);
    ASSERT_NE(cannotContinue, nullptr);
    ASSERT_NE(continueEvent, nullptr);
    ASSERT_NE(terminateEvent, nullptr);
    
    MarkerData data;
    data.array = array;
    data.arraySize = arraySize;
    data.markerID = 1;
    data.startEvent = startEvent;
    data.cannotContinueEvent = cannotContinue;
    data.continueEvent = continueEvent;
    data.terminateEvent = terminateEvent;
    data.cs = &cs;
    data.markedCount = 0;
    data.blockedIndex = -1;
    
    HANDLE hThread = CreateThread(NULL, 0, MarkerThread, &data, 0, NULL);
    ASSERT_NE(hThread, nullptr);
    
    SetEvent(startEvent);
    SetEvent(terminateEvent);
    
    DWORD waitResult = WaitForSingleObject(hThread, 1000);
    EXPECT_EQ(waitResult, WAIT_OBJECT_0);
    
    CloseHandle(hThread);
    CloseHandle(startEvent);
    CloseHandle(cannotContinue);
    CloseHandle(continueEvent);
    CloseHandle(terminateEvent);
    DeleteCriticalSection(&cs);
}
 
TEST(MarkerThreadTest, ThreadMarksElements) {
    const int32_t arraySize = 10;
    int32_t array[arraySize] = {0};
    
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    
    HANDLE startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE cannotContinue = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE continueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE terminateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    
    MarkerData data;
    data.array = array;
    data.arraySize = arraySize;
    data.markerID = 1;
    data.startEvent = startEvent;
    data.cannotContinueEvent = cannotContinue;
    data.continueEvent = continueEvent;
    data.terminateEvent = terminateEvent;
    data.cs = &cs;
    data.markedCount = 0;
    data.blockedIndex = -1;
    
    HANDLE hThread = CreateThread(NULL, 0, MarkerThread, &data, 0, NULL);
    ASSERT_NE(hThread, nullptr);
    
    SetEvent(startEvent);
    
    Sleep(100);
    
    int32_t markedCount = 0;
    for (int32_t i = 0; i < arraySize; i++) {
        if (array[i] == 1) {
            markedCount++;
        }
    }
    EXPECT_GT(markedCount, 0);
    
    SetEvent(terminateEvent);
    WaitForSingleObject(hThread, 1000);
    
    CloseHandle(hThread);
    CloseHandle(startEvent);
    CloseHandle(cannotContinue);
    CloseHandle(continueEvent);
    CloseHandle(terminateEvent);
    DeleteCriticalSection(&cs);
}
 
TEST(MarkerThreadTest, ThreadClearsOnTermination) {
    const int32_t arraySize = 20;
    int32_t array[arraySize] = {0};
    
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    
    HANDLE startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE cannotContinue = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE continueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE terminateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    
    MarkerData data;
    data.array = array;
    data.arraySize = arraySize;
    data.markerID = 1;
    data.startEvent = startEvent;
    data.cannotContinueEvent = cannotContinue;
    data.continueEvent = continueEvent;
    data.terminateEvent = terminateEvent;
    data.cs = &cs;
    data.markedCount = 0;
    data.blockedIndex = -1;
    
    HANDLE hThread = CreateThread(NULL, 0, MarkerThread, &data, 0, NULL);
    ASSERT_NE(hThread, nullptr);
    
    SetEvent(startEvent);
    
    Sleep(200);
    
    SetEvent(terminateEvent);
    WaitForSingleObject(hThread, 5000);
    
    int32_t markedCount = 0;
    for (int32_t i = 0; i < arraySize; i++) {
        if (array[i] == 1) {
            markedCount++;
        }
    }
    EXPECT_EQ(markedCount, 0);
    
    CloseHandle(hThread);
    CloseHandle(startEvent);
    CloseHandle(cannotContinue);
    CloseHandle(continueEvent);
    CloseHandle(terminateEvent);
    DeleteCriticalSection(&cs);
}
 
TEST(MarkerThreadTest, InvalidNullParameter) {
    DWORD result = MarkerThread(nullptr);
    EXPECT_NE(result, 0);
}
 
TEST(MarkerThreadTest, InvalidArrayPointer) {
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    
    HANDLE startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE cannotContinue = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE continueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE terminateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    
    MarkerData data;
    data.array = nullptr;
    data.arraySize = 10;
    data.markerID = 1;
    data.startEvent = startEvent;
    data.cannotContinueEvent = cannotContinue;
    data.continueEvent = continueEvent;
    data.terminateEvent = terminateEvent;
    data.cs = &cs;
    
    DWORD result = MarkerThread(&data);
    EXPECT_NE(result, 0);
    
    CloseHandle(startEvent);
    CloseHandle(cannotContinue);
    CloseHandle(continueEvent);
    CloseHandle(terminateEvent);
    DeleteCriticalSection(&cs);
}

TEST(IntegrationTest, TwoThreadsShareArray) {
    const int32_t arraySize = 30;
    int32_t array[arraySize] = {0};
    
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    
    MarkerData data1, data2;
    
    data1.array = array;
    data1.arraySize = arraySize;
    data1.markerID = 1;
    data1.startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    data1.cannotContinueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    data1.continueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    data1.terminateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    data1.cs = &cs;
    data1.markedCount = 0;
    
    data2.array = array;
    data2.arraySize = arraySize;
    data2.markerID = 2;
    data2.startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    data2.cannotContinueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    data2.continueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    data2.terminateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    data2.cs = &cs;
    data2.markedCount = 0;
    
    HANDLE hThread1 = CreateThread(NULL, 0, MarkerThread, &data1, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, MarkerThread, &data2, 0, NULL);
    
    ASSERT_NE(hThread1, nullptr);
    ASSERT_NE(hThread2, nullptr);
    
    SetEvent(data1.startEvent);
    SetEvent(data2.startEvent);
    
    Sleep(300);
    
    int32_t count1 = 0, count2 = 0;
    for (int32_t i = 0; i < arraySize; i++) {
        if (array[i] == 1) count1++;
        if (array[i] == 2) count2++;
    }
    
    EXPECT_GT(count1, 0);
    EXPECT_GT(count2, 0);
    
    SetEvent(data1.terminateEvent);
    SetEvent(data2.terminateEvent);
    
    WaitForSingleObject(hThread1, 5000);
    WaitForSingleObject(hThread2, 5000);
    
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(data1.startEvent);
    CloseHandle(data1.cannotContinueEvent);
    CloseHandle(data1.continueEvent);
    CloseHandle(data1.terminateEvent);
    CloseHandle(data2.startEvent);
    CloseHandle(data2.cannotContinueEvent);
    CloseHandle(data2.continueEvent);
    CloseHandle(data2.terminateEvent);
    DeleteCriticalSection(&cs);
}

TEST(ResourceLeakTest, EventsNoLeak) {
    for (int32_t i = 0; i < 100; i++) {
        HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);
        ASSERT_NE(event, nullptr);
        CloseHandle(event);
    }
}
 
TEST(ResourceLeakTest, CriticalSectionsNoLeak) {
    for (int32_t i = 0; i < 100; i++) {
        CRITICAL_SECTION cs;
        InitializeCriticalSection(&cs);
        EnterCriticalSection(&cs);
        LeaveCriticalSection(&cs);
        DeleteCriticalSection(&cs);
    }
}
 
TEST(ResourceLeakTest, MultipleThreadsNoLeak) {
    const int32_t arraySize = 10;
    int32_t array[arraySize] = {0};
    
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    
    for (int32_t i = 0; i < 10; i++) {
        HANDLE startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        HANDLE cannotContinue = CreateEvent(NULL, TRUE, FALSE, NULL);
        HANDLE continueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        HANDLE terminateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        
        MarkerData data;
        data.array = array;
        data.arraySize = arraySize;
        data.markerID = 1;
        data.startEvent = startEvent;
        data.cannotContinueEvent = cannotContinue;
        data.continueEvent = continueEvent;
        data.terminateEvent = terminateEvent;
        data.cs = &cs;
        data.markedCount = 0;
        
        HANDLE hThread = CreateThread(NULL, 0, MarkerThread, &data, 0, NULL);
        SetEvent(terminateEvent);
        WaitForSingleObject(hThread, 1000);
        
        CloseHandle(hThread);
        CloseHandle(startEvent);
        CloseHandle(cannotContinue);
        CloseHandle(continueEvent);
        CloseHandle(terminateEvent);
    }
    
    DeleteCriticalSection(&cs);
}

int32_t main(int32_t argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}