#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>

using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.

string divide(string target, int len, int i, int j) {
    char buf[len];
    strcpy(buf, target.c_str());
    string res = "";
    for (int k = len - i - 1; k <= len - j - 1; k++) {
        res += buf[k];
    }
    return res;
}

bitset<32> signextend(bitset<16> imm) {
    char buf[16];
    strcpy(buf, imm.to_string().c_str());
    if (buf[0] == '0') {
        return bitset<32>(imm.to_string());
    } else {
        //负数没有实现
    }
}

/**
 * Register File
 */
class RF {
public:
    bitset<32> ReadData1, ReadData2;

    RF() {
        Registers.resize(32); //数据空间设置32
        Registers[0] = bitset<32>(0); //0号空间固定为常数0
    }

    void show() {
        cout << "-------------------" << endl;
        cout << "RF Show : " << endl;
        for (int i = 0; i < 32; i++) {
            cout << Registers[i] << endl;
        }
        cout << "-------------------" << endl;
    }

    void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable) {
        // implement the funciton by you.
        ReadData1 = Registers[RdReg1.to_ulong()];
        ReadData2 = Registers[RdReg2.to_ulong()];
        if (WrtEnable.to_ulong() == 1) {
            Registers[WrtReg.to_ulong()] = WrtData;
            cout << "WB RF-addr : " << WrtReg.to_ulong() << endl;
            cout << "WB Data : " << WrtData << endl;
            return;
        }
    }

    void OutputRF() // write RF results to file
    {
        ofstream rfout;
        rfout.open("RFresult.txt", std::ios_base::app);
        if (rfout.is_open()) {
            rfout << "A state of RF:" << endl;
            for (int j = 0; j < 32; j++) {
                rfout << Registers[j] << endl;
            }

        } else cout << "Unable to open file";
        rfout.close();

    }

private:
    vector<bitset<32>> Registers;

};

/**
 * ALU
 */
class ALU {
public:
    bitset<32> ALUresult;

    bitset<32> ALUOperation(bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2) {
        // implement the ALU operations by you.
        switch (ALUOP.to_ulong()) {
            case 1: //addu: 无符号加法
                long res = oprand1.to_ulong() + oprand2.to_ulong();
                bitset<32> bitres(res);
                ALUresult = bitres;
                return bitres;
        }
        return ALUresult;
    }
};

/**
 * Instruction Memory
 */
class INSMem {
public:
    bitset<32> Instruction;

    INSMem() // read instruction memory
    {
        IMem.resize(MemSize);
        ifstream imem;
        string line;
        int i = 0;
        imem.open("../imem.txt");
        if (imem.is_open()) {
            while (getline(imem, line)) {
                IMem[i] = bitset<8>(line);
                i++;
            }
            //打印读取的InstructionMemory数据
            cout << "-------------------" << endl;
            cout << "Initial InstructionMemory :" << endl;
            for (int p = 0; p < i; p++) {
                cout << IMem[p] << endl;
                if (p > 0 && p != i - 1 && (p + 1) % 4 == 0) {
                    cout << "" << endl;
                }
            }
            cout << "-------------------" << endl;
        } else cout << "Unable to open file - INSMen" << endl;
        imem.close();

    }

    bitset<32> ReadMemory(bitset<32> ReadAddress) {
        // implement by you. (Read the byte at the ReadAddress and the following three byte).
        string s = "";
        for (int i = 0; i < 4; i++) {
            s += IMem[ReadAddress.to_ulong() + i].to_string();
        }
        bitset<32> ins(s);
        Instruction = ins;
        return Instruction;
    }

private:
    vector<bitset<8>> IMem;

};

/**
 * DataMemory
 */
class DataMem {
public:
    bitset<32> readdata;

    DataMem() // DataMemory构造函数，初始化DataMemory数据
    {
        DMem.resize(MemSize);
        ifstream dmem;
        string line;
        int i = 0;
        dmem.open("../dmem.txt");
        if (dmem.is_open()) {
            while (getline(dmem, line)) {
                DMem[i] = bitset<8>(line);
                i++;
            }
            //打印读取的DataMemory数据
            cout << "-------------------" << endl;
            cout << "Initial DataMemory :" << endl;
            for (int p = 0; p < i; p++) {
                cout << DMem[p] << endl;
                if (p > 0 && p != i - 1 && (p + 1) % 4 == 0) {
                    cout << "" << endl;
                }
            }
            cout << "-------------------" << endl;
        } else cout << "Unable to open file - DataMem" << endl;
        dmem.close();
    }

