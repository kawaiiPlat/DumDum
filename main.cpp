#include <stdlib.h>
#include <string>

int main(){
    char command[100];
    std::string test = "python test.py";
    system(test.c_str());

}