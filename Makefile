MODULE=processor

# Defines for running algos
DEFINES ?= -DALGO_PASCAL
export DEFINES

.PHONY:tests
tests: waveform_tests.vcd

.PHONY:algo
algo: waveform_algo.vcd

.PHONY:prog
prog: algo_define waveform_prog.vcd runsw

waveform_tests.vcd: ./obj_dir/V$(MODULE)_tests
	@echo
	@echo "### SIMULATING ###"
	@./obj_dir/V$(MODULE) +verilator+rand+reset+2

./obj_dir/V$(MODULE)_tests: .stamp.verilate_tests
	@echo
	@echo "### BUILDING SIM ###"
	make -C obj_dir -f V$(MODULE).mk V$(MODULE)

.stamp.verilate_tests: $(MODULE).sv tb_$(MODULE).cpp tests/*
	@echo
	@echo "### VERILATING ###"
	verilator -Wall --trace --x-assign unique --x-initial unique -Wno-UNUSED -Wno-style -cc $(MODULE).sv --exe tb_$(MODULE).cpp
	@touch .stamp.verilate_tests


waveform_algo.vcd: ./obj_dir/V$(MODULE)_algo
	@echo
	@echo "### SIMULATING ###"
	@./obj_dir/V$(MODULE) +verilator+rand+reset+2

./obj_dir/V$(MODULE)_algo: .stamp.verilate_algo
	@echo
	@echo "### BUILDING SIM ###"
	make -C obj_dir -f V$(MODULE).mk V$(MODULE)

.stamp.verilate_algo: $(MODULE).sv tb_algo.cpp algorithms/*
	@echo
	@echo "### VERILATING ###"
	verilator -Wall --trace --x-assign unique --x-initial unique -Wno-UNUSED -Wno-style -cc $(MODULE).sv --exe tb_algo.cpp
	@touch .stamp.verilate_algo


waveform_prog.vcd: .stamp.verilate_prog ./obj_dir/Vtb_prog riscv_prog
	@echo
	@echo "### SIMULATING ###"
	@./obj_dir/Vtb_prog 

# ./obj_dir/Vtb_prog: .stamp.verilate_prog riscv_prog
# 	@echo
# 	@echo "### BUILDING SIM ###"
# 	make -C obj_dir -f Vtb_prog.mk Vtb_prog

.stamp.verilate_prog: $(MODULE).sv tb_prog.sv 
	@echo
	@echo "### VERILATING ###"
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
