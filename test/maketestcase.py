import numpy as np
import sys
import random

dim = int(sys.argv[1])
# a = np.random.rand(dim, dim)
# a.sort(axis=0)
# a.sort(axis=1)
# a = np.ceil((10 * dim**2) * a)
# a = a.astype(int)
total_queries = 10**4

# with open("./test/test.csv", 'w') as f:
#     f.write("{}\n".format(dim))
#     f.write("\n".join([" ".join([str(ele) for ele in row]) for row in a]))


with open("test/query3.in", "w") as f:
    f.write("{}\n".format(total_queries))    
    arr = [str(random.randint(1, dim**2)) for _ in range(total_queries) ]
    f.write("\n".join(arr))

# with open("test/answer.out", "w") as f:
#     for ele in arr:
#         if int(ele) in a:
#             f.write("1\n")
#         else:
#             f.write("0\n")