//
// Created by hsl on 2024/12/11.
//
#include<iostream>
#include<unordered_set>
#include<algorithm>
#include<queue>
#include<functional>
using namespace std;

int diff_arr[10];

//数组排序的demo
//algorithm算法库中的sort可以给数组和向量排序
void sort_demo(){
    diff_arr[0] = 23;
    diff_arr[1] = 21;
    diff_arr[2] = 20;
    sort(diff_arr + 1, diff_arr + 3);
    for(int idx=0;idx<10;idx++){
        cout << diff_arr[idx] << " ";
    }
}

//bool只有0和1两个值
void bool_demo(){
    bool i = 0;
    i = 2;
    int a = 1;
    int b = a+i;
    cout<<b<<endl;
}

int a[2];
int b[2];

void swap(int* arr1,int* arr2,int index1,int index2){
    int temp = arr1[index1];
    arr1[index1] = arr2[index2];
    arr2[index2] = temp;
}


int main(){
//    sort_demo();
//    bool_demo();
    int y = 2;
    cout<<y^5;
    return 0;
}
