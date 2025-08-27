// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vprocessor__Syms.h"


void Vprocessor::traceChgTop0(void* userp, VerilatedVcd* tracep) {
    Vprocessor__Syms* __restrict vlSymsp = static_cast<Vprocessor__Syms*>(userp);
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    {
        vlTOPp->traceChgSub0(userp, tracep);
    }
}

void Vprocessor::traceChgSub0(void* userp, VerilatedVcd* tracep) {
    Vprocessor__Syms* __restrict vlSymsp = static_cast<Vprocessor__Syms*>(userp);
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    vluint32_t* const oldp = tracep->oldp(vlSymsp->__Vm_baseCode + 1);
    if (false && oldp) {}  // Prevent unused
    // Body
    {
        if (VL_UNLIKELY(vlTOPp->__Vm_traceActivity[1U])) {
            tracep->chgIData(oldp+0,(vlTOPp->processor__DOT__registers[0]),32);
            tracep->chgIData(oldp+1,(vlTOPp->processor__DOT__registers[1]),32);
            tracep->chgIData(oldp+2,(vlTOPp->processor__DOT__registers[2]),32);
            tracep->chgIData(oldp+3,(vlTOPp->processor__DOT__registers[3]),32);
            tracep->chgIData(oldp+4,(vlTOPp->processor__DOT__registers[4]),32);
            tracep->chgIData(oldp+5,(vlTOPp->processor__DOT__registers[5]),32);
            tracep->chgIData(oldp+6,(vlTOPp->processor__DOT__registers[6]),32);
            tracep->chgIData(oldp+7,(vlTOPp->processor__DOT__registers[7]),32);
            tracep->chgIData(oldp+8,(vlTOPp->processor__DOT__registers[8]),32);
            tracep->chgIData(oldp+9,(vlTOPp->processor__DOT__registers[9]),32);
            tracep->chgIData(oldp+10,(vlTOPp->processor__DOT__registers[10]),32);
            tracep->chgIData(oldp+11,(vlTOPp->processor__DOT__registers[11]),32);
            tracep->chgIData(oldp+12,(vlTOPp->processor__DOT__registers[12]),32);
            tracep->chgIData(oldp+13,(vlTOPp->processor__DOT__registers[13]),32);
            tracep->chgIData(oldp+14,(vlTOPp->processor__DOT__registers[14]),32);
            tracep->chgIData(oldp+15,(vlTOPp->processor__DOT__registers[15]),32);
            tracep->chgIData(oldp+16,(vlTOPp->processor__DOT__registers[16]),32);
            tracep->chgIData(oldp+17,(vlTOPp->processor__DOT__registers[17]),32);
            tracep->chgIData(oldp+18,(vlTOPp->processor__DOT__registers[18]),32);
            tracep->chgIData(oldp+19,(vlTOPp->processor__DOT__registers[19]),32);
            tracep->chgIData(oldp+20,(vlTOPp->processor__DOT__registers[20]),32);
            tracep->chgIData(oldp+21,(vlTOPp->processor__DOT__registers[21]),32);
            tracep->chgIData(oldp+22,(vlTOPp->processor__DOT__registers[22]),32);
            tracep->chgIData(oldp+23,(vlTOPp->processor__DOT__registers[23]),32);
            tracep->chgIData(oldp+24,(vlTOPp->processor__DOT__registers[24]),32);
            tracep->chgIData(oldp+25,(vlTOPp->processor__DOT__registers[25]),32);
            tracep->chgIData(oldp+26,(vlTOPp->processor__DOT__registers[26]),32);
            tracep->chgIData(oldp+27,(vlTOPp->processor__DOT__registers[27]),32);
            tracep->chgIData(oldp+28,(vlTOPp->processor__DOT__registers[28]),32);
            tracep->chgIData(oldp+29,(vlTOPp->processor__DOT__registers[29]),32);
            tracep->chgIData(oldp+30,(vlTOPp->processor__DOT__registers[30]),32);
            tracep->chgIData(oldp+31,(vlTOPp->processor__DOT__registers[31]),32);
            tracep->chgIData(oldp+32,(vlTOPp->processor__DOT__pc),32);
        }
        if (VL_UNLIKELY(vlTOPp->__Vm_traceActivity[2U])) {
            tracep->chgCData(oldp+33,(vlTOPp->processor__DOT__aluOp),4);
            tracep->chgCData(oldp+34,((0x1fU & (vlTOPp->processor__DOT__ins 
                                                >> 0xfU))),5);
            tracep->chgCData(oldp+35,((0x1fU & (vlTOPp->processor__DOT__ins 
                                                >> 0x14U))),5);
            tracep->chgCData(oldp+36,((0x1fU & (vlTOPp->processor__DOT__ins 
                                                >> 7U))),5);
            tracep->chgCData(oldp+37,(vlTOPp->processor__DOT__opcode),5);
            tracep->chgCData(oldp+38,((7U & (vlTOPp->processor__DOT__ins 
                                             >> 0xcU))),3);
            tracep->chgCData(oldp+39,(vlTOPp->processor__DOT__funct7),7);
            tracep->chgIData(oldp+40,(vlTOPp->processor__DOT__ins),32);
            tracep->chgIData(oldp+41,(vlTOPp->processor__DOT__imm),32);
            tracep->chgIData(oldp+42,(vlTOPp->processor__DOT__data_1),32);
            tracep->chgIData(oldp+43,(vlTOPp->processor__DOT__data_2),32);
            tracep->chgIData(oldp+44,(vlTOPp->processor__DOT__src1),32);
            tracep->chgIData(oldp+45,(vlTOPp->processor__DOT__src2),32);
            tracep->chgIData(oldp+46,(vlTOPp->processor__DOT__aluOut),32);
            tracep->chgIData(oldp+47,(vlTOPp->processor__DOT__memRead),32);
            tracep->chgIData(oldp+48,(vlTOPp->processor__DOT__memWrite),32);
            tracep->chgBit(oldp+49,(vlTOPp->processor__DOT__isArithmetic));
            tracep->chgBit(oldp+50,(vlTOPp->processor__DOT__isImm));
            tracep->chgBit(oldp+51,(vlTOPp->processor__DOT__isMemRead));
            tracep->chgBit(oldp+52,(vlTOPp->processor__DOT__isLoadUI));
            tracep->chgBit(oldp+53,(vlTOPp->processor__DOT__isMemWrite));
            tracep->chgBit(oldp+54,(vlTOPp->processor__DOT__isBranch));
            tracep->chgBit(oldp+55,(vlTOPp->processor__DOT__isJAL));
            tracep->chgBit(oldp+56,(vlTOPp->processor__DOT__isJALR));
            tracep->chgBit(oldp+57,(vlTOPp->processor__DOT__isMUL));
            tracep->chgBit(oldp+58,(vlTOPp->processor__DOT__isAUIPC));
            tracep->chgBit(oldp+59,(vlTOPp->processor__DOT__isBranchR));
            tracep->chgBit(oldp+60,(vlTOPp->processor__DOT__isBranchC));
            tracep->chgBit(oldp+61,((((((((IData)(vlTOPp->processor__DOT__isArithmetic) 
                                          | (IData)(vlTOPp->processor__DOT__isImm)) 
                                         | (IData)(vlTOPp->processor__DOT__isMemRead)) 
                                        | (IData)(vlTOPp->processor__DOT__isLoadUI)) 
                                       | (IData)(vlTOPp->processor__DOT__isJAL)) 
                                      | (IData)(vlTOPp->processor__DOT__isJALR)) 
                                     | (IData)(vlTOPp->processor__DOT__isAUIPC))));
        }
        tracep->chgBit(oldp+62,(vlTOPp->clock));
        tracep->chgBit(oldp+63,(vlTOPp->reset));
        tracep->chgBit(oldp+64,(vlTOPp->memEn));
        tracep->chgIData(oldp+65,(vlTOPp->memData),32);
        tracep->chgIData(oldp+66,(vlTOPp->memAddr),32);
        tracep->chgIData(oldp+67,(vlTOPp->gp),32);
        tracep->chgIData(oldp+68,(vlTOPp->a7),32);
        tracep->chgIData(oldp+69,(vlTOPp->a0),32);
        tracep->chgIData(oldp+70,((((IData)(vlTOPp->processor__DOT__isJALR) 
                                    | (IData)(vlTOPp->processor__DOT__isJAL))
                                    ? ((IData)(4U) 
                                       + vlTOPp->processor__DOT__pc)
                                    : ((IData)(vlTOPp->processor__DOT__isMemRead)
                                        ? vlTOPp->processor__DOT__memRead
                                        : vlTOPp->processor__DOT__aluOut))),32);
    }
}

void Vprocessor::traceCleanup(void* userp, VerilatedVcd* /*unused*/) {
    Vprocessor__Syms* __restrict vlSymsp = static_cast<Vprocessor__Syms*>(userp);
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlSymsp->__Vm_activity = false;
        vlTOPp->__Vm_traceActivity[0U] = 0U;
        vlTOPp->__Vm_traceActivity[1U] = 0U;
        vlTOPp->__Vm_traceActivity[2U] = 0U;
    }
}
