// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vprocessor.h for the primary calling header

#include "Vprocessor.h"
#include "Vprocessor__Syms.h"

//==========

void Vprocessor::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vprocessor::eval\n"); );
    Vprocessor__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        vlSymsp->__Vm_activity = true;
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("processor.sv", 3, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vprocessor::_eval_initial_loop(Vprocessor__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    vlSymsp->__Vm_activity = true;
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        _eval_settle(vlSymsp);
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("processor.sv", 3, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vprocessor::_sequent__TOP__1(Vprocessor__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vprocessor::_sequent__TOP__1\n"); );
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*7:0*/ __Vdlyvval__processor__DOT__mainMemory__v0;
    CData/*7:0*/ __Vdlyvval__processor__DOT__mainMemory__v1;
    CData/*0:0*/ __Vdlyvset__processor__DOT__mainMemory__v1;
    CData/*7:0*/ __Vdlyvval__processor__DOT__mainMemory__v2;
    CData/*0:0*/ __Vdlyvset__processor__DOT__mainMemory__v2;
    CData/*7:0*/ __Vdlyvval__processor__DOT__mainMemory__v3;
    CData/*7:0*/ __Vdlyvval__processor__DOT__mainMemory__v4;
    CData/*0:0*/ __Vdlyvset__processor__DOT__mainMemory__v4;
    CData/*7:0*/ __Vdlyvval__processor__DOT__mainMemory__v5;
    CData/*7:0*/ __Vdlyvval__processor__DOT__mainMemory__v6;
    CData/*7:0*/ __Vdlyvval__processor__DOT__mainMemory__v7;
    CData/*7:0*/ __Vdlyvval__processor__DOT__mainMemory__v8;
    CData/*0:0*/ __Vdlyvset__processor__DOT__mainMemory__v8;
    CData/*7:0*/ __Vdlyvval__processor__DOT__mainMemory__v9;
    CData/*7:0*/ __Vdlyvval__processor__DOT__mainMemory__v10;
    CData/*7:0*/ __Vdlyvval__processor__DOT__mainMemory__v11;
    CData/*4:0*/ __Vdlyvdim0__processor__DOT__registers__v0;
    CData/*0:0*/ __Vdlyvset__processor__DOT__registers__v0;
    SData/*11:0*/ __Vdlyvdim0__processor__DOT__mainMemory__v0;
    SData/*11:0*/ __Vdlyvdim0__processor__DOT__mainMemory__v1;
    SData/*11:0*/ __Vdlyvdim0__processor__DOT__mainMemory__v2;
    SData/*11:0*/ __Vdlyvdim0__processor__DOT__mainMemory__v3;
    SData/*11:0*/ __Vdlyvdim0__processor__DOT__mainMemory__v4;
    SData/*11:0*/ __Vdlyvdim0__processor__DOT__mainMemory__v5;
    SData/*11:0*/ __Vdlyvdim0__processor__DOT__mainMemory__v6;
    SData/*11:0*/ __Vdlyvdim0__processor__DOT__mainMemory__v7;
    SData/*11:0*/ __Vdlyvdim0__processor__DOT__mainMemory__v8;
    SData/*11:0*/ __Vdlyvdim0__processor__DOT__mainMemory__v9;
    SData/*11:0*/ __Vdlyvdim0__processor__DOT__mainMemory__v10;
    SData/*11:0*/ __Vdlyvdim0__processor__DOT__mainMemory__v11;
    IData/*31:0*/ __Vdly__processor__DOT__pc;
    IData/*31:0*/ __Vdlyvval__processor__DOT__registers__v0;
    // Body
    __Vdly__processor__DOT__pc = vlTOPp->processor__DOT__pc;
    __Vdlyvset__processor__DOT__registers__v0 = 0U;
    __Vdlyvset__processor__DOT__mainMemory__v1 = 0U;
    __Vdlyvset__processor__DOT__mainMemory__v2 = 0U;
    __Vdlyvset__processor__DOT__mainMemory__v4 = 0U;
    __Vdlyvset__processor__DOT__mainMemory__v8 = 0U;
    __Vdly__processor__DOT__pc = ((IData)(vlTOPp->reset)
                                   ? 0U : ((((IData)(vlTOPp->processor__DOT__isJAL) 
                                             | (IData)(vlTOPp->processor__DOT__isJALR)) 
                                            | (IData)(vlTOPp->processor__DOT__isBranchR))
                                            ? vlTOPp->processor__DOT__aluOut
                                            : ((IData)(4U) 
                                               + vlTOPp->processor__DOT__pc)));
    if ((((((((IData)(vlTOPp->processor__DOT__isArithmetic) 
              | (IData)(vlTOPp->processor__DOT__isImm)) 
             | (IData)(vlTOPp->processor__DOT__isMemRead)) 
            | (IData)(vlTOPp->processor__DOT__isLoadUI)) 
           | (IData)(vlTOPp->processor__DOT__isJAL)) 
          | (IData)(vlTOPp->processor__DOT__isJALR)) 
         | (IData)(vlTOPp->processor__DOT__isAUIPC))) {
        __Vdlyvval__processor__DOT__registers__v0 = 
            (((IData)(vlTOPp->processor__DOT__isJALR) 
              | (IData)(vlTOPp->processor__DOT__isJAL))
              ? ((IData)(4U) + vlTOPp->processor__DOT__pc)
              : ((IData)(vlTOPp->processor__DOT__isMemRead)
                  ? vlTOPp->processor__DOT__memRead
                  : vlTOPp->processor__DOT__aluOut));
        __Vdlyvset__processor__DOT__registers__v0 = 1U;
        __Vdlyvdim0__processor__DOT__registers__v0 
            = (0x1fU & (vlTOPp->processor__DOT__ins 
                        >> 7U));
    }
    __Vdlyvval__processor__DOT__mainMemory__v0 = (0xffU 
                                                  & ((IData)(vlTOPp->memEn)
                                                      ? vlTOPp->memData
                                                      : 
                                                     vlTOPp->processor__DOT__mainMemory
                                                     [
                                                     (0xfffU 
                                                      & vlTOPp->memAddr)]));
    __Vdlyvdim0__processor__DOT__mainMemory__v0 = (0xfffU 
                                                   & vlTOPp->memAddr);
    if (vlTOPp->processor__DOT__isMemWrite) {
        if ((0U == (7U & (vlTOPp->processor__DOT__ins 
                          >> 0xcU)))) {
            __Vdlyvval__processor__DOT__mainMemory__v1 
                = (0xffU & vlTOPp->processor__DOT__memWrite);
            __Vdlyvset__processor__DOT__mainMemory__v1 = 1U;
            __Vdlyvdim0__processor__DOT__mainMemory__v1 
                = (0xfffU & vlTOPp->processor__DOT__aluOut);
        } else {
            if ((1U == (7U & (vlTOPp->processor__DOT__ins 
                              >> 0xcU)))) {
                __Vdlyvval__processor__DOT__mainMemory__v2 
                    = (0xffU & vlTOPp->processor__DOT__memWrite);
                __Vdlyvset__processor__DOT__mainMemory__v2 = 1U;
                __Vdlyvdim0__processor__DOT__mainMemory__v2 
                    = (0xfffU & vlTOPp->processor__DOT__aluOut);
                __Vdlyvval__processor__DOT__mainMemory__v3 
                    = (0xffU & (vlTOPp->processor__DOT__memWrite 
                                >> 8U));
                __Vdlyvdim0__processor__DOT__mainMemory__v3 
                    = (0xfffU & ((IData)(1U) + vlTOPp->processor__DOT__aluOut));
            } else {
                if ((2U == (7U & (vlTOPp->processor__DOT__ins 
                                  >> 0xcU)))) {
                    __Vdlyvval__processor__DOT__mainMemory__v4 
                        = (0xffU & vlTOPp->processor__DOT__memWrite);
                    __Vdlyvset__processor__DOT__mainMemory__v4 = 1U;
                    __Vdlyvdim0__processor__DOT__mainMemory__v4 
                        = (0xfffU & vlTOPp->processor__DOT__aluOut);
                    __Vdlyvval__processor__DOT__mainMemory__v5 
                        = (0xffU & (vlTOPp->processor__DOT__memWrite 
                                    >> 8U));
                    __Vdlyvdim0__processor__DOT__mainMemory__v5 
                        = (0xfffU & ((IData)(1U) + vlTOPp->processor__DOT__aluOut));
                    __Vdlyvval__processor__DOT__mainMemory__v6 
                        = (0xffU & (vlTOPp->processor__DOT__memWrite 
                                    >> 0x10U));
                    __Vdlyvdim0__processor__DOT__mainMemory__v6 
                        = (0xfffU & ((IData)(2U) + vlTOPp->processor__DOT__aluOut));
                    __Vdlyvval__processor__DOT__mainMemory__v7 
                        = (0xffU & (vlTOPp->processor__DOT__memWrite 
                                    >> 0x18U));
                    __Vdlyvdim0__processor__DOT__mainMemory__v7 
                        = (0xfffU & ((IData)(3U) + vlTOPp->processor__DOT__aluOut));
                } else {
                    __Vdlyvval__processor__DOT__mainMemory__v8 
                        = vlTOPp->processor__DOT__mainMemory
                        [(0xfffU & vlTOPp->processor__DOT__aluOut)];
                    __Vdlyvset__processor__DOT__mainMemory__v8 = 1U;
                    __Vdlyvdim0__processor__DOT__mainMemory__v8 
                        = (0xfffU & vlTOPp->processor__DOT__aluOut);
                    __Vdlyvval__processor__DOT__mainMemory__v9 
                        = vlTOPp->processor__DOT__mainMemory
                        [(0xfffU & ((IData)(1U) + vlTOPp->processor__DOT__aluOut))];
                    __Vdlyvdim0__processor__DOT__mainMemory__v9 
                        = (0xfffU & ((IData)(1U) + vlTOPp->processor__DOT__aluOut));
                    __Vdlyvval__processor__DOT__mainMemory__v10 
                        = vlTOPp->processor__DOT__mainMemory
                        [(0xfffU & ((IData)(2U) + vlTOPp->processor__DOT__aluOut))];
                    __Vdlyvdim0__processor__DOT__mainMemory__v10 
                        = (0xfffU & ((IData)(2U) + vlTOPp->processor__DOT__aluOut));
                    __Vdlyvval__processor__DOT__mainMemory__v11 
                        = vlTOPp->processor__DOT__mainMemory
                        [(0xfffU & ((IData)(3U) + vlTOPp->processor__DOT__aluOut))];
                    __Vdlyvdim0__processor__DOT__mainMemory__v11 
                        = (0xfffU & ((IData)(3U) + vlTOPp->processor__DOT__aluOut));
                }
            }
        }
    }
    if (__Vdlyvset__processor__DOT__registers__v0) {
        vlTOPp->processor__DOT__registers[__Vdlyvdim0__processor__DOT__registers__v0] 
            = __Vdlyvval__processor__DOT__registers__v0;
    }
    vlTOPp->processor__DOT__pc = __Vdly__processor__DOT__pc;
    vlTOPp->processor__DOT__mainMemory[__Vdlyvdim0__processor__DOT__mainMemory__v0] 
        = __Vdlyvval__processor__DOT__mainMemory__v0;
    if (__Vdlyvset__processor__DOT__mainMemory__v1) {
        vlTOPp->processor__DOT__mainMemory[__Vdlyvdim0__processor__DOT__mainMemory__v1] 
            = __Vdlyvval__processor__DOT__mainMemory__v1;
    }
    if (__Vdlyvset__processor__DOT__mainMemory__v2) {
        vlTOPp->processor__DOT__mainMemory[__Vdlyvdim0__processor__DOT__mainMemory__v2] 
            = __Vdlyvval__processor__DOT__mainMemory__v2;
        vlTOPp->processor__DOT__mainMemory[__Vdlyvdim0__processor__DOT__mainMemory__v3] 
            = __Vdlyvval__processor__DOT__mainMemory__v3;
    }
    if (__Vdlyvset__processor__DOT__mainMemory__v4) {
        vlTOPp->processor__DOT__mainMemory[__Vdlyvdim0__processor__DOT__mainMemory__v4] 
            = __Vdlyvval__processor__DOT__mainMemory__v4;
        vlTOPp->processor__DOT__mainMemory[__Vdlyvdim0__processor__DOT__mainMemory__v5] 
            = __Vdlyvval__processor__DOT__mainMemory__v5;
        vlTOPp->processor__DOT__mainMemory[__Vdlyvdim0__processor__DOT__mainMemory__v6] 
            = __Vdlyvval__processor__DOT__mainMemory__v6;
        vlTOPp->processor__DOT__mainMemory[__Vdlyvdim0__processor__DOT__mainMemory__v7] 
            = __Vdlyvval__processor__DOT__mainMemory__v7;
    }
    if (__Vdlyvset__processor__DOT__mainMemory__v8) {
        vlTOPp->processor__DOT__mainMemory[__Vdlyvdim0__processor__DOT__mainMemory__v8] 
            = __Vdlyvval__processor__DOT__mainMemory__v8;
        vlTOPp->processor__DOT__mainMemory[__Vdlyvdim0__processor__DOT__mainMemory__v9] 
            = __Vdlyvval__processor__DOT__mainMemory__v9;
        vlTOPp->processor__DOT__mainMemory[__Vdlyvdim0__processor__DOT__mainMemory__v10] 
            = __Vdlyvval__processor__DOT__mainMemory__v10;
        vlTOPp->processor__DOT__mainMemory[__Vdlyvdim0__processor__DOT__mainMemory__v11] 
            = __Vdlyvval__processor__DOT__mainMemory__v11;
    }
    vlTOPp->gp = vlTOPp->processor__DOT__registers[3U];
    vlTOPp->a7 = vlTOPp->processor__DOT__registers[0x11U];
    vlTOPp->a0 = vlTOPp->processor__DOT__registers[0xaU];
}

VL_INLINE_OPT void Vprocessor::_combo__TOP__3(Vprocessor__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vprocessor::_combo__TOP__3\n"); );
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->processor__DOT__ins = ((IData)(vlTOPp->memEn)
                                    ? 0x13U : ((vlTOPp->processor__DOT__mainMemory
                                                [(0xfffU 
                                                  & ((IData)(3U) 
                                                     + vlTOPp->processor__DOT__pc))] 
                                                << 0x18U) 
                                               | ((vlTOPp->processor__DOT__mainMemory
                                                   [
                                                   (0xfffU 
                                                    & ((IData)(2U) 
                                                       + vlTOPp->processor__DOT__pc))] 
                                                   << 0x10U) 
                                                  | ((vlTOPp->processor__DOT__mainMemory
                                                      [
                                                      (0xfffU 
                                                       & ((IData)(1U) 
                                                          + vlTOPp->processor__DOT__pc))] 
                                                      << 8U) 
                                                     | vlTOPp->processor__DOT__mainMemory
                                                     [
                                                     (0xfffU 
                                                      & vlTOPp->processor__DOT__pc)]))));
    vlTOPp->processor__DOT__data_1 = ((0U == (0x1fU 
                                              & (vlTOPp->processor__DOT__ins 
                                                 >> 0xfU)))
                                       ? 0U : vlTOPp->processor__DOT__registers
                                      [(0x1fU & (vlTOPp->processor__DOT__ins 
                                                 >> 0xfU))]);
    vlTOPp->processor__DOT__data_2 = ((0U == (0x1fU 
                                              & (vlTOPp->processor__DOT__ins 
                                                 >> 0x14U)))
                                       ? 0U : vlTOPp->processor__DOT__registers
                                      [(0x1fU & (vlTOPp->processor__DOT__ins 
                                                 >> 0x14U))]);
    vlTOPp->processor__DOT__funct7 = (0x7fU & (vlTOPp->processor__DOT__ins 
                                               >> 0x19U));
    vlTOPp->processor__DOT__opcode = (0x1fU & (vlTOPp->processor__DOT__ins 
                                               >> 2U));
    vlTOPp->processor__DOT__isArithmetic = ((0xcU == (IData)(vlTOPp->processor__DOT__opcode)) 
                                            & (~ (IData)(vlTOPp->processor__DOT__funct7)));
    vlTOPp->processor__DOT__isMUL = ((0xcU == (IData)(vlTOPp->processor__DOT__opcode)) 
                                     & (IData)(vlTOPp->processor__DOT__funct7));
    vlTOPp->processor__DOT__isImm = (4U == (IData)(vlTOPp->processor__DOT__opcode));
    vlTOPp->processor__DOT__isMemRead = (0U == (IData)(vlTOPp->processor__DOT__opcode));
    vlTOPp->processor__DOT__isLoadUI = (0xdU == (IData)(vlTOPp->processor__DOT__opcode));
    vlTOPp->processor__DOT__isMemWrite = (8U == (IData)(vlTOPp->processor__DOT__opcode));
    vlTOPp->processor__DOT__isBranch = (0x18U == (IData)(vlTOPp->processor__DOT__opcode));
    vlTOPp->processor__DOT__isJAL = (0x1bU == (IData)(vlTOPp->processor__DOT__opcode));
    vlTOPp->processor__DOT__isJALR = (0x19U == (IData)(vlTOPp->processor__DOT__opcode));
    vlTOPp->processor__DOT__isAUIPC = (5U == (IData)(vlTOPp->processor__DOT__opcode));
    vlTOPp->processor__DOT__imm = ((((IData)(vlTOPp->processor__DOT__isImm) 
                                     | (IData)(vlTOPp->processor__DOT__isMemRead)) 
                                    | (IData)(vlTOPp->processor__DOT__isJALR))
                                    ? VL_EXTENDS_II(32,12, 
                                                    (0xfffU 
                                                     & (vlTOPp->processor__DOT__ins 
                                                        >> 0x14U)))
                                    : (((IData)(vlTOPp->processor__DOT__isLoadUI) 
                                        | (IData)(vlTOPp->processor__DOT__isAUIPC))
                                        ? (0xfffff000U 
                                           & vlTOPp->processor__DOT__ins)
                                        : ((IData)(vlTOPp->processor__DOT__isMemWrite)
                                            ? VL_EXTENDS_II(32,12, 
                                                            ((0xfe0U 
                                                              & (vlTOPp->processor__DOT__ins 
                                                                 >> 0x14U)) 
                                                             | (0x1fU 
                                                                & (vlTOPp->processor__DOT__ins 
                                                                   >> 7U))))
                                            : ((IData)(vlTOPp->processor__DOT__isBranch)
                                                ? VL_EXTENDS_II(32,13, 
                                                                ((0x1000U 
                                                                  & (vlTOPp->processor__DOT__ins 
                                                                     >> 0x13U)) 
                                                                 | ((0x800U 
                                                                     & (vlTOPp->processor__DOT__ins 
                                                                        << 4U)) 
                                                                    | ((0x7e0U 
                                                                        & (vlTOPp->processor__DOT__ins 
                                                                           >> 0x14U)) 
                                                                       | (0x1eU 
                                                                          & (vlTOPp->processor__DOT__ins 
                                                                             >> 7U))))))
                                                : ((IData)(vlTOPp->processor__DOT__isJAL)
                                                    ? 
                                                   VL_EXTENDS_II(32,21, 
                                                                 ((0x100000U 
                                                                   & (vlTOPp->processor__DOT__ins 
                                                                      >> 0xbU)) 
                                                                  | ((0xff000U 
                                                                      & vlTOPp->processor__DOT__ins) 
                                                                     | ((0x800U 
                                                                         & (vlTOPp->processor__DOT__ins 
                                                                            >> 9U)) 
                                                                        | (0x7feU 
                                                                           & (vlTOPp->processor__DOT__ins 
                                                                              >> 0x14U))))))
                                                    : 0U)))));
    vlTOPp->processor__DOT__memWrite = ((0U == (7U 
                                                & (vlTOPp->processor__DOT__ins 
                                                   >> 0xcU)))
                                         ? (0xffU & vlTOPp->processor__DOT__data_2)
                                         : ((1U == 
                                             (7U & 
                                              (vlTOPp->processor__DOT__ins 
                                               >> 0xcU)))
                                             ? (0xffffU 
                                                & vlTOPp->processor__DOT__data_2)
                                             : ((2U 
                                                 == 
                                                 (7U 
                                                  & (vlTOPp->processor__DOT__ins 
                                                     >> 0xcU)))
                                                 ? vlTOPp->processor__DOT__data_2
                                                 : 0U)));
    vlTOPp->processor__DOT__isBranchC = (1U & ((0U 
                                                == 
                                                (3U 
                                                 & (vlTOPp->processor__DOT__ins 
                                                    >> 0xdU)))
                                                ? (
                                                   (vlTOPp->processor__DOT__ins 
                                                    >> 0xcU) 
                                                   ^ 
                                                   (vlTOPp->processor__DOT__data_1 
                                                    == vlTOPp->processor__DOT__data_2))
                                                : (
                                                   (2U 
                                                    == 
                                                    (3U 
                                                     & (vlTOPp->processor__DOT__ins 
                                                        >> 0xdU)))
                                                    ? 
                                                   ((vlTOPp->processor__DOT__ins 
                                                     >> 0xcU) 
                                                    ^ 
                                                    VL_LTS_III(1,32,32, vlTOPp->processor__DOT__data_1, vlTOPp->processor__DOT__data_2))
                                                    : 
                                                   ((3U 
                                                     == 
                                                     (3U 
                                                      & (vlTOPp->processor__DOT__ins 
                                                         >> 0xdU))) 
                                                    & ((vlTOPp->processor__DOT__ins 
                                                        >> 0xcU) 
                                                       ^ 
                                                       (vlTOPp->processor__DOT__data_1 
                                                        < vlTOPp->processor__DOT__data_2))))));
    vlTOPp->processor__DOT__isBranchR = ((IData)(vlTOPp->processor__DOT__isBranchC) 
                                         & (IData)(vlTOPp->processor__DOT__isBranch));
    vlTOPp->processor__DOT__aluOp = ((IData)(vlTOPp->processor__DOT__isMUL)
                                      ? ((0x4000U & vlTOPp->processor__DOT__ins)
                                          ? 0xaU : 9U)
                                      : ((IData)(vlTOPp->processor__DOT__isArithmetic)
                                          ? ((8U & 
                                              ((IData)(vlTOPp->processor__DOT__funct7) 
                                               >> 2U)) 
                                             | (7U 
                                                & (vlTOPp->processor__DOT__ins 
                                                   >> 0xcU)))
                                          : ((IData)(vlTOPp->processor__DOT__isImm)
                                              ? ((0x3ffffff8U 
                                                  & (((IData)(vlTOPp->processor__DOT__funct7) 
                                                      >> 2U) 
                                                     & ((5U 
                                                         == 
                                                         (7U 
                                                          & (vlTOPp->processor__DOT__ins 
                                                             >> 0xcU))) 
                                                        << 3U))) 
                                                 | (7U 
                                                    & (vlTOPp->processor__DOT__ins 
                                                       >> 0xcU)))
                                              : (((
                                                   ((((IData)(vlTOPp->processor__DOT__isAUIPC) 
                                                      | (IData)(vlTOPp->processor__DOT__isJAL)) 
                                                     | (IData)(vlTOPp->processor__DOT__isJALR)) 
                                                    | (IData)(vlTOPp->processor__DOT__isBranch)) 
                                                   | (IData)(vlTOPp->processor__DOT__isMemRead)) 
                                                  | (IData)(vlTOPp->processor__DOT__isMemWrite))
                                                  ? 0U
                                                  : 0xfU))));
    vlTOPp->processor__DOT__src1 = ((((IData)(vlTOPp->processor__DOT__isJAL) 
                                      | (IData)(vlTOPp->processor__DOT__isBranch)) 
                                     | (IData)(vlTOPp->processor__DOT__isAUIPC))
                                     ? vlTOPp->processor__DOT__pc
                                     : vlTOPp->processor__DOT__data_1);
    vlTOPp->processor__DOT__src2 = (((((((((IData)(vlTOPp->processor__DOT__isImm) 
                                           | (IData)(vlTOPp->processor__DOT__isMemRead)) 
                                          | (IData)(vlTOPp->processor__DOT__isLoadUI)) 
                                         | (IData)(vlTOPp->processor__DOT__isJAL)) 
                                        | (IData)(vlTOPp->processor__DOT__isJALR)) 
                                       | (IData)(vlTOPp->processor__DOT__isMemWrite)) 
                                      | (IData)(vlTOPp->processor__DOT__isBranch)) 
                                     | (IData)(vlTOPp->processor__DOT__isAUIPC))
                                     ? vlTOPp->processor__DOT__imm
                                     : vlTOPp->processor__DOT__data_2);
    vlTOPp->processor__DOT__aluOut = ((8U & (IData)(vlTOPp->processor__DOT__aluOp))
                                       ? ((4U & (IData)(vlTOPp->processor__DOT__aluOp))
                                           ? ((2U & (IData)(vlTOPp->processor__DOT__aluOp))
                                               ? ((1U 
                                                   & (IData)(vlTOPp->processor__DOT__aluOp))
                                                   ? vlTOPp->processor__DOT__src2
                                                   : 0U)
                                               : ((1U 
                                                   & (IData)(vlTOPp->processor__DOT__aluOp))
                                                   ? 
                                                  VL_SHIFTRS_III(32,32,5, vlTOPp->processor__DOT__src1, 
                                                                 (0x1fU 
                                                                  & vlTOPp->processor__DOT__src2))
                                                   : 0U))
                                           : ((2U & (IData)(vlTOPp->processor__DOT__aluOp))
                                               ? ((1U 
                                                   & (IData)(vlTOPp->processor__DOT__aluOp))
                                                   ? 0U
                                                   : 
                                                  VL_DIV_III(32, vlTOPp->processor__DOT__src1, vlTOPp->processor__DOT__src2))
                                               : ((1U 
                                                   & (IData)(vlTOPp->processor__DOT__aluOp))
                                                   ? 
                                                  (vlTOPp->processor__DOT__src1 
                                                   * vlTOPp->processor__DOT__src2)
                                                   : 
                                                  (vlTOPp->processor__DOT__src1 
                                                   - vlTOPp->processor__DOT__src2))))
                                       : ((4U & (IData)(vlTOPp->processor__DOT__aluOp))
                                           ? ((2U & (IData)(vlTOPp->processor__DOT__aluOp))
                                               ? ((1U 
                                                   & (IData)(vlTOPp->processor__DOT__aluOp))
                                                   ? 
                                                  (vlTOPp->processor__DOT__src1 
                                                   & vlTOPp->processor__DOT__src2)
                                                   : 
                                                  (vlTOPp->processor__DOT__src1 
                                                   | vlTOPp->processor__DOT__src2))
                                               : ((1U 
                                                   & (IData)(vlTOPp->processor__DOT__aluOp))
                                                   ? 
                                                  (vlTOPp->processor__DOT__src1 
                                                   >> 
                                                   (0x1fU 
                                                    & vlTOPp->processor__DOT__src2))
                                                   : 
                                                  (vlTOPp->processor__DOT__src1 
                                                   ^ vlTOPp->processor__DOT__src2)))
                                           : ((2U & (IData)(vlTOPp->processor__DOT__aluOp))
                                               ? ((1U 
                                                   & (IData)(vlTOPp->processor__DOT__aluOp))
                                                   ? 
                                                  (vlTOPp->processor__DOT__src1 
                                                   < vlTOPp->processor__DOT__src2)
                                                   : 
                                                  VL_LTS_III(32,32,32, vlTOPp->processor__DOT__src1, vlTOPp->processor__DOT__src2))
                                               : ((1U 
                                                   & (IData)(vlTOPp->processor__DOT__aluOp))
                                                   ? 
                                                  (vlTOPp->processor__DOT__src1 
                                                   << 
                                                   (0x1fU 
                                                    & vlTOPp->processor__DOT__src2))
                                                   : 
                                                  (vlTOPp->processor__DOT__src1 
                                                   + vlTOPp->processor__DOT__src2)))));
    vlTOPp->processor__DOT__memRead = ((0U == (3U & 
                                               (vlTOPp->processor__DOT__ins 
                                                >> 0xcU)))
                                        ? ((0x4000U 
                                            & vlTOPp->processor__DOT__ins)
                                            ? ((0xffffff00U 
                                                & ((- (IData)(
                                                              (1U 
                                                               & (vlTOPp->processor__DOT__mainMemory
                                                                  [
                                                                  (0xfffU 
                                                                   & vlTOPp->processor__DOT__aluOut)] 
                                                                  >> 7U)))) 
                                                   << 8U)) 
                                               | vlTOPp->processor__DOT__mainMemory
                                               [(0xfffU 
                                                 & vlTOPp->processor__DOT__aluOut)])
                                            : vlTOPp->processor__DOT__mainMemory
                                           [(0xfffU 
                                             & vlTOPp->processor__DOT__aluOut)])
                                        : ((1U == (3U 
                                                   & (vlTOPp->processor__DOT__ins 
                                                      >> 0xcU)))
                                            ? (IData)(
                                                      ((0x4000U 
                                                        & vlTOPp->processor__DOT__ins)
                                                        ? 
                                                       (((QData)((IData)(
                                                                         ((0xffffff00U 
                                                                           & ((- (IData)(
                                                                                (1U 
                                                                                & (vlTOPp->processor__DOT__mainMemory
                                                                                [
                                                                                (0xfffU 
                                                                                & ((IData)(1U) 
                                                                                + vlTOPp->processor__DOT__aluOut))] 
                                                                                >> 7U)))) 
                                                                              << 8U)) 
                                                                          | vlTOPp->processor__DOT__mainMemory
                                                                          [
                                                                          (0xfffU 
                                                                           & ((IData)(1U) 
                                                                              + vlTOPp->processor__DOT__aluOut))]))) 
                                                         << 8U) 
                                                        | (QData)((IData)(
                                                                          vlTOPp->processor__DOT__mainMemory
                                                                          [
                                                                          (0xfffU 
                                                                           & vlTOPp->processor__DOT__aluOut)])))
                                                        : 
                                                       (((QData)((IData)(
                                                                         vlTOPp->processor__DOT__mainMemory
                                                                         [
                                                                         (0xfffU 
                                                                          & ((IData)(1U) 
                                                                             + vlTOPp->processor__DOT__aluOut))])) 
                                                         << 8U) 
                                                        | (QData)((IData)(
                                                                          vlTOPp->processor__DOT__mainMemory
                                                                          [
                                                                          (0xfffU 
                                                                           & vlTOPp->processor__DOT__aluOut)])))))
                                            : ((2U 
                                                == 
                                                (3U 
                                                 & (vlTOPp->processor__DOT__ins 
                                                    >> 0xcU)))
                                                ? (
                                                   (vlTOPp->processor__DOT__mainMemory
                                                    [
                                                    (0xfffU 
                                                     & ((IData)(3U) 
                                                        + vlTOPp->processor__DOT__aluOut))] 
                                                    << 0x18U) 
                                                   | ((vlTOPp->processor__DOT__mainMemory
                                                       [
                                                       (0xfffU 
                                                        & ((IData)(2U) 
                                                           + vlTOPp->processor__DOT__aluOut))] 
                                                       << 0x10U) 
                                                      | ((vlTOPp->processor__DOT__mainMemory
                                                          [
                                                          (0xfffU 
                                                           & ((IData)(1U) 
                                                              + vlTOPp->processor__DOT__aluOut))] 
                                                          << 8U) 
                                                         | vlTOPp->processor__DOT__mainMemory
                                                         [
                                                         (0xfffU 
                                                          & vlTOPp->processor__DOT__aluOut)])))
                                                : 0U)));
}

void Vprocessor::_eval(Vprocessor__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vprocessor::_eval\n"); );
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->clock) & (~ (IData)(vlTOPp->__Vclklast__TOP__clock)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
        vlTOPp->__Vm_traceActivity[1U] = 1U;
    }
    vlTOPp->_combo__TOP__3(vlSymsp);
    vlTOPp->__Vm_traceActivity[2U] = 1U;
    // Final
    vlTOPp->__Vclklast__TOP__clock = vlTOPp->clock;
}

VL_INLINE_OPT QData Vprocessor::_change_request(Vprocessor__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vprocessor::_change_request\n"); );
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vprocessor::_change_request_1(Vprocessor__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vprocessor::_change_request_1\n"); );
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vprocessor::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vprocessor::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clock & 0xfeU))) {
        Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((reset & 0xfeU))) {
        Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((memEn & 0xfeU))) {
        Verilated::overWidthError("memEn");}
}
#endif  // VL_DEBUG
