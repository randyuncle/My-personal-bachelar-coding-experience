import random
import math
from typing import List
import itertools

"""
Calculating the distance between the regions in adjacency list `mat`
"""
def total_distance(mat, path):

    dist = 0
    for i in range(len(path) - 1):
        dist += mat[path[i]][path[i + 1]]

    return dist

"""`The Brute Force for homework problem
Args:
    mat (List[List[int]]): The adjacency list of the distances between the regions.
Return:
    shortest_path (List[int]): The current shortest path after Brute Force.
    shortest_distance (int): The current shortest distance after Brute Force.
"""
def BF(mat):

    # list the node without start and end points
    unvisited_nodes = list(range(1, len(mat)))
    # using permutations in itertools to generate every possible combinations between regions
    permutations = list(itertools.permutations(unvisited_nodes))

    # init. properties in shortest path
    shortest_path = [0] + unvisited_nodes + [0]
    shortest_distance = total_distance(mat, shortest_path)

    for perm in permutations:
        path = [0] + list(perm) + [0]
        distance = total_distance(mat, path)
        if distance < shortest_distance:
            shortest_path = path
            shortest_distance = distance

    return shortest_path, shortest_distance

if __name__ == "__main__":

    # initial variables
    mat =  [# A, B, C, D, E
            [ 0, 1, 9, 8, 40], # A
            [ 1, 0, 2, 35, 50], # B
            [ 9, 2, 0, 30, 10], # C
            [ 8, 35, 30, 0, 5], # D
            [40, 50, 10, 5, 0], # E
           ] # be seen as an adjacency matrix
    
    best_path, best_dist = BF(mat)
    print("最短路徑：", best_path) # 最短路徑：[0, 1, 2, 4, 3, 0]
    print("最短距離：", best_dist) # 最短距離：26
