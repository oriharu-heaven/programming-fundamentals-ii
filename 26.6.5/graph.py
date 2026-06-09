import matplotlib
matplotlib.use("Agg") # needed to prevent segmentation fault in some environment
import matplotlib.pyplot as plt
import sys

if len(sys.argv) != 2:
    print("give one argument, file path to the CSV with 2 coloumns")
    quit()
path = sys.argv[1]
f = open(path, "r")
lx = []
ly = []
i = 0
for line in f:
    if i == 0:
        i = 1
        continue;
    line = line.strip();
    x, y = line.split(",");
    lx.append(float(x))
    ly.append(float(y))
    
plt.scatter(lx, ly, marker="o", s=1, label='f(x)')
plt.gca().set_aspect("equal", adjustable="box")
#
## 3. Add labels, title, and grid
plt.xlabel('x')
plt.ylabel('y')
plt.grid(True)
plt.legend()
#
## 4. Save the figure as an SVG file
plt.savefig(path + "-out.svg")
