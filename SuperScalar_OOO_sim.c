#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstring>
#include<cstdio>
#include<cstdlib>

//1 NOT READY INVALID
//0 READY VALID

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
void Rename(int rob_size, int width, int time);
void Regread(int width, int time);
void Dispatch(int iq_size, int width, int time);
void Issue(int iq_size,int width, int time);
void Execute(int iq_size, int width, int time);

class RMT
{
    private:
    int valid, ROB_entry;

    public:
    RMT();
    RMT(int valid, int ROB_entry);
    int get_valid() { return valid; }
    int get_ROB_entry() { return ROB_entry; }
};

RMT::RMT()
{
    valid = 0;
    ROB_entry = -1;
}

RMT::RMT(int valid, int ROB_entry)
{
    this->valid = valid;
    this->ROB_entry = ROB_entry;
}

class RMT **rmt;

class ROB
{
    private:
    long PC;
    int Dest_Reg, Ready, ROB_entry;

    public:
    ROB(int i);
    ROB(long PC, int Dest_Reg, int Ready);
    long get_PC() { return PC; }
    long get_Ready() { return Ready; }
    int get_ROB_entry() { return ROB_entry; }
};

ROB::ROB(int i)
{
    PC = -1;
    Dest_Reg = -1;
    Ready = 1;
    ROB_entry = i;
}

ROB::ROB(long PC, int Dest_Reg, int Ready)
{
    this->PC = PC;
    this->Dest_Reg = Dest_Reg;
    this->Ready = Ready;
}

class ROB *head, *tail;
class ROB **rob;
// Public Functions
// rename src regs using RMT if valid bit is zero need not rename else associate a ROB entry using head, rename dest regs for an instruction by associating a new ROB entry and updating RMT

class Instruction
{
    private:
    long PC;
    int seq, OpType, Dest_Reg, Src1_Reg, Src2_Reg, Renamed_Dest_Reg, Renamed_Src1_Reg, Renamed_Src2_Reg, Renamed_Src1_Reg_Ready, Renamed_Src2_Reg_Ready, valid, execution_latency, FE_begin_cycle, FE_duration_cycle, DE_begin_cycle, DE_duration_cycle, RN_begin_cycle, RN_duration_cycle, RR_begin_cycle, RR_duration_cycle, DI_begin_cycle, DI_duration_cycle, IQ_begin_cycle, IQ_duration_cycle, EX_begin_cycle, EX_duration_cycle, WB_begin_cycle, WB_duration_cycle, RT_begin_cycle, RT_duration_cycle;

    public:
    Instruction();
    Instruction(int seq, long PC, int op_type, int DR, int SR1, int SR2, int FE_begin_cycle);
    void print();
    void incr_FE() {FE_duration_cycle++;}
    void incr_DE() {DE_duration_cycle++;}
    void incr_RR() {RR_duration_cycle++;}
    void incr_RN() {RN_duration_cycle++;}
    void incr_DI() {DI_duration_cycle++;}
    void incr_IQ() {IQ_duration_cycle++;}
    void incr_EX() {EX_duration_cycle++;}
    void incr_WB() {WB_duration_cycle++;}
    void incr_RT() {RT_duration_cycle++;}
    
    void set_begin_FE(int time) {FE_begin_cycle=time;}
    void set_begin_DE(int time) {DE_begin_cycle=time;}
    void set_begin_RR(int time) {RR_begin_cycle=time;}
    void set_begin_RN(int time) {RN_begin_cycle=time;}
    void set_begin_DI(int time) {DI_begin_cycle=time;}
    void set_begin_IQ(int time) {IQ_begin_cycle=time;}
    void set_begin_EX(int time) {EX_begin_cycle=time;}
    void set_begin_WB(int time) {WB_begin_cycle=time;}
    void set_begin_RT(int time) {RT_begin_cycle=time;}
    
