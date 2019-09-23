#include <iostream>
#include <cmath>
#include <iomanip>
#include <queue>
#include <random>
#include <tuple>
#include <vector>
#include "Random.hpp"
// イベントの種類を定義
enum EventState
{
    ARRIVE = 0,
    FINISH = 1
};
// シミュレーション状態を定義
enum SimStat
{
    StandBy = 0,
    Execution = 1,
    Finish = 2
};
// イベント型を定義
struct EVENT
{
    // イベントの種類
    EventState eventState;
    // イベントの発生時間
    double time;
    // 選んだ列名
    int selectedLine;
    // Priority queue 用のオーバロード関数
    bool operator>(const EVENT &event2) const
    {
        return time > event2.time;
    }
};
class MM3
{
    private:
        double currentTime;                                              // 現在時刻
        EVENT event;                                                     // イベントを格納する変数
        priority_queue<EVENT, vector<EVENT>, greater<EVENT>> eventQueue; // イベントキュー
        double simEndJobs;                                                  // シミュレーション終了までに発生したジョブ数
        double stayTime;                                                 // 待ち時間
        double visitors;                                                 // 客数
    public:
        std::tuple<double, double> simulation(double rambda, double mu, double startTime, double endTime);
};