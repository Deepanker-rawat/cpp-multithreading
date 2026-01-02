====================================================
LeetCode 1115 — Print FooBar Alternately
====================================================

Problem Summary (1-liner)
----------------------------------------------------
Two threads must alternately print "foo" and "bar" exactly n times in the order: foobarfoobar...

----------------------------------------------------
My Solution (Mutex + Condition Variable)
----------------------------------------------------

Core Idea:
- Use a shared `turn` variable
- `turn = 1` → foo's turn
- `turn = 0` → bar's turn
- Threads wait using condition_variable until their turn arrives

Code Used:
----------------------------------------------------
#include <mutex>
#include <condition_variable>

class FooBar {
private:
    int n;
    std::mutex mtx;
    std::condition_variable cv;
    int turn = 1;

public:
    FooBar(int n) { this->n = n; }

    void foo(function<void()> printFoo) {
        for (int i = 0; i < n; i++) {
            std::unique_lock<std::mutex> lock(mtx);
            while (turn != 1)
                cv.wait(lock);

            printFoo();
            turn = 0;
            cv.notify_all();
        }
    }

    void bar(function<void()> printBar) {
        for (int i = 0; i < n; i++) {
            std::unique_lock<std::mutex> lock(mtx);
            while (turn != 0)
                cv.wait(lock);

            printBar();
            turn = 1;
            cv.notify_all();
        }
    }
};
----------------------------------------------------

Why This Works:
- Mutex ensures mutual exclusion
- Condition variable blocks thread without CPU spinning
- `while` loop prevents spurious wakeups
- `notify_all()` ensures correct wakeup order

Time Complexity:
- O(n)

Space Complexity:
- O(1)

----------------------------------------------------
Important Concurrency Concepts Used
----------------------------------------------------
✔ Mutual Exclusion (mutex)
✔ Thread Coordination (condition_variable)
✔ Spurious Wakeup Handling (while instead of if)
✔ Deterministic Alternation

----------------------------------------------------
Common Mistakes (Interview Red Flags)
----------------------------------------------------
✘ Using `if` instead of `while` with cv.wait()
✘ Forgetting to hold mutex before cv.wait()
✘ Using sleep() instead of synchronization
✘ Using notify_one() incorrectly (can deadlock)
✘ Busy waiting without blocking

----------------------------------------------------
Alternative Approach 1: Atomic Variable (Spin-based)
----------------------------------------------------

Concept:
- Use atomic int to represent turn
- Threads spin until their turn arrives

Code:
----------------------------------------------------
#include <atomic>

class FooBar {
private:
    int n;
    atomic<int> turn{0};

public:
    FooBar(int n) { this->n = n; }

    void foo(function<void()> printFoo) {
        for (int i = 0; i < n; i++) {
            while (turn != 0);
            printFoo();
            turn = 1;
        }
    }

    void bar(function<void()> printBar) {
        for (int i = 0; i < n; i++) {
            while (turn != 1);
            printBar();
            turn = 0;
        }
    }
};
----------------------------------------------------

Pros:
✔ Very simple
✔ No mutex overhead

Cons:
✘ CPU intensive (busy wait)
✘ Not recommended for production

Interview Note:
Use only if interviewer explicitly allows spinning.

----------------------------------------------------
Alternative Approach 2: Semaphore (Classic)
----------------------------------------------------

Concept:
- Use two semaphores
- foo starts unlocked
- bar starts locked

Pseudo:
----------------------------------------------------
sem foo(1), bar(0)

foo():
  wait(foo)
  printFoo
  signal(bar)

bar():
  wait(bar)
  printBar
  signal(foo)
----------------------------------------------------

Pros:
✔ Clean logic
✔ No shared state variable

Cons:
✘ Semaphores not native in standard C++

----------------------------------------------------
Alternative Approach 3: Two Mutex Locks (Lock Passing)
----------------------------------------------------

Concept:
- foo mutex initially unlocked
- bar mutex initially locked
- Threads unlock each other

Pseudo:
----------------------------------------------------
mutex foo_mtx, bar_mtx
bar_mtx.lock()

foo():
  lock(foo_mtx)
  printFoo
  unlock(bar_mtx)

bar():
  lock(bar_mtx)
  printBar
  unlock(foo_mtx)
----------------------------------------------------

Pros:
✔ No condition variables
✔ Simple lock handoff

Cons:
✘ Risky if unlock order is wrong

----------------------------------------------------
Comparison Summary
----------------------------------------------------
Approach              CPU Usage   Blocking   Interview Safe
----------------------------------------------------
Mutex + CV            Low         Yes        ⭐⭐⭐⭐⭐
Atomic Spin           High        No         ⭐⭐
Semaphore             Low         Yes        ⭐⭐⭐⭐
Dual Mutex            Low         Yes        ⭐⭐⭐

----------------------------------------------------
When to Use Which?
----------------------------------------------------
• Production / Real Systems → Mutex + Condition Variable
• Embedded / Low-level → Semaphore
• Teaching / Concept → Atomic spin
• Interview Default → Mutex + CV


