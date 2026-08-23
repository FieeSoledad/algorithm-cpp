//
// Created by hsl19 on 2026/8/22.
//
#include <iostream>
typedef long long ll;
using namespace std;

const int MAXN = 1e5 + 1;
int n, m;
int arr[MAXN];
int fa[MAXN];
int lc[MAXN];
int rc[MAXN];
bool rev[MAXN];// 翻转标记
int stack_[MAXN];// 提根时处理沿途的翻转标记
int xorsum[MAXN];// 本题需要维护的异或和


void up(int x){
    xorsum[x] = xorsum[lc[x]] ^ xorsum[rc[x]] ^ arr[x];
}

/**
 * 判断是否是辅助splay-tree的顶点
 * @param x
 * @return
 */
bool isroot(int x){
    return lc[fa[x]] != x && rc[fa[x]] != x;
}

/**
 * 如果是左儿子返回0
 * @param x
 * @return
 */
int lr(int x){
    return lc[fa[x]] == x ? 0 : 1;
}

/**
 * 将x向上旋转
 * 如果在同向，先祖节点翻转，在翻转父节点
 * @param x
 */
void rotate(int x){
    int f = fa[x];
    int g = fa[f];
    if (lr(x) == 0){
        // 当前是左孩子
        lc[f] = rc[x];
        if (lc[f] != 0){
            fa[lc[f]] = f;
        }
        rc[x] = f;
    }else{// 如果是右孩子
        rc[f] = lc[x];
        if(rc[f] != 0){
            fa[rc[f]] = f;
        }
        lc[x] = f;
    }
    if (!isroot(f)){// 此时f还是g的儿子
        if (lr(f) == 0){
            lc[g] = x;
        }else{
            rc[g] = x;
        }
    }
    fa[f] = x;
    fa[x] = g;
    up(f);
    up(x);
    // 跟拍
}


/**
 * 翻转以x为根的辅助splay，交换左右儿子，打上翻转标记
 * 这个什么用
 * @param x
 */
void reverse(int x){
    if (x != 0){
        int tmp = lc[x];
        lc[x] = rc[x];
        rc[x] = tmp;
        rev[x] = !rev[x];
    }
}

/**
 * 懒信息下发，这大概就是
 * @param x
 */
void down(int x){
    if (rev[x]){
        reverse(lc[x]);
        reverse(rc[x]);
        rev[x] = false;
    }
}

/**
 * splay树的提根操作
 * 将x提到辅助splay的顶部
 * @param x
 */
void splay(int x){
    int siz = 0;
    stack_[++siz] = x;
    for(int y = x; !isroot(y); y = fa[y]){// 判断条件是root不执行，所以下面要使用fa[y]确保访问到root
        stack_[++siz] = fa[y];
    }
    while(siz != 0){
        down(stack_[siz--]);// 懒信息下发
    }
    while(!isroot(x)){
        int f = fa[x];
        if (!isroot(f)){
            if (lr(x) == lr(f)){
                rotate(f);
            }else{
                rotate(x);
            }
        }
        rotate(x);
    }
}

/**
 * 打通当前原树根到x的路径，使其成为实链
 * @param x
 */
void access(int x){
    for(int y = 0; x != 0; y = x, x = fa[x]){// 因此x的右树为空
        splay(x);// 内部提到根部
        rc[x] = y;
        up(x);// 这里是因为右孩子改变了
    }
}

/**
 * 让x变为原树的根，不改变连通方式
 * @param x
 */
void makeroot(int x){
    access(x);// 构建实链
    splay(x);// 将x提根
    reverse(x);// 为什么要reverse？// 提根不会改变中序遍历的顺序
}

/**
 * 建立根到x的实链，根变为辅助splay的顶，返回x所在原树的根
 * @param x
 * @return
 */
int findroot(int x){
    access(x);
    splay(x);
    down(x);
    while(lc[x] != 0){
        x = lc[x];
        down(x);
    }
    splay(x);// 将实链的顶部提根
    return x;
}


void split(int x, int y){
    makeroot(x);//
    access(y);
    splay(y);// 将y提根，这样方便从左子树获取路径信息
}

void link(int x, int y){
    makeroot(x);// x不仅是原树的root也是splay的顶
    if (findroot(y) != x){// y所在原树的根
        fa[x] = y;
    }
}

void cut(int x, int y){
    makeroot(x);
    if (findroot(y) == x && fa[y] ==x && lc[y] == 0 && rc[x]==y){// y的左孩子为空，保证直接相邻
        fa[y] = rc[x] = 0;
        up(x);
    }
}

// https://www.luogu.com.cn/problem/P3690
// Q：什么是实链？
// 重链剖分它是静态的，剖分依据是树的大小，一旦切掉边或连新边，树剖就失效了
// 实链是动态的，剖分依据是刚才访问了谁，有一个access，打通所有的操作
// 整个路径的信息是在查询过程中通过access动态构建的，虚边只用于表示连通关系
// splay保持中序遍历为实链深度顺序，是为了避免access的过程中将靠近root的节点丢掉
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    for(int i = 1; i <= n; i++){
        cin>>arr[i];
        xorsum[i] = arr[i];
    }
    for(int idx = 1; idx <= m; idx++){
        int op,x,y;
        cin>>op>>x>>y;
        if(op==0){
            // 询问
            split(x,y);
            cout << (xorsum[y]) << "\n";// endl每次都会强制刷新缓冲区
        }else if(op==1){
            // link，虚边表示连通关系，这个虚边也是直接边，若已经连通则无需连接
            link(x,y);
        }else if(op==2){
            // cut，这里是假设两者之间有直接边时消除
            cut(x, y);
        }else{
            // x convert to y
            splay(x);// 虚边和实边
            arr[x] = y;
            up(x);
        }
    }
    return 0;
}