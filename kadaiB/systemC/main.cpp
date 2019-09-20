#include <stdio.h>
#include "cmdline.h"
#include "MM1.hpp"
#include <tuple>
using namespace std;

int main(int argc, char **argv)
{
    // 初期化
    MM1 mm1;                       // MM1 クラスの宣言
    cmdline::parser p;             // コマンドライン引数用の宣言
    double rho;                    // 利用率
    double system_people = 0.0;    // システム内客数
    double system_time = 0.0;      // システム内a時間
    tuple<double, double> results; // シミュレーション結果取得用変数
    double start_time = 1000.0;    // 過渡状態とみなすまでの時間
    double end_time = 11000.0;     // シミュレーション終了時間
    int trials = 10;
    /* コマンドライン引数の定義
    // −a 数値 とすると到着率　初期値は 1.0
    // −s 数値 とするとサービス率を代入　初期値は 2.0 */
    p.add<double>("arrival",'a', "arrival rate", false, 1.0);
    p.add<double>("service",'s', "service rate", false, 2.0);
    p.parse_check(argc, argv);
    double lambda = p.get<double>("arrival"); // 到着率
    double mu = p.get<double>("service");     // サービス率

    rho = (lambda / (mu*3)); // 到着率およびサービス率から利用率算出
    FILE *fp;
    if ((fp = fopen("Results.txt", "w")) == NULL)
    {
        printf("file open error !!\n");
        getchar();
        exit(EXIT_FAILURE);
    }

    // シミュレーション開始 (trials 回繰り返してその平均値を取得)
    for (int i = 0; i < trials; i++)
    {
        results = mm1.simulation(lambda, mu, start_time, end_time);
        system_people = system_people + get<0>(results); // システム内客数を取得
        system_time = system_time + get<1>(results);     // システム内時間を取得
        // fprintf(fp, "%lf,%lf,%lf\n", rho, get<0>(results), get<1>(results));
        fprintf(fp, "%lf,", get<0>(results));
    }
    fprintf(fp, "\n");
    system_people = system_people / trials;
    system_time = system_time / trials;

    // 理論値および観測値を保存
    //ここ直すだけ
    double logical_ave_customers = rho / (1 - rho);
    double logical_ave_wait_time = (1/(mu*3)) *  (1 / (1 - rho));
    // システム利用率ρ，システム内客数（サービスを受けている人＋サービスを待っている人），平均システム遅延（システム到着から出るまで）
    fprintf(fp, "% lf\t % lf\t % lf\n", rho, logical_ave_customers, logical_ave_wait_time);
    fprintf(fp, "% lf\t % lf\t % lf\n", rho, system_people, system_time);
    fclose(fp);
}