//
// Created by hsl on 2025/1/17.
//

#include<iostream>
using namespace std;

//L:0 R:1
const int MAXN = 2*100000+1;
char arr[MAXN];
int len[MAXN<<2];
int pre[MAXN<<2];
int suf[MAXN<<2];
char pre_item[MAXN<<2];
char suf_item[MAXN<<2];
//维护信息的设计？
int n,q;

void up(int lc,int rc,int i){
    int l = i<<1;
    int r = i<<1|1;
    //增加上可能出现的新序列
    len[i] = max(max(len[l],len[r]),suf_item[l] != pre_item[r]?suf[l]+pre[r]:0);
    pre[i] = lc==pre[l] && suf_item[l]!=pre_item[r]?(pre[l]+pre[r]):pre[l];
    suf[i] = rc==suf[r] && suf_item[l]!=pre_item[r]?(suf[r]+suf[l]):suf[r];
    //TODO 第二个错误
    pre_item[i] = pre_item[l];
    suf_item[i] = suf_item[r];
}

void build(int l,int r,int i){
    if(l==r){
        //都是1
        len[i] = 1;
        pre[i] = 1;
        suf[i] = 1;
        //反正arr中全是0
        pre_item[i] = arr[l];
        suf_item[i] = arr[l];
        //rev[i] = false;
    }else{
        int mid = (l+r)>>1;
        build(l,mid,i<<1);
        build(mid+1,r,i<<1|1);
        up(mid-l+1,r-mid,i);
        //rev[i] = false;
    }
}


void reverse(int jobl,int jobr,int l,int r,int i){
    //这个好像不需要懒操作
    if(l==r){
        //TODO 第一个错误
        pre_item[i] = 1-pre_item[i];
        suf_item[i] = 1-suf_item[i];
    }else{
        int mid = (l+r)>>1;
        //没有懒操作
        //down()
        if(jobl<=mid){
            reverse(jobl,jobr,l,mid,i<<1);
        }
        if(jobr>mid){
            reverse(jobl,jobr,mid+1,r,i<<1|1);
        }
        up(mid-l+1,r-mid,i);
    }
}

int query(int jobl,int jobr,int l,int r,int i,int type){
    if(jobl<=l && jobr>=r){
        if(type==0){
            return len[i];
        }else if(type==1){
            return pre[i];
        }else if(type==2){
            return suf[i];
        }else if(type==3){
            return pre_item[i];
        }else if(type==4){
            return suf_item[i];
        }
    }else if(type==0){
        int mid = (l+r)>>1;
        if(jobr<=mid){
            return query(jobl,jobr,l,mid,i<<1,0);
        }
        if(jobl>mid){
            return query(jobl,jobr,mid+1,r,i<<1|1,0);
        }
        //没有懒信息下推
        int ans = 0;
        int len1 = 0,len2 = 0;
        len1 = query(jobl,mid,l,mid,i<<1,0);
        len2 = query(mid+1,jobr,mid+1,r,i<<1|1,0);

        int suf = query(jobl,mid,l,mid,i<<1,2);
        int pre = query(mid+1,jobr,mid+1,r,i<<1|1,1);

        int item1 = query(jobl,mid,l,mid,i<<1,4);
        int item2 = query(mid+1,jobr,mid+1,r,i<<1|1,3);

        return max(max(len1,len2),item1!=item2?suf+pre:0);
    }else if(type==1){//查前缀
        int mid = (l+r)>>1;
        if(jobr<=mid){
            return query(jobl,jobr,l,mid,i<<1,1);
        }
        if(jobl>mid){
            return query(jobl,jobr,mid+1,r,i<<1|1,1);
        }

        int pre1 = query(jobl,mid,l,mid,i<<1,1);
        int pre2 = query(mid+1,jobr,mid+1,r,i<<1|1,1);

        int item1 = query(jobl,mid,l,mid,i<<1,4);
        int item2 = query(mid+1,jobr,mid+1,r,i<<1|1,3);

        return pre1==mid-jobl+1 && item1!=item2?(pre1+pre2):pre1;
    }else if(type==2){//查后缀
        int mid = (l+r)>>1;
        if(jobr<=mid){
            return query(jobl,jobr,l,mid,i<<1,2);
        }
        if(jobl>mid){
            return query(jobl,jobr,mid+1,r,i<<1|1,2);
        }

        int suf1 = query(jobl,mid,l,mid,i<<1,2);
        int suf2 = query(mid+1,jobr,mid+1,r,i<<1|1,2);

        int item1 = query(jobl,mid,l,mid,i<<1,4);
        int item2 = query(mid+1,jobr,mid+1,r,i<<1|1,3);

        return suf2==jobr-mid && item1!=item2?(suf1+suf2):suf2;
    }else if(type==3){//查第一个
        int mid = (l+r)>>1;
        //让我们思考如果这里路走岔了会怎么样，会出现无限递归
        if(jobr<=mid){
            return query(jobl,jobr,l,mid,i<<1,3);
        }
        if(jobl>mid){
            return query(jobl,jobr,mid+1,r,i<<1|1,3);
        }

        //直接返回左区间的
        return query(jobl,mid,1,mid,i<<1,3);

    }else if(type==4) {//查最后一个
        int mid = (l + r) >> 1;
        if (jobr <= mid) {
            return query(jobl, jobr, l, mid, i << 1, 4);
        }
        if (jobl > mid) {
            return query(jobl, jobr, mid + 1, r, i << 1 | 1, 4);
        }
        return query(mid + 1, jobr, mid + 1, r, i << 1 | 1, 4);
    }
}

//https://www.luogu.com.cn/problem/P6492
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>q;
    build(1,n,1);
    int ax;
    for(int idx=0;idx<q;idx++){
        cin>>ax;
        reverse(ax,ax,1,n,1);
        int ans = query(1,n,1,n,1,0);
        cout<<ans<<endl;
    }
    return 0;
}