import pathlib
import numpy as np
import matplotlib.pyplot as plt
import os


# making result dir

if not os.path.exists("results/"):
    os.makedirs("results")


# reading all .out files
os.chdir("out/")

currentPath = pathlib.Path()
listOfFiles = []


for el in currentPath.iterdir():
    tmpEl = str(el)
    listOfFiles.append(tmpEl)


progress = 0
# analyzing data from files and saving histogram

for fileData in listOfFiles:

    data = []

    with open(fileData, "r") as file:
        for line in file:
            data.append(float(line.strip()))

    dataArray = np.array(data)
    # calculating mean, median and std

    statisticalAnalysis = f"""Mean - {dataArray.mean():.2f}
Median - {np.median(dataArray):.2f}
Std - {dataArray.std():.2f}"""

    # plotting
    plt.style.use("seaborn-v0_8")
    fig, ax = plt.subplots(figsize=(10, 6))

    # Create histogram with better styling
    n, bins, patches = ax.hist(
        dataArray, bins=30, alpha=0.7, color="skyblue", edgecolor="black", linewidth=0.5
    )

    # Add vertical lines for mean and median
    mean_val = dataArray.mean()
    median_val = np.median(dataArray)

    ax.axvline(
        mean_val,
        color="red",
        linestyle="--",
        linewidth=2,
        label=f"Mean: {mean_val:.2f}",
    )
    ax.axvline(
        median_val,
        color="green",
        linestyle="--",
        linewidth=2,
        label=f"Median: {median_val:.2f}",
    )

    # Customize the plot
    ax.set_title(
        f"Distribution Analysis: {fileData[:-4]}", fontsize=16, fontweight="bold"
    )
    ax.set_xlabel("Value", fontsize=12)
    ax.set_ylabel("Frequency", fontsize=12)
    ax.legend(fontsize=10)
    ax.grid(True, alpha=0.3)

    # Add statistics text box
    stats_text = (
        f"Mean: {mean_val:.2f}\nMedian: {median_val:.2f}\nStd: {dataArray.std():.2f}"
    )
    ax.text(
        0.02,
        0.98,
        stats_text,
        transform=ax.transAxes,
        fontsize=10,
        verticalalignment="top",
        bbox=dict(boxstyle="round", facecolor="wheat", alpha=0.8),
    )
    # saving data into different directory
    os.chdir("../results/")
    with open(fileData, "w") as file:
        file.write(statisticalAnalysis)

    plt.tight_layout()
    plt.savefig(fileData[:-3] + "png", dpi=300, bbox_inches="tight")
    plt.close()

    os.chdir("../out/")

    # progress bar
    progress += 1
    progressPercents = progress / len(listOfFiles) * 100

    barLength = 50
    filledLength = int(barLength * progress / len(listOfFiles))

    bar = "█" * filledLength + "-" * (barLength - filledLength)

    print(f"\r[+] {bar} {progressPercents:.2f}%", end="", flush=True)

    if progress == len(listOfFiles):
        print()