    long get_PC() { return PC; }
    int get_Dest_Reg() { return Dest_Reg; }
    int get_Src1_Reg() { return Src1_Reg; }
    int get_Src2_Reg() { return Src2_Reg; }
    int get_valid() { return valid; }
    void set_Renamed_Src1_Reg(int reg) { this->Renamed_Src1_Reg = reg; }
    void set_Renamed_Src2_Reg(int reg) { this->Renamed_Src2_Reg = reg; }
    void set_Renamed_Dest_Reg(int reg) { this->Renamed_Dest_Reg = reg; }
    void set_Renamed_Src1_Reg_Ready(int reg) { this->Renamed_Src1_Reg_Ready = reg; }
    void set_Renamed_Src2_Reg_Ready(int reg) { this->Renamed_Src2_Reg_Ready = reg; }
    void set_valid(int reg) { this->valid = reg; }
    void set_execute_latency(int reg) { this->execution_latency = reg; }
    int get_execute_latency() { return execution_latency; }
    int get_EX_duration_cycle() { return EX_duration_cycle; }
    int get_op_type() { return OpType; }
    int get_Renamed_Src1_Reg() { return Renamed_Src1_Reg; }
    int get_Renamed_Src2_Reg() { return Renamed_Src2_Reg; }
    int get_Renamed_Dest_Reg() { return Renamed_Dest_Reg; }
    int get_Renamed_Src1_Reg_Ready() { return Renamed_Src1_Reg_Ready; }
    int get_Renamed_Src2_Reg_Ready() { return Renamed_Src2_Reg_Ready; }
};

void Instruction::print()
{
    cout<<seq<<" fu{"<<OpType<<"} src{"<<Src1_Reg<<","<<Src2_Reg<<"} dst{"<<Dest_Reg<<"} FE{"<<FE_begin_cycle<<","<<FE_duration_cycle<<"} DE{"<<DE_begin_cycle<<","<<DE_duration_cycle<<"} RN{"<<RN_begin_cycle<<","<<RN_duration_cycle<<"} RR{"<<RR_begin_cycle<<","<<RR_duration_cycle<<"} DI{"<<DI_begin_cycle<<","<<DI_duration_cycle<<"} IQ{"<<IQ_begin_cycle<<","<<IQ_duration_cycle<<"} EX{"<<EX_begin_cycle<<","<<EX_duration_cycle<<"} WB{"<<WB_begin_cycle<<","<<WB_duration_cycle<<"} RT{"<<RT_begin_cycle<<","<<RT_duration_cycle<<"}"<<endl; 
}

Instruction::Instruction(int seq, long PC, int op_type, int DR, int SR1, int SR2, int FE_begin_cycle)
{
    this->seq = seq;
    this->PC = PC;
    this->OpType = op_type;
    this->Dest_Reg = DR;
    this->Src1_Reg = SR1;
    this->Src2_Reg = SR2;
    Renamed_Dest_Reg = -1;
    Renamed_Src1_Reg = -1;
    Renamed_Src2_Reg = -1;
    Renamed_Src1_Reg_Ready = 1;
    Renamed_Src2_Reg_Ready = 1;
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
    IQ_begin_cycle = 0;
    IQ_duration_cycle = 0;
    EX_begin_cycle = 0;
    EX_duration_cycle = 0;
    WB_begin_cycle = 0;
    WB_duration_cycle = 0;
    RT_begin_cycle = 0;
    RT_duration_cycle = 0;
}

Instruction::Instruction()
{
    PC = -1;
    OpType = -1;
    Dest_Reg = -1;
    Src1_Reg = -1;
    Src2_Reg = -1;
    Renamed_Dest_Reg = -1;
    Renamed_Src1_Reg = -1;
    Renamed_Src2_Reg = -1;
    Renamed_Src1_Reg_Ready = 1;
    Renamed_Src2_Reg_Ready = 1;
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
    IQ_begin_cycle = 0;
    IQ_duration_cycle = 0;
    EX_begin_cycle = 0;
    EX_duration_cycle = 0;
    WB_begin_cycle = 0;
    WB_duration_cycle = 0;
    RT_begin_cycle = 0;
    RT_duration_cycle = 0;
}

void Fetch(int width, int time)
{
    if(check_vacant_pointers(FE,width) < width)
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
            cout<<"Fetched Following Instructions"<<endl;
            for(int i=0;i<width;i++)
                DE[i]->print();
        }
    }
}

void Decode(int width, int time)
{
    if(check_vacant_pointers(DE,width) < width)
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
                RN[i]->set_begin_RN(time);
                DE[i] = NULL;
            }
            cout<<"Decoded Following Instructions"<<endl;
            for(int i=0;i<width;i++)
                RN[i]->print();
        }
    }
}

