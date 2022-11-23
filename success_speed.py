import csv

from matplotlib import pyplot as plt

filename = 'success.csv'
speed = []
success = []

with open(filename) as csvfile:
    csv_reader = csv.reader(csvfile)  # 使用csv.reader读取csvfile中的文件
    # header = next(csv_reader)        # 读取第一行每一列的标题
    for row in csv_reader:  # 将csv 文件中的数据保存到data中
        speed.append(row[0])  # 选择某一列加入到data数组中
        success.append(row[1])


    plt.plot(speed, success, 'r-')
    plt.title("probability of success")
    plt.xlabel("speed")
    plt.ylabel("success")
    plt.show()
