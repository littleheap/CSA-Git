#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>

using namespace std;
#define MemSize 1000 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.

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

struct IFStruct {
    bitset<32> PC;
    bool nop;

    IFStruct() {
        PC = bitset<32>(0);
        nop = false;
    }
};

struct IDStruct {
    bitset<32> Instr;
    bool nop;

    IDStruct() {
        Instr = bitset<32>(0);
        nop = true;
    }
};

struct EXStruct {
    bitset<32> Read_data1;
    bitset<32> Read_data2;
    bitset<16> Imm;
    bitset<5> Rs;
    bitset<5> Rt;
    bitset<5> Wrt_reg_addr;
    bool is_I_type;
    bool rd_mem;
    bool wrt_mem;
    bool alu_op;     //1 for addu, lw, sw, 0 for subu
    bool wrt_enable;
    bool nop;

    EXStruct() {
        Read_data1 = bitset<32>(0);
        Read_data2 = bitset<32>(0);
        Imm = bitset<16>(0);
        Rs = bitset<5>(0);
        Rt = bitset<5>(0);
        Wrt_reg_addr = bitset<5>(0);
        is_I_type = false;
        rd_mem = false;
        wrt_mem = false;
        alu_op = true;
        wrt_enable = false;
        nop = true;
    }
};

struct MEMStruct {
    bitset<32> ALUresult;
    bitset<32> Store_data;
    bitset<5> Rs;
    bitset<5> Rt;
    bitset<5> Wrt_reg_addr;
    bool rd_mem;
    bool wrt_mem;
    bool wrt_enable;
    bool nop;

    MEMStruct() {
        ALUresult = bitset<32>(0);
        Store_data = bitset<32>(0);
        Rs = bitset<5>(0);
        Rt = bitset<5>(0);
        Wrt_reg_addr = bitset<5>(0);
        rd_mem = false;
        wrt_mem = false;
        wrt_enable = false;
        nop = true;
    }
};

struct WBStruct {
    bitset<32> Wrt_data;
    bitset<5> Rs;
    bitset<5> Rt;
    bitset<5> Wrt_reg_addr;
    bool wrt_enable;
    bool nop;

    WBStruct() {
        Wrt_data = bitset<32>(0);
        Rs = bitset<5>(0);
        Rt = bitset<5>(0);
        Wrt_reg_addr = bitset<5>(0);
        wrt_enable = false;
        nop = true;
    }
};

struct stateStruct {
    IFStruct IF;
    IDStruct ID;
    EXStruct EX;
    MEMStruct MEM;
    WBStruct WB;
};

class RF {
public:
    bitset<32> Reg_data;

    RF() {
        Registers.resize(32);
        Registers[0] = bitset<32>(0);
    }

    bitset<32> readRF(bitset<5> Reg_addr) {
        Reg_data = Registers[Reg_addr.to_ulong()];
        return Reg_data;
    }

    void writeRF(bitset<5> Reg_addr, bitset<32> Wrt_reg_data) {
        Registers[Reg_addr.to_ulong()] = Wrt_reg_data;
    }

    void outputRF() {
        ofstream rfout;
        rfout.open("RFresult.txt", std::ios_base::app);
        if (rfout.is_open()) {
            rfout << "State of RF:\t" << endl;
            for (int j = 0; j < 32; j++) {
                rfout << Registers[j] << endl;
            }
        } else cout << "Unable to open file";
        rfout.close();
    }

private:
    vector<bitset<32> > Registers;
};

class INSMem {
public:
    bitset<32> Instruction;

    INSMem() {
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
        } else cout << "Unable to open INSMem file";
        imem.close();
    }

    bitset<32> readInstr(bitset<32> ReadAddress) {
        string insmem;
        insmem.append(IMem[ReadAddress.to_ulong()].to_string());
        insmem.append(IMem[ReadAddress.to_ulong() + 1].to_string());
        insmem.append(IMem[ReadAddress.to_ulong() + 2].to_string());
        insmem.append(IMem[ReadAddress.to_ulong() + 3].to_string());
        Instruction = bitset<32>(insmem);        //read instruction memory
        return Instruction;
    }

private:
    vector<bitset<8> > IMem;
};

class DataMem {
public:
    bitset<32> ReadData;