void Rename(int rob_size, int width, int time)
{
    if(check_vacant_pointers(RN,width) < width)
    {
        for(int i=0;i<width;i++)
            RN[i]->incr_RN();
        
        //checking free entries in ROB
        int rob_tail = tail->get_ROB_entry();
        for(int i=0;i<width;i++)
        {
            rob_tail++;
            if(rob_tail == 67)
                rob_tail = 0;
            
            if(rob[rob_tail]->get_PC() != -1)
                return;
        }
        
        int vacant = check_vacant_pointers(RR,width);
        cout<<vacant<<" RR pointers vacant\n";

        if(vacant == width)
        {
            for(int i=0; i<width;i++)
            {
                //use RMT to figure if ARF value is available or ROB is needed
                int Src1_Reg = RN[i]->get_Src1_Reg();
                if(Src1_Reg != -1)
                {
                    if(rmt[Src1_Reg]->get_valid() == 1)
                        RN[i]->set_Renamed_Src1_Reg(rmt[Src1_Reg]->get_ROB_entry());
                    else
                        RN[i]->set_Renamed_Src1_Reg_Ready(0);
                }
                else
                        RN[i]->set_Renamed_Src1_Reg_Ready(0);
                
                int Src2_Reg = RN[i]->get_Src2_Reg();
                if(Src2_Reg != -1)
                {
                    if(rmt[Src2_Reg]->get_valid() == 1)
                        RN[i]->set_Renamed_Src2_Reg(rmt[Src2_Reg]->get_ROB_entry());
                    else
                        RN[i]->set_Renamed_Src2_Reg_Ready(0);
                }
                else
                        RN[i]->set_Renamed_Src2_Reg_Ready(0);
                
                //map new ROB and RMT entry for destination register
                if(RN[i]->get_Dest_Reg() != -1)
                {
                    int Dest_Reg = RN[i]->get_Dest_Reg();
                    tail = new ROB(RN[i]->get_PC(), Dest_Reg, 1);
                    rmt[Dest_Reg] = new RMT(1,tail->get_ROB_entry());
                    RN[i]->set_Renamed_Dest_Reg(rmt[Dest_Reg]->get_ROB_entry());
                }
                else //Add conditions when there is no Dest Reg.
                {
                    tail = new ROB(RN[i]->get_PC(),-1,1);
                    RN[i]->set_Renamed_Dest_Reg(-1);
                }
                                
                if(tail->get_ROB_entry() == 66)
                    tail = rob[0];
                else
                    tail = rob[tail->get_ROB_entry() + 1];
                
                RR[i] = RN[i];
                RR[i]->set_begin_RR(time);
                RN[i] = NULL;
            }
            cout<<"Renamed Following Instructions"<<endl;
            for(int i=0;i<width;i++)
                RR[i]->print();
        }
    }
}

void Regread(int width, int time)
{
    if(check_vacant_pointers(RR,width) < width)
    {
        for(int i=0;i<width;i++)
            RR[i]->incr_RR();

        int vacant = check_vacant_pointers(DI,width);
        cout<<vacant<<" DI pointers vacant\n";
        if(vacant == width)
        {
            for(int i=0; i<width;i++)
            {
                if(RR[i]->get_Renamed_Src1_Reg_Ready() == 1 && RR[i]->get_Renamed_Src1_Reg() != -1)
                    if(rob[RR[i]->get_Renamed_Src1_Reg()]->get_Ready() == 0)
                        RR[i]->set_Renamed_Src1_Reg_Ready(0);
                
                if(RR[i]->get_Renamed_Src2_Reg_Ready() == 1 && RR[i]->get_Renamed_Src2_Reg() != -1)
                    if(rob[RR[i]->get_Renamed_Src2_Reg()]->get_Ready() == 0)
                        RR[i]->set_Renamed_Src2_Reg_Ready(0);

                DI[i] = RR[i];
                DI[i]->set_begin_DI(time);
                RR[i] = NULL;
            }
            cout<<"Register Read Following Instructions"<<endl;
            for(int i=0;i<width;i++)
                DI[i]->print();
        }
    }
}

