#pragma once

#include <future>
#include <functional>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>
#include <condition_variable>
#include <vector>
#include <queue>

class ThreadPool {
public:
    explicit ThreadPool(size_t);

    ~ThreadPool();

    template<class F, class... Args>
    auto addTask(F&& f, Args&& ... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;

    bool isTasksReady();

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> stop;

    int taskReadySync = 0;
};

ThreadPool::ThreadPool(size_t threadsCount) : stop(false) {
    for (size_t i = 0; i < threadsCount; i++) {
        threads.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this]() { return !tasks.empty() || stop == true; });
                    if (stop && tasks.empty()) return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}

template<class F, class... Args>
auto ThreadPool::addTask(F&& function, Args&& ... args)
-> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(function), std::forward<Args>(args)...)
    );
    auto result = task->get_future();

    {
        std::unique_lock<std::mutex> lock(mtx);
        if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.emplace([task, this]() { taskReadySync++; (*task)(); taskReadySync--; });
    }

    cv.notify_one();
    return result;
};

bool ThreadPool::isTasksReady() {
    return tasks.empty() && taskReadySync == 0;
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();

    for (std::thread& thread : threads) {
        thread.join();
    }
}