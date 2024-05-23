 
import subprocess

def test_echo():
    # Start the process
    process = subprocess.Popen(['path/to/process_to_test.exe'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Send input to the process
    input_data = b'Test input\n'
    process.stdin.write(input_data)
    process.stdin.flush()

    # Read the output
    output, errors = process.communicate()
    
    # Check if the output is as expected
    assert output.strip() == b'Test input', f'Expected "Test input", got "{output.strip()}"'

if __name__ == '__main__':
    test_echo()