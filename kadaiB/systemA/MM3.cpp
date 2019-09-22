#include "MM3.hpp"
#include <algorithm>
#include <random>
#define SERVICE_NUM 3

std::tuple<double, double> MM3::simulation(double lambda, double mu, double startTime, double endTime)
{
    // 初期化
    vector<double> service;    // 窓口
    queue<double> systems[SERVICE_NUM]; //待ち行列
    int lastSelectedLine;
    Random random;             // ランダムクラスの宣言
    SimStat simStat = StandBy; // 過渡状態、過渡状態以降を定義
    double service_interval;   // サービス間隔
    double arrive;             // 到着間隔
    currentTime = 0.0;
    simEndJobs = 0;
    stayTime = 0.0;
    visitors = 0.0;
    // 処理開始（最初のイベントをイベントキューに登録）
    event.selectedLine = random.genRand(0, 2);
    event.time = currentTime + random.expDistribution(lambda);
    event.eventState = ARRIVE;
    eventQueue.push(event);
    // 窓口初期化
    for (int i = 0; i < SERVICE_NUM; i++){
        service.push_back(-1.0);
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
            systems[event.selectedLine].push(event.time);

            // イベントキューへのサービス到着イベントの登録
            event.selectedLine = random.genRand(0, 2);
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
            int cnt_in_service = 0;
            for (int i = 0; i < SERVICE_NUM; i++)
            {
                if(service[i] != -1.0 ){
                    cnt_in_service++;
                }
            }
            visitors += cnt_in_service + systems[0].size() + systems[1].size() + systems[2].size() - 1.0;
            // printf("visitors = %f\n", visitors);

            // printf("%f - %f ,staytime = %f\n", currentTime, service[event.selectedLine], currentTime - service[event.selectedLine]);
            stayTime += currentTime - service[event.selectedLine];
            // printf("currentTime= %f, service[%d] = %f, stayTime = %f\n", currentTime, event.selectedLine, service[event.selectedLine], stayTime);
            simEndJobs++;

            // 窓口のクリア（先頭の要素だけ削除）
            if(service.size() > 0){
                // service.erase(service.begin() + selectedLine);
                service[event.selectedLine] = -1.0;
            }
            break;
        }
        // 3つの列の窓口で，並んでる列の窓口に人がいなくて，待ち行列に人がいる場合
        for (int i = 0; i < SERVICE_NUM; i++){
            if (service[i] == -1.0 && systems[i].empty() == false)
            {
                // 待ち行列の先頭から窓口への移動
                service[i] = systems[i].front();
                systems[i].pop();
                // debug
                // {for(int i=0;i<service.size();i++) printf("%f,",service[i]);}
                // printf("size = %lu\n", service.size());
                // イベントキューへのサービス到着イベントの登録
                event.selectedLine = i;
                event.time = currentTime + random.expDistribution(mu);
                event.eventState = FINISH;
                eventQueue.push(event);
                break;
            }
        }

    }
    // 結果の返却
    // return std::forward_as_tuple(システム内客数, システム内時間);
    return std::forward_as_tuple(visitors / simEndJobs, stayTime / simEndJobs);
}
