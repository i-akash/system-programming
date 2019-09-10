#include<filesystem>
#include<iostream>
#include<thread>


void search(std::filesystem::path &path){

    auto it=std::filesystem::directory_iterator(path);
    
    // for(std::filesystem::directory_entry &file:it){

    // }
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

