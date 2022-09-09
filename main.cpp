#include <iostream>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <chrono>


void StringToScope(std::string& str){

    //change upper char to lower
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    //create multiset to find same characters
    // use std::move to  show explicitly the compiler that the object is temporary
    std::unordered_multiset<char> cont_set = std::move(std::unordered_multiset<char>(str.begin(),str.end()));

    //change string
    for(auto itb = str.begin(); itb != str.end(); itb++){

        if(cont_set.count(*itb)>1){
            *itb = ')';
        }
        else{
            *itb = '(';
        }
    }
}
int main() {
    std::string first = "din"; //"((("
    StringToScope(first);
    std::string second = "recede"; //"()()()"
    StringToScope(second);
    auto start = std::chrono::high_resolution_clock::now();
    std::string third = "Success"; //")())())"
    StringToScope(third);
    auto stop = std::chrono::high_resolution_clock::now();
    std::string fourth = "(( @"; //`"))(("
    StringToScope(fourth);



    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout<<"microseconds: "<<duration.count()<<std::endl;
    std::cout<<first<<std::endl<<second<<std::endl<<third<<std::endl<<fourth<<std::endl;

    return 0;
}
