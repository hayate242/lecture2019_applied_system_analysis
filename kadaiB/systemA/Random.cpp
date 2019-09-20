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
int Random::genRand(int start, int end)
{
    std::random_device rnd;     // 非決定的な乱数生成器を生成
    std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    std::uniform_int_distribution<> rand0_2(start, end);        // [0, 2] 範囲の一様乱数
    return rand0_2(mt);
}