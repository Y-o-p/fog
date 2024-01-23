import matplotlib.pyplot as plt
import numpy as np
import argparse

def speedup(processors, sample_points, counts):
    fig, ax = plt.subplots(1, 2)
    minimum = np.min(counts)
    for i, name in enumerate(processors):
        ax[0].plot(sample_points, counts[i] / np.min(counts[1]))
        ax[0].set_xticks([64, 128, 256, 512])
    for i, name in enumerate(processors):
        ax[1].plot(sample_points, counts[i] / np.min(counts[i]))
        ax[0].set_xticks([64, 128, 256, 512])

def bars(processors, sample_points, counts):
    fig, ax = plt.subplots()
    colors = ["tab:red", "tab:orange", "tab:blue", "tab:green"]
    multiplier = 0
    width = 0.2
    x = np.arange(len(sample_points))
    for i, name in enumerate(processors):
        offset = width * multiplier
        rects = ax.bar(x + offset, counts[i], width, label=name)
        ax.bar_label(rects, padding=4)
        multiplier += 1
    ax.set_ylim(.001, 1000.0)
    ax.set_ylabel("Mean FPS")
    ax.set_xticks(x + width + 0.1, sample_points)
    ax.set_xlabel("Sample Points Per Ray")
    ax.set_title("Mean FPS of All Implementations")
    ax.set_yscale("log")
    ax.legend()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--names", type=str, nargs='+')
    parser.add_argument("-c", "--counts", type=float, nargs='+')
    parser.add_argument("-s", "--samples", type=int, nargs='+', default=[256])
    args = parser.parse_args()

    # Sort the information
    # sorted_names = [name for _, name in sorted(zip(args.counts, args.names))]
    # sorted_counts = sorted(args.counts)

    # bars(sorted_names, sorted_counts)
    names = np.array(["gpu", "cpuO3", "cpuO2", "cpu"])
    names = np.flip(names)
    speedup_counts = np.array([[49.1862, 89.6334, 164.545, 279.168],\
                               [0.652227, 1.37443, 2.63114, 5.16624],\
                               [0.300938, 0.597095, 1.21082, 2.4019],\
                               [0.0426892, 0.0863052, 0.173265, 0.341076]])
    speedup_counts = np.flip(speedup_counts)
    sample_points = np.array([64, 128, 256, 512])
    bars(names, sample_points, speedup_counts)
    plt.show()