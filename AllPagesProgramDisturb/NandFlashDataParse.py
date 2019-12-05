import os
import csv

FILE_DIR = "ADD_THE_RESULT_FILE_FOLDER_HERE"
SOURCE = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"]
RESULT = "result.csv"
THRESHOLD = 8

data_map = {}

for source in SOURCE:
    iFile = os.path.join(FILE_DIR, source)
    with open(iFile + "_final", 'r') as fin:
        lines = list(fin)
        for line_num, line in enumerate(lines):
            elements = line.split(' ')
            for index, element in enumerate(elements):
                key = source
                if element == 'aa':
                    try:
                        data_map[key].append(index)
                    except Exception:
                        data_map[key] = [index]


def findCorrelation(page):
    """
    :param page: List of indexes with Stable Bytes
    :return: List of indexes that passes the threshold
    """
    res = []
    min_value = min(page)
    max_value = max(page)
    for stable_byte in range(min_value, max_value):
        if page.count(stable_byte) >= THRESHOLD:
            res.append(stable_byte)
    return res


with open(os.path.join(FILE_DIR, RESULT), 'w') as f:
    w = csv.DictWriter(f, data_map.keys())
    w.writeheader()
    res = {}
    for key in data_map.keys():
        data_map[key].sort()
        rest_list = findCorrelation(data_map[key])
        res[key] = len(rest_list)
        print(rest_list)
    w.writerow(res)










