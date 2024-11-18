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
    print(" -> constructiong arg files for GNU parallel")
    dir = "../output/" + repr(l)
    file = open("polyfp.arg", 'w')
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
    print(" -> launch polyfp in parallel")
    os.system("parallel -a polyfp.arg --colsep ' ' ./polyfp.sh")
    sum_fp = np.float128(0)
    proba = np.float128(0)
    for filename in list_files:
        #temp = np.fromfile(filename + '.fp', dtype='d')
        #res += temp
        #print(filename, temp)
        file = open(filename + '.fp', 'rb')
        bytes = file.read(16)
        temp = unpack('<dd', bytes)
        sum_fp += np.float128(temp[0])
        proba += np.float128(temp[1])
#        print(filename, temp)
        #file.close()'''
    print("Fp sum :", sum_fp)
    print("Probability :", proba)

