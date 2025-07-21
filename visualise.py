import pathlib
import numpy as np
import matplotlib.pyplot as plt


currentPath = pathlib.Path()
listOfFiles = []

for el in currentPath.iterdir():
    tmpEl = str(el)
    if len(tmpEl) > 3 and tmpEl[-3:] == "out":
        listOfFiles.append(tmpEl)


for fileData in listOfFiles:

    data = []

    with open(fileData, "r") as file:
        for line in file:
            data.append(float(line.strip()))

    dataArray = np.array(data)

    print("-" * 10)
    print(f"Statistic Analysis of {fileData}")
    print(f"Mean - {dataArray.mean():.2f}")
    print(f"Median - {np.median(dataArray):.2f}")
    print(f"Std - {dataArray.std():.2f}")
    print("-" * 10)

    # plotting

    fig, ax = plt.subplots()

    ax.hist(dataArray)

    
    plt.savefig(fileData + ".png")