#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstring>
#include<cstdio>
#include<cstdlib>

using namespace std;

class Instruction **Inst;
class Instruction **RT; 
class Instruction **WB;
class Instruction **EX;
class Instruction **IQ;
class Instruction **IS;
class Instruction **DI;
class Instruction **RR;
class Instruction **RN;
class Instruction **DE;
class Instruction **FE;

string *line;

int check_vacant_pointers (class Instruction **pointer, int size);
void Fetch(int width, int time);
void Decode(int width, int time);

// class RMT
// private members
// valid
// ROB entry
//
// public functions
//
//
// class ROB
// private members
// PC
// Dest Reg (-1  indicates empty ROB slot)
// Ready
// ROB entry
//
// Public Functions
// check number of free ROB entries..if free return free spot else -1...basically check if tail points to an empty ROB slot like it shud usually do
// rename src regs using RMT if valid bit is zero need not rename else associate a ROB entry using head, rename dest regs for an instruction by associating a new ROB entry and updating RMT
//
// Objects
// head
// tail

class Instruction
{
    private:
    unsigned long PC;
    int seq, OpType, Dest_Reg, Src1_Reg, Src2_Reg, Renamed_Dest_Reg, Renamed_Src1_Reg, Renamed_Src2_Reg, Renamed_Src1_Reg_Ready, Renamed_Src2_Reg_Ready, valid, execution_latency, FE_begin_cycle, FE_duration_cycle, DE_begin_cycle, DE_duration_cycle, RN_begin_cycle, RN_duration_cycle, RR_begin_cycle, RR_duration_cycle, DI_begin_cycle, DI_duration_cycle, IS_begin_cycle, IS_duration_cycle, EX_begin_cycle, EX_duration_cycle, WB_begin_cycle, WB_duration_cycle, RT_begin_cycle, RT_duration_cycle;

    public:
    Instruction();
    Instruction(int seq, unsigned long PC, int op_type, int DR, int SR1, int SR2, int FE_begin_cycle);
    void print();
    void incr_FE() {FE_duration_cycle++;}
    void incr_DE() {DE_duration_cycle++;}
    void incr_RR() {RR_duration_cycle++;}
    void incr_RN() {RN_duration_cycle++;}
    void incr_DI() {DI_duration_cycle++;}
    void incr_IS() {IS_duration_cycle++;}
    void incr_EX() {EX_duration_cycle++;}
    void incr_WB() {WB_duration_cycle++;}
    void incr_RT() {RT_duration_cycle++;}
    
    void set_begin_FE(int time) {FE_begin_cycle=time;}
    void set_begin_DE(int time) {DE_begin_cycle=time;}
    void set_begin_RR(int time) {RR_begin_cycle=time;}
    void set_begin_RN(int time) {RN_begin_cycle=time;}
    void set_begin_DI(int time) {DI_begin_cycle=time;}
    void set_begin_IS(int time) {IS_begin_cycle=time;}
    void set_begin_EX(int time) {EX_begin_cycle=time;}
    void set_begin_WB(int time) {WB_begin_cycle=time;}
    void set_begin_RT(int time) {RT_begin_cycle=time;}
};

void Instruction::print()
{
    cout<<seq<<" fu{"<<OpType<<"} src{"<<Src1_Reg<<","<<Src2_Reg<<"} dst{"<<Dest_Reg<<"} FE{"<<FE_begin_cycle<<","<<FE_duration_cycle<<"} DE{"<<DE_begin_cycle<<","<<DE_duration_cycle<<"} RN{"<<RN_begin_cycle<<","<<RN_duration_cycle<<"} RR{"<<RR_begin_cycle<<","<<RR_duration_cycle<<"} DI{"<<DI_begin_cycle<<","<<DI_duration_cycle<<"} IS{"<<IS_begin_cycle<<","<<IS_duration_cycle<<"} EX{"<<EX_begin_cycle<<","<<EX_duration_cycle<<"} WB{"<<WB_begin_cycle<<","<<WB_duration_cycle<<"} RT{"<<RT_begin_cycle<<","<<RT_duration_cycle<<"}"<<endl; 
}

