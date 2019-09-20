#include "MM1.hpp"
#include <algorithm>

std::tuple<double, double> MM1::simulation(double lambda, double mu, double startTime, double endTime)
{
    // 初期化
    vector<double> service;    // 窓口
    queue<double> system;      // 待ち行列
    Random random;             // ランダムクラスの宣言
    SimStat simStat = StandBy; // 過渡状態、過渡状態以降を定義
    double service_interval;   // サービス間隔
    double arrive;             // 到着間隔
    currentTime = 0.0;
    simEndJobs = 0;
    stayTime = 0.0;
    visitors = 0.0;
    // 処理開始（最初のイベントをイベントキューに登録）
    event.time = currentTime + random.expDistribution(lambda);
    event.eventState = ARRIVE;
    eventQueue.push(event);

    // 終了時間まで繰り返す
    while (currentTime < endTime)
    {
        // 最初のイベントを取り出す
        event = eventQueue.top();
        eventQueue.pop();
        // 現在時刻をイベント発生時刻まですすめる
        currentTime = event.time;
        switch (event.eventState)
        {
        // 到着イベントの場合
        case ARRIVE:
            // 待ち行列への登録
            system.push(event.time);
            // イベントキューへのサービス到着イベントの登録
            event.time = currentTime + random.expDistribution(lambda);
            event.eventState = ARRIVE;
            eventQueue.push(event);
            break;
        // サービス終了イベントの場合
        case FINISH:
            // 過渡状態中の統計値はリセットする
            if (currentTime > startTime && simStat == StandBy)
            {
                simStat = Execution;
                stayTime = 0;
                visitors = 0;
                simEndJobs = 0;
            }
            // 統計処理（待ち時間，客数等のカウント）
            stayTime += currentTime - service[0];
            visitors += system.size() + service.size();
            simEndJobs++;

            // 窓口のクリア（先頭の要素だけ削除）
            if(service.size() > 0){
                service.erase(service.begin());
            }
            break;
        }
        // 3つの窓口のうち，一つでもあいていて、待ち行列に人がいる場合
        // if (service.empty() && system.empty() == false)
        if (service.size() < 3 && system.empty() == false)
        {
            // 待ち行列の先頭から窓口への移動
            service.push_back(system.front());
            system.pop();
            // 窓口の対応終了時間が早い順(降順)で並び替え
            std::sort(service.begin(), service.end(), std::greater<int>());
            // debug
            // {for(int i=0;i<service.size();i++) printf("%f,",service[i]);}
            // printf("size = %lu\n", service.size());
            // イベントキューへのサービス到着イベントの登録
            event.time = currentTime + random.expDistribution(mu);
            event.eventState = FINISH;
            eventQueue.push(event);
        }
        // queue<double> tmp = system;
        // for (int i = 0; i < tmp.size(); i++){
        //     printf("%f, cnt = %d", tmp.front(), i);
        //     tmp.pop();
        // }
        // printf("\n");
    }
    // 結果の返却
    // return std::forward_as_tuple(システム内客数, システム内時間);
    return std::forward_as_tuple(visitors / simEndJobs, stayTime / simEndJobs);
}