#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <bitset>
#include <map>
#include <list>

using namespace std;

int main(int argc, char **argv) {
    ifstream config;
    config.open(argv[1]);

    int m, k;
    config >> m >> k;

    config.close();

    int col = pow(2, k);

    ofstream out;
    string out_file_name = string(argv[2]) + ".out";
    out.open(out_file_name.c_str());

    ifstream trace;
    trace.open(argv[2]);

    map <string, vector<int> > mapAdd;
    vector<int> BHR(k, 1);


    while (!trace.eof()) {

        // 获取输入
        bool prediction;
        unsigned long pc;
        bool taken;
        trace >> std::hex >> pc >> taken;

        // 获取地址字符串 + taken结果
        bitset<32> address(pc);
        string key = address.to_string().substr(32 - m, 32);
        cout << key << endl;
        cout << taken << endl;

        //更新BHR
        if (taken) {
            BHR.insert(BHR.begin(), 1);
            BHR.pop_back();
        } else {
            BHR.insert(BHR.begin(), 0);
            BHR.pop_back();
        }


        // 存储-判定-更新
        int c = mapAdd.count(key);
        if (c == 0) {
            cout << "new" << endl;

            //预测一定是ST
            prediction = true;
            //初始化该地址历史记录
            vector<int> history(col, 11);
            //计算列索引
            string bhr = "";
            for (int i = 0; i < k; i++) {
                bhr += to_string(BHR[i]);
            }
            int index = bitset<20>(bhr).to_ulong();
            cout << index << endl;
            //如果新地址第一次是not taken，对应记录更新为10
            if (!taken) {
                history[index] = 10;
            }
            //赋值记录
            mapAdd[key] = history;
            //打印历史记录
            for (int i = 0; i < history.size(); ++i) {
                cout << history[i] << ' ';
            }
            cout << "-------" << endl;
        } else {
            cout << "old" << endl;
        }

        //prediction = true;
        out << prediction;
    }

    trace.close();
    out.close();
}
