"""Create comparison plots for benchmarking run"""

import os
import re
import json
import argparse
import statistics
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages as pdf

plt.rcParams["font.size"] = 22
plt.rcParams["lines.linewidth"] = 3
plt.rcParams["axes.linewidth"] = 3
plt.rcParams["grid.linewidth"] = 2
plt.rcParams["boxplot.capprops.linewidth"] = 3
plt.rcParams["boxplot.boxprops.linewidth"] = 3
plt.rcParams["boxplot.flierprops.linewidth"] = 3
plt.rcParams["boxplot.medianprops.linewidth"] = 3
plt.rcParams["axes.grid"] = True

arg_parser = argparse.ArgumentParser()
arg_parser.add_argument("res_dir")
res_dir = arg_parser.parse_args().res_dir
print("Create plots for results: ", res_dir)

res_files = [file for file in os.listdir(res_dir) if file.endswith(".json")]
for file in res_files:
   print("   ", file)
   name = file.replace("results-", "").replace(".json", "")
   with open(res_dir+"/"+file) as of:
      res = json.load(of)["results"]

   labels = [r["command"] for r in res]
   times  = [r["times"] for r in res]

   plt.figure(figsize = (16,9), constrained_layout = True)
   plt.title(name)
   plt.boxplot(times)
   plt.xticks(list(range(1,len(labels)+1)), labels, rotation=90)
   plt.ylabel("Runtime (s)")

   # For normalized plots, divide the first mean out of the timings:
   first_mean =  sum(times[0])/len(times[0])
   scaled_times = [[t/first_mean for t in runtimes] for runtimes in times]
   plt.figure(figsize = (16,9), constrained_layout = True)
   plt.title(name)
   plt.boxplot(scaled_times)
   plt.xticks(list(range(1,len(labels)+1)), labels, rotation=90)
   plt.ylabel("Normalized Runtime")

   # For speedup plots, divide the first mean by the timings:
   speedup_raw = [[first_mean/t for t in runtimes] for runtimes in times]
   speedup_means = [statistics.mean(su) for su in speedup_raw]
   speedup_err = [statistics.stdev(su) for su in speedup_raw]
   # Extract worker counts from labels
   cpus = []
   for l in labels:
      if l.startswith("form"):
         cpus.append(1)
      elif l.startswith("tform"):
         cpu = re.findall(r"-w(\d+)", l)
         cpus.append(int(cpu[0]))
      else:
         print("Could not determine workers in ", l)
         cpus.append(0)
   speedup_ideal = [int(cpu/min(cpus)) for cpu in cpus]

   plt.figure(figsize = (16,9), constrained_layout = True)
   plt.title(name)
   plt.plot(cpus, speedup_ideal, linestyle=":", color="maroon")
   plt.errorbar(cpus[0:1], speedup_means[0:1], yerr=speedup_err[0:1], fmt="o", capsize=5, capthick=2, color="red")
   plt.errorbar(cpus[1:], speedup_means[1:], yerr=speedup_err[1:], fmt="o", capsize=5, capthick=2)
   plt.xticks(range(min(cpus),max(cpus)+1))
   plt.yticks(range(min(speedup_ideal),max(speedup_ideal)+1))
   plt.ylabel("Parallel speedup w.r.t "+labels[0])
   plt.xlabel("Workers")

   p = pdf(res_dir+"/plot-"+name+".pdf")
   figs = [plt.figure(num) for num in plt.get_fignums()]
   for fig in figs:
      fig.savefig(p, format="pdf")
      fig.clear()
      plt.close(fig)
   p.close()

