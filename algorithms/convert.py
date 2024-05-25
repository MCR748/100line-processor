def read_binary_and_write_dat(input_file_path, output_file_path):
    try:
        # Open the binary file for reading
        with open(input_file_path, 'rb') as bin_file:
            # Open the.dat file for writing
            with open(output_file_path, 'w') as dat_file:
                # Read the binary file in 4-byte chunks
                while True:
                    chunk = bin_file.read(4)
                    if not chunk:
                        break  # End of file reached
                    # Reverse the byte sequence
                    reversed_chunk = chunk[::-1]
                    # Convert the reversed chunk to a string representation
                    chunk_str = reversed_chunk.hex()  # Convert bytes to hex string
                    # Write the chunk to the.dat file
                    dat_file.write(f"{chunk_str}\n")
    except FileNotFoundError:
        print(f"Input file {input_file_path} not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

# Example usage
input_file_path = '/media/Education/Project-self/Comp-arch-course/algortihms/Algo/fib.bin'
output_file_path = '/media/Education/Project-self/Comp-arch-course/algortihms/Algo/fib.dat'
read_binary_and_write_dat(input_file_path, output_file_path)
