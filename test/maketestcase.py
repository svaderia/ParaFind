import numpy as np
import sys
import random

dim = int(sys.argv[1])
a = np.random.rand(dim, dim)
a.sort(axis=0)
a.sort(axis=1)
a = np.ceil((10 * dim**2) * a)
a = a.astype(int)
with open("./test/test.csv", 'w') as f:
    f.write("{}\n".format(dim))
    f.write("\n".join([" ".join([str(ele) for ele in row]) for row in a]))


with open("test/quary.in", "w") as f:
    f.write("\n".join([str(random.randint(1, 10 * dim**2)) for _ in range(10**4) ]))