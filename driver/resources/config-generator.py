import re
import sys
from pathlib import Path


def main():
    if len(sys.argv) != 2:
        print("Usage: config-generator.py <path-to-raw-clinfo-output>")

    filepath = sys.argv[1]
    output = Path(filepath).stem + ".ini"

    converted = convertFile(filepath)

    with open(output, "w") as config:
        config.write(converted)


def convertFile(filepath: str) -> str:
    with open(filepath) as clInfoOutput:
        resultLines = []
        for line in clInfoOutput:
            (_, parameter, parameterValue) = (re.split(r"\s{2,}", line.strip()) + ["0"])[:3]
            resultLines.append(f"{parameter} = {parameterValue}")
        return "\n".join(resultLines)


if __name__ == "__main__":
    main()
