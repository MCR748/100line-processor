import sys
import os



folder_path = "/media/Education/Project-self/Comp-arch-course/100linev2/tests"
for file_name in os.listdir(folder_path):
    #print(file_name)
    file_extension = os.path.splitext(file_name)[1][1:]
    #print(file_extension)
    if file_extension == 'dump':
        with open(file_name, 'r') as f_in, open(file_name + '.dat', 'w') as f_out:
            count = 0
            f_out.write("00000013" + "\n")
            for line in f_in:

                # Check if the line has at least 9 characters and the 9th byte is ":"
                if len(line) > 8 and line[8] == ':':
                    count += 1
                    if count > 396 /4:
                        instr = line[10:18] + "\n"
                        #The pass condition
                        if line[10:18] == "c0001073":
                            break

                        f_out.write(instr)
                    
