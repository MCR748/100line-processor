from glob import glob
from subprocess import run
import pytest

'''
Compile once, simulate in a loop
'''
# verilator_cmd = f'verilator --binary -j 0 -O3 --trace --top tb_processor processor.sv --Mdir ./'
# verilator_result = run(verilator_cmd.split()).returncode
# assert verilator_result == 0, f'Failed to compile simualtion'

'''
Foreach C file in the c folder:
 1. Compile for x86
 2. Run the compiled binary
 3. Capture the stdout as expected result
 4. Compile for RISC-V
 5. Run the testbench with the compiled binary
 6. Read the output from testbench
 7. Compare with expected result

'''
@pytest.mark.parametrize("source", glob('c/*.c'))
def test_source(source):
    print(source)
    x86_result = run(['gcc', source, '-o', 'c/x86/a.out']).returncode
    assert x86_result == 0, f'Failed to compile {source} for x86'

    exp_result = run(['c/x86/a.out'], capture_output=True)
    assert exp_result.returncode == 0, f'Failed to execute {source} in x86'

    expected = exp_result.stdout.decode('utf-8')
    print('Expected result is:', expected)

    # compile_result = run(['riscv32-unknown-linux-gnu-gcc -march=rv32imafd', source, '-o', 'c/riscv/a.out']).returncode
    # assert compile_result == 0, f'Failed to compile {source} for RISC-V'

    # sim_result = run(['./obj_dir/Vprocessor'])
    # assert sim_result.returncode == 0, f'Failed to simulate {source} with verilator'

    # actual = ... (some file written by TB)
    # assert actual == expected, f'Test for {source}. Expected: {expected}, got: {actual}'
