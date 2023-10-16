import pandas
import sys
import matplotlib.pyplot as plt


file_path = sys.argv[1]
graph_data_frame = pandas.read_csv(file_path)
x = graph_data_frame.iloc[:,0].tolist()
y = graph_data_frame.iloc[:,1].tolist()

plt.scatter(x,y)
ax = plt.gca()
ax.set_aspect('equal', adjustable='box')
plt.show()