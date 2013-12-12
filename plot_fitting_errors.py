#!/usr/bin/python

import matplotlib.pyplot as plt
from matplotlib import cm
import pylab
import numpy

data_array = numpy.genfromtxt("fitting_log.csv", delimiter=',', names=True)

figure = plt.figure(figsize=(12,6))

ax1 = plt.subplot(1, 2, 1)

marker_count = len(data_array.dtype.names)
marker_names = data_array.dtype.names
marker_colors = [None] * marker_count

average_errors = numpy.zeros(marker_count)
for col_name in sorted(data_array.dtype.names):
    marker_index = data_array.dtype.names.index(col_name)
    average_errors[marker_index] = numpy.average(data_array[col_name])
    marker_colors[marker_index] = cm.jet (1. * marker_index / marker_count)

bar_pos = numpy.arange(marker_count) + 0.5
rects = ax1.bar(bar_pos, average_errors, align='center', color=marker_colors)

ax1.axhline(numpy.average(average_errors), label="average", color="red")

pylab.xticks(bar_pos, marker_names, rotation=90)
ax1.legend()
ax1.set_title ('Average Marker Errors')
ax1.set_ylabel ('(m)')
pylab.xlim([0, marker_count])
pylab.ylim([0, 0.035])

ax2 = plt.subplot(1, 2, 2)

for col_name in sorted(data_array.dtype.names):
    marker_index = data_array.dtype.names.index(col_name)
    plt.plot(data_array[col_name], color=marker_colors[marker_index])
    pylab.xlim([0, len(data_array[col_name])])

ax2.set_title ('Marker Error per Capture Frame')
ax2.set_ylabel ('(m)')
pylab.ylim([0, 0.035])

plt.subplots_adjust(left=0.08, right = 0.98)
plt.show()