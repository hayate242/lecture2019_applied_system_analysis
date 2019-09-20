#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
using namespace std;
class Random
{
private:
    random_device rnd; // 乱数用のシード
public:
    double expDistribution(double variable); // 指数乱数生成関数
    int genRand(int start, int end);
};