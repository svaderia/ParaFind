import numpy as np
import sys

dim = int(sys.argv[1])
order = int(sys.argv[2])
a = np.random.rand(dim, dim)
a.sort(axis=0)
a.sort(axis=1)
a = np.ceil((10**order)*a)
a = a.astype(int)
# np.savetxt('test.csv', a ,fmt = '%{}.{}d'.format(order, order), delimiter=',')
with open("test.csv", 'w') as f:
    f.write("{} {}\n".format(dim, order))
    for row in a:
        for ele in row[:-1]:            
            f.write('{:0{align}d}, '.format(ele, align=order))
        f.write('{:0{align}d}'.format(row[-1], align=order))
        f.write("\n")
