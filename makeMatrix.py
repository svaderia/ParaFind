import numpy as np
import sys

dim = int(sys.argv[1])
order = int(sys.argv[2])
a = np.random.rand(dim, dim)
a.sort(axis=0)
a.sort(axis=1)
a = np.ceil((10**order)*a)
a = a.astype(int)
np.savetxt('test.csv', a ,fmt = '%{}.{}d'.format(order, order), delimiter=',')

