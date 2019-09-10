#include<iostream>
#include<thread>
#include<vector>
#include<mutex>
#include<condition_variable>
#include<functional>
#include<future>
#include<queue>

class Pool
{
    public:
        Pool();
        Pool(int nthreads);
        template<typename T>
        auto enque(T task)->std::future<decltype(task())>;
        ~Pool();

    private:
        using Task=std::function<void()>;
        std::vector<std::thread>rthreads;        
        std::queue<std::function<void()>>tasks;
        int nthreads;
        std::mutex mutex;
        std::condition_variable ipc;
        bool stopping;

        void start();
        void run();
        void stop()noexcept;
};



Pool::Pool()
{
    stopping=false;
    this->nthreads=10;
    start();
}

Pool::Pool(int nthreads){
   stopping=false;
    this->nthreads=nthreads;
    start();
}

Pool::~Pool()
{
     stop();
}


void Pool::start(){
     for(int nthread=0;nthread<nthreads;nthread++){
        rthreads.emplace_back([=]{run();});
    }
}
template<typename T>
auto Pool::enque(T task)->std::future<decltype(task())>
{
    auto wrapper=std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));

    {
        std::unique_lock<std::mutex> lock(mutex);
        tasks.emplace([=]{
            (*wrapper)();
        });
    }
    ipc.notify_one();
    return wrapper->get_future();
}


void Pool::run(){
    
    while (true)
            {
                Task task;
               {
                    std::unique_lock<std::mutex> lock(mutex);
                    ipc.wait(lock,[=]{return (stopping || !tasks.empty());});
                    if(stopping && tasks.empty())
                            break;
                    task=tasks.front();
                    tasks.pop();
               }
               task();
            }
}

void Pool::stop()noexcept{
    
    {
        std::unique_lock<std::mutex> lock(mutex);
        stopping=true;
    }
    ipc.notify_all();

    for(int nthread=0;nthread<nthreads;nthread++)
    {
        rthreads[nthread].join();
    }
    

}


int main(int argc, char const *argv[])
{
    Pool pool(2);
    auto f1=pool.enque([=]{
        std::cout<<std::this_thread::get_id()<<"\n";
        return 2;
        });

    auto f2=pool.enque([=]{
        std::cout<<std::this_thread::get_id()<<"\n";
        return 2;
        });

    auto f3=pool.enque([=]{
            std::cout<<std::this_thread::get_id()<<"\n";
            return 2;
            });
    auto f4=pool.enque([=]{
            std::cout<<std::this_thread::get_id()<<"\n";
            return 2;
            });
    auto f5=pool.enque([=]{
            std::cout<<std::this_thread::get_id()<<"\n";
            return 2;
            });
    auto f6=pool.enque([=]{
            std::cout<<std::this_thread::get_id()<<"\n";
            return 2;
            });
    auto f7=pool.enque([=]{
            std::cout<<std::this_thread::get_id()<<"\n";
            return 2;
            });

    std::cout<<f1.get()+f2.get()+f3.get()+f4.get()+f5.get()+f6.get()+f7.get()<<"\n";

    return 0;
}
