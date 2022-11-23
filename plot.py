import csv

from matplotlib import pyplot as plt

filename = 'bang_bang/40.csv'
address = []
left_sensor = []
centre_sensor = []
right_sensor = []
eline = []
with open(filename) as csvfile:
    csv_reader = csv.reader(csvfile)  # 使用csv.reader读取csvfile中的文件
    # header = next(csv_reader)        # 读取第一行每一列的标题
    for row in csv_reader:  # 将csv 文件中的数据保存到data中
        address.append(row[0])  # 选择某一列加入到data数组中
        left_sensor.append(row[1])
        centre_sensor.append(row[2])
        right_sensor.append(row[3])
        eline.append(row[4])

    print(address)

    # Address-left sensor
    plt.plot(address, left_sensor, 'r-')
    plt.title("address-left sensor")
    plt.xlabel("address")
    plt.ylabel("left sensor value")
    plt.show()
    # Address-centre sensor
    plt.plot(address, centre_sensor, 'r-')
    plt.title("address-centre sensor")
    plt.xlabel("address")
    plt.ylabel("centre sensor value")
    plt.show()
    # Address-right sensor
    plt.plot(address, right_sensor, 'r-')
    plt.title("address-right sensor")
    plt.xlabel("address")
    plt.ylabel("right sensor value")
    plt.show()
    # Address-eline
    plt.plot(address, eline, 'r-')
    plt.title("address-eline")
    plt.xlabel("address")
    plt.ylabel("eline")
    plt.show()

    # draw different line in one graph
    plt.plot(address, left_sensor, color='red', marker='*', label='Left')
    plt.plot(address, centre_sensor, color='green', marker='*', label='Centre')
    plt.plot(address, right_sensor, color='blue', label='Right')
    plt.legend()
    plt.title("address-right sensor")
    plt.xlabel("address")
    plt.ylabel("sensor values")

    plt.show()
