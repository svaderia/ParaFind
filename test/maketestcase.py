import numpy as np
import sys

dim = int(sys.argv[1])
a = np.random.rand(dim, dim)
a.sort(axis=0)
a.sort(axis=1)
a = np.ceil((10 * dim**2) * a)
a = a.astype(int)
with open("test.csv", 'w') as f:
    f.write("{}\n".format(dim, order))
    for row in a:
        for ele in row[:-1]:
            f.write('{} '.format(ele, align=order))
        f.write('{}'.format(row[-1], align=order))
        f.write("\n")
