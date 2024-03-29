#ifndef _341_SEMAPHORE_H_
#define _341_SEMAPHORE_H_

#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
    Semaphore(int count) : count_(count) {}

    void acquire()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (count_ == 0)
        {
            cv_.wait(lock);
        }
        count_--;
    }

    void release()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        count_++;
        cv_.notify_one();
    }

private:
    int count_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

#endif // _341_SEMAPHORE_H_
