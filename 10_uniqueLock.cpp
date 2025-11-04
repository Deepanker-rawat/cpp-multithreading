// TOPIC: unique_lock In C++ (std::unique_lock<mutex> lock(m1))

// NOTES:
// 1. The class unique_lock is a mutex ownership wrapper.
// 2. It Allows:
//	   a. Can Have Different Locking Strategies
//	   b. time-constrained attempts at locking (try_lock_for, try_lock_until)
//	   c. recursive locking
//	   d. transfer of lock ownership (move not copy)
//	   e. condition variables. (See this in coming videos)

// Locking Strategies
//    TYPE       	  	EFFECTS(S)
// 1. defer_lock		do not acquire ownership of the mutex.
// 2. try_to_lock		try to acquire ownership of the mutex without blocking.
// 3. adopt_lock		assume the calling thread already has ownership of the mutex.

#include<iostream>
#include<thread>
#include<mutex>
using namespace std;

mutex m1;
int buffer = 0;

void task(const char* c,int loopFor){
    unique_lock<mutex> lock(m1);  //Automatically calls lock on mutex m1.
    // m1.lock();
    for(int i=0;i<loopFor;i++){
        buffer++;
        cout<<c<<buffer<<endl;
    }
    // m1.unlock();
}

int main(){
    thread t1(task,"T1 ",10);
    thread t2(task,"T2 ",10);
    t1.join();
    t2.join();
    return 0;
}

/*
<-----Output------>
deepankerrawat@MacBookAir Multi_Threading % ./a.out
T1 1
T1 2
T1 3
T1 4
T1 5
T1 6
T1 7
T1 8
T1 9
T1 10
T2 11
T2 12
T2 13
T2 14
T2 15
T2 16
T2 17
T2 18
T2 19
T2 20
*/


// EXAMPLE: 2
/*
For defer lock:-
unique_lock<mutex> lock(m1,defer_lock);  //Does not call lock on mutex m1, because used defer_lock .
// we can also write no of line codes before lock.
lock.lock();   //But then we will have to explicitly tell to lock when ever we want lock mutex m1.
*/

void task(const char* threadNumber, int loopFor) {
	std::unique_lock<mutex> lock(m1, std::defer_lock); // Does not call lock on mutex m1, because used defer_lock
	lock.lock();  // But then we will have to explicitly tell to lock when ever we want to lock mutex m1.
	for(int i=0; i<loopFor; ++i){
		buffer++;
		cout << threadNumber << buffer << endl;
	}
	// lock.unlock(); is not needed as it will be unlocked in destructor of unique_lock.
}

int main() {
	thread t1(task, "T1 ", 10);
	thread t2(task, "T2 ", 10);
	t1.join();
	t2.join();
	return 0;
}

/*
<-------Output------->
deepankerrawat@MacBookAir Multi_Threading % ./a.out                  
T1 1
T1 2
T1 3
T1 4
T1 5
T1 6
T1 7
T1 8
T1 9
T1 10
T2 11
T2 12
T2 13
T2 14
T2 15
T2 16
T2 17
T2 18
T2 19
T2 20
*/
