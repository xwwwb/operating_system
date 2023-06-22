import random

# 以覆写模式打开occupy.txt
f = open("occupy.txt", "w")

for i in range(0, 1000):
    # 随机 0 或 1 的整数 存入num
    num = random.randint(0, 1)
    # 依次写入 i num
    f.write(str(i) + " " + str(num) + "\n")

# 关闭文件
f.close()
