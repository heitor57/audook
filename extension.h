#ifndef EXTENSION_H
#define EXTENSION_H
//get extension size
#include <string>
using namespace std;
#include <QString>

//
inline int ext_size(const string &str){
    unsigned int i=0,count=0;
    for(; i<str.length() ; i++){
        if(str.at(i)=='.'){
            i++;
            break;
        }
    }
    for(;i<str.length();i++,count++){}
    return count;
}
inline int ext_size(const QString &str){
    int i=0,count=0;
    for(; i<str.length() ; i++){
        if(str.at(i)=='.'){
            i++;
            break;
        }
    }
    for(;i<str.length();i++,count++){}
    return count;
}
//remove extension
inline string ext_del(string str){
    str.resize(str.length()-ext_size(str));
    return str;
}
inline QString ext_del(QString str){
    str.resize(str.length()-ext_size(str));
    return str;
}
//switch extension
inline string ext_switch(string str,string new_ext){
    str = ext_del(str);
    str+=new_ext;

    return str;
}
inline QString ext_switch(QString str,QString new_ext){
    str = ext_del(str);
    str += new_ext;

    return str;
}
inline string ext_name(const string &str){
    return str.substr(str.length()-ext_size(str),str.size()-1);
}
#endif // EXTENSION_H
