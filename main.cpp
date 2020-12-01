#include <stdlib.h>
#include <string>

int main(){
    char command[100];
    std::string test = "python python_scripts/test.py";
    system(test.c_str());

}