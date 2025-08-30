MODULE=processor

.PHONY:tests
tests: waveform_tests.vcd

.PHONY:algo
algo: waveform_algo.vcd

.PHONY:prog
prog: waveform_prog.vcd

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


waveform_prog.vcd: ./obj_dir/V$(MODULE)_prog
	@echo
	@echo "### SIMULATING ###"
	@./obj_dir/V$(MODULE) +verilator+rand+reset+2

./obj_dir/V$(MODULE)_prog: .stamp.verilate_prog
	@echo
	@echo "### BUILDING SIM ###"
	make -C obj_dir -f V$(MODULE).mk V$(MODULE)

.stamp.verilate_prog: $(MODULE).sv tb_prog.cpp prog/*
	@echo
	@echo "### VERILATING ###"
	verilator -Wall --trace --x-assign unique --x-initial unique -Wno-UNUSED -Wno-style -cc $(MODULE).sv --exe tb_prog.cpp
	@touch .stamp.verilate_prog


.PHONY: clean
clean:
	rm -rf .stamp.*;
	rm -rf ./obj_dir
	rm -rf tests/*.vcd

.PHONY: clean-all
clean-all:
	rm -rf .stamp.*;
	rm -rf ./obj_dir
	rm -rf results/*.txt
