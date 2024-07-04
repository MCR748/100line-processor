# 100line-processor
A RV32I processor written to support a choosen set of instructions. Inprogress. 

## Supported Instructions

### Arithmetic type
```diff
  ADD  : x[rd] = x[rs1] +     x[rs2]
  SUB  : x[rd] = x[rs1] -     x[rs2]
  SLL  : x[rd] = x[rs1] <<    x[rs2]
  SLT  : x[rd] = x[rs1] <s    x[rs2]
  SLTU : x[rd] = x[rs1] <u    x[rs2]
  XOR  : x[rd] = x[rs1] ^     x[rs2]
  SRL  : x[rd] = x[rs1] >>u   x[rs2]
  SRA  : x[rd] = x[rs1] >>s   x[rs2]
  OR   : x[rd] = x[rs1] |     x[rs2]
  AND  : x[rd] = x[rs1] &     x[rs2]
+ MUL  : x[rd] = x[rs1] (s×s) x[rs2]
+ DIV  : x[rd] = x[rs1] /s    x[rs2]
```

### Arithmetic with immediate
```diff
  ADDI  : x[rd] = x[rs1] +   sext(immediate)
  ANDI  : x[rd] = x[rs1] &   sext(immediate)
  ORI   : x[rd] = x[rs1] |   sext(immediate)
  XORI  : x[rd] = x[rs1] ^   sext(immediate)
  SLLI  : x[rd] = x[rs1] <<  shamt
  SRLI  : x[rd] = x[rs1] >>u shamt
  SRAI  : x[rd] = x[rs1] >>s shamt
  SLTI  : x[rd] = x[rs1] <s  sext(immediate)
  SLTIU : x[rd] = x[rs1] <u  sext(immediate)
  LUI   : x[rd] = sext(immediate[31:12] << 12)
```


### Load

```diff
  LW  : x[rd] = sext(M[x[rs1] + sext(offset)][31:0])
- LH  : x[rd] = sext(M[x[rs1] + sext(offset)][15:0])
- LB  : x[rd] = sext(M[x[rs1] + sext(offset)][7:0])
- LHU : x[rd] = M[x[rs1] + sext(offset)][15:0]
- LBU : x[rd] = M[x[rs1] + sext(offset)][7:0]
```

### Store 
```diff
  SW : M[x[rs1] + sext(offset)] = x[rs2][31:0]
- SH : M[x[rs1] + sext(offset)] = x[rs2][15:0]
- SB : M[x[rs1] + sext(offset)] = x[rs2][7:0]
```

### Conditional Branch
```diff
  BEQ  : if (x[rs1] ==  x[rs2]) pc += sext(offset)
  BNE  : if (x[rs1] !=  x[rs2]) pc += sext(offset)
  BLT  : if (x[rs1] <s  x[rs2]) pc += sext(offset)
  BGE  : if (x[rs1] >=s x[rs2]) pc += sext(offset)
  BLTU : if (x[rs1] <u  x[rs2]) pc += sext(offset)
  BGEU : if (x[rs1] >=u x[rs2]) pc += sext(offset)
```

### Unconditional branch
```diff
  JAL   : x[rd] = pc+4; pc += sext(offset)
  JALR  : t = pc+4; pc=(x[rs1]+sext(offset))&∼1; x[rd]=t
  AUIPC : x[rd] = pc + sext(immediate[31:12] << 12)
```

### System
```diff
- fence
- fence.i
- csrrw
- csrrs
- csrrc
- csrrwi
- csrrwi
- csrrsi
- csrrci
- ecall
- ebreak
- uret
- sret
- mret
- wfi
- sfence.vma
```