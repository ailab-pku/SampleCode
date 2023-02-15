#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <random>
#include <ctime>

template<typename T>
struct A{
    virtual A<T>& a() = 0;
};

struct B : public A<int> {
public :
    std::vector<B> x;
    B& a() override {
        static B b;
        b.x.emplace_back(*this);
        return b;
    }
    B() : x(0) {}
};

int main(){
    B b, c, d;
    b = c.a();
    printf("%d ", b.x.size());
    d = c.a();
    printf("%d, %d\n", b.x.size(), d.x.size());
    printf("%x, %x, %x", &b.x[0], &d.x[0], &d.x[1]);
    
    return 0;
}