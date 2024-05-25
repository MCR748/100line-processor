PROGRAM=fib

.PHONY:riscv
riscv: .stamp.temps

.PHONY:dump
dump: .stamp.dump

.PHONY:image
image: .stamp.image

.PHONY:init
init: .stamp.init

.stamp.link: .stamp.asmf .stamp.init
	/home/mcr-paradox/Downloads/riscv-glibc/bin/riscv32-unknown-linux-gnu-ld -Tvirt.ld init.o $(PROGRAM).o -o $(PROGRAM).x
	@touch .stamp.link


.stamp.asmf: .stamp.temps  $(PROGRAM).s
	/home/mcr-paradox/Downloads/riscv-glibc/bin/riscv32-unknown-linux-gnu-as $(PROGRAM).s -o $(PROGRAM).o
	@touch .stamp.asmf


.stamp.temps: $(PROGRAM).c
	/home/mcr-paradox/Downloads/riscv-glibc/bin/riscv32-unknown-linux-gnu-gcc -march=rv32imafd -S $(PROGRAM).c -o $(PROGRAM).s 
	@touch .stamp.temps

.stamp.dump:.stamp.link
	/home/mcr-paradox/Downloads/riscv-glibc/bin/riscv32-unknown-linux-gnu-objdump -D $(PROGRAM).x > $(PROGRAM).dump
	@touch .stamp.dump 

.stamp.image: .stamp.link
	/home/mcr-paradox/Downloads/riscv-glibc/bin/riscv32-unknown-linux-gnu-objcopy -O binary $(PROGRAM).x $(PROGRAM).bin
	python3 convert.py
	@touch .stamp.image

.stamp.init: init.s
	/home/mcr-paradox/Downloads/riscv-glibc/bin/riscv32-unknown-linux-gnu-as init.s -o init.o
	@touch .stamp.init
	
.PHONY: clean
clean:
	rm -rf .stamp.*;
	rm $(PROGRAM).s
	rm *.o
