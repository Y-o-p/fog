import matplotlib.pyplot as plt
import numpy as np
import argparse

def speedup(sample_points, counts):
    fig, ax = plt.subplots()

def bars(processors, counts):
    fig, ax = plt.subplots()
    ax.bar(processors, counts, label=processors)
    ax.set_ylabel("Mean FPS")
    ax.set_title("Mean FPS of All Implementations")
    ax.set_yscale("log")
    ax.legend()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--names", type=str, nargs='+')
    parser.add_argument("-c", "--counts", type=float, nargs='+')
    parser.add_argument("-s", "--samples", type=int, nargs='+')
    args = parser.parse_args()

    # Sort the information
    sorted_names = [name for _, name in sorted(zip(args.counts, args.names))]
    sorted_counts = sorted(args.counts)

    bars(sorted_names, sorted_counts)
    speedup(sorted_names, sorted_counts)
    plt.show()