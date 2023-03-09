
import argparse
import json
import os
import sys
from pprint import pprint as pp

parser = argparse.ArgumentParser( \
    prog=os.path.basename(__file__),
    description="Used for testing the tester only",
    formatter_class=argparse.ArgumentDefaultsHelpFormatter)

parser.add_argument("-D", action="store_true",
                    help="Decode mode")
parser.add_argument("-E", action="store_true",
                    help="Encode mode")
parser.add_argument("-k", type=str, required=False,
                    help="Secret key value")
parser.add_argument("-i", type=str, required=False,
                    help="Path to input file")
parser.add_argument("-o", type=str, required=False,
                    help="Path to output file")
args = parser.parse_args()

def loadTest(args):
    testName = os.path.splitext(os.path.basename(args.i))[0]
    try:
        with open("tests.json", "r", encoding="utf-8") as fd:
            tests = json.load(fd)
            for test in tests:
                if test["t"] == testName:
                    if args.D:
                        test["in"] = f"tests/{testName}.ct"
                        test["out"] = f"tests/{testName}.pt"
                        test["key"] = f"tests/{testName}.kt"
                        test["mode"] = "D"
                    elif args.E:
                        test["in"] = f"tests/{testName}.pt"
                        test["out"] = f"tests/{testName}.ct"
                        test["key"] = f"tests/{testName}.kt"
                        test["mode"] = "E"
                    else:
                        sys.exit("Abort: Say What?")
                    return test
    except IOError as e:
         sys.exit(f"Abort: {e}")
    sys.exit(f"Error: No matching test found: {testName}")

def read(path):
    try:
        with open(path, "r",  encoding="utf-8") as fd:
            content = fd.read()
            return content.strip()
    except IOError as e:
        print(f"Read Failed: {e}")
        sys.exit(1)

def write(path, content):
    try:
        with open(path, "w", encoding="utf-8") as fd:
            fd.write(content)
    except IOError as e:
        print(f"Write Failed: {e}")
        sys.exit(1)

def decrypt(t):
    write(t["in"], t["c"])
    write(t["out"], t["p"])
    write(t["key"], t["k"])

def encrypt(t):
    write(t["in"], t["p"])
    write(t["out"], t["c"])
    write(t["key"], t["k"])

t = loadTest(args)
os.makedirs("./tests", exist_ok=True)

if args.D:
    decrypt(t)
elif args.E:
    encrypt(t)
