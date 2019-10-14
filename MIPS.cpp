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


unsigned long subbit(bitset<32> bit, int l, int r) {
            unsigned long res;
            //const int x = s;
            int s = r-l+1;
            bitset<32> rb(0);
            for(int i=0; i < s; ++i) {
              rb.set(i,bit[l+i]);
            }
            //cout << rb << endl;
            return rb.to_ulong();
          }

bitset<32> signExt(bitset<16> input) {
    bitset<32> ext;
    for(int i = 0; i < 16; ++i) {
        ext.set(i,input[i]);
    }
    for(int i = 16; i < 31; ++i) {
        ext.set(i, input[15]);
    }
    return ext;
}

bitset<32> zeroExt(bitset<16> input) {
    bitset<32> ext;
    for(int i = 0; i < 16; ++i) {
        ext.set(i,input[i]);
    }
    for(int i = 16; i < 31; ++i) {
        ext.set(i, 0);
    }
    return ext;
}



class RF
{
    public:
        bitset<32> ReadData1, ReadData2;
     	RF()
    	{
          Registers.resize(32);
          Registers[0] = bitset<32> (0);
        }

        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {
            if(WrtEnable == true) {
              Registers[WrtReg.to_ulong()] = WrtData;
            }
            ReadData1 = Registers[RdReg1.to_ulong()];
            ReadData2 = Registers[RdReg2.to_ulong()];

            // implement the funciton by you.
         }

	void OutputRF() // write RF results to file
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {
                        rfout << Registers[j]<<endl;
                      }

                  }
                  else cout<<"Unable to open file";
                  rfout.close();

               }
	private:
            vector<bitset<32> >Registers;

};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {
                 // implement the ALU operations by you.
                unsigned long op, res;

                op = ALUOP.to_ulong();
                if(op == 1) {
                  res = oprand1.to_ulong() + oprand2.to_ulong();
                  bitset<32> ALUresult1(res);
                  ALUresult = ALUresult1;
                } else if(op == 3) {
                  res = oprand1.to_ulong() - oprand2.to_ulong();
                  bitset<32> ALUresult1(res);
                  ALUresult = ALUresult1;
                } else if(op == 4) {
                  ALUresult = oprand1 & oprand2;
                } else if(op == 5) {
                  ALUresult = oprand1 | oprand2;
                } else if(op == 7) {
                  ALUresult = oprand1 | oprand2;
                  ALUresult.flip();
                }
                 return ALUresult;
               }
};

class INSMem
{
      public:
          bitset<32> Instruction;
          INSMem() // read instruction memory
          {       IMem.resize(MemSize);
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {
                        IMem[i] = bitset<8>(line);
                        i++;
                     }

                  }
                  else cout<<"Unable to open file";
                  imem.close();

            }

          bitset<32> ReadMemory (bitset<32> ReadAddress)
              {
               // implement by you. (Read the byte at the ReadAddress and the following three byte).
                string s="";
                for(int i = 0; i < 4; ++i) {
                  s += IMem[ReadAddress.to_ulong() + i].to_string();
                }
                bitset<32> t(s);
                Instruction = t;
               return Instruction;
              }

      private:
           vector<bitset<8> > IMem;

};

class DataMem
{
      public:
          bitset<32> readdata;
          DataMem() // read data memory
          {
             DMem.resize(MemSize);
             ifstream dmem;
                  string line;
                  int i=0;
                  dmem.open("dmem.txt");
                  if (dmem.is_open())
                  {
                  while (getline(dmem,line))
                       {
                        DMem[i] = bitset<8>(line);
                        i++;
                       }
                  }
                  else cout<<"Unable to open file";
                  dmem.close();

          }

          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
          {

               // implement by you.
              if(readmem == true) {
                string s = "";
                for(int i = 0; i < 4; ++i) {
                  s += DMem[Address.to_ulong() + i].to_string();
                }
                bitset<32> t(s);
                readdata = t;
              }
              if(writemem == true) {
                for(int i = 0; i < 4; ++i) {
                  bitset<8> t(subbit(WriteData, 0 + i * 8, 7 + i * 8));
                  DMem[Address.to_ulong() +3 - i ] = t;
                }

              }
               return readdata;
          }

          void OutputDataMem()  // write dmem results to file
          {
               ofstream dmemout;
                  dmemout.open("dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {
                        dmemout << DMem[j]<<endl;
                       }

                  }
                  else cout<<"Unable to open file";
                  dmemout.close();

               }

      private:
           vector<bitset<8> > DMem;

};


