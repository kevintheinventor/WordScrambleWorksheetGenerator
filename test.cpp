#include <iostream> 

using namespace std;

class Force
{
    public:
    Force(int mag) : magnitude(mag){};
    Force& operator += (const Force &otherForce);
    private:
    int magnitude;
};

int main()
{
    return 0;
}