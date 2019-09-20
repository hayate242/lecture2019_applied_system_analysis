#include "Random.hpp"
double Random::expDistribution(double variable)
{
   double expDist = 0.0;                             // 指数乱数用の宣言
   mt19937 mt(rnd());                                // メルセンヌ・ツイスター用の宣言
   uniform_real_distribution<> uni_random(0.0, 1.0); // 0−1 までの一様分布乱数
   // 指数乱数の取得
   expDist = -log(1.0-uni_random(mt))/variable;
   return expDist;
}
