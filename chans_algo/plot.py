import numpy as np
import pandas as pd
import matplotlib
matplotlib.use("pgf")
matplotlib.rcParams.update({
    "pgf.texsystem": "pdflatex",
    'font.family': 'serif',
    'text.usetex': True,
    'pgf.rcfonts': False,
})
matplotlib.pyplot.style.use("ggplot")

n = 100000

df = [None] * 3

df[0] = pd.read_csv('results/chan_results.csv')
df[1] = pd.read_csv('results/graham_results.csv')
df[2] = pd.read_csv('results/jarvis_results.csv')

h = [None] * 3
rt = [None] * 3

for i in range(3):
    df[i] = df[i][df[i]['n'] == n]
    h[i] = df[i]['h'].values
    rt[i] = df[i]['runtime'].values

labels = ["Chan's", "Graham Scan", "Jarvis March"]

for i in [0, 1]:
    matplotlib.pyplot.plot(h[i], rt[i], label=labels[i])

matplotlib.pyplot.xlabel("h")
matplotlib.pyplot.ylabel("time (us)")
matplotlib.pyplot.xscale("log")
matplotlib.pyplot.title(f"Running time comparison for n = {n}")
matplotlib.pyplot.legend()
# matplotlib.pyplot.show()
matplotlib.pyplot.savefig(f'results/plots/n{n}.eps', format='eps')
