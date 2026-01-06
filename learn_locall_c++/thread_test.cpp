/*
  学习c++11 thread 线程库 相关知识用法
 */
#include <functional>
#include<iostream>
#include<thread>
#include<mutex>

// void func()
// {
//     cout << "func" << endl;
// }

// void add(int x,int y)
// {
//    int c=x+y;
//    cout<<c<<endl;
// }
// int main()
// {
//     //构造函数:
//     //默认构造，不会关联任何线程
//     thread t1;
//     //带参构造，关联一个线程
//     thread t2(func);
//     thread t3(add,1,2);
//     t2.join();
//     t3.join();
//     return 0;
// }







/*
    测试传值和传引用
*/
// void func1(int x)
// {
//    x=5;
//    cout<<"func x="<<x<<endl;
// }
// void func2(int&x)
// {
//     x=5;
//     cout<<"func2 x="<<x<<endl;
// }
// int main()
// {
//   int a=10;
//   thread t1(func1,a);//传值
//   t1.join();
//   cout<<"main a="<<a<<endl;

//   //传引用要用ref(a)或cref，
//   thread t2(func2,ref(a));//传引用
//   t2.join();
//   cout<<"main a="<<a<<endl;
//   return 0;
// }



// std::mutex mtx;
// void print(int n,int i)
// {
//   for(;i<n;i++)
//   {
//     mtx.lock();
//     cout<<this_thread::get_id()<<" "<<i<<endl;
//     mtx.unlock();
//   }
// }

// int main()
// {
//   thread t1(print,10,0);
//   thread t2(print,20,10);
//   t1.join();
//   t2.join();  
//   cout<<this_thread::get_id()<<endl;
//   return 0;
// }


/*
    this_thread用法  模拟倒计时
 */
//  int main()
//  {
//     //sleep_for模拟倒计时
//     for(int i=10;i>0;i--)
//     {
//         std::cout<<i<<std::endl;
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
//     std::cout<<"time's up"<<std::endl;
//     return 0;
//  }



/*
    mutex;
*/

// void print(int n,int&x,std::mutex&mtx)
// {
//   mtx.lock();
//   for(int i=0;i<n;i++)
//   {
//     ++x;
//   }
//   mtx.unlock();
// }
// int main()
// {
//   int x=0;
//   std::mutex mtx;
//   std::thread t1(print,1000,std::ref(x),std::ref(mtx));
//   std::thread t2(print,10000,std::ref(x),std::ref(mtx));
//   t1.join();
//   t2.join();
//   std::cout<<"x="<<x<<std::endl;
//   return 0;
// }


/*
    这种方式存在一定的风险，如果在占有锁期间，发生了异常，那么锁将无法被释放，导致死锁。
    解决这个问题，可以使用:
    std::lock_guard<std::mutex> lock(mtx);
    std::unique_lock<std::mutex> lock(mtx);
*/
// int main()
// {
//   int x=0;
//   std::mutex mtx;
//   std::thread t1([&](){
//     mtx.lock();
//     for(int i=0;i<1000;i++)
//     {
//       ++x;
//     }
//     mtx.unlock();
//   });
//   std::thread t2([&](){
//     mtx.lock();
//     for(int i=0;i<1000;i++)
//     {
//       ++x;
//     }
//     mtx.unlock();
//   });
//   t1.join();
//   t2.join();
//   std::cout<<"x="<<x<<std::endl;
//   return 0;
// }

// int main()
// {
//   std::mutex mtx;
//   int x=0;
//   auto Print=[&](int n){
//     std::lock_guard<std::mutex> lock(mtx);
//     for(int i=0;i<n;i++)
//     {
//       ++x;
//     }
//   };
//   std::thread t1(Print,1000);
//   std::thread t2(Print,10000);
//   t1.join();
//   t2.join();
//   std::cout<<"x="<<x<<std::endl;
//   return 0;
// }



// std::mutex mtx;
// void foo()
// {
//   std::lock_guard<std::mutex> lck(mtx);
//     std::cout << "In foo, lock acquired!" << std::endl;
// }

// void bar()
// {
//     std::lock_guard<std::mutex> lck(mtx);  // 采用已加锁的互斥锁
//     std::cout << "In bar, lock acquired!" << std::endl;
// }

// int main() {
//   std::thread t1(foo);
//   std::thread t2(bar);
//   t1.join();
//   t2.join();
//   std::cout<<"main end"<<std::endl;
//     return 0;
// }


