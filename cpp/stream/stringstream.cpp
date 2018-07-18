#include <iostream>
#include <sstream>
#include <string>
using namespace std;

string foo(string name) {
    stringstream stream;
    stream << "hello there my name is " << name << endl;
    return stream.str();
}

int main(int argc, char** argv)
{
    string name = "xxxxx";
    string ret = foo(name);
    std::cout << "ret:" << ret << std::endl;
}
