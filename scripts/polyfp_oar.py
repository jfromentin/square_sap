import sys
import os
import numpy as np
from struct import *
nb_prefixs = 64
nb_header_lines_data_file = 5
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage : python polyfp.py l f")
        print("where l is an even integer in [2, 40] and f is the frequency of info to store")
        exit(0)
    l = int(sys.argv[1])
    f = int(sys.argv[2])
    print("-----------------------------------")
    print(" Compute Fp of SAP of length", l)
    print("-----------------------------------")
    print(" -> frequency is", f)
    print(" -> constructiong arg files for OAR array submission")
    dir = "/nfs/data/unprotected/lmpa/jfromentin/sap_output/" + repr(l)
    file = open("polyfp3.arg", 'w')
    list_files = []
    for i in range(nb_prefixs):
        filename = dir + "/sap_" + repr(i) + ".dat"
        file_data = open(filename, 'r')
        lines = file_data.readlines()
        for j in range(nb_header_lines_data_file, len(lines)):
            s = lines[j].split('\t')
            index = s[0]
            number = s[1]
            ind = j - nb_header_lines_data_file
            input_base_filename = dir + '/sap_' + repr(i)
            if ind > 0:
                input_base_filename += '_' + repr(ind)
            list_files.append(input_base_filename)
            file.write(number + ' ' + repr(f) + ' ' + repr(l) + ' ' + input_base_filename + '.sap ' + input_base_filename + '\n')
        file_data.close()
    file.close()
