import argparse
import csv
import subprocess as sp
from io import StringIO
from math import dist
from pathlib import Path

import matplotlib
import numpy as np
from sklearn.datasets import make_blobs

matplotlib.use("Agg")
import matplotlib.pyplot as plt


def next_run_id(results_file: Path):
    lines = results_file.read_text().splitlines()
    try:
        row = next(csv.reader([lines[-1]]))
        return int(row[0]) + 1
    except (ValueError, IndexError):
        return 1


def gen_clusters_range(min_points, max_points, min_centers, max_centers, state=None):
    assert all(
        1 <= val <= 1e10 for val in (min_points, max_points, min_centers, max_centers)
    ), "value(s) out of range"
    num_points = np.random.randint(min_points, max_points + 1)
    num_centers = np.random.randint(min_centers, min(num_points, max_centers) + 1)
    return make_blobs(
        n_samples=num_points,
        centers=num_centers,
        return_centers=True,
        random_state=state,
    )


def gen_clusters_exact(num_points, num_centers, state=None):
    return make_blobs(
        n_samples=num_points,
        centers=num_centers,
        return_centers=True,
        random_state=state,
    )


def run_clustering(exec_path: Path, points, num_centers: int):
    inp = StringIO()
    print(len(points), num_centers, file=inp)
    for point in points:
        print(*point, file=inp)
    return sp.check_output([str(exec_path)], input=inp.getvalue(), text=True)


def parse_output(output, n, k):
    lines = output.splitlines()
    centers = np.array(
        [tuple(map(float, line[1:-1].split(", "))) for line in lines[:k]]
    )
    labels = np.array(list(map(int, lines[k : k + n])))
    dist = np.sqrt(np.array(list(map(float, lines[k + n : k + 2 * n]))))
    runtime = int(lines[k + 2 * n])
    return centers, labels, dist, runtime


def plot_single(ax, points, labels, centers, draw_legends=False):
    scatter1 = ax.scatter(points[:, 0], points[:, 1], marker=".", c=labels)
    scatter2 = ax.scatter(
        centers[:, 0], centers[:, 1], marker=".", color="r", label="Center"
    )
    if draw_legends:
        handles1, labels1 = scatter1.legend_elements(fmt="Cluster {x:.0f}")
        ax.legend([*handles1, scatter2], [*labels1, "Center"])


def plot(
    fig_path,
    points,
    opt_labels,
    opt_centers,
    opt_max_dist,
    approx_labels,
    approx_centers,
    approx_max_dist,
):
    fig, (ax1, ax2) = plt.subplots(ncols=2, sharex=True)

    ax1.set_title(f"Optimal clustering (cost: {opt_max_dist:.3f})")
    plot_single(ax1, points, opt_labels, opt_centers)

    ax2.set_title(f"Greedy clustering (cost: {approx_max_dist:.3f})")
    plot_single(ax2, points, approx_labels, approx_centers)

    fig.suptitle(
        f"Clustering comparision ({len(points)} points, {len(opt_centers)} centers)"
    )
    plt.savefig(str(fig_path))
    plt.close(fig)


def apply_labels_sorted(centers, labels):
    return {
        cur_label: new_label
        for new_label, cur_label in enumerate(centers[:, 1].argsort())
    }


def run(
    exec_path: Path,
    results_dir: Path,
    run_id: str,
    points: np.ndarray,
    opt_labels,
    opt_centers,
    do_plot=True,
):
    opt_max_dist = max(
        dist(point, opt_centers[label]) for point, label in zip(points, opt_labels)
    )

    output = run_clustering(exec_path, points, len(opt_centers))

    # TODO: Fix mismatch of labels
    approx_centers, approx_labels, approx_dist, runtime = parse_output(
        output, len(points), len(opt_centers)
    )
    approx_max_dist = max(approx_dist)

    if do_plot:
        opt_labels_map = apply_labels_sorted(opt_centers, opt_labels)
        approx_labels_map = apply_labels_sorted(approx_centers, approx_labels)
        indices = np.random.permutation(len(opt_centers))
        shuf_opt_labels = np.array([indices[opt_labels_map[x]] for x in opt_labels])
        shuf_approx_labels = np.array(
            [indices[approx_labels_map[x]] for x in approx_labels]
        )
        plot(
            results_dir / "images" / run_id,
            points,
            shuf_opt_labels,
            opt_centers,
            opt_max_dist,
            shuf_approx_labels,
            approx_centers,
            approx_max_dist,
        )

    np.savez(
        results_dir / "data" / run_id,
        points=points,
        opt_labels=opt_labels,
        opt_centers=opt_centers,
        approx_labels=approx_labels,
        approx_centers=approx_centers,
    )
    return opt_max_dist, approx_max_dist, runtime


def gen_log_scale(start=1, stop=1e9, scale=10):
    step = start
    while start <= stop:
        yield start
        if start == step * scale:
            step *= scale
        start += step


def gen_imp_nums(start=10, stop=1e7, scale=10):
    while start * scale <= stop:
        yield start
        yield start * 2
        yield start * 5
        start *= scale


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("exec_path", type=Path, help="Path to clustering executable")
    parser.add_argument(
        "--results_dir",
        type=Path,
        default=(Path.cwd() / "results"),
        help="Path where results will be stored",
    )
    """
    parser.add_argument("--num_iters", type=int, default=10, help="Number of iterations")
    points_group = parser.add_mutually_exclusive_group(required=True)
    exact_group = points_group.add_argument_group()
    exact_group.add_argument("-n", "--num_points", type=int, default=100, help="Number of points")
    exact_group.add_argument("-k", "--num_centers", type=int, default=3, help="Number of centers")
    range_group = points_group.add_argument_group()
    range_group.add_argument("--min_points", type=int, default=1, help="Minimum number of points")
    range_group.add_argument("--max_points", type=int, default=1e7, help="Maximum number of points")
    range_group.add_argument("--min_centers", type=int, default=1, help="Minimum number of centers")
    range_group.add_argument("--max_centers", type=int, default=1e3, help="Maximum number of centers")
    """
    args = parser.parse_args()
    (args.results_dir / "data").mkdir(parents=True, exist_ok=True)
    (args.results_dir / "images").mkdir(exist_ok=True)
    results_file = args.results_dir / "results.csv"
    if not results_file.exists():
        # write headers if file is new
        with open(results_file, "w") as f:
            w = csv.writer(f)
            w.writerow(
                (
                    "id",
                    "num_points",
                    "num_centers",
                    "opt_max_dist",
                    "approx_max_dist",
                    "runtime",
                )
            )
        run_id = 1
    else:
        run_id = next_run_id(results_file)

    # for num_points, imp_point in gen_log_scale(start=10, stop=1e5):
    #     for num_centers, imp_center in gen_log_scale(stop=num_points / 10, scale=10):

    for num_points in gen_imp_nums():
        for num_centers in gen_imp_nums(stop=num_points // 2):
            points, opt_labels, opt_centers = gen_clusters_exact(
                num_points, num_centers
            )
            opt_max_dist, approx_max_dist, runtime = run(
                args.exec_path,
                args.results_dir,
                f"{run_id:08}",
                points,
                opt_labels,
                opt_centers,
                do_plot=(num_points < 1e4 and num_centers < num_points / 10),
            )
            with open(results_file, "a") as f:
                w = csv.writer(f)
                w.writerow(
                    (
                        f"{run_id:08}",
                        num_points,
                        num_centers,
                        opt_max_dist,
                        approx_max_dist,
                        runtime,
                    )
                )
            run_id += 1


if __name__ == "__main__":
    main()
