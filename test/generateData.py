import random
import string

def generate_random_name(length=8):
    """生成一个随机的英文名，不超过8个字符"""
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for _ in range(random.randint(1, length))).capitalize()

def generate_data(n):
    """生成包含n条数据的列表"""
    data_list = []
    for i in range(1, n + 1):
        id = i
        name = generate_random_name()
        age = random.randint(1, 100)
        sex = random.choice([0, 1])
        data_list.append(f"1;{id},{name},{age},{sex}")
    return data_list

def write_to_file(data_list, filename="output.txt"):
    """将数据列表写入txt文件"""
    with open(filename, 'w') as file:
        for data in data_list:
            file.write(data + "\n")

if __name__ == "__main__":
    n = int(input("请输入生成数据的条数: "))
    data_list = generate_data(n)
    write_to_file(data_list)
    print(f"成功生成{n}条数据到文件output.txt中")
