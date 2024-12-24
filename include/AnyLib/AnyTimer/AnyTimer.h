#ifndef ANYTIMER_H
#define ANYTIMER_H

#include <thread>
#include <functional>
#include <atomic>
#include <chrono>

class AnyTimer
{
public:
    // 构造函数，接受一个任务函数
    AnyTimer(std::function<void()> task)
        : task(task), running(false) {}

    // 禁用拷贝构造和赋值
    AnyTimer(const AnyTimer &) = delete;
    AnyTimer &operator=(const AnyTimer &) = delete;

    // 启动定时任务
    void start(int interval_seconds)
    {
        stop(); // 确保没有多余的线程在运行
        running = true;
        worker_thread = std::thread([this, interval_seconds]()
                                    {
            while (running) {
                task(); // 执行任务
                // 分割睡眠以检查停止信号
                for (int i = 0; i < interval_seconds && running; ++i) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            } });
    }

    // 停止定时任务
    void stop()
    {
        if (!running)
        {
            return; // 如果任务已经停止，则直接返回
        }
        running = false; // 停止标志
        if (worker_thread.joinable())
        {
            worker_thread.join(); // 等待线程完成
        }
    }

    // 析构函数，确保线程被正确停止
    ~AnyTimer()
    {
        if (worker_thread.joinable())
        {
            stop(); // 确保线程被正确停止
        }
    }

private:
    std::function<void()> task; // 定时任务函数
    std::atomic<bool> running;  // 运行状态标志
    std::thread worker_thread;  // 工作线程
};

#endif // ANYTIMER_H
