// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vprocessor__Syms.h"


//======================

void Vprocessor::trace(VerilatedVcdC* tfp, int, int) {
    tfp->spTrace()->addInitCb(&traceInit, __VlSymsp);
    traceRegister(tfp->spTrace());
}

void Vprocessor::traceInit(void* userp, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vprocessor__Syms* __restrict vlSymsp = static_cast<Vprocessor__Syms*>(userp);
    if (!Verilated::calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
                        "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->module(vlSymsp->name());
    tracep->scopeEscape(' ');
    Vprocessor::traceInitTop(vlSymsp, tracep);
    tracep->scopeEscape('.');
}

//======================


void Vprocessor::traceInitTop(void* userp, VerilatedVcd* tracep) {
    Vprocessor__Syms* __restrict vlSymsp = static_cast<Vprocessor__Syms*>(userp);
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlTOPp->traceInitSub0(userp, tracep);
    }
}

void Vprocessor::traceInitSub0(void* userp, VerilatedVcd* tracep) {
    Vprocessor__Syms* __restrict vlSymsp = static_cast<Vprocessor__Syms*>(userp);
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    const int c = vlSymsp->__Vm_baseCode;
    if (false && tracep && c) {}  // Prevent unused
    // Body
    {
        tracep->declBit(c+63,"clock", false,-1);
        tracep->declBit(c+64,"reset", false,-1);
        tracep->declBit(c+65,"memEn", false,-1);
        tracep->declBus(c+66,"memData", false,-1, 31,0);
        tracep->declBus(c+67,"memAddr", false,-1, 31,0);
        tracep->declBus(c+68,"gp", false,-1, 31,0);
        tracep->declBus(c+69,"a7", false,-1, 31,0);
        tracep->declBus(c+70,"a0", false,-1, 31,0);
        tracep->declBus(c+72,"processor WIDTH", false,-1, 31,0);
        tracep->declBus(c+72,"processor NUM_REGS", false,-1, 31,0);
        tracep->declBus(c+73,"processor DATA_WIDTH", false,-1, 31,0);
        tracep->declBus(c+74,"processor MEM_DEPTH", false,-1, 31,0);
        tracep->declBit(c+63,"processor clock", false,-1);
        tracep->declBit(c+64,"processor reset", false,-1);
        tracep->declBit(c+65,"processor memEn", false,-1);
        tracep->declBus(c+66,"processor memData", false,-1, 31,0);
        tracep->declBus(c+67,"processor memAddr", false,-1, 31,0);
        tracep->declBus(c+68,"processor gp", false,-1, 31,0);
        tracep->declBus(c+69,"processor a7", false,-1, 31,0);
        tracep->declBus(c+70,"processor a0", false,-1, 31,0);
        {int i; for (i=0; i<32; i++) {
                tracep->declBus(c+1+i*1,"processor registers", true,(i+0), 31,0);}}
        tracep->declBus(c+34,"processor aluOp", false,-1, 3,0);
        tracep->declBus(c+35,"processor rs1", false,-1, 4,0);
        tracep->declBus(c+36,"processor rs2", false,-1, 4,0);
        tracep->declBus(c+37,"processor rd", false,-1, 4,0);
        tracep->declBus(c+38,"processor opcode", false,-1, 4,0);
        tracep->declBus(c+39,"processor funct3", false,-1, 2,0);
        tracep->declBus(c+40,"processor funct7", false,-1, 6,0);
        tracep->declBus(c+41,"processor ins", false,-1, 31,0);
        tracep->declBus(c+42,"processor imm", false,-1, 31,0);
        tracep->declBus(c+33,"processor pc", false,-1, 31,0);
        tracep->declBus(c+43,"processor data_1", false,-1, 31,0);
        tracep->declBus(c+44,"processor data_2", false,-1, 31,0);
        tracep->declBus(c+71,"processor regDataIn", false,-1, 31,0);
        tracep->declBus(c+45,"processor src1", false,-1, 31,0);
        tracep->declBus(c+46,"processor src2", false,-1, 31,0);
        tracep->declBus(c+47,"processor aluOut", false,-1, 31,0);
        tracep->declBus(c+48,"processor memRead", false,-1, 31,0);
        tracep->declBus(c+49,"processor memWrite", false,-1, 31,0);
        tracep->declBit(c+50,"processor isArithmetic", false,-1);
        tracep->declBit(c+51,"processor isImm", false,-1);
        tracep->declBit(c+52,"processor isMemRead", false,-1);
        tracep->declBit(c+53,"processor isLoadUI", false,-1);
        tracep->declBit(c+54,"processor isMemWrite", false,-1);
        tracep->declBit(c+55,"processor isBranch", false,-1);
        tracep->declBit(c+56,"processor isJAL", false,-1);
        tracep->declBit(c+57,"processor isJALR", false,-1);
        tracep->declBit(c+58,"processor isMUL", false,-1);
        tracep->declBit(c+59,"processor isAUIPC", false,-1);
        tracep->declBit(c+60,"processor isBranchR", false,-1);
        tracep->declBit(c+61,"processor isBranchC", false,-1);
        tracep->declBit(c+62,"processor regWriteEn", false,-1);
        tracep->declBus(c+75,"processor ADD", false,-1, 3,0);
        tracep->declBus(c+76,"processor SLL", false,-1, 3,0);
        tracep->declBus(c+77,"processor SLT", false,-1, 3,0);
        tracep->declBus(c+78,"processor SLTU", false,-1, 3,0);
        tracep->declBus(c+79,"processor XOR", false,-1, 3,0);
        tracep->declBus(c+80,"processor SRL", false,-1, 3,0);
        tracep->declBus(c+81,"processor OR", false,-1, 3,0);
        tracep->declBus(c+82,"processor AND", false,-1, 3,0);
        tracep->declBus(c+83,"processor SUB", false,-1, 3,0);
        tracep->declBus(c+84,"processor MUL", false,-1, 3,0);
        tracep->declBus(c+85,"processor DIV", false,-1, 3,0);
        tracep->declBus(c+86,"processor SRA", false,-1, 3,0);
        tracep->declBus(c+87,"processor PASS", false,-1, 3,0);
    }
}

