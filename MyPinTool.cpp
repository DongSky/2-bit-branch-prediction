#include <pin.h>
#include <cstdio>
#include <map>
struct BP_Info {
    bool Taken;
    ADDRINT PredTarget;
};
struct BranchPredictor {
    map<ADDRINT, ADDRINT> target;
    map<ADDRINT, int> state;
    BP_Info GetPrediction(ADDRINT PC) {
        BP_Info res;
        if (state[PC] >= 2)
            res.Taken = 1;
        else
            res.Taken = 0;
        res.PredTarget = target[PC];
        return res;
    }
    void Update(ADDRINT PC, bool BrTaken, ADDRINT targetPC) {
        switch(state[PC]) {
            case 0: if (BrTaken == 1) state[PC] = 1; else state[PC] = 0; break;
            case 1: if (BrTaken == 1) state[PC] = 3; else state[PC] = 0; break;
            case 2: if (BrTaken == 1) state[PC] = 3; else state[PC] = 0; break;
            case 3: if (BrTaken == 1) state[PC] = 3; else state[PC] = 2; break;
        }
        target[PC] = targetPC;
    }
} myBPU;
int total = 0, right = 0;


void ProcessBranch(ADDRINT PC, ADDRINT targetPC, bool BrTaken) {
    BP_Info pred = myBPU.GetPrediction(PC);
    int flag = 1;
    if (pred.Taken != BrTaken) {
        flag = 0;
    }
    if (pred.PredTarget != targetPC){
        if (BrTaken == true)
            flag = 0;
    }
    total ++;
    right += flag;
    myBPU.Update(PC, BrTaken, targetPC);
}

void Instruction(INS ins, void *v) {
    if (INS_IsDirectBranchOrCall(ins))// || INS_HasFallThrough(ins))
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR) ProcessBranch,
        IARG_ADDRINT, INS_Address(ins),
        IARG_ADDRINT, INS_DirectBranchOrCallTargetAddress(ins),
        IARG_BRANCH_TAKEN, IARG_END);
}

void Fini(INT32 code, void *v) {
    printf("Total Branches: %d\n", total);
    printf("Branches that predicted correctly: %d\n", right);
    printf("Correct rate: %.3lf%%\n", right * 100.0 / total);
}

int main(int argc, char * argv[]) {
    PIN_Init(argc, argv);
    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);
    PIN_StartProgram();
    return 0;
}