int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

    unsigned long PC = 0;
    bitset<32> pc(PC);
    bitset<32> ins  = myInsMem.ReadMemory(pc);
    cout << ins << endl;
    int i = 0b111;
    //cout << i<<endl;

    while (1)
	{
        bitset<32> pc(PC);
        bitset<32> ins  = myInsMem.ReadMemory(pc);
        //cout << ins << endl;

        unsigned long op,rs,rt,rd,shamt,funct,imm,addr,funct_s;
        op = subbit(ins, 26, 31);
        rs = subbit(ins, 21, 25);
        rt = subbit(ins, 16, 20);
        rd = subbit(ins, 11, 15);
        shamt = subbit(ins, 6, 10);
        funct = subbit(ins, 0, 5);
        funct_s = subbit(ins, 0, 2);
        imm = subbit(ins, 0, 15);
        addr = subbit(ins, 0, 25);

        bitset<5> RT(rt);
        bitset<5> RS(rs);
        bitset<5> RD(rd);
        bitset<16> IMM(imm);
        bitset<3> fun(funct_s);

        cout <<op << endl;

        if(op == 0) { // R type
            cout <<  "R" << endl;

            myRF.ReadWrite(RS,RT,0,0,0);
            bitset<32> res;
            res = myALU.ALUOperation(fun,myRF.ReadData1,myRF.ReadData2);
            cout <<res << endl;
            myRF.ReadWrite(0, 0, RD, res, 1);

        } else if(op == 9){
            cout << "addiu" << endl;
            //bitset<5> RS(rs);
            myRF.ReadWrite(RS,0,0,0,0);
            //bitset<16> IMM(imm);
            unsigned long t = myRF.ReadData1.to_ulong() + signExt(IMM).to_ulong();
            cout <<imm << endl;
            cout <<myRF.ReadData1.to_ulong() << endl;
            cout << t<< endl;
            bitset<32> T(t);
            //bitset<5> RT(rt);
            myRF.ReadWrite(0,0,RT,T,1);

        } else if(op == 4){
            cout << "beq" << endl;

            unsigned long branchaddr;
            bitset<14> head(IMM[15]);
            bitset<2> tz(0); // two zero
            string s = head.to_string() + IMM.to_string() + tz.to_string();
            bitset<32> ba(s);
            branchaddr = ba.to_ulong();

            myRF.ReadWrite(RT,RS,0,0,0);
            if(myRF.ReadData1 == myRF.ReadData2) {
                PC = PC + 4 + branchaddr;
                myRF.OutputRF();
                continue;
            }

        } else if(op == 2){
            cout << "j" << endl;

            unsigned long jumpaddr;
            bitset<32> t(PC + 4);
            bitset<4> head(subbit(t,28,31));
            bitset<26> ad(addr);
            bitset<2> tz(0); // two zero
            string s = head.to_string() + ad.to_string() + tz.to_string();
            bitset<32> ja(s);
            jumpaddr = ja.to_ulong();
            PC = jumpaddr;
            myRF.OutputRF();
            continue;
        } else if(op == 0x23){
            cout << "lw" << endl;

            myRF.ReadWrite(RS,0,0,0,0);
            unsigned long it = myRF.ReadData1.to_ulong() + signExt(IMM).to_ulong();
            bitset<32> addrt(it);
            myRF.ReadWrite(0,0,RT,myDataMem.MemoryAccess(addrt,0,1,0),1);

            //cout << myDataMem.MemoryAccess(addrt,0,1,0).to_ulong() << endl;
            //cout << it << endl;
        } else if(op == 0x2B){
            cout << "sw" << endl;

            myRF.ReadWrite(RT,RS,0,0,0);
            unsigned long it = myRF.ReadData2.to_ulong() + signExt(IMM).to_ulong();
            bitset<32> ad(it);
            cout << myRF.ReadData1 << endl;
            cout <<myRF.ReadData1.to_ulong() <<endl;
            cout << ad << endl;
            myDataMem.MemoryAccess(ad, myRF.ReadData1, 0, 1);

        } else if(op == 0x3f){
            cout << "halt" << endl;
            break;
        }
        PC += 4;
        // Fetch
      //bitset<32> ins  = myInsMem.ReadMemory(bitset<32> pc(PC));
		// If current insturciton is "11111111111111111111111111111111", then break;
        //break;
		// decode(Read RF)

		// Execute

		// Read/Write Mem

		// Write back to RF

        myRF.OutputRF(); // dump RF;
    }
        myDataMem.OutputDataMem(); // dump data mem

        return 0;

}
// try the speed . i think it is ok....
