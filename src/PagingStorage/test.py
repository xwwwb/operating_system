import random
f = open("occupy.txt", "w")
for i in range(0, 1000):
    # 随机 0 或者 1 1占1/3
    num = random.randint(0, 2)
    if num == 1:
        num = 1
    else:
        num = 0
    # 依次写入 i num
    f.write(str(i) + " " + str(num) + "\n")
# 关闭文件
f.close()