    DataMem() {
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
        } else cout << "Unable to open DataMem file";
        dmem.close();
    }

    bitset<32> readDataMem(bitset<32> Address) {
        string datamem;
        datamem.append(DMem[Address.to_ulong()].to_string());
        datamem.append(DMem[Address.to_ulong() + 1].to_string());
        datamem.append(DMem[Address.to_ulong() + 2].to_string());
        datamem.append(DMem[Address.to_ulong() + 3].to_string());
        ReadData = bitset<32>(datamem);        //read data memory
        return ReadData;
    }

    void writeDataMem(bitset<32> Address, bitset<32> WriteData) {
        DMem[Address.to_ulong()] = bitset<8>(WriteData.to_string().substr(0, 8));
        DMem[Address.to_ulong() + 1] = bitset<8>(WriteData.to_string().substr(8, 8));
        DMem[Address.to_ulong() + 2] = bitset<8>(WriteData.to_string().substr(16, 8));
        DMem[Address.to_ulong() + 3] = bitset<8>(WriteData.to_string().substr(24, 8));
    }

    void outputDataMem() {
        ofstream dmemout;
        dmemout.open("dmemresult.txt");
        if (dmemout.is_open()) {
            for (int j = 0; j < 1000; j++) {
                dmemout << DMem[j] << endl;
            }

        } else cout << "Unable to open file";
        dmemout.close();
    }

private:
    vector<bitset<8> > DMem;
};

void printState(stateStruct state, int cycle) {
    ofstream printstate;
    printstate.open("stateresult.txt", std::ios_base::app);
    if (printstate.is_open()) {
        printstate << "State after executing cycle:\t" << cycle << endl;

        printstate << "IF.PC:\t" << state.IF.PC.to_ulong() << endl;
        printstate << "IF.nop:\t" << state.IF.nop << endl;

        printstate << "ID.Instr:\t" << state.ID.Instr << endl;
        printstate << "ID.nop:\t" << state.ID.nop << endl;

        printstate << "EX.Read_data1:\t" << state.EX.Read_data1 << endl;
        printstate << "EX.Read_data2:\t" << state.EX.Read_data2 << endl;
        printstate << "EX.Imm:\t" << state.EX.Imm << endl;
        printstate << "EX.Rs:\t" << state.EX.Rs << endl;
        printstate << "EX.Rt:\t" << state.EX.Rt << endl;
        printstate << "EX.Wrt_reg_addr:\t" << state.EX.Wrt_reg_addr << endl;
        printstate << "EX.is_I_type:\t" << state.EX.is_I_type << endl;
        printstate << "EX.rd_mem:\t" << state.EX.rd_mem << endl;
        printstate << "EX.wrt_mem:\t" << state.EX.wrt_mem << endl;
        printstate << "EX.alu_op:\t" << state.EX.alu_op << endl;
        printstate << "EX.wrt_enable:\t" << state.EX.wrt_enable << endl;
        printstate << "EX.nop:\t" << state.EX.nop << endl;

        printstate << "MEM.ALUresult:\t" << state.MEM.ALUresult << endl;
        printstate << "MEM.Store_data:\t" << state.MEM.Store_data << endl;
        printstate << "MEM.Rs:\t" << state.MEM.Rs << endl;
        printstate << "MEM.Rt:\t" << state.MEM.Rt << endl;
        printstate << "MEM.Wrt_reg_addr:\t" << state.MEM.Wrt_reg_addr << endl;
        printstate << "MEM.rd_mem:\t" << state.MEM.rd_mem << endl;
        printstate << "MEM.wrt_mem:\t" << state.MEM.wrt_mem << endl;
        printstate << "MEM.wrt_enable:\t" << state.MEM.wrt_enable << endl;
        printstate << "MEM.nop:\t" << state.MEM.nop << endl;

        printstate << "WB.Wrt_data:\t" << state.WB.Wrt_data << endl;
        printstate << "WB.Rs:\t" << state.WB.Rs << endl;
        printstate << "WB.Rt:\t" << state.WB.Rt << endl;
        printstate << "WB.Wrt_reg_addr:\t" << state.WB.Wrt_reg_addr << endl;
        printstate << "WB.wrt_enable:\t" << state.WB.wrt_enable << endl;
        printstate << "WB.nop:\t" << state.WB.nop << endl;
    } else cout << "Unable to open file";
    printstate.close();
}