Instruction::Instruction(int seq, unsigned long PC, int op_type, int DR, int SR1, int SR2, int FE_begin_cycle)
{
    this->seq = seq;
    this->PC = PC;
    this->OpType = op_type;
    this->Dest_Reg = DR;
    this->Src1_Reg = SR1;
    this->Src2_Reg = SR2;
    Renamed_Dest_Reg = 0;
    Renamed_Src1_Reg = 0;
    Renamed_Src2_Reg = 0;
    Renamed_Src1_Reg_Ready = 0;
    Renamed_Src2_Reg_Ready = 0;
    valid = -1;
    execution_latency = 0;
    this->FE_begin_cycle = FE_begin_cycle;
    FE_duration_cycle = 0;
    DE_begin_cycle = 0;
    DE_duration_cycle = 0;
    RN_begin_cycle = 0;
    RN_duration_cycle = 0;
    RR_begin_cycle = 0;
    RR_duration_cycle = 0;
    DI_begin_cycle = 0;
    DI_duration_cycle = 0;
    IS_begin_cycle = 0;
    IS_duration_cycle = 0;
    EX_begin_cycle = 0;
    EX_duration_cycle = 0;
    WB_begin_cycle = 0;
    WB_duration_cycle = 0;
    RT_begin_cycle = 0;
    RT_duration_cycle = 0;
}

Instruction::Instruction()
{
    PC = 0;
    OpType = 0;
    Dest_Reg = 0;
    Src1_Reg = 0;
    Src2_Reg = 0;
    Renamed_Dest_Reg = 0;
    Renamed_Src1_Reg = 0;
    Renamed_Src2_Reg = 0;
    Renamed_Src1_Reg_Ready = 0;
    Renamed_Src2_Reg_Ready = 0;
    valid = -1;
    execution_latency = 0;
    FE_begin_cycle = 0;
    FE_duration_cycle = 0;
    DE_begin_cycle = 0;
    DE_duration_cycle = 0;
    RN_begin_cycle = 0;
    RN_duration_cycle = 0;
    RR_begin_cycle = 0;
    RR_duration_cycle = 0;
    DI_begin_cycle = 0;
    DI_duration_cycle = 0;
    IS_begin_cycle = 0;
    IS_duration_cycle = 0;
    EX_begin_cycle = 0;
    EX_duration_cycle = 0;
    WB_begin_cycle = 0;
    WB_duration_cycle = 0;
    RT_begin_cycle = 0;
    RT_duration_cycle = 0;
}

void Fetch(int width, int time)
{
    if(check_vacant_pointers(FE,width) == -1)
    {
        for(int i=0;i<width;i++)
            FE[i]->incr_FE();
        int vacant = check_vacant_pointers(DE,width);
        cout<<vacant<<" DE pointers vacant\n";
        if(vacant == width)
        {
            for(int i=0; i<width;i++)
            {
                DE[i] = FE[i];
                DE[i]->set_begin_DE(time);
                FE[i] = NULL;
            }
        }

        cout<<"Fetched Following Instructions"<<endl;
        for(int i=0;i<width;i++)
            DE[i]->print();
    }
}

void Decode(int width, int time)
{
    if(check_vacant_pointers(DE,width) == -1)
    {
        for(int i=0;i<width;i++)
            DE[i]->incr_DE();

        int vacant = check_vacant_pointers(RN,width);
        cout<<vacant<<" RN pointers vacant\n";
        if(vacant == width)
        {
            for(int i=0; i<width;i++)
            {
                RN[i] = DE[i];
                DE[i] = NULL;
            }
        }

        cout<<"Decoded Following Instructions"<<endl;
        for(int i=0;i<width;i++)
            RN[i]->print();
    }
}

int check_vacant_pointers (class Instruction **pointer, int size)
{
    int count = 0;
    for(int i=0;i<size;i++)
    {
        if(pointer[i] == NULL)
            count++;
    }

    if (count == 0)
        return -1;
    else
        return count;
}