// #include<unistd.h>
// std::mutex mtx;
// void print(int n)
// {
//   std::unique_lock<std::mutex> lock(mtx);
//   std::cout<<"print: "<<n<<std::endl;
// }
// int main()
// {
//    std::thread threads[10];
//    for(int i=0;i<10;i++)
//    {
//       threads[i]=std::thread(print,i+1);
//    }
//    for(int i=0;i<10;i++)
//    {
//       threads[i].join();
//    }
//    return 0;
// }


/*
  lock()跟try_lock()
*/





/*使用lock()会阻塞，直到所有锁都被获得，这样可以明确获得锁的顺序，避免死锁*/
#include<unistd.h>
std::mutex mtx1;
std::mutex mtx2;
// void task_a()
// {
//    std::lock(mtx1,mtx2);
//    std::cout<<"task_a end"<<std::endl;
//    std::lock_guard<std::mutex> lock1(mtx1,std::adopt_lock);
//    std::lock_guard<std::mutex> lock2(mtx2,std::adopt_lock);
// }
// void task_b()
// {
//    std::lock(mtx1,mtx2);
//    std::cout<<"task_b end"<<std::endl;
//    std::lock_guard<std::mutex> lock1(mtx1,std::adopt_lock);
//    std::lock_guard<std::mutex> lock2(mtx2,std::adopt_lock);
// }
// int main()
// {
//   mtx1.lock();
//   std::thread t1(task_a);
//   std::thread t2(task_b);
//   std::cout<<"t1 t2等待main线程获得锁mtx2"<<std::endl;
//   sleep(5);
//   std::cout<<"t1 t2等待main线程获得锁mtx2成功"<<std::endl;
//   mtx2.lock();
//   std::cout<<"main end"<<std::endl;
//   mtx1.unlock();
//   mtx2.unlock();
//   t1.join();
//   t2.join();
//   return 0;
// }

/*死锁，解决办法使用std::lock()函数模版*/
// void task_a()
// {
//   std::cout<<"t1请求mtx1"<<std::endl;
//   mtx1.lock();
//   std::cout<<"t1请求mtx1成功"<<std::endl;
//   sleep(1);
//   std::cout<<"t1请求mtx2"<<std::endl;
//   mtx2.lock();
//   std::cout<<"t1请求mtx2成功"<<std::endl;
//   sleep(1);
//   mtx1.unlock();
//   mtx2.unlock();
// }
// void task_b()
// {
//   std::cout<<"t2请求mtx2"<<std::endl;
//   mtx2.lock();
//   std::cout<<"t2请求mtx2成功"<<std::endl;
//   sleep(1);
//   std::cout<<"t2请求mtx1"<<std::endl;
//   mtx1.lock();
//   std::cout<<"t2请求mtx1成功"<<std::endl;
//   sleep(1);
//   mtx2.unlock();
//   mtx1.unlock();
// }
// int main()
// {
//   std::thread t1(task_a);
//   std::thread t2(task_b);
//   t1.join();
//   t2.join();
//   std::cout<<"main end"<<std::endl;
//   return 0;
// }



/*try_lock()非阻塞等待，获取锁成功返回true，失败的话返回未获取锁的编号，下标从0开始*/
//eg::
// std::mutex mtx3;
// void task_a()
// {
//    mtx1.lock();
//    std::cout<<"task_a请求mtx1成功"<<std::endl;
//    mtx2.lock();
//    std::cout<<"task_a请求mtx2成功"<<std::endl;
//    mtx3.lock();
//    std::cout<<"task_a请求mtx3成功"<<std::endl;
//    //释放锁;
//    mtx1.unlock();
//    mtx2.unlock();
//    mtx3.unlock();
// }
// void task_b()
// {
//    int ret=try_lock(mtx1,mtx2,mtx3);
//    if(ret==-1)
//    {
//       std::cout<<"task_b获取所有锁成功"<<std::endl;
//       //释放锁;
//       mtx1.unlock();
//       mtx2.unlock();
//       mtx3.unlock();
//    }
//    else
//    {
//       std::cout<<"task_b获取锁失败，失败锁的编号为:"<<ret<<std::endl;
//    }
// }
// int main()
// {
//   std::thread t1(task_a);
//   std::thread t2(task_b);
//   t1.join();
//   t2.join();
//   std::cout<<"main end"<<std::endl;
//   return 0;
// }