void Dispatch(int iq_size, int width, int time)
{
    if(check_vacant_pointers(DI,width) < width)
    {
        for(int i=0;i<width;i++)
            DI[i]->incr_DI();

        int vacant = check_vacant_pointers(IQ,iq_size);
        cout<<vacant<<" IQ pointers vacant\n";
        if(vacant >= width)
        {
            cout<<"Dispatched Following Instructions"<<endl;
            for(int i=0; i<width;i++)
            {
                if(DI[i]->get_Renamed_Src1_Reg() != -1 && DI[i]->get_Renamed_Src1_Reg_Ready() == 1)
                    if(rob[DI[i]->get_Renamed_Src1_Reg()]->get_Ready() == 0)
                        DI[i]->set_Renamed_Src1_Reg_Ready(0);
                
                if(DI[i]->get_Renamed_Src2_Reg() != -1 && DI[i]->get_Renamed_Src2_Reg_Ready() == 1)
                    if(rob[DI[i]->get_Renamed_Src2_Reg()]->get_Ready() == 0)
                        DI[i]->set_Renamed_Src2_Reg_Ready(0);

                int found_spot = 0;
                //Find empty IQ index
                for(int j=0;j<iq_size;j++)
                    if(IQ[j] == NULL)
                    {
                        found_spot = 1;
                        IQ[j] = DI[i];
                        IQ[j]->set_begin_IQ(time);
                        IQ[j]->set_valid(0);
                        IQ[j]->print();
                        DI[i] = NULL;
                        break;
                    }

                if(found_spot == 0)
                {
                     for(int j=0;j<iq_size;j++)
                     {
                         if(IQ[j]->get_valid() == 1)
                         {
                             found_spot = 1;
                             IQ[j] = DI[i];
                             IQ[j]->set_begin_IQ(time);
                             IQ[j]->set_valid(0);
                             IQ[j]->print();
                             DI[i] = NULL;
                             break;
                         }
                     }
                }
            }

        }
    }
}

