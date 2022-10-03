import time
import sys
start_time = time.time()
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def logWarn (*arg) :
    print( bcolors.WARNING,"[", "%.4f"%(time.time() - start_time), "] Warn |", *arg, bcolors.ENDC)


def logError (*arg) :
    print(bcolors.FAIL, "[", "%.4f"%(time.time() - start_time), "] Error|", *arg, bcolors.ENDC)

def log (*arg) :
    print(bcolors.BOLD, "[", "%.4f"%(time.time() - start_time), "] Log  |", *arg, bcolors.ENDC)
