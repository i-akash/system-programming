#include<filesystem>
#include<iostream>


class Search
{
private:
public:
    Search();
    ~Search();
};

Search::Search()
{
}

Search::~Search()
{
}


int main(int argc, char const *argv[])
{
    if(argc<2)return 0;
    auto path=std::filesystem::current_path();
    for (auto &d:std::filesystem::recursive_directory_iterator(path))
    {
        if(d.is_directory())
            continue;
            
        if(d.path().filename()==argv[1])
            {
                std::cout<<"Got it : "<<d.path().filename()<<"\n";
                break;
            } 
        std::cout<<"searching: "<<d.path().filename()<<"\n";       
    }
    return 0;
}

