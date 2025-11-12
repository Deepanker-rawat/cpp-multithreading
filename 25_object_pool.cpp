#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <memory>
#include <vector>
#include <chrono>

class Object
{
public:
    int id;
    Object(int id) : id(id) {}
    void use()
    {
        std::cout << "Object " << id << " used by thread "
                  << std::this_thread::get_id() << "\n";
    }
};

class ObjectPool
{
    std::queue<std::shared_ptr<Object>> pool;
    std::mutex mtx;

public:
    ObjectPool(int size)
    {
        for (int i = 0; i < size; ++i)
            pool.push(std::make_shared<Object>(i));
    }

    std::shared_ptr<Object> acquire()
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (pool.empty())
            return nullptr;
        auto obj = pool.front();
        pool.pop();
        return obj;
    }

    void release(std::shared_ptr<Object> obj)
    {
        std::lock_guard<std::mutex> lock(mtx);
        pool.push(obj);
    }
};

void worker(ObjectPool &pool)
{
    for (int i = 0; i < 5; ++i)
    {
        auto obj = pool.acquire();
        if (obj)
        {
            obj->use();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            pool.release(obj);
        }
        else
        {
            std::cout << "Thread " << std::this_thread::get_id()
                      << " waiting (no objects)\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

int main()
{
    ObjectPool pool(2); // pool of 2 reusable objects
    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i)
        threads.emplace_back(worker, std::ref(pool));

    for (auto &t : threads)
        t.join();

    return 0;
}
