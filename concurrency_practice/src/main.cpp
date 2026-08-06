#include <iostream>
#include "ThreadSafeQueue.hpp"


int main()
{
    ThreadSafeQueue pendingValues;

    pendingValues.push(5);
    pendingValues.push(9);
    pendingValues.push(6);//1233
    pendingValues.push(5);
    pendingValues.push(4);
    pendingValues.push(2);
    pendingValues.push(9);

    for (int i = 0; i < 7; ++i)
    {
        int value = pendingValues.waitAndPop();

        std::cout << "Popped value: "
                  << value
                  << '\n';
    }

    return 0;
}