void Issue(int iq_size,int width, int time)
{
    if(check_vacant_pointers(IQ,iq_size) < iq_size)
    {
        for(int i=0;i<iq_size;i++)
            if(IQ[i] != NULL)
                if(IQ[i]->get_valid() == 0)
                    IQ[i]->incr_IQ();

        int vacant = check_vacant_pointers(EX,5*width);
        cout<<vacant<<" EX pointers vacant\n";
        if(vacant >= width)
        {
            //Chuck up to width oldest instructions to EX whose both inputs are ready
            cout<<"Issued Following Instructions"<<endl;
            for(int i=0; i<width;i++)
            {
                int k;
                for(k=0;k<5*width;k++)
                    if(EX[k] == NULL)
                        break;

                if(k == 5*width)
                    break;

                for(int j=0;j<iq_size;j++)
                {
                    if(IQ[j] != NULL)
                    {
                        if(IQ[j]->get_valid() == 0)
                        {
                            if(IQ[j]->get_Renamed_Src1_Reg() != -1 && IQ[j]->get_Renamed_Src1_Reg_Ready() == 1)
                                if(rob[IQ[j]->get_Renamed_Src1_Reg()]->get_Ready() == 0)
                                    IQ[j]->set_Renamed_Src1_Reg_Ready(0);
                            
                            if(IQ[j]->get_Renamed_Src2_Reg() != -1 && IQ[j]->get_Renamed_Src2_Reg_Ready() == 1)
                                if(rob[IQ[j]->get_Renamed_Src2_Reg()]->get_Ready() == 0)
                                    IQ[j]->set_Renamed_Src2_Reg_Ready(0);
               
                            if(IQ[j]->get_Renamed_Src1_Reg_Ready() == 0 && IQ[j]->get_Renamed_Src2_Reg_Ready() == 0)
                            {
                                EX[k] = IQ[j];
                                EX[k]->set_begin_EX(time);
                                switch(EX[k]->get_op_type())
                                {
                                    case 0: EX[k]->set_execute_latency(1); break;
                                    case 1: EX[k]->set_execute_latency(2); break;
                                    case 2: EX[k]->set_execute_latency(5); break;
                                    default: cout<<"Erroneous op_type "<<EX[k]->get_op_type()<<endl; exit(-1);
                                }
                                EX[k]->print();
                                IQ[j]->set_valid(1);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Execute(int iq_size, int width, int time)
{
    if(check_vacant_pointers(EX,5*width) < 5*width)
    {
        for(int i=0;i<5*width;i++)
            if(EX[i] != NULL)
                EX[i]->incr_EX();

        int vacant = check_vacant_pointers(WB,5*width);
        cout<<vacant<<" WB pointers vacant\n";
        if(vacant >= width)
        {
            cout<<"Executed Following Instructions"<<endl;
            for(int i=0; i<5*width;i++)
            {
                if(EX[i] != NULL)
                {
                    if(EX[i]->get_EX_duration_cycle() == EX[i]->get_execute_latency())
                    {
                        //Find Empty WB slot
                        int k;
                        for(k=0;k<5*width;k++)
                            if(WB[k] == NULL)
                                break;
                        
                        if(k == 5*width)
                            break;
                        //Wakeup source operands in RR, DI and IQ through bypasses
                        for(int j=0;j<width;j++)
                        {
                            if(RR[j] != NULL)
                            {
                                if(RR[j]->get_Renamed_Src1_Reg_Ready() == 1 && RR[j]->get_Renamed_Src1_Reg() != -1)
                                    if(RR[j]->get_Renamed_Src1_Reg() == EX[i]->get_Renamed_Dest_Reg())
                                        RR[j]->set_Renamed_Src1_Reg_Ready(0);
                                
                                if(RR[j]->get_Renamed_Src2_Reg_Ready() == 1 && RR[j]->get_Renamed_Src2_Reg() != -1)
                                    if(RR[j]->get_Renamed_Src2_Reg() == EX[i]->get_Renamed_Dest_Reg())
                                        RR[j]->set_Renamed_Src2_Reg_Ready(0);
                            }
                            
                            if(DI[j] != NULL)
                            {
                                if(DI[j]->get_Renamed_Src1_Reg_Ready() == 1 && DI[j]->get_Renamed_Src1_Reg() != -1)
                                    if(DI[j]->get_Renamed_Src1_Reg() == EX[i]->get_Renamed_Dest_Reg())
                                        DI[j]->set_Renamed_Src1_Reg_Ready(0);
                                
                                if(DI[j]->get_Renamed_Src2_Reg_Ready() == 1 && DI[j]->get_Renamed_Src2_Reg() != -1)
                                    if(DI[j]->get_Renamed_Src2_Reg() == EX[i]->get_Renamed_Dest_Reg())
                                        DI[j]->set_Renamed_Src2_Reg_Ready(0);
                            }
                        }
                        
                        for(int j=0;j<iq_size;j++)
                        {
                            if(IQ[j] != NULL)
                            {
                                if(IQ[j]->get_Renamed_Src1_Reg_Ready() == 1 && IQ[j]->get_Renamed_Src1_Reg() != -1)
                                    if(IQ[j]->get_Renamed_Src1_Reg() == EX[i]->get_Renamed_Dest_Reg())
                                        IQ[j]->set_Renamed_Src1_Reg_Ready(0);
                                
                                if(IQ[j]->get_Renamed_Src2_Reg_Ready() == 1 && IQ[j]->get_Renamed_Src2_Reg() != -1)
                                    if(IQ[j]->get_Renamed_Src2_Reg() == EX[i]->get_Renamed_Dest_Reg())
                                        IQ[j]->set_Renamed_Src2_Reg_Ready(0);
                            }
                        }

                        WB[k] = EX[i];
                        WB[k]->set_begin_WB(time);
                        WB[k]->print();
                        EX[i] = NULL;
                    }
                }
            }
        }
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
    rob = new ROB*[rob_size];
    rmt = new RMT*[67];
    
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
    
    for(int i=0;i<iq_size;i++)
        IQ[i] = NULL;
    
    for(int i=0;i<5*width;i++)
    {
        EX[i] = NULL;
        WB[i] = NULL;
    }
    
    for(int i=0;i<rob_size;i++)
        rob[i] = new ROB(i);
    
    head = rob[0];
    tail = rob[0];

    for(int i=0;i<67;i++)
        rmt[i] = new RMT;

    //Read Trace
    FILE *pFile = fopen(argv[4], "r");
    int i = 0;
    int j = 0;

    while(1)
    {
        long PC;
        int op_type,DR,SR1,SR2;
        fscanf(pFile," %lx %i %i %i %i\n", &PC, &op_type, &DR, &SR1, &SR2);
        cout<<i<<"\t"<<PC<<" "<<op_type<<" "<<DR<<" "<<SR1<<" "<<SR2<<endl;
        FE[i%width] = new Instruction(i,PC,op_type,DR,SR1,SR2,j);
        i++;
//At the end when traces are less than width...supply a new param left_over in place of width...do not change width
        if(i%width == 0 || feof(pFile))
        {
            j++;
            Execute(iq_size,width,j);
            Issue(iq_size,width,j);
            Dispatch(iq_size,width,j);
            Regread(width,j);
            Rename(rob_size,width,j);
            Decode(width,j);
            Fetch(width,j);
        }
        
        if(feof(pFile))
            break;
    }

    //Trace Completed...Finish the existing instructions
    while(1)
    {
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
