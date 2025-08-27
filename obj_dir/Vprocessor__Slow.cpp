// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vprocessor.h for the primary calling header

#include "Vprocessor.h"
#include "Vprocessor__Syms.h"

//==========

VL_CTOR_IMP(Vprocessor) {
    Vprocessor__Syms* __restrict vlSymsp = __VlSymsp = new Vprocessor__Syms(this, name());
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vprocessor::__Vconfigure(Vprocessor__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vprocessor::~Vprocessor() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = nullptr);
}

void Vprocessor::_settle__TOP__2(Vprocessor__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vprocessor::_settle__TOP__2\n"); );
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->gp = vlTOPp->processor__DOT__registers[3U];
    vlTOPp->a7 = vlTOPp->processor__DOT__registers[0x11U];
    vlTOPp->a0 = vlTOPp->processor__DOT__registers[0xaU];
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

void Vprocessor::_eval_initial(Vprocessor__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vprocessor::_eval_initial\n"); );
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clock = vlTOPp->clock;
}

void Vprocessor::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vprocessor::final\n"); );
    // Variables
    Vprocessor__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vprocessor::_eval_settle(Vprocessor__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vprocessor::_eval_settle\n"); );
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
    vlTOPp->__Vm_traceActivity[2U] = 1U;
    vlTOPp->__Vm_traceActivity[1U] = 1U;
    vlTOPp->__Vm_traceActivity[0U] = 1U;
}

void Vprocessor::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vprocessor::_ctor_var_reset\n"); );
    // Body
    clock = VL_RAND_RESET_I(1);
    reset = VL_RAND_RESET_I(1);
    memEn = VL_RAND_RESET_I(1);
    memData = VL_RAND_RESET_I(32);
    memAddr = VL_RAND_RESET_I(32);
    gp = VL_RAND_RESET_I(32);
    a7 = VL_RAND_RESET_I(32);
    a0 = VL_RAND_RESET_I(32);
    for (int __Vi0=0; __Vi0<4096; ++__Vi0) {
        processor__DOT__mainMemory[__Vi0] = VL_RAND_RESET_I(8);
    }
    for (int __Vi0=0; __Vi0<32; ++__Vi0) {
        processor__DOT__registers[__Vi0] = VL_RAND_RESET_I(32);
    }
    processor__DOT__aluOp = VL_RAND_RESET_I(4);
    processor__DOT__opcode = VL_RAND_RESET_I(5);
    processor__DOT__funct7 = VL_RAND_RESET_I(7);
    processor__DOT__ins = VL_RAND_RESET_I(32);
    processor__DOT__imm = VL_RAND_RESET_I(32);
    processor__DOT__pc = VL_RAND_RESET_I(32);
    processor__DOT__data_1 = VL_RAND_RESET_I(32);
    processor__DOT__data_2 = VL_RAND_RESET_I(32);
    processor__DOT__src1 = VL_RAND_RESET_I(32);
    processor__DOT__src2 = VL_RAND_RESET_I(32);
    processor__DOT__aluOut = VL_RAND_RESET_I(32);
    processor__DOT__memRead = VL_RAND_RESET_I(32);
    processor__DOT__memWrite = VL_RAND_RESET_I(32);
    processor__DOT__isArithmetic = VL_RAND_RESET_I(1);
    processor__DOT__isImm = VL_RAND_RESET_I(1);
    processor__DOT__isMemRead = VL_RAND_RESET_I(1);
    processor__DOT__isLoadUI = VL_RAND_RESET_I(1);
    processor__DOT__isMemWrite = VL_RAND_RESET_I(1);
    processor__DOT__isBranch = VL_RAND_RESET_I(1);
    processor__DOT__isJAL = VL_RAND_RESET_I(1);
    processor__DOT__isJALR = VL_RAND_RESET_I(1);
    processor__DOT__isMUL = VL_RAND_RESET_I(1);
    processor__DOT__isAUIPC = VL_RAND_RESET_I(1);
    processor__DOT__isBranchR = VL_RAND_RESET_I(1);
    processor__DOT__isBranchC = VL_RAND_RESET_I(1);
    for (int __Vi0=0; __Vi0<3; ++__Vi0) {
        __Vm_traceActivity[__Vi0] = VL_RAND_RESET_I(1);
    }
}
