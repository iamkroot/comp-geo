import argparse
from pathlib import Path
from sklearn.datasets import make_blobs
import numpy as np
from math import dist as euclid
import subprocess as sp
import matplotlib
matplotlib.use("pdf")  # TODO: Shift to png
import matplotlib.pyplot as plt

DATASET_DIR = Path.cwd().parent / "datasets"
DATASET_DIR.mkdir(exist_ok=True)


def get_new_dataset_path():
    old = max(DATASET_DIR.iterdir(), default=DATASET_DIR / "00.txt")
    return old.with_name(f"{int(old.stem) + 1:02}.txt")


def write_dataset(points, centers, file):
    print(len(points), len(centers), file=file)
    for point in points:
        print(*point, file=file)


def run_clustering(exec_path: Path, points, centers):
    dataset_path = get_new_dataset_path()
    with open(dataset_path, "w") as file:
        write_dataset(points, centers, file)
    return sp.check_output([str(exec_path), str(dataset_path)], text=True)


def parse_output(output, n, k):
    lines = output.split("\n")
    centers = np.array([tuple(map(float, line[1:-1].split(", "))) for line in lines[:k]])
    labels = np.array(list(map(int, lines[k:n + k])))
    dist = np.sqrt(np.array(list(map(float, lines[n + k:-1]))))
    return centers, labels, dist


def plot(ax, points, labels, centers):
    scatter1 = ax.scatter(points[:, 0], points[:, 1], marker="o", c=labels)
    scatter2 = ax.scatter(centers[:, 0], centers[:, 1], marker="o", label="Center")
    handles1, labels1 = scatter1.legend_elements(fmt="Cluster {x:.0f}")
    ax.legend([*handles1, scatter2], [*labels1, "Center"])


def run(exec_path: Path):
    points, labels, centers = make_blobs(return_centers=True)
    max_dist = max(euclid(point, centers[label]) for point, label in zip(points, labels))

    fig, (ax1, ax2) = plt.subplots(ncols=2, sharex=True)

    ax1.set_title(f"Optimal clustering (cost: {max_dist:.3f})")
    plot(ax1, points, labels, centers)

    output = run_clustering(exec_path, points, centers)

    # TODO: Fix mismatch of labels
    approx_centers, approx_labels, approx_dist = parse_output(output, len(points), len(centers))
    approx_max_dist = max(approx_dist)

    ax2.set_title(f"Greedy clustering (cost: {approx_max_dist:.3f})")
    plot(ax2, points, approx_labels, approx_centers)

    fig.suptitle("Clustering comparision")
    plt.savefig("a.pdf")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("exec_path", type=Path, help="Path to clustering executable")
    args = parser.parse_args()
    run(args.exec_path)


if __name__ == '__main__':
    main()
