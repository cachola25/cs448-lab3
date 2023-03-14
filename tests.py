import argparse
import json
import os
import subprocess
import sys
from pprint import pprint as pp

parser = argparse.ArgumentParser( \
    prog=os.path.basename(__file__),
    description="Run this to verify your vigenere program.",
    formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-T", action="store_true",
                    help="Test the tester mode")
args = parser.parse_args()

class Result:

    def __init__(self):
        self.ret = None
        self.out = None
        self.err = None
        self.cmd = None

class Test:

    def __init__(self, test, test_mode=False):
        self.t = test["t"]
        self.c = test["c"]
        self.k = test["k"]
        self.p = test["p"]
        self.kpath = f"./tests/{self.t}.kt"
        self.cpath = f"./tests/{self.t}.ct"
        self.ppath = f"./tests/{self.t}.pt"
        self.exe = "./vigenere"
        if test_mode:
            self.exe = "python dummy.py"

    def decryption(self):
        r = self.run(f"-D -k '{self.k}' -i '{self.cpath}' -o '{self.ppath}'")
        if r.ret:
            print(r.err.decode())
            print(r.out.decode())
            sys.exit(f"BAD ({self.t}): Your Vigenère returned a non-zero exit status: {r.ret}")
        pt = Test.read(self.ppath)
        if pt != self.p:
            print(f"BAD Decryption ({self.t}): {r.cmd}")
            print(f"    {self.p} != {pt}")
            sys.exit(1)
        print(f"OK Decryption ({self.t}): {r.cmd}")
        print(f"    {self.p} == {pt}")

    def encryption(self):
        r = self.run(f"-E -k '{self.k}' -i '{self.ppath}' -o '{self.cpath}'")
        if r.ret:
            print(r.err.decode())
            print(r.out.decode())
            sys.exit(f"BAD ({self.t}): Your Vigenère returned a non-zero exit status: {r.ret}")
        ct = Test.read(self.cpath)
        if ct != self.c:
            print(f"BAD Encryption ({self.t}): {r.cmd}")
            print(f"    {self.c} != {ct}")
            sys.exit(1)
        print(f"OK Encryption ({self.t}): {r.cmd}")
        print(f"    {self.c} == {ct}")

    def init(self):
        self.write(self.ppath, self.p)

    @staticmethod
    def read(path):
        try:
            with open(path, "r",  encoding="utf-8") as fd:
                content = fd.read()
                return content.strip()
        except IOError as e:
            sys.exit(f"Read Failed: {e}")

    def run(self, args):
        r = Result()
        r.cmd = f"{self.exe} {args}"
        p = subprocess.Popen(r.cmd,
            stdout = subprocess.PIPE,
            stderr = subprocess.PIPE,
            shell=True)
        r.out, r.err = p.communicate()
        r.ret = p.returncode
        return r

    @staticmethod
    def write(path, content):
        try:
            with open(path, "w", encoding="utf-8") as fd:
                fd.write(content)
        except IOError as e:
            sys.exit(f"Write Failed: {e}")

if __name__ == "__main__":
    os.makedirs("./tests", exist_ok=True)
    try:
        with open("tests.json", "r",
                  encoding="utf-8") as fd:
            tests = json.load(fd)
            for test in tests:
                t = Test(test, test_mode=args.T)
                t.init()
                t.encryption()
                t.decryption()
        print("\nCongratulations. All tests passed!")

    except IOError as e:
        print("Abort: {0}".format(e))
        sys.exit(1)
