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

all_speedup_means = []
all_speedup_err = []
all_cpus = []
all_labels = []
all_commands = []


res_files = [file for file in os.listdir(res_dir) if file.endswith(".json")]
if len(res_files) == 0:
   print("No .json results found in ", res_dir)
   exit()


for file in res_files:
   print("   ", file)
   name = file.replace("results-", "").replace(".json", "")
   with open(res_dir+"/"+file) as of:
      res = json.load(of)["results"]

   commands = [r["command"] for r in res]
   times    = [r["times"] for r in res]

   plt.figure(figsize = (16,9), constrained_layout = True)
   plt.title(name)
   plt.boxplot(times)
   plt.xticks(list(range(1,len(commands)+1)), commands, rotation=90)
   plt.ylabel("Runtime (s)")

   # For normalized plots, divide the first mean out of the timings:
   first_mean =  sum(times[0])/len(times[0])
   scaled_times = [[t/first_mean for t in runtimes] for runtimes in times]
   plt.figure(figsize = (16,9), constrained_layout = True)
   plt.title(name)
   plt.boxplot(scaled_times)
   plt.xticks(list(range(1,len(commands)+1)), commands, rotation=90)
   plt.ylabel("Normalized Runtime")

   # For speedup plots, divide the first mean by the timings:
   speedup_raw = [[first_mean/t for t in runtimes] for runtimes in times]
   speedup_means = [statistics.mean(su) for su in speedup_raw]
   speedup_err = [statistics.stdev(su) for su in speedup_raw]
   # Extract worker counts from commands
   cpus = []
   for l in commands:
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
   plt.xticks(cpus)
   plt.yticks(speedup_ideal)
   plt.ylabel("Parallel speedup w.r.t "+commands[0])
   plt.xlabel("Workers")

   p = pdf(res_dir+"/plot-"+name+".pdf")
   figs = [plt.figure(num) for num in plt.get_fignums()]
   for fig in figs:
      fig.savefig(p, format="pdf")
      fig.clear()
      plt.close(fig)
   p.close()

   # Store data for combined plot
   all_speedup_means.append(speedup_means)
   all_speedup_err.append(speedup_err)
   all_cpus.append(cpus)
   all_labels.append(name)
   all_commands.append(commands[0])



print("Create combined scaling plot...")

# Order data for nice legend
ordering = [i[0] for i in sorted(enumerate(all_labels), key=lambda x:x[1])]
all_cpus = [all_cpus[i] for i in ordering]
all_speedup_means = [all_speedup_means[i] for i in ordering]
all_speedup_err = [all_speedup_err[i] for i in ordering]
all_labels = [all_labels[i] for i in ordering]
all_commands = [all_commands[i] for i in ordering]

speedup_ideal = [int(cpu/min(all_cpus[0])) for cpu in all_cpus[0]]
plt.figure(figsize = (16,9), constrained_layout = True)
plt.xticks(all_cpus[0])
plt.yticks(speedup_ideal)
plt.ylabel("Parallel speedup w.r.t "+str(all_commands[0]))
plt.xlabel("Workers")
plt.plot(all_cpus[0], speedup_ideal, linestyle=":", color="maroon", label="ideal")
all_commands.append("ideal")
for i in range(0,len(all_cpus)):
   plt.errorbar(all_cpus[i], all_speedup_means[i], yerr=all_speedup_err[i], fmt="-", capsize=5, capthick=2, label=all_labels[i])
   print("   ", all_labels[i])
plt.legend(loc="upper left")
plt.tick_params(width=2, grid_linewidth=1)
plt.savefig(res_dir+"/plot-combined.pdf")
