//
// Created by hsl on 2025/5/30.
//


#include<iostream>
#include<cstdio>
//#include<bits/stdc++.h>
using namespace std;

const int n = 101;//身高范围有多少个数字
const int m = 1001;//活跃度范围有多少个数字
const int MINX = 100;
const int MAXX = 200;
const int MINY = 0;
const int MAXY = 1000;

int tree[n<<2][m<<2];//维护的是区间最大值，没有懒信息

int innerBuild(int yl,int yr,int xi,int yi){
    tree[xi][yi] = -1;
    if(yl<yr){
        int mid = (yl+yr)>>1;
        innerBuild(yl,mid,xi,yi<<1);
        innerBuild(mid+1,yr,xi,yi<<1|1);
    }
}

void innerUpdate(int jobi,int jobv,int yl,int yr,int xi,int yi){
    if(yl==yr){
        tree[xi][yi] = max(tree[xi][yi],jobv);
    }else{
        int mid = (yl+yr)>>1;
        if(jobi<=mid){
            innerUpdate(jobi,jobv,yl,mid,xi,yi<<1);
        }else{
            innerUpdate(jobi,jobv,mid+1,yr,xi,yi<<1|1);
        }
        tree[xi][yi] = max(tree[xi][yi<<1],tree[xi][(yi<<1)|1]);
    }
}

/**
 * 内部区间查询
 * @param jobl
 * @param jobr
 * @param yl
 * @param yr
 * @param xi
 * @param yi
 * @return
 */
int innertQuery(int jobl,int jobr,int yl,int yr,int xi,int yi){
    if(jobl<=yl && jobr>=yr){
        return tree[xi][yi];
    }else{
        int mid = (yl+yr)>>1;
        int ans = -1;
        if(jobl<=mid){
            //TODO
            ans = max(ans,innertQuery(jobl,jobr,yl,mid,xi,yi<<1));
        }
        if(jobr>mid){
            ans = max(ans, innertQuery(jobl,jobr,mid+1,yr,xi,yi<<1|1));
        }
        return ans;
    }
}

/**
 * 构建外部线段树
 * @param xl
 * @param xr
 * @param xi
 */
void outerBuild(int xl,int xr,int xi){
    innerBuild(MINY,MAXY,xi,1);
    if(xl<xr){
        int mid = (xl+xr)>>1;
        outerBuild(xl,mid,xi<<1);
        outerBuild(mid+1,xr,xi<<1|1);
    }
}

/**
 *
 * @param jobx 身高
 * @param joby 活泼度
 * @param jobv 缘分值
 * @param xl
 * @param xr
 * @param xi 外部线段树的索引
 */
void outerUpdate(int jobx,int joby,int jobv,int xl,int xr,int xi){
    innerUpdate(joby,jobv,MINY,MAXY,xi,1);
    if(xl<xr){//一路向下，所有的线段树都需要更新
        int mid = (xl+xr)>>1;
        if(jobx<=mid){
            outerUpdate(jobx,joby,jobv,xl,mid,xi<<1);
        }
        if(jobx>mid){
            outerUpdate(jobx,joby,jobv,mid+1,xr,xi<<1|1);
        }
    }
}

/**
 * 查询身高在[jobxl,jobxr],活泼度在[jobyl,jobyr]中缘分值最高的
 * @param jobxl
 * @param jobxr
 * @param jobyl
 * @param jobyr
 * @param xl
 * @param xr
 * @param xi
 * @return
 */
int outerQuery(int jobxl,int jobxr,int jobyl,int jobyr,int xl,int xr,int xi){
    if(jobxl<=xl && jobxr>=xr){
        return innertQuery(jobyl,jobyr,MINY,MAXY,xi,1);
    }else{
        int mid = (xl+xr)>>1;
        int ans = -1;
        if(jobxl<=mid){
            //TODO
            ans = max(ans,outerQuery(jobxl,jobxr,jobyl,jobyr,xl,mid,xi<<1));
        }
        if(jobxr>mid){
            ans = max(ans, outerQuery(jobxl,jobxr,jobyl,jobyr,mid+1,xr,xi<<1|1));
        }
        return ans;
    }
}


//一道树套树的例题，线段树套线段树
//https://acm.hdu.edu.cn/showproblem.php?pid=1823
//身高，活跃度，缘分值
//身高[100,200]，活跃度[0.0,100.0]
//tree in tree
//int main(){
//    ios::sync_with_stdio(false);
//    cin.tie(nullptr);
//    int q;
//    cin>>q;
//    while(q!=0){
//        int op_num;
//        op_num = q;
//        char op;
//        outerBuild(MINX,MAXX,1);
//        for(int idx=1;idx<=op_num;idx++){
//            cin>>op;
//            int h;
//            double a,l;
//            int hl,hr;
//            double al,ar;
//            if(op=='I'){
//                cin>>h>>a>>l;
//                //
//                outerUpdate(h,a*10,l*10,MINX,MAXX,1);
//            }else if(op=='Q'){
//                cin>>hl>>hr>>al>>ar;
//                //交换
//                if(hl>hr){
//                    hr = hl^hr;
//                    hl = hl^hr;
//                    hr = hl^hr;
//                }
//                if(al>ar){
//                    double temp = ar;
//                    ar = al;
//                    al = temp;
//                }
//                double res = (double)outerQuery(hl,hr,al*10,ar*10,MINX,MAXX,1)/10;
//                printf("%.1lf\n",res);
//            }
//        }
//        cin>>q;
//    }
//    return 0;
//}

int main() {
    int q;
    scanf("%d", &q);
    while(q != 0) {
        outerBuild(MINX, MAXX, 1);
        for (int i = 0; i < q; i++) {
            char op[2];
            scanf("%s", op);
            if (op[0] == 'I') {
                int a;
                double b, c;
                scanf("%d %lf %lf", &a, &b, &c);
                outerUpdate(a, (int)(b * 10), (int)(c * 10), MINX, MAXX, 1);
            } else {
                int a, b;
                double c, d;
                scanf("%d %d %lf %lf", &a, &b, &c, &d);
                int xl = min(a, b);
                int xr = max(a, b);
                int yl = min((int)(c * 10), (int)(d * 10));
                int yr = max((int)(c * 10), (int)(d * 10));
                int ans = outerQuery(xl, xr, yl, yr, MINX, MAXX, 1);
                if (ans == -1) {
                    printf("-1\n");
                } else {
                    printf("%.1f\n", ans / 10.0);
                }
            }
        }
        scanf("%d", &q);
    }
    return 0;
}
