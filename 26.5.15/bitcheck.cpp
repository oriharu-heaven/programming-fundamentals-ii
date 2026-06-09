#include <format>
#include <iostream>
int main() {
    int a = 0b0101;
    int b = 0b0011;
    std::cout << "     a: " << std::format("{:04b}", a) << std::endl;
    std::cout << " a & b: " << std::format("{:04b}", a & b) << std::endl;
    std::cout << " a | b: " << std::format("{:04b}", a | b) << std::endl;
    std::cout << " a ^ b: " << std::format("{:04b}", a ^ b) << std::endl;
    std::cout << "    ~a: " << std::format("{:04b}", ~a) << std::endl;
    std::cout << "a << 1: " << std::format("{:04b}", a << 1) << std::endl;
    std::cout << "a >> 1: " << std::format("{:04b}", a >> 1) << std::endl;
    
}
