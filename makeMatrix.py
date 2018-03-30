import numpy as np
a = np.random.rand(128,128)
a.sort(axis=0)
a.sort(axis=1)
a = np.ceil(10000*a)
a = a.astype(int)
np.savetxt('Matrix.csv', a ,fmt = '%7.7d', delimiter=',')

