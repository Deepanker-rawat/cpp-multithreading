// TOPIC: lock_guard In C++ (std::lock_guard<mutex> lock(m1))
// where lock() is an object and m1 -> is the utex object.

// NOTES:
// 0. It is very light weight wrapper for owning mutex on scoped basis.
// 1. It aquires mutex lock the moment you create the object of lock_guard.
// 2. It automatically removes the lock while goes out of scope.
// 3. You can not explicitly unlock the lock_guard.
// 4. You can not copy lock_guard.

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

std::mutex m1;
int buffer = 0;

void task(const char* threadNumber, int loopFor) {

    std::lock_guard<mutex> lock(m1);
	for(int i=0; i<loopFor; ++i){
		buffer++;
		cout << threadNumber << buffer << endl;
	}
}

int main() {
	thread t1(task, "T0 ", 10);
	thread t2(task, "T1 ", 10);
	t1.join();
	t2.join();
	return 0;
}

/*
deepankerrawat@MacBookAir Multi_Threading % ./a.out                 
T0 1
T0 2
T0 3
T0 4
T0 5
T0 6
T0 7
T0 8
T0 9
T0 10
T1 11
T1 12
T1 13
T1 14
T1 15
T1 16
T1 17
T1 18
T1 19
T1 20
*/
