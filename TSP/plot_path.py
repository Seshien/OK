import matplotlib.pyplot as plt
import numpy as np
import sys

def load_data(filename):
    dx = []
    dy = []
    try:
        with open(filename, "r") as f:
            _ = f.readline()
            for line in f:
                data = line.split()
                if len(data) == 3:
                    index, x, y = line.split()
                    dx.append(float(x))
                    dy.append(float(y))
    except IOError:
        print("Nie można znaleźć pliku", filename)
        sys.exit(-1)
    return dx, dy


filename =""
path = []
if len(sys.argv) < 2:

    filename = "bier127.txt"
    paths = "1 16 2 51 57 54 45 103 44 40 35 37 36 41 14 12 30 27 31 80 79 77 18 74 73 67 8 72 19 23 4 22 21 17 20 108 24 9 11 3 90 116 60 59 62 61 91 100 58 64 113 65 99 92 89 125 104 85 86 87 88 119 63 96 109 110 71 68 70 69 75 76 78 117 84 81 126 82 83 102 101 98 97 28 122 33 32 29 25 26 38 39 34 43 42 123 95 93 127 107 111 112 94 46 48 118 49 53 47 55 66 124 52 5 56 121 50 13 115 10 120 7 105 15 106 6 114 "
    paths = paths.split()
    path = []
    for p in paths:
        path.append(int(p))
else:
    filename = sys.argv[1]
    path = list(map(int, sys.argv[2:]))
if min(path) == 1:
    path = [p - 1 for p in path]
x,y = load_data(filename)
X = [x[p] for p in path]
Y = [y[p] for p in path]

plt.plot(X, Y, 'o-', linewidth=.5, markersize=3)
plt.show()