int main() {

    RF myRF;
    INSMem myInsMem;
    DataMem myDataMem;

    int cycle = 0;
    stateStruct state;
    while (1) {

        stateStruct newState;

        newState.IF.PC = bitset<32>(state.IF.PC.to_ulong() + 4);

        /* --------------------- WB stage --------------------- */
        if (state.WB.nop) {
            //nothing
        } else {
            if (state.WB.wrt_enable) {
                //执行WriteBack
                myRF.writeRF(state.WB.Wrt_reg_addr, state.WB.Wrt_data);
            } else {
                //nothing
            }
        }

        /* --------------------- MEM stage --------------------- */
        if (state.MEM.nop) {
            //当前cycle为nop：主要任务就是继续将前面EX传来的信息传给WB
            newState.WB.Wrt_data = state.WB.Wrt_data;
            newState.WB.Rs = state.MEM.Rs;
            newState.WB.Rt = state.MEM.Rt;
            newState.WB.Wrt_reg_addr = state.MEM.Wrt_reg_addr;
            newState.WB.wrt_enable = state.MEM.wrt_enable;
            newState.WB.nop = state.MEM.nop;//与当前nop一致

        } else {
            //当前stage不为nop：要进行一些MEM操作
            if (state.MEM.wrt_mem) {
                //SW：MEM执行写操作（无返回值）
                myDataMem.writeDataMem(state.MEM.ALUresult, state.MEM.Store_data);
                newState.WB.Wrt_data = myDataMem.readDataMem(state.MEM.ALUresult);
            } else if (state.MEM.rd_mem) {
                //LW：MEM执行读操作（返回32位数据）
                newState.WB.Wrt_data = myDataMem.readDataMem(state.MEM.ALUresult);
            } else if (state.MEM.wrt_enable) {
                //addu或subu
                newState.WB.Wrt_data = state.MEM.ALUresult;
            }
        }

        newState.WB.Rs = state.MEM.Rs;
        newState.WB.Rt = state.MEM.Rt;
        newState.WB.Wrt_reg_addr = state.MEM.Wrt_reg_addr;
        newState.WB.wrt_enable = state.MEM.wrt_enable;
        if (state.MEM.wrt_enable == true) {
            //说明下一个cycle的WB要写回
            newState.WB.nop = state.MEM.nop;//与当前nop一致
        } else {
            //说明下一个cycle的WB不需要写回
            newState.WB.nop = state.MEM.nop;//与当前nop一致
        }

        /* --------------------- EX stage --------------------- */
        if (state.EX.nop) {
            newState.MEM.ALUresult = state.MEM.ALUresult;
            newState.MEM.Store_data = state.MEM.Store_data;
            newState.MEM.Rs = state.MEM.Rs;
            newState.MEM.Rt = state.MEM.Rt;
            newState.MEM.Wrt_reg_addr = state.MEM.Wrt_reg_addr;
            newState.MEM.wrt_enable = state.MEM.wrt_enable;
            newState.MEM.rd_mem = state.MEM.rd_mem;
            newState.MEM.wrt_mem = state.MEM.wrt_mem;
            newState.MEM.nop = state.EX.nop; //与当前nop一致
        } else {
            if (state.EX.alu_op) {
                //addu,lw,sw
                if (state.EX.is_I_type) {
                    //lw, sw
                    long res = state.EX.Read_data1.to_ulong() + signextend(state.EX.Imm).to_ulong();
                    newState.MEM.ALUresult = bitset<32>(res);
                    //指令是lw,sw说明下一cycle中MEM要激活
                    newState.MEM.nop = state.EX.nop; //与当前nop一致
                } else {
                    //addu
                    long res = state.EX.Read_data1.to_ulong() + state.EX.Read_data2.to_ulong();
                    newState.MEM.ALUresult = bitset<32>(res);
                    //指令是R-type的addu，说明下一cycle中MEM不需要激活
                    newState.MEM.nop = state.EX.nop; //与当前nop一致
                }
            } else {
                //subu
                long res = state.EX.Read_data1.to_ulong() - state.EX.Read_data2.to_ulong();
                newState.MEM.ALUresult = bitset<32>(res);
                //指令是R-type的subu，说明下一cycle中MEM不需要激活
                newState.MEM.nop = state.EX.nop; //与当前nop一致
            }
            newState.MEM.Store_data = state.EX.Read_data2;
            newState.MEM.Rs = state.EX.Rs;
            newState.MEM.Rt = state.EX.Rt;
            newState.MEM.Wrt_reg_addr = state.EX.Wrt_reg_addr;
            newState.MEM.wrt_enable = state.EX.wrt_enable;
            newState.MEM.rd_mem = state.EX.rd_mem;
            newState.MEM.wrt_mem = state.EX.wrt_mem;
        }


        /* --------------------- ID stage --------------------- */
        if (state.ID.nop) {
            newState.EX.nop = state.ID.nop;
            newState.EX.Read_data1 = state.EX.Read_data1;
            newState.EX.Read_data2 = state.EX.Read_data2;
            newState.EX.is_I_type = state.EX.is_I_type;
            newState.EX.wrt_enable = state.EX.wrt_enable;
            newState.EX.rd_mem = state.EX.rd_mem;
            newState.EX.wrt_mem = state.EX.wrt_mem;
            newState.EX.Wrt_reg_addr = state.EX.Wrt_reg_addr;
            newState.EX.Imm = state.EX.Imm;
            newState.EX.Rs = state.EX.Rs;
            newState.EX.Rt = state.EX.Rt;
            newState.EX.alu_op = state.EX.alu_op;
        } else {
            newState.EX.nop = state.ID.nop; // 保持一致
            bitset<6> opcode(divide(state.ID.Instr.to_string(), 32, 31, 26)); //获取opcode
            if (opcode.to_ulong() == 0) {
                //指令为R-type：addu或者subu
                bitset<5> rs(divide(state.ID.Instr.to_string(), 32, 25, 21));
                bitset<5> rt(divide(state.ID.Instr.to_string(), 32, 20, 16));
                bitset<5> rd(divide(state.ID.Instr.to_string(), 32, 15, 11));
                bitset<5> shamt(divide(state.ID.Instr.to_string(), 32, 10, 6));
                bitset<6> func(divide(state.ID.Instr.to_string(), 32, 5, 0));
                newState.EX.Read_data1 = myRF.readRF(rs);
                newState.EX.Read_data2 = myRF.readRF(rt);
                newState.EX.is_I_type = false;
                newState.EX.wrt_enable = true;
                newState.EX.rd_mem = false;
                newState.EX.wrt_mem = false;
                newState.EX.Wrt_reg_addr = rd;
                newState.EX.Imm = bitset<16>(divide(state.ID.Instr.to_string(), 32, 15, 0));
                newState.EX.Rs = rs;
                newState.EX.Rt = rt;
                if (bitset<6>(divide(state.ID.Instr.to_string(), 32, 5, 0)) == 33) {
                    //是addu指令
                    newState.EX.alu_op = true;
                } else {
                    //是subu指令
                    newState.EX.alu_op = false;
                }
            } else {
                //指令为I-type：lw或sw或beq
                bitset<5> rs(divide(state.ID.Instr.to_string(), 32, 25, 21));
                bitset<5> rt(divide(state.ID.Instr.to_string(), 32, 20, 16));
                bitset<16> imm(divide(state.ID.Instr.to_string(), 32, 15, 0));
                newState.EX.alu_op = true;
                newState.EX.Read_data1 = myRF.readRF(rs);
                newState.EX.Read_data2 = myRF.readRF(rt);
                newState.EX.is_I_type = true;
                newState.EX.Wrt_reg_addr = rt;
                newState.EX.Imm = imm;
                newState.EX.Rs = rs;
                newState.EX.Rt = rt;
                switch (bitset<6>(divide(state.ID.Instr.to_string(), 32, 31, 26)).to_ulong()) {
                    case 35: {
                        //lw
                        newState.EX.wrt_enable = true;
                        newState.EX.rd_mem = true;
                        newState.EX.wrt_mem = false;
                        break;
                    }
                    case 43: {
                        //sw
                        newState.EX.wrt_enable = false;
                        newState.EX.rd_mem = false;
                        newState.EX.wrt_mem = true;
                        break;
                    }
                    case 4: {
                        //beq
                        newState.EX.wrt_enable = false;
                        newState.EX.rd_mem = false;
                        newState.EX.wrt_mem = false;
                        break;
                    }
                }
            }
        }

        /* --------------------- IF stage --------------------- */
        if (state.IF.nop) {
            //当前IF是nop
            newState.IF.nop = true;
            newState.IF.PC = state.IF.PC;
            newState.ID.nop = true;
            newState.ID.Instr = myInsMem.readInstr(state.IF.PC);
        } else {
            newState.ID.Instr = myInsMem.readInstr(state.IF.PC);
            if (myInsMem.readInstr(state.IF.PC).to_string() == "11111111111111111111111111111111") {
                //说明是halt
                newState.IF.nop = true;
                newState.ID.nop = true;
                newState.IF.PC = state.IF.PC; //不再PC+4
            } else {
                //正常指令
                newState.IF.nop = false;
                //newState.IF.PC = bitset<32>(state.IF.PC.to_ulong() + 4); //写在前面了，防止beq更新
                newState.ID.nop = false;
            }
        }

        if (state.IF.nop && state.ID.nop && state.EX.nop && state.MEM.nop && state.WB.nop)
            break;

        printState(newState, cycle); //print states after executing cycle 0, cycle 1, cycle 2 ...

        state = newState; /*The end of the cycle and updates the current state with the values calculated in this cycle */

        cycle += 1;
    }

    myRF.outputRF(); // dump RF;
    myDataMem.outputDataMem(); // dump data mem

    return 0;
}