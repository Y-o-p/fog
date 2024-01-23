import matplotlib.pyplot as plt
import numpy as np
import argparse

def speedup(processors, sample_points, counts):
    fig, ax = plt.subplots((1, 2))
    for name in processors:
        ax[0].plot(sample_points, counts)

def bars(processors, counts):
    fig, ax = plt.subplots()
    colors = ["tab:blue", "tab:green", "tab:orange", "tab:red"]
    rects = ax.bar(processors, counts, label=processors, color=colors)
    ax.bar_label(rects, padding=3)
    ax.set_ylim(.05, 10000.0)
    ax.set_ylabel("Mean FPS")
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
    sorted_names = [name for _, name in sorted(zip(args.counts, args.names))]
    sorted_counts = sorted(args.counts)

    bars(sorted_names, sorted_counts)
    #speedup(sorted_names, sorted_counts)
    plt.show()