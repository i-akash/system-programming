#include<filesystem>
#include<iostream>
#include<sys/stat.h>
#include<grp.h>
#include<pwd.h>
#include<string.h>

std::string getFileType(short mode){
    switch (mode & S_IFMT)
    {
        case S_IFDIR:
            return "directory";
        case S_IFREG:
            return "file     ";
        case S_IFLNK:
            return "link     ";
        case S_IFBLK:
        case S_IFCHR:
            return "device   ";
        case S_IFSOCK:
            return "socket   ";        
        default:
            return "link     ";
    }
}

char getPermissionSpecifier(short bit){
    switch (bit%3)
    {
    case 2:
        return 'r';
    case 1:
        return 'w';
    default:
        return 'x';
    }
}

std::string getFilePermission(short mode){
    std::string permision="";

    for(short i=8;i>=0;i--){
        if(mode&1<<i)
            permision+=getPermissionSpecifier(i);
        else 
            permision+='-';
    }
    return permision;
}




void print_l(const char *filepath,std::string name){

        struct stat sb;
        stat(filepath,&sb);
        mode_t mode=sb.st_mode;
        group *grp=nullptr;
        passwd *own=nullptr;


        grp=getgrgid(sb.st_gid);
        own=getpwuid(sb.st_uid);
        


        char timestamp[250];
        strftime(timestamp,sizeof timestamp,"%D %T",gmtime(&sb.st_mtim.tv_sec));

        std::cout<<getFileType(mode)<<" "
                 <<getFilePermission(mode)<<" "
                 <<sb.st_nlink<<" ";
        if(own!=nullptr)    
                 std::cout<<own->pw_name<<" ";
        else 
                std::cout<<"--"<<" ";

        if(grp!=nullptr)
                 std::cout<<grp->gr_name<<" ";
        else 
                std::cout<<"--"<<" ";
                 
        std::cout<<sb.st_size<<" "
        <<timestamp<<" "
        <<name<<"\n";
}

void print(std::string name){
        std::cout<<name<<" ";
}

void setArgs(const char argv,bool *a,bool *l){

        if(argv=='a')
            *a=false;
        if(argv=='l')
            *l=true;
}

int main(int argc, char const *argv[])
{

    bool a=true,l=false;
    if(argc==2){
        int length=strlen(argv[1]);
        if(length>1)
            setArgs(argv[1][1],&a,&l);
        if(length>2)
            setArgs(argv[1][2],&a,&l);
    }


    auto path=std::filesystem::current_path();
    auto iterator=std::filesystem::directory_iterator(path,std::filesystem::directory_options::follow_directory_symlink);
    
    std::cout<<"\n";
    for(auto &file:iterator){
        const char  *filepath=file.path().c_str();
        std::string name=file.path().filename();
        
        if(a  && name[0]=='.')
            continue;

        if(l) print_l(filepath,name);
        else  print(name);     
    }
    std::cout<<"\n";
       
    return 0;
}
