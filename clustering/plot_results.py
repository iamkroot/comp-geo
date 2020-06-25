import matplotlib
import numpy as np
from collections import defaultdict
# matplotlib.use("agg")
matplotlib.use("pgf")
matplotlib.rcParams.update({
    "pgf.texsystem": "pdflatex",
    'font.family': 'serif',
    'text.usetex': True,
    'pgf.rcfonts': False,
})
import matplotlib.pyplot as plt

from pathlib import Path
import csv

results_file = Path("results") / "results.csv"


with open(results_file) as f:
    reader = csv.DictReader(f)
    data = list(reader)


def split(data, x, y, on):
    output = defaultdict(list)
    for row in data:
        output[int(row[on])].append((int(row[x]), int(row[y])))
    return {k: np.array(v) for k, v in output.items()}

x = "num_centers"
s = "num_points"

d = split(data, x, "runtime", s)
fig, ax = plt.subplots()

for k, v in d.items():
    line = ax.plot(v[:, 0], v[:,1], ".-", label=k)

ax.set_xscale('log')
ax.set_yscale('log')
plt.xlabel(x.replace("_", " "))
plt.ylabel(r"runtime ($\mu$s)")
lgd = plt.legend(title=s.replace("_", " "), loc='center left', bbox_to_anchor=(1, 0.5))
plt.savefig("temp", bbox_extra_artists=[lgd], bbox_inches='tight')
