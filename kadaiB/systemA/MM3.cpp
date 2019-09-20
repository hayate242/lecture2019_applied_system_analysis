#include "MM3.hpp"
#include <algorithm>
#include <random>
#define SERVICE_NUM 3

std::tuple<double, double> MM3::simulation(double lambda, double mu, double startTime, double endTime)
{
    // 初期化
    vector<double> service;    // 窓口
    queue<double> systems[SERVICE_NUM];
    int selectedLine;
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
    // 窓口初期化
    for (int i = 0; i < SERVICE_NUM; i++){
        service.push_back(-1.0);
    }
    systems[0].push(3.0);
    for (int i = 0; i < SERVICE_NUM; i++)
    {
        systems[i].push(12.0);
        systems[i].push(2.0);
    }

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
            selectedLine = random.genRand(0, 2);
            systems[selectedLine].push(event.time);

            // イベントキューへのサービス到着イベントの登録
            event.selectedLine = selectedLine;
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
            visitors += systems[0].size() + systems[1].size() + systems[2].size();
            printf("visitors = %f\n", visitors);
            stayTime += currentTime - service[event.selectedLine];
            printf("stayTime = %f\n", stayTime);
            simEndJobs++;

            // 窓口のクリア（先頭の要素だけ削除）
            if(service.size() > 0){
                // service.erase(service.begin() + selectedLine);
                service[event.selectedLine] = -1.0;
            }
            break;
        }
        // 3つの列の窓口で，並んでる列の窓口に人がいなくて，待ち行列に人がいる場合
        if (service[event.selectedLine] == -1.0 && systems[event.selectedLine].empty() == false)
        {
            // printf("%f\n", service[event.selectedLine]);
            // 待ち行列の先頭から窓口への移動
            service.push_back(systems[selectedLine].front());
            systems[selectedLine].pop();
            // debug
            // {for(int i=0;i<service.size();i++) printf("%f,",service[i]);}
            // printf("size = %lu\n", service.size());
            // イベントキューへのサービス到着イベントの登録
            selectedLine = random.genRand(0, 2);
            event.selectedLine = selectedLine;
            event.time = currentTime + random.expDistribution(mu);
            event.eventState = FINISH;
            eventQueue.push(event);
        }
        // for (int i = 0; i < 3; i++){
        //     for (int k = 0; k < systems[i].size(); k++){
        //         printf("%f, ", systems[i]);
        //     }
        // }
        // printf("\n");
    }
    // 結果の返却
    // return std::forward_as_tuple(システム内客数, システム内時間);
    return std::forward_as_tuple(visitors / simEndJobs, stayTime / simEndJobs);
}
