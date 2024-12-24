#include <iostream>
#include "AnyLib/AnyRand/AnyRand.h"
#include "AnyLib/AnyTimer/AnyTimer.h"

void print_message()
{
    std::cout << "Hello, this is a timed message!" << std::endl;
}

int main()
{
    AnyTimer timer(print_message);
    timer.start(2); // 每2秒打印一次消息

    std::this_thread::sleep_for(std::chrono::seconds(10)); // 主线程等待10秒

    timer.stop(); // 停止定时任务

    return 0;
}