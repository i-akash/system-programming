#include<filesystem>
#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<vector>
#include<queue>
#include<functional>
#include<future>
#include<iostream>

class ThreadPool
{
private:
    using Task=std::function<void()>;
    int numOfThreads;
    bool stoping,forceStop;
    std::mutex mutex;
    std::condition_variable ipc;
    std::vector<std::thread> threadpool;
    std::queue<Task>tasks;
    void run();
    
public:
    ThreadPool();
    ThreadPool(int numOfThreads);
    void start();
    template<typename T>
    auto addTask(T task)->std::future<decltype(task())>;
    void stop() noexcept;
    ~ThreadPool() noexcept;
};

ThreadPool::ThreadPool()
{
    this->numOfThreads=5;
    this->stoping=false;
    this->forceStop=false;
}

ThreadPool::ThreadPool(int numOfThreads)
{
    this->numOfThreads=numOfThreads;
    this->stoping=false;
    this->forceStop=false;
}

void ThreadPool::start(){
    
    for(int i=0;i<numOfThreads;i++){
        threadpool.emplace_back([=]{run();});
    }
}

template<typename T>
auto ThreadPool::addTask(T task)->std::future<decltype(task())>{
    auto wrapper=std::make_shared<std::packaged_task<decltype(task()) ()>>(std::move(task));
    {
        std::unique_lock<std::mutex> lock(mutex);
        tasks.emplace([=]{
            (*wrapper) ();
        });
    }
    ipc.notify_one();
    return wrapper->get_future();
}

void ThreadPool::run(){
    while (true)
    {
        Task task;
        {
            std::unique_lock<std::mutex> lock(mutex);
            ipc.wait(lock,[=]{return stoping||!tasks.empty();});

            if((stoping && tasks.empty()) || forceStop)
                break;
            task=tasks.front();
            tasks.pop();
            task();
        }
    }
}

void ThreadPool::stop()noexcept{
    {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->forceStop=true;
    }
    ipc.notify_all();
}

ThreadPool::~ThreadPool() noexcept
{
    {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->stoping=true;
    }
    ipc.notify_all();

    for(auto &t:threadpool)
        t.join();
}











class Search
{
    private:
    public:
        Search();
        bool run(std::filesystem::directory_entry d,const char filename[]);
        ~Search();
};

Search::Search()
{}

bool Search::run(std::filesystem::directory_entry d,const char filename[]){
                if(d.is_directory())
                    return false;
                for(int i=0;i<10000;i++)
                        ;
                            
                if(d.path().filename()==filename)
                    {
                        std::cout<<"---------------------------------------------------\n";
                        std::cout<<"Got it : "<<d.path()<<"\n"<<d.path().filename()<<"\n";
                        return true;
                    } 
                std::cout<<"Searching: "<<d.path().filename()<<"\n";       
            return false;
}

Search::~Search()
{

}



int cnt=0;
    
int main(int argc, char const *argv[])
{
    if(argc<2)return 0;
    
    {
        ThreadPool pool(200); 
        pool.start();

        auto path=std::filesystem::current_path();        
        auto it=std::filesystem::recursive_directory_iterator(path);
        
        for (auto &d:it)
        {
           auto f=pool.addTask([=]{
                Search search;
                return search.run(d,argv[1]);
            });

            if(f.get()){
                pool.stop();
                break;
            }
        }

    }

    return 0;
}