    void show() {
        cout << "-------------------" << endl;
        cout << "DM Show : " << endl;
        for (int i = 0; i < 40; i++) {
            cout << DMem[i] << endl;
        }
        cout << "......" << endl;
        cout << "-------------------" << endl;
    }

    string divide(string target, int len, int i, int j) {
        char buf[len];
        strcpy(buf, target.c_str());
        string res = "";
        for (int k = len - i - 1; k <= len - j - 1; k++) {
            res += buf[k];
        }
        return res;
    }

    bitset<32> MemoryAccess(bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) {
        // implement by you.
        if (readmem.to_ulong() == 1) {
            //读操作
            string res = "";
            for (int i = 0; i < 4; i++) {
                res += DMem[Address.to_ulong() + i].to_string();
            }
            readdata = bitset<32>(res);
            return readdata;
        } else if (writemem.to_ulong() == 1) {
            //写操作
            for (int i = 0; i < 4; i++) {
                DMem[Address.to_ulong() + i] = bitset<8>(
                        divide(WriteData.to_string(), 32, 32 - 8 * i - 1, 32 - 8 * (i + 1)));
            }
            cout << "DM Write-addr : " << Address << endl;
            cout << "DM Write-data : " << DMem[Address.to_ulong()] << DMem[Address.to_ulong() + 1]
                 << DMem[Address.to_ulong() + 2]
                 << DMem[Address.to_ulong() + 3] << endl;
        }

        return readdata;
    }

    void OutputDataMem()  // write dmem results to file
    {
        ofstream dmemout;
        dmemout.open("dmemresult.txt");
        if (dmemout.is_open()) {
            for (int j = 0; j < 1000; j++) {
                dmemout << DMem[j] << endl;
            }

        } else cout << "Unable to open file - OutputDataMem" << endl;
        dmemout.close();

    }

private:
    vector<bitset<8>> DMem;

};