void Vprocessor::traceRegister(VerilatedVcd* tracep) {
    // Body
    {
        tracep->addFullCb(&traceFullTop0, __VlSymsp);
        tracep->addChgCb(&traceChgTop0, __VlSymsp);
        tracep->addCleanupCb(&traceCleanup, __VlSymsp);
    }
}

void Vprocessor::traceFullTop0(void* userp, VerilatedVcd* tracep) {
    Vprocessor__Syms* __restrict vlSymsp = static_cast<Vprocessor__Syms*>(userp);
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlTOPp->traceFullSub0(userp, tracep);
    }
}

void Vprocessor::traceFullSub0(void* userp, VerilatedVcd* tracep) {
    Vprocessor__Syms* __restrict vlSymsp = static_cast<Vprocessor__Syms*>(userp);
    Vprocessor* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    vluint32_t* const oldp = tracep->oldp(vlSymsp->__Vm_baseCode);
    if (false && oldp) {}  // Prevent unused
    // Body
    {
        tracep->fullIData(oldp+1,(vlTOPp->processor__DOT__registers[0]),32);
        tracep->fullIData(oldp+2,(vlTOPp->processor__DOT__registers[1]),32);
        tracep->fullIData(oldp+3,(vlTOPp->processor__DOT__registers[2]),32);
        tracep->fullIData(oldp+4,(vlTOPp->processor__DOT__registers[3]),32);
        tracep->fullIData(oldp+5,(vlTOPp->processor__DOT__registers[4]),32);
        tracep->fullIData(oldp+6,(vlTOPp->processor__DOT__registers[5]),32);
        tracep->fullIData(oldp+7,(vlTOPp->processor__DOT__registers[6]),32);
        tracep->fullIData(oldp+8,(vlTOPp->processor__DOT__registers[7]),32);
        tracep->fullIData(oldp+9,(vlTOPp->processor__DOT__registers[8]),32);
        tracep->fullIData(oldp+10,(vlTOPp->processor__DOT__registers[9]),32);
        tracep->fullIData(oldp+11,(vlTOPp->processor__DOT__registers[10]),32);
        tracep->fullIData(oldp+12,(vlTOPp->processor__DOT__registers[11]),32);
        tracep->fullIData(oldp+13,(vlTOPp->processor__DOT__registers[12]),32);
        tracep->fullIData(oldp+14,(vlTOPp->processor__DOT__registers[13]),32);
        tracep->fullIData(oldp+15,(vlTOPp->processor__DOT__registers[14]),32);
        tracep->fullIData(oldp+16,(vlTOPp->processor__DOT__registers[15]),32);
        tracep->fullIData(oldp+17,(vlTOPp->processor__DOT__registers[16]),32);
        tracep->fullIData(oldp+18,(vlTOPp->processor__DOT__registers[17]),32);
        tracep->fullIData(oldp+19,(vlTOPp->processor__DOT__registers[18]),32);
        tracep->fullIData(oldp+20,(vlTOPp->processor__DOT__registers[19]),32);
        tracep->fullIData(oldp+21,(vlTOPp->processor__DOT__registers[20]),32);
        tracep->fullIData(oldp+22,(vlTOPp->processor__DOT__registers[21]),32);
        tracep->fullIData(oldp+23,(vlTOPp->processor__DOT__registers[22]),32);
        tracep->fullIData(oldp+24,(vlTOPp->processor__DOT__registers[23]),32);
        tracep->fullIData(oldp+25,(vlTOPp->processor__DOT__registers[24]),32);
        tracep->fullIData(oldp+26,(vlTOPp->processor__DOT__registers[25]),32);
        tracep->fullIData(oldp+27,(vlTOPp->processor__DOT__registers[26]),32);
        tracep->fullIData(oldp+28,(vlTOPp->processor__DOT__registers[27]),32);
        tracep->fullIData(oldp+29,(vlTOPp->processor__DOT__registers[28]),32);
        tracep->fullIData(oldp+30,(vlTOPp->processor__DOT__registers[29]),32);
        tracep->fullIData(oldp+31,(vlTOPp->processor__DOT__registers[30]),32);
        tracep->fullIData(oldp+32,(vlTOPp->processor__DOT__registers[31]),32);
        tracep->fullIData(oldp+33,(vlTOPp->processor__DOT__pc),32);
        tracep->fullCData(oldp+34,(vlTOPp->processor__DOT__aluOp),4);
        tracep->fullCData(oldp+35,((0x1fU & (vlTOPp->processor__DOT__ins 
                                             >> 0xfU))),5);
        tracep->fullCData(oldp+36,((0x1fU & (vlTOPp->processor__DOT__ins 
                                             >> 0x14U))),5);
        tracep->fullCData(oldp+37,((0x1fU & (vlTOPp->processor__DOT__ins 
                                             >> 7U))),5);
        tracep->fullCData(oldp+38,(vlTOPp->processor__DOT__opcode),5);
        tracep->fullCData(oldp+39,((7U & (vlTOPp->processor__DOT__ins 
                                          >> 0xcU))),3);
        tracep->fullCData(oldp+40,(vlTOPp->processor__DOT__funct7),7);
        tracep->fullIData(oldp+41,(vlTOPp->processor__DOT__ins),32);
        tracep->fullIData(oldp+42,(vlTOPp->processor__DOT__imm),32);
        tracep->fullIData(oldp+43,(vlTOPp->processor__DOT__data_1),32);
        tracep->fullIData(oldp+44,(vlTOPp->processor__DOT__data_2),32);
        tracep->fullIData(oldp+45,(vlTOPp->processor__DOT__src1),32);
        tracep->fullIData(oldp+46,(vlTOPp->processor__DOT__src2),32);
        tracep->fullIData(oldp+47,(vlTOPp->processor__DOT__aluOut),32);
        tracep->fullIData(oldp+48,(vlTOPp->processor__DOT__memRead),32);
        tracep->fullIData(oldp+49,(vlTOPp->processor__DOT__memWrite),32);
        tracep->fullBit(oldp+50,(vlTOPp->processor__DOT__isArithmetic));
        tracep->fullBit(oldp+51,(vlTOPp->processor__DOT__isImm));
        tracep->fullBit(oldp+52,(vlTOPp->processor__DOT__isMemRead));
        tracep->fullBit(oldp+53,(vlTOPp->processor__DOT__isLoadUI));
        tracep->fullBit(oldp+54,(vlTOPp->processor__DOT__isMemWrite));
        tracep->fullBit(oldp+55,(vlTOPp->processor__DOT__isBranch));
        tracep->fullBit(oldp+56,(vlTOPp->processor__DOT__isJAL));
        tracep->fullBit(oldp+57,(vlTOPp->processor__DOT__isJALR));
        tracep->fullBit(oldp+58,(vlTOPp->processor__DOT__isMUL));
        tracep->fullBit(oldp+59,(vlTOPp->processor__DOT__isAUIPC));
        tracep->fullBit(oldp+60,(vlTOPp->processor__DOT__isBranchR));
        tracep->fullBit(oldp+61,(vlTOPp->processor__DOT__isBranchC));
        tracep->fullBit(oldp+62,((((((((IData)(vlTOPp->processor__DOT__isArithmetic) 
                                       | (IData)(vlTOPp->processor__DOT__isImm)) 
                                      | (IData)(vlTOPp->processor__DOT__isMemRead)) 
                                     | (IData)(vlTOPp->processor__DOT__isLoadUI)) 
                                    | (IData)(vlTOPp->processor__DOT__isJAL)) 
                                   | (IData)(vlTOPp->processor__DOT__isJALR)) 
                                  | (IData)(vlTOPp->processor__DOT__isAUIPC))));
        tracep->fullBit(oldp+63,(vlTOPp->clock));
        tracep->fullBit(oldp+64,(vlTOPp->reset));
        tracep->fullBit(oldp+65,(vlTOPp->memEn));
        tracep->fullIData(oldp+66,(vlTOPp->memData),32);
        tracep->fullIData(oldp+67,(vlTOPp->memAddr),32);
        tracep->fullIData(oldp+68,(vlTOPp->gp),32);
        tracep->fullIData(oldp+69,(vlTOPp->a7),32);
        tracep->fullIData(oldp+70,(vlTOPp->a0),32);
        tracep->fullIData(oldp+71,((((IData)(vlTOPp->processor__DOT__isJALR) 
                                     | (IData)(vlTOPp->processor__DOT__isJAL))
                                     ? ((IData)(4U) 
                                        + vlTOPp->processor__DOT__pc)
                                     : ((IData)(vlTOPp->processor__DOT__isMemRead)
                                         ? vlTOPp->processor__DOT__memRead
                                         : vlTOPp->processor__DOT__aluOut))),32);
        tracep->fullIData(oldp+72,(0x20U),32);
        tracep->fullIData(oldp+73,(8U),32);
        tracep->fullIData(oldp+74,(0x1000U),32);
        tracep->fullCData(oldp+75,(0U),4);
        tracep->fullCData(oldp+76,(1U),4);
        tracep->fullCData(oldp+77,(2U),4);
        tracep->fullCData(oldp+78,(3U),4);
        tracep->fullCData(oldp+79,(4U),4);
        tracep->fullCData(oldp+80,(5U),4);
        tracep->fullCData(oldp+81,(6U),4);
        tracep->fullCData(oldp+82,(7U),4);
        tracep->fullCData(oldp+83,(8U),4);
        tracep->fullCData(oldp+84,(9U),4);
        tracep->fullCData(oldp+85,(0xaU),4);
        tracep->fullCData(oldp+86,(0xdU),4);
        tracep->fullCData(oldp+87,(0xfU),4);
    }
}
