MODULE=processor

# Defines for running algos
DEFINES ?= -DALGO_PASCAL
export DEFINES

.PHONY:tests
tests: waveform_tests.vcd

.PHONY: prog
prog: algo_define waveform_prog.vcd runsw

waveform_tests.vcd: .stamp.verilate_tests ./obj_dir/Vtb_$(MODULE)
	@echo
	@echo "### SIMULATING ###"
	@./obj_dir/Vtb_$(MODULE) 

.stamp.verilate_tests: $(MODULE).sv tb_$(MODULE).sv tests/test_list.txt tests/*.dat
	@echo
	@echo "### VERILATING AND BUILDING SIM ###"
	verilator -Wall --trace --x-assign unique --x-initial unique -Wno-UNUSED -Wno-style --timing --cc --binary tb_$(MODULE).sv $(MODULE).sv
	@touch .stamp.verilate_tests


waveform_prog.vcd: .stamp.verilate_prog ./obj_dir/Vtb_prog riscv_prog
	@echo
	@echo "### SIMULATING ###"
	@./obj_dir/Vtb_prog 

.stamp.verilate_prog: $(MODULE).sv tb_prog.sv 
	@echo
	@echo "### VERILATING AND BUILDING SIM ###"
	verilator -Wall --trace --x-assign unique --x-initial unique -Wno-UNUSED -Wno-style --timing --cc --binary tb_prog.sv $(MODULE).sv
	@touch .stamp.verilate_prog

.PHONY: algo_define
algo_define:
	@echo "$(DEFINES)" | cmp -s - .stamp.algos || echo "$(DEFINES)" > .stamp.algos

.PHONY: riscv_prog
riscv_prog: .stamp.algos
	@echo
	@echo "### BUILDING PROGRAM ###"
	$(MAKE) -C prog 
	$(MAKE) -C prog dump

.PHONY: runsw
runsw: algo_define sw_main
	@echo
	@echo "### RUNNING SW PROGRAM ###"
	./sw_main

sw_main: sw_main.cpp user_code/* .stamp.algos
	g++ -Iuser_code $(DEFINES) sw_main.cpp -o sw_main

.PHONY: clean
clean:
	rm -rf .stamp.*;
	rm -rf ./obj_dir
	rm -rf tests/*.vcd
	$(MAKE) -C prog clean

.PHONY: clean-all
clean-all:
	rm -rf .stamp.*;
	rm -rf ./obj_dir
	rm -rf results/*.txt
	$(MAKE) -C prog clean
