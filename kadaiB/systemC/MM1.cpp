#include "MM1.hpp"
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
            visitors += system.size();
            simEndJobs++;

            // 窓口のクリア
            service.clear();
            break;
        }
        // 窓口があいていて、待ち行列に人がいる場合
        // if (service.empty() && system.empty() == false)
        if (service.empty() && system.empty() == false)
        {
            // 待ち行列の先頭から窓口への移動
            service.push_back(system.front());
            system.pop();
            // イベントキューへのサービス到着イベントの登録
            event.time = currentTime + random.expDistribution(3*mu);
            event.eventState = FINISH;
            eventQueue.push(event);
        }
        // printf("%f \n", currentTime);
    }
    // 結果の返却
    // return std::forward_as_tuple(システム内客数, システム内時間);
    return std::forward_as_tuple(visitors / simEndJobs, stayTime / simEndJobs);
}