/*
call_once()函数模版，保证某个函数只被调用一次
   @param flag: 一次性的标记，保证某个函数只被调用一次
   @param func: 要执行的函数
   @param args: 要执行的函数的参数
   @return: void

*/
// std::once_flag flag;

// void winner(int id)
// {
//   std::cout<<"winner is "<<id<<std::endl;
// }
// void complete_task(int id)
// {
//   for (int i=0; i<1000; ++i)
//   std::this_thread::sleep_for(std::chrono::milliseconds(1));
//   std::call_once(flag,winner,id);
// }
// int main()
// {
//   std::thread threads[10];
//   for(int i=0;i<10;i++)
//   {
//      threads[i]=std::thread(complete_task,i+1);
//   }
//   for(int i=0;i<10;i++)
//   {
//     threads[i].join();
//   }
//   return 0;
// }




// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <queue>

// std::queue<int> buffer;         // 共享缓冲区
// std::mutex mtx;                 // 用于同步访问缓冲区的互斥锁
// std::condition_variable cv;     // 条件变量

// void producer() {
//     for (int i = 1; i <= 5; ++i) {
//         std::this_thread::sleep_for(std::chrono::milliseconds(500));  // 模拟生产过程
//         std::lock_guard<std::mutex> lock(mtx);  // 加锁
//         buffer.push(i);  // 向缓冲区中添加数据
//         std::cout << "Produced: " << i << std::endl;
//         cv.notify_one();  // 通知消费者线程
//     }
// }

// void consumer() {
//     while (true) {
//         std::unique_lock<std::mutex> lock(mtx);  // 加锁
//         cv.wait(lock, [](){ return !buffer.empty(); });  // 如果缓冲区为空，等待生产者
//         int item = buffer.front();  // 获取数据
//         buffer.pop();  // 从缓冲区中移除数据
//         std::cout << "Consumed: " << item << std::endl;
//         lock.unlock();  // 解锁
//         if (item == 5) break;  // 当消费到 5 时，退出循环
//     }
// }

// int main() {
//     std::thread t1(producer);  // 启动生产者线程
//     std::thread t2(consumer);  // 启动消费者线程

//     t1.join();  // 等待生产者线程完成
//     t2.join();  // 等待消费者线程完成

//     return 0;
// }




/*                               test                                   */



/*编写一个程序，创建两个线程来分别增加一个全局变量 counter 的值。
两个线程都对 counter 进行 1000 次加 1 操作。
使用 std::mutex 来保证对 counter 的访问是线程安全的。最后输出 counter 的值。*/

// int counter=0;
// std::mutex mtx;
// void add_counter()
// {
//    for(int i=0;i<1000;i++)
//    {
//     std::lock_guard<std::mutex> lock(mtx);
//     counter++;
//    }
// }
// int main()
// {
//   std::thread t1(add_counter);
//   std::thread t2(add_counter);
//   t1.join();
//   t2.join();
//   std::cout<<"counter is "<<counter<<std::endl;
//   return 0;
// }


/*使用 std::condition_variable 来解决生产者消费者问题。
一个生产者线程不断向缓冲区中生产数据，消费者线程从缓冲区中消费数据。
使用 std::mutex 和 std::condition_variable 来同步生产者和消费者线程。*/

#include<queue>
#include<mutex>
#include<condition_variable>

// std::queue<int> buffer;
// std::mutex mtx;
// std::condition_variable cv;
// #define MAX_BUFFER_SIZE 10
// int  i=0;
// void producer()
// {
//     while(true)
//     {
//        std::unique_lock<std::mutex> lock(mtx);
//        //等待知道buffer没有超过最大值
//        cv.wait(lock,[&](){
//            return buffer.size()<MAX_BUFFER_SIZE;
//        });
//        //wait成功,生产数据;
//        buffer.push(i++);
//        std::cout<<"product :"<<i-1<<std::endl;
//        //唤醒消费者;
//        cv.notify_all();
//     }
//  }
//  void consumer()
//  {
//     while(true)
//     {
//       std::unique_lock<std::mutex> lock(mtx);
//       cv.wait(lock,[&](){
//           return !buffer.empty();
//       });
//       int con=buffer.front();
//       std::cout<<"consum :"<<con<<std::endl;
//       buffer.pop();
//       cv.notify_all();
//     }

//  }
// int main()
// {
//   std::thread t1(producer);
//   std::thread t2(consumer);
//   t1.join();
//   t2.join();
//   return 0;
// }

