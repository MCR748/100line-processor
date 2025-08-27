// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VPROCESSOR_H_
#define _VPROCESSOR_H_  // guard

#include "verilated.h"

//==========

class Vprocessor__Syms;
class Vprocessor_VerilatedVcd;


//----------

VL_MODULE(Vprocessor) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(memEn,0,0);
    VL_IN(memData,31,0);
    VL_IN(memAddr,31,0);
    VL_OUT(gp,31,0);
    VL_OUT(a7,31,0);
    VL_OUT(a0,31,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*3:0*/ processor__DOT__aluOp;
    CData/*4:0*/ processor__DOT__opcode;
    CData/*6:0*/ processor__DOT__funct7;
    CData/*0:0*/ processor__DOT__isArithmetic;
    CData/*0:0*/ processor__DOT__isImm;
    CData/*0:0*/ processor__DOT__isMemRead;
    CData/*0:0*/ processor__DOT__isLoadUI;
    CData/*0:0*/ processor__DOT__isMemWrite;
    CData/*0:0*/ processor__DOT__isBranch;
    CData/*0:0*/ processor__DOT__isJAL;
    CData/*0:0*/ processor__DOT__isJALR;
    CData/*0:0*/ processor__DOT__isMUL;
    CData/*0:0*/ processor__DOT__isAUIPC;
    CData/*0:0*/ processor__DOT__isBranchR;
    CData/*0:0*/ processor__DOT__isBranchC;
    IData/*31:0*/ processor__DOT__ins;
    IData/*31:0*/ processor__DOT__imm;
    IData/*31:0*/ processor__DOT__pc;
    IData/*31:0*/ processor__DOT__data_1;
    IData/*31:0*/ processor__DOT__data_2;
    IData/*31:0*/ processor__DOT__src1;
    IData/*31:0*/ processor__DOT__src2;
    IData/*31:0*/ processor__DOT__aluOut;
    IData/*31:0*/ processor__DOT__memRead;
    IData/*31:0*/ processor__DOT__memWrite;
    CData/*7:0*/ processor__DOT__mainMemory[4096];
    IData/*31:0*/ processor__DOT__registers[32];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vclklast__TOP__clock;
    CData/*0:0*/ __Vm_traceActivity[3];
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vprocessor__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vprocessor);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vprocessor(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vprocessor();
    /// Trace signals in the model; called by application code
    void trace(VerilatedVcdC* tfp, int levels, int options = 0);
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
    static void _eval_initial_loop(Vprocessor__Syms* __restrict vlSymsp);
    void __Vconfigure(Vprocessor__Syms* symsp, bool first);
  private:
    static QData _change_request(Vprocessor__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vprocessor__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__3(Vprocessor__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vprocessor__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vprocessor__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vprocessor__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__1(Vprocessor__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(Vprocessor__Syms* __restrict vlSymsp) VL_ATTR_COLD;
  private:
    static void traceChgSub0(void* userp, VerilatedVcd* tracep);
    static void traceChgTop0(void* userp, VerilatedVcd* tracep);
    static void traceCleanup(void* userp, VerilatedVcd* /*unused*/);
    static void traceFullSub0(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceFullTop0(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceInitSub0(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceInitTop(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    void traceRegister(VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceInit(void* userp, VerilatedVcd* tracep, uint32_t code) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