int main() {
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

    int pc = 0;
    while (1) {
        cout << "-------------------" << endl;

        // Fetch
        cout << "PC = " << pc << endl;

        bitset<32> ins = myInsMem.ReadMemory(pc);
        pc += 4;
        cout << "Instruction = " << ins << endl;

        // If current insturciton is "11111111111111111111111111111111", then break;
        if (ins.to_string() == "11111111111111111111111111111111") {
            break;
        }

        switch (atoi(divide(ins.to_string(), 32, 31, 26).c_str())) {
            case 100011: {//lw指令
                //lw指令解析
                cout << "No." << pc / 4 << " Instruction is Load" << endl;
                bitset<6> opcode(divide(ins.to_string(), 32, 31, 26));
                bitset<5> rs(divide(ins.to_string(), 32, 25, 21));
                bitset<5> rt(divide(ins.to_string(), 32, 20, 16));
                bitset<16> imm(divide(ins.to_string(), 32, 15, 0));
                cout << "opcode : " << opcode << endl;
                cout << "rs : " << rs << endl;
                cout << "rt : " << rt << endl;
                cout << "imm : " << imm << endl;

                //RF
                bitset<5> rd1 = rs;
                myRF.ReadWrite(rd1, bitset<5>(0), bitset<5>(0), bitset<32>(0), bitset<1>(0));
                cout << "RF IN : " << rs << endl;
                cout << "RF OUT : " << myRF.ReadData1 << endl;

                //ALU
                bitset<3> ALUOP('001');
                myALU.ALUOperation(ALUOP, myRF.ReadData1, signextend(imm));
                cout << "ALU IN : " << myRF.ReadData1 << " and " << signextend(imm) << endl;
                cout << "ALU OUT : " << myALU.ALUresult << endl;

                //DataMemory
                bitset<32> addr = myALU.ALUresult;
                bitset<1> r(1);
                myDataMem.MemoryAccess(addr, bitset<32>(0), r, bitset<1>(0));
                cout << "DM IN : " << addr << endl;
                cout << "DM OUT : " << myDataMem.readdata << endl;

                //WriteBack
                bitset<5> writeaddress = rt;
                bitset<1> writeenable(1);
                myRF.ReadWrite(bitset<5>(0), bitset<5>(0), writeaddress, myDataMem.readdata, writeenable);
                break;
            }
            case 101011: {//sw指令
                //sw指令解析
                cout << "No." << pc / 4 << " Instruction is Store" << endl;
                bitset<6> opcode(divide(ins.to_string(), 32, 31, 26));
                bitset<5> rs(divide(ins.to_string(), 32, 25, 21));
                bitset<5> rt(divide(ins.to_string(), 32, 20, 16));
                bitset<16> imm(divide(ins.to_string(), 32, 15, 0));
                cout << "opcode : " << opcode << endl;
                cout << "rs : " << rs << endl;
                cout << "rt : " << rt << endl;
                cout << "imm : " << imm << endl;

                //RF
                bitset<5> rd1 = rs;
                bitset<5> rd2 = rt;
                myRF.ReadWrite(rd1, rt, bitset<5>(0), bitset<32>(0), bitset<1>(0));
                cout << "RF IN : " << rs << " and " << rt << endl;
                cout << "RF OUT : " << myRF.ReadData1 << " and " << myRF.ReadData2 << endl;

                //ALU
                bitset<3> ALUOP('001');
                myALU.ALUOperation(ALUOP, myRF.ReadData1, signextend(imm));
                cout << "ALU IN : " << myRF.ReadData1 << " and " << signextend(imm) << endl;
                cout << "ALU OUT : " << myALU.ALUresult << endl;

                //DataMemory
                bitset<32> addr = myALU.ALUresult;
                bitset<1> w(1);
                cout << "DM IN : " << addr << " and " << myRF.ReadData2 << endl;
                myDataMem.MemoryAccess(addr, myRF.ReadData2, bitset<1>(0), w);
                break;
            }

            case 000000: {//R-Type
                switch (atoi(divide(ins.to_string(), 32, 2, 0).c_str())) {
                    case ADDU: {//Addu
                        //Addu指令解析
                        cout << "No." << pc / 4 << " Instruction is Addu" << endl;
                        bitset<6> opcode(divide(ins.to_string(), 32, 31, 26));
                        bitset<5> rs(divide(ins.to_string(), 32, 25, 21));
                        bitset<5> rt(divide(ins.to_string(), 32, 20, 16));
                        bitset<5> rd(divide(ins.to_string(), 32, 15, 11));
                        bitset<5> shamt(divide(ins.to_string(), 32, 10, 6));
                        bitset<6> func(divide(ins.to_string(), 32, 5, 0));
                        cout << "opcode : " << opcode << endl;
                        cout << "rs : " << rs << endl;
                        cout << "rt : " << rt << endl;
                        cout << "rd : " << rd << endl;
                        cout << "shamt : " << shamt << endl;
                        cout << "func : " << func << endl;

                        //RF
                        bitset<5> rd1 = rs;
                        bitset<5> rd2 = rt;
                        myRF.ReadWrite(rd1, rd2, bitset<5>(0), bitset<32>(0), bitset<1>(0));
                        cout << "RF IN : " << rs << " and " << rt << endl;
                        cout << "RF OUT : " << myRF.ReadData1 << " and " << myRF.ReadData2 << endl;

                        //ALU
                        bitset<3> ALUOP("001");
                        myALU.ALUOperation(ALUOP, myRF.ReadData1, myRF.ReadData2);
                        cout << "ALU IN : " << myRF.ReadData1 << " and " << myRF.ReadData2 << endl;
                        cout << "ALU OUT : " << myALU.ALUresult << endl;

                        //WriteBack
                        bitset<5> writeaddress = rd;
                        bitset<1> writeenable(1);
                        myRF.ReadWrite(bitset<5>(0), bitset<5>(0), writeaddress, myALU.ALUresult, writeenable);
                        break;
                    }
                    case 2: {
                        break;
                    }
                }
                break;
            }
        }
        myRF.OutputRF(); // dump RF;    
    }

    myRF.show();
    myDataMem.show();

    myDataMem.OutputDataMem(); // dump data mem

    return 0;

}
