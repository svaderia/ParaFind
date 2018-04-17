import numpy as np
import sys
import random

dim = int(sys.argv[1])
a = np.random.rand(dim, dim)
a.sort(axis=0)
a.sort(axis=1)
a = np.ceil((10 * dim**2) * a)
a = a.astype(int)

total_queries = 10**4

with open("./test/test.csv", "w") as f:
    f.write("{}\n".format(dim))
    for row in a:
        for ele in row:
            f.write("{}\n".format(ele))
        # f.write("{}\n".format(row[-1]))
    # f.write("\n".join([" ".join([str(ele) for ele in row]) for row in a]))

arr = [str(random.randint(1, 10 * dim**2)) for _ in range(total_queries) ]

with open("test/query3.in", "w") as f:
    f.write("{}\n".format(total_queries))    
    f.write("\n".join(arr))

a = a.flatten()
# print(a.shape)
# a = set([str(ele) for ele in a])
# b = set()
# for ele in a:
#     b.add(ele)
# # free(a)

# print(type(a))
# with open("test/answer.out", "w") as f:
#     for ele in arr:
#         # print(ele, end=", ")
#         if int(ele) in b:
#             f.write("1\n")
#         else:
#             f.write("0\n")