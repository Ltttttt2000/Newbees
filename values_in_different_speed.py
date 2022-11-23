import csv
import os
from matplotlib import pyplot as plt
from collections import defaultdict

address = []
left = dict()
centre = dict()
right = dict()
eline = dict()


def getFilePathList(path, filetype):
    pathList = []
    for root, dirs, files in os.walk(path):
        for file in files:
            if file.endswith(filetype):
                pathList.append(os.path.join(root, file))
    return pathList  # 输出以filetype为后缀的列表


pathlist = getFilePathList('/Users/tl/Projects/Robotic/bang_bang/', 'csv')
namelist = []
for path in pathlist:
    name = path.replace('/Users/tl/Projects/Robotic/bang_bang/', '').replace('.csv', '')  # speed
    namelist.append(name)
    left[name] = []
    centre[name] = []
    right[name] = []
    eline[name] = []
    # print(name)
    with open(path) as csvfile:
        csv_reader = csv.reader(csvfile)  # 使用csv.reader读取csvfile中的文件
        # header = next(csv_reader)        # 读取第一行每一列的标题
        for row in csv_reader:  # 将csv 文件中的数据保存到data中
            left[name].append(row[1])
            centre[name].append(row[2])
            right[name].append(row[3])
            eline[name].append(row[4])


with open('bang_bang/20.csv') as csvfile:
    csv_reader = csv.reader(csvfile)  # 使用csv.reader读取csvfile中的文件
    # header = next(csv_reader)        # 读取第一行每一列的标题
    for row in csv_reader:  # 将csv 文件中的数据保存到data中
        address.append(row[0])

# plot the left sensor value in different speed
for left_i in namelist:
    # print(left_i)  # the value is the speed
    plt.plot(address, left[left_i], label=left_i)

plt.title('Left sensor value')
plt.xlabel('address')
plt.ylabel('left sensor value')
plt.legend()
plt.show()

# plot the centre sensor value in different speed
for centre_i in namelist:
    # print(centre_i)  # the value is the speed
    plt.plot(address, left[centre_i], label=centre_i)

plt.title('Centre sensor value')
plt.xlabel('address')
plt.ylabel('centre sensor value')
plt.legend()
plt.show()

# plot the right sensor value in different speed
for right_i in namelist:
    # print(right_i)  # the value is the speed
    plt.plot(address, left[right_i], label=right_i)

plt.title('Right sensor value')
plt.xlabel('address')
plt.ylabel('right sensor value')
plt.legend()
plt.show()

# plot the eline value in different speed
for eline_i in namelist:
    # print(eline_i)  # the value is the speed
    plt.plot(address, left[eline_i], label=eline_i)

plt.title('Eline value')
plt.xlabel('address')
plt.ylabel('eline value')
plt.legend()
plt.show()