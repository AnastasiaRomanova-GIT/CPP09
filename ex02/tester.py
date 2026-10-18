#!/usr/bin/env python3

import math
import random
import re
import statistics
import subprocess
import sys
from pathlib import Path


# ============================================================
# HEADER / CONFIGURATION
# ============================================================

# Number of integers generated for each PmergeMe run.
N = 21

# Number of independent PmergeMe runs.
K = 1000

# Generated positive integer range.
RANDOM_MIN = 1
RANDOM_MAX = 100

# Use distinct values. This keeps the comparison test closer to the
# usual Ford-Johnson analysis and avoids duplicate-output ambiguity.
UNIQUE_NUMBERS = True

# Set to an integer for reproducible tests. Use None for fresh data.
RANDOM_SEED = None

# Statistical confidence for the "missed violation" estimate.
CONFIDENCE = 0.95

# Project paths. The script is meant to live in project09/ex02.
ROOT = Path(__file__).resolve().parent
EXECUTABLE = ROOT / "PmergeMe"


# ============================================================
# REAL PMERGEME OUTPUT PARSING
# ============================================================

COMPARISON_PATTERN = re.compile(r"Comparisons Number:\s*(\d+)")
AFTER_PATTERN = re.compile(r"^After:\s*(.*)$", re.MULTILINE)


def parse_int_sequence(text):
    if text.strip() == "":
        return []
    return [int(token) for token in text.split()]


def parse_pmergeme_output(output):
    after_match = AFTER_PATTERN.search(output)
    comparison_matches = COMPARISON_PATTERN.findall(output)

    if after_match is None:
        raise ValueError("Could not find the After line in PmergeMe output.")

    if len(comparison_matches) != 2:
        raise ValueError(
            "Expected exactly two comparison counters, got {}.".format(
                len(comparison_matches)
            )
        )

    sorted_values = parse_int_sequence(after_match.group(1))
    vector_comparisons = int(comparison_matches[0])
    list_comparisons = int(comparison_matches[1])

    return sorted_values, vector_comparisons, list_comparisons


# ============================================================
# BUILD
# ============================================================

def build_counter_binary():
    process = subprocess.run(
        ["make", "count_operations"],
        cwd=str(ROOT),
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    )

    if process.returncode != 0:
        print("Could not build PmergeMe with comparison counters.")
        print(process.stdout)
        sys.exit(1)


# ============================================================
# FORD-JOHNSON THEORETICAL WORST-CASE
# ============================================================

def ford_johnson_max_comparisons(n):
    total = 0

    for k in range(1, n + 1):
        total += math.ceil(math.log2((3.0 * k) / 4.0))

    return total


# ============================================================
# RANDOM INPUT
# ============================================================

def generate_numbers():
    if UNIQUE_NUMBERS:
        available = RANDOM_MAX - RANDOM_MIN + 1

        if N > available:
            raise ValueError(
                "N is larger than the number of unique values available."
            )

        return random.sample(range(RANDOM_MIN, RANDOM_MAX + 1), N)

    return [random.randint(RANDOM_MIN, RANDOM_MAX) for _ in range(N)]


# ============================================================
# RUN PMERGEME
# ============================================================

def run_pmergeme(numbers):
    command = [str(EXECUTABLE)]
    command.extend(str(number) for number in numbers)

    process = subprocess.run(
        command,
        cwd=str(ROOT),
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    )

    if process.returncode != 0:
        raise RuntimeError(
            "PmergeMe exited with {}.\n{}".format(
                process.returncode,
                process.stdout
            )
        )

    sorted_values, vector_count, list_count = parse_pmergeme_output(
        process.stdout
    )

    expected = sorted(numbers)
    if sorted_values != expected:
        raise AssertionError(
            "PmergeMe sorting mismatch.\n"
            "Input:    {}\n"
            "Expected: {}\n"
            "Actual:   {}\n\n"
            "Program output:\n{}".format(
                " ".join(str(x) for x in numbers),
                " ".join(str(x) for x in expected),
                " ".join(str(x) for x in sorted_values),
                process.stdout
            )
        )

    return vector_count, list_count


# ============================================================
# STATISTICS
# ============================================================

def unseen_event_upper_bound(runs, confidence):
    alpha = 1.0 - confidence
    return 1.0 - alpha ** (1.0 / runs)


def row_stats(values, theoretical_max):
    above = sum(value > theoretical_max for value in values)

    return {
        "average": statistics.mean(values),
        "minimum": min(values),
        "maximum": max(values),
        "above": above,
        "missed_upper": (
            unseen_event_upper_bound(len(values), CONFIDENCE)
            if above == 0
            else None
        ),
    }


def print_result_row(name, values, theoretical_max):
    stats = row_stats(values, theoretical_max)

    if stats["missed_upper"] is None:
        missed_text = "observed {:.4%}".format(
            float(stats["above"]) / float(len(values))
        )
    else:
        missed_text = "<= {:.4%}".format(stats["missed_upper"])

    print(
        "{:<10} {:>10.2f} {:>10} {:>10} {:>12} {:>18}".format(
            name,
            stats["average"],
            stats["minimum"],
            stats["maximum"],
            stats["above"],
            missed_text
        )
    )


# ============================================================
# MAIN
# ============================================================

def main():
    if RANDOM_SEED is not None:
        random.seed(RANDOM_SEED)

    build_counter_binary()

    theoretical_max = ford_johnson_max_comparisons(N)
    vector_results = []
    list_results = []

    print("Testing real PmergeMe sorting and comparison counters")
    print("N = {}, K = {}".format(N, K))
    print("Executable: {}".format(EXECUTABLE))
    print(
        "Theoretical Ford-Johnson upper comparison count for N={}: {}".format(
            N,
            theoretical_max
        )
    )
    print()

    for run in range(1, K + 1):
        numbers = generate_numbers()

        try:
            vector_count, list_count = run_pmergeme(numbers)
        except Exception as error:
            print("Failure on run {} / {}.".format(run, K))
            print(error)
            sys.exit(1)

        vector_results.append(vector_count)
        list_results.append(list_count)

        if K >= 100 and run % max(1, K // 10) == 0:
            print("Completed {:>6} / {:>6} runs".format(run, K))

    print()
    print("=" * 90)
    print("COMPARISON TABLE")
    print("=" * 90)
    print(
        "{:<10} {:>10} {:>10} {:>10} {:>12} {:>18}".format(
            "Container",
            "Average",
            "Minimum",
            "Maximum",
            "Above bound",
            "Missed-above risk"
        )
    )
    print("-" * 90)

    print_result_row("Vector", vector_results, theoretical_max)
    print_result_row("List", list_results, theoretical_max)

    print()
    print(
        "Missed-above risk: if no run exceeded the theoretical number, "
        "this is the {:.0f}% one-sided upper bound for the probability "
        "that one random input would exceed it.".format(CONFIDENCE * 100)
    )

    differences = sum(
        vector != list_
        for vector, list_ in zip(vector_results, list_results)
    )

    print()
    print(
        "Vector/list comparison counts differed in {} out of {} runs.".format(
            differences,
            K
        )
    )


if __name__ == "__main__":
    main()
