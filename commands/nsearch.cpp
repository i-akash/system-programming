#include<filesystem>
#include<iostream>
    
int main(int argc, char const *argv[])
{
    
    
    if(argc<2)return 0;

    auto path=std::filesystem::current_path();

    for (auto &d:std::filesystem::recursive_directory_iterator(path))
    {
        if(d.is_directory())
            continue;
        
        for(int i=0;i<10000;i++)
                ;    
        if(d.path().filename()==argv[1])
            {
                std::cout<<"---------------------------------------------------\n";
                std::cout<<"Got it : "<<d.path()<<"\n"<<d.path().filename()<<"\n";
                break;
            } 
        std::cout<<"searching: "<<d.path().filename()<<"\n";       
    }
    return 0;
}

