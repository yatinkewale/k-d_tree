#!/usr/bin/env python3

import random
import sys

if len(sys.argv) < 3:
    print(f"Usage:\npython3 {sys.argv[0]} output_file.csv n_points")
    exit(0)

out_fname = str(sys.argv[1])
N = int(sys.argv[2])

outf = open(out_fname, 'w+')

outf.write(f"{N:d}\n")

random.seed("The world is on fire right now")

for i in range(N):
    x, y = random.uniform(-10, 10), random.uniform(-10, 10)
    outf.write(f"{x:.6f}, {y:.6f}\n")

outf.close()