int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        cout<<"Usage: ./sim <ROB SIZE> <IQ SIZE> <WIDTH> <TraceFile>"<<endl;
        exit(-1);
    }
    
    int rob_size = atoi(argv[1]);
    int iq_size = atoi(argv[2]);
    int width = atoi(argv[3]);

    //Mem Allocation for class Instruction
    Inst = new Instruction*[width];
    RT = new Instruction*[width];
    WB = new Instruction*[5*width];
    EX = new Instruction*[5*width];
    IQ = new Instruction*[iq_size];
    IS = new Instruction*[width];
    DI = new Instruction*[width];
    RR = new Instruction*[width];
    RN = new Instruction*[width];
    DE = new Instruction*[width];
    FE = new Instruction*[width];
    
    for(int i=0;i<width;i++)
    {
        Inst[i] = new Instruction;
        RT[i] = NULL; 
        WB[i] = NULL; 
        IS[i] = NULL; 
        DI[i] = NULL; 
        RR[i] = NULL; 
        RN[i] = NULL; 
        DE[i] = NULL; 
        FE[i] = NULL; 
    }
    
    for(int i=0;i<5*width;i++)
    {
        EX[i] = NULL;
        WB[i] = NULL;
    }

    //Read Trace
    FILE *pFile = fopen(argv[4], "r");
    int i = 0;
    int j = 0;

    while(1)
    {
        unsigned long PC;
        int op_type,DR,SR1,SR2;
        fscanf(pFile," %lx %i %i %i %i\n", &PC, &op_type, &DR, &SR1, &SR2);
        cout<<i<<"\t"<<PC<<" "<<op_type<<" "<<DR<<" "<<SR1<<" "<<SR2<<endl;
        FE[i%width] = new Instruction(i,PC,op_type,DR,SR1,SR2,j);
        i++;

        if(i%width == 0 || feof(pFile))
        {
            j++;
            cout<<"Pipeline\n";
            Decode(width,j);
            Fetch(width,j);
        }
        
        if(feof(pFile))
            break;
    }

    //Trace Completed...Finish the existing instructions
    while(1)
    {
        cout<<"Pipeline\n";
        break;
    }
    return 0;
}

// Retire
//      From Head in ROB, retire upto width consecutive instructions from RT
//      Set *RT to NULL
//      Reset valid bit in RMT
//
// Writeback
//      Set ready bit 1 in ROB for each instruction
//      Transfer  *WB  to *RT
//      Mark *WB as NULL
//
// Execute
//      Increment EX duration cycle
//      if EX duration cycle == execution_latency then
//          Transfer *EX to *WB
//          Set *EX to NULL
//          Since values of Regs are available through bypasses, set ready flags for instructions in RR, DI and IQ
//      end
//      
// Issue
//      Transfer up to Width instructions from IQ to *EX based on chronological history and that both Src Regs ready flags are 1
//      set execution latency for each instruction based on op_type
//      op_type 0 : 1
//      op_type 1 : 2
//      op_type 2 : 5
//      set *IQ to NULL
//      check if src Regs 1,2's ROB entry is ready
//      if ready then
//          set renamed src Regs ready flag to 1..(by default 0)
//      end
//
// Dispatch
//      check if *DI is NULL
//      if not NULL then
//          check if src Regs 1,2's ROB entry is ready
//          if ready then
//              set renamed src Regs ready flag to 1..(by default 0)
//          end
//
//          if number of free IQ entries >= size of dispatch bundle
//              Allocate IssueQueue entry for each *DI
//              set *DI to NULL
//          end
//      end
//
// Regread
//      check empty *DI pointers and if *RR is not NULL
//      if empty then
//          check if src Regs 1,2's ROB entry is ready
//          if ready then
//              set renamed src Regs ready flag to 1..(by default 0)
//          end
//          Transfer *RR to *DI
//          set *RR to NULL
//      end
//
// Rename
//      check empty RR pointers and empty ROB entries and *RN is not NULL
//      if both empty then
//          check RMT for src Regs1,2
//          if RMT valid == 0, no renaming
//          if RMT valid == 1, assign ROB entry to the Renamed Src Reg
//          Allocate new ROB entry for Dest Reg and assign ROB entry to Renamed Dest Reg
//          Transfer *RN to *RR
//          set *RN to NULL
//      end
//
// Decode
//      check empty RN pointers
//      if empty then
//          Transfer *DE to *RN
//          Set *DE to NULL
//      end
//
// 
