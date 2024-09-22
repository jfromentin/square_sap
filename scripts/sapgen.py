import sys
import os

prefixs = ["RRRRRR", "RRRRRUR_RRRRRUL", "RRRRRUU", "RRRRUR", "RRRRUL", "RRRRUUR_RRRRUUL", "RRRRUUU", "RRRURR", "RRRURUR_RRRURUL", "RRRURUU", "RRRURD_RRRULL", "RRRULU", "RRRUUR", "RRRUUL", "RRRUUUR_RRRUUUL", "RRRUUUU", "RRURRR", "RRURRUR_RRURRUL", "RRURRUU", "RRURRD_RRURUR", "RRURUL", "RRURUUR_RRURUUL", "RRURUUU", "RRURD", "RRULUR_RRULUL", "RRULUU", "RRUURR", "RRUURU", "RRUURD_RRUULL", "RRUULU", "RRUUUR", "RRUUUL", "RRUUUU", "RURRRR", "RURRRU", "RURRRD_RURRUR", "RURRUL", "RURRUU", "RURRD", "RURURR", "RURURU", "RURURD_RURULL", "RURULU", "RURUUR", "RURUUL", "RURUUU", "RURDRR", "RURDRU", "RUURRR", "RUURRU_RUURRD", "RUURUR", "RUURUL", "RUURUU", "RUURD", "RUULL", "RUULUR_RUULUL", "RUULUU", "RUUURR", "RUUURU_RUUURD", "RUUULL", "RUUULU_RUUULD", "RUUUUR", "RUUUUU", "RUUUUL"]       

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage : python sapgen.py l s")
        print(" where l is an aven integer in [2, 40] and s is the plsit number")
        exit(0)
    l = int(sys.argv[1])
    s = int(sys.argv[2])
    print("------------------------------")
    print(" Generate SAP of length", l)
    print("------------------------------")
    print(" -> split number is", s)
    print(" -> constructiong arg files for GNU parallel")
    dir = "output/" + repr(l)
    os.system("rm -rf " + dir)
    os.makedirs(dir)
    file = open("sapgen.arg", 'w')
    for i in range(len(prefixs)):
        file.write(prefixs[i] + " " + repr(l) + " " + dir + "/sap_" + repr(i) + " " + repr(s) + "\n")
    file.close()
    print(" -> output dir:", dir);
    print(" -> launch sapgen in parallel")
    os.system("parallel -a sapgen.arg --colsep ' ' ./sapgen.sh")
