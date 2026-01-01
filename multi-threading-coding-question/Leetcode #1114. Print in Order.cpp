LeetCode 1114 – Print in Order: Ensure three threads execute in sequence (first → second → third) using synchronization primitives.

////////////////////////////////////////////////////////////////////////////
1. Mutex + Condition Variable (Your Solution – BEST PRACTICE)

Idea:
Use a shared variable (turn) and block threads using condition_variable until their turn arrives.

✔ Thread-safe
✔ Handles spurious wakeups
✔ Interview-preferred

Core logic:

while (turn != expected) cv.wait(lock);
//////////////////////////
######CODE##########

#include <mutex>
#include <condition_variable>
#include <functional>

class Foo {
    std::mutex mtx;
    std::condition_variable cv;
    int turn = 1;

public:
    void first(std::function<void()> printFirst) {
        std::unique_lock<std::mutex> lock(mtx);
        while (turn != 1) cv.wait(lock);
        printFirst();
        turn = 2;
        cv.notify_one();
    }

    void second(std::function<void()> printSecond) {
        std::unique_lock<std::mutex> lock(mtx);
        while (turn != 2) cv.wait(lock);
        printSecond();
        turn = 3;
        cv.notify_one();
    }

    void third(std::function<void()> printThird) {
        std::unique_lock<std::mutex> lock(mtx);
        while (turn != 3) cv.wait(lock);
        printThird();
    }
};

////////////////////////////////////////////////////
2. Semaphore-Based Solution

(Conceptual alternative)

Use 3 semaphores:

sem1 = 1, sem2 = 0, sem3 = 0

Each function waits on its semaphore and signals the next.

Used in OS-level concurrency questions.
