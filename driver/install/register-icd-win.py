import os
import sys

REGISTER_FILE_CONTENTS = """Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\\SOFTWARE\\Khronos]

[HKEY_LOCAL_MACHINE\\SOFTWARE\\Khronos\\OpenCL]

[HKEY_LOCAL_MACHINE\\SOFTWARE\\Khronos\\OpenCL\\Vendors]
"{0}"=dword:00000000

[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Khronos]

[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Khronos\\OpenCL]

[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Khronos\\OpenCL\\Vendors]
"{0}"=dword:00000000
"""

REGISTER_FILE_NAME = "red-o-lator-icd.reg"


def main():
    if len(sys.argv) != 2:
        print("Usage: register-icd.py <path-to-red-o-lator-lib>")

    libPath = sys.argv[1]

    with open(REGISTER_FILE_NAME, "w") as regFile:
        regFile.write(REGISTER_FILE_CONTENTS.format(libPath))

    os.system("regedit /S red-o-lator-icd.reg")


if __name__ == "__main__":
    main()
