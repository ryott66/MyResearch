#ifndef TSP_METHODS_HPP
#define TSP_METHODS_HPP

#include <array>

constexpr int N = 5;
constexpr int N2 = N * N;
constexpr std::array<double, 10> Cost = {30,25,40,45,50,40,50,35,20,40};
/*	4都市(6)	ver1{5,35,65,65,35,25}
	5都市(10)　	ver1{30,25,40,45,50,40,50,35,20,40}
  			ver2{30,30,80,80,80,30,80,10,30,30}
  			ver3{25,40,30,20,10000,30,50,10,40,10000}
	10都市(45)　ver1{48,96,75,52,30,40,54,21,5,23,54,100,20,13,11,94,8,7,84,100,30,22,23,47,42,25,35,30,9,47,74,72,1,44,75,53,73,33,33,67,32,8,68,36,73}
			ver2{43,79,90,85,9,43,39,24,45,35,58,28,67,42,78,7,44,79,33,2,30,81,71,37,42,65,34,94,25,93,15,23,39,58,100,41,44,11,31,10,39,24,92,58,96}
	12都市(66)　ver1{1,42,64,1,92,47,44,29,92,60,44,73,100,37,95,54,6,55,32,62,17,67,19,19,73,80,29,22,1,18,34,33,73,9,5,23,55,2,6,23,76,97,28,56,87,32,41,42,48,13,23,23,81,54,10,51,89,48,51,55,48,18,1,99,52,10}
*/
constexpr double Ini_X = 0.55;
constexpr double Nu = 0.005;
constexpr double Lambda = 0.5;
constexpr double Myu = 0.5;

class Weight {
public:
    int u;           // 接続先レーンインデックス
    double weight;   // 重み W[Vk][u]
};


void calcweight(std::vector<std::vector<Weight>>& W, std::vector<std::vector<double>>& costarray) {
    int l, s = 0;

    for (int v = 0; v < N2; v++) {
        int ct = 0;  // このノード v の有効エッジ数

        for (int u = 0; u < N2; u++) {
            if ((v / N) == (u / N)) {  // 同じ都市（都市番号）
                if (v != u) {
                    W[v][ct].u = u;
                    W[v][ct].weight = -Lambda;
                    ct++;
                }
            } else {  // 異なる都市
                if ((v % N) == (u % N)) {  // 順番が同じ
                    W[v][ct].u = u;
                    W[v][ct].weight = -Myu;
                    ct++;
                } else if ((abs((v % N) - (u % N)) != 1) &&
                           (abs((v % N) - (u % N)) != N - 1)) {
                    // 順番の差が1やN-1でなければ何もしない
                } else {
                    // 順番の差が1またはN-1（隣接順）
                    if ((v / N) > (u / N)) {
                        l = v / N;
                        s = u / N;
                    } else {
                        l = u / N;
                        s = v / N;
                    }
                    W[v][ct].u = u;
                    W[v][ct].weight = - costarray[s][l] * Nu;
                    ct++;
                }
            }
        }
    }
}


/* 
void calcL(double *Lvk, double *Xst, const std::vector<std::vector<Weight>>& W, double *dX, int *Nvk){ //Lの計算、それに伴うXの更新等

    double dt1=0;
    double dt2=0;
    double sum=0;
    double Outdt=0;
    double sumO=0;    //  全レーンのOutの合計
    double Indt=0;
    int Loffcity[N]={0};
    int Loff=0;


    for(int vk=0;vk<N2;vk++){   //全レーンでLを計算
        dt1=0;
        dt2=0;
        sum=0;
        for(int ct=0 ; ct < 4*(N-1) ; ct++){     //2重繰り返し
            dt1=Xst[W[vk][ct].u];//+rnd_generate(-0.003,0.003)
            dt2=sigmoid(35,0.6,dt1);
            sum+=W[vk][ct].weight *dt2;  //W[vk][ul]*sigmoid(Xul)
        }

        Lvk[vk]=1-sigmoid(1000,-0.5,sum);

        if(Lvk[vk] > 0.5){  //光照射されたレーンは粘菌が縮むのでOutを計算
            Outdt=sigmoid(30,0.2,Xst[vk]);//20?
            dX[vk]=0.002*Outdt;   //Ovkを固定
            sumO+=dX[vk];
        }
    }
    //Stock Redistribution(再分配)

    for(int name=0;name<N;name++){
        for(int num=0;num<N;num++){
            if(Lvk[name*N+num]<0.5){
                Loffcity[name]++;
                Loff++;
            }
        }
    }


    for(int vk=0;vk<N2;vk++){//広いInのレーンをみる
        if((Lvk[vk]<=0.5)){
            dX[vk]= (sumO / (double)Loff) + (Nvk[vk]*0.00025);  //全レーンのInを更新  LoffではなくN^2で割る案も
            Nvk[vk]=0;
        }
    }


    for(int vk=0;vk<N2;vk++){  //Xstの更新
        dX[vk]+=rnd_generate(-0.002,0.002);
        if(Lvk[vk] < 0.5){ //光照射なし
            if(Xst[vk]<1.0){
                //Inを足す
                Xst[vk] += dX[vk];
            }else{
             //NR
            }
        }else{
            Xst[vk] -= dX[vk]; 
        }
    }

}

*/

#endif
