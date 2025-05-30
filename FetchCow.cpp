#define farmer 0
#include <bits/stdc++.h>
using namespace std;

//P1518 两只塔姆沃思牛
/*
 * 错误：我tm把两种情况写在一起了。
 */

/**
 * 这道洛谷oj又犯病了
 */

//需要将边框变为障碍物，这样方便判断
char matrix[12][12];
int f[3],c[3],ans=0;
bool uniqueToken[160005];
/**
 * 这是一道典型的模拟题
 *
 * 他们这么多骗分的方法吗？到底如何判断抓不到？
 *
 * 牛逼的生成专属值
 */

/**
 * 移动
 *
 * 0北
 * 1东
 * 2南
 * 3西
 *
 * d方向
 * type 0农夫 1奶牛
 */
void move(int x,int y,int d,int type){
    if(d==0){
        //遇到障碍物
        if(matrix[x-1][y]=='*'){
            if(type==farmer){
                f[0]=1;
            }else{
                c[0]=1;
            }
        }else{
            if(type==farmer){
                f[1]--;
            }else{
                c[1]--;
            }
        }
    }else if(d==1){
        if(matrix[x][y+1]=='*'){
            if(type==0){
                f[0]=2;
            }else{
                c[0]=2;
            }
        }else{
            if(type==0){
                f[2]++;
            }else{
                c[2]++;
            }
        }
    }else if(d==2){
        if(matrix[x+1][y]=='*'){
            if(type==0){
                f[0]=3;
            }else{
                c[0]=3;
            }
        }else{
            if(type==0){
                f[1]++;
            }else{
                c[1]++;
            }
        }
    }else if(d==3){
        if(matrix[x][y-1]=='*'){
            if(type==0){
                f[0]=0;
            }else{
                c[0]=0;
            }
        }else{
            if(type==0){
                f[2]--;
            }else{
                c[2]--;
            }
        }
    }
}


/**
 * 判断结果
 * @return
 */
int result(){
    if(f[1]==c[1]&&f[2]==c[2]){
        return 0;
    }else{
        return 1;
    }
}



/**
 * 主要难点在于判断是否相遇
 * 两只塔姆沃斯牛，模拟题
 * @return
 */
int main(){
    for(int idx=0;idx<12;idx++) matrix[idx][0] = '*',matrix[idx][11] = '*',matrix[0][idx] = '*',matrix[11][idx] = '*';
    for(int i=1;i<=10;i++){
        for(int j=1;j<=10;j++){
            cin>> matrix[i][j];
            if(matrix[i][j]=='F') f[1] = i,f[2]=j;
            if(matrix[i][j]=='C') c[1] = i,c[2]=j;
        }
    }
    f[0]=0;
    f[0]=0;

    int token = 0;

    while(result()){
        token = f[1]+f[2]*10+c[1]*100+c[2]*1000+f[0]*10000+c[0]*40000;
        //如果已经存在过这种状态
        if(uniqueToken[token]){
            cout<<0<<endl;
            return 0;
        }
        uniqueToken[token] = 1;
        //移动农夫
        //移动奶牛
        move(f[1],f[2],f[0],0);
        move(c[1],c[2],c[0],1);

        //记录秒数
        ans++;
    }
    cout<<ans<<endl;
    return 0;
}