import random
import math
from typing import List

def BF(mat: List[List[int]]):

    # initialization
    best_solution = None
    best_distance = float('inf')
    stack = [int(i) for i in range(len(mat))]
    stack.append(0)

    while stack:
        element = stack.pop()

if __name__ == "__main__":

    # initial variables
    mat =  [# A, B, C, D, E
            [ 0, 1, 9, 8, 40], # A
            [ 1, 0, 2, 35, 50], # B
            [ 9, 2, 0, 30, 10], # C
            [ 8, 35, 30, 0, 5], # D
            [40, 50, 10, 5, 0], # E
           ] # be seen as an adjacency list
    
    best_path, best_dist = BF(mat)
    print("最短路徑：", best_path) # 最短路徑：[0, 1, 2, 4, 3, 0]
    print("最短距離：", best_dist) # 最短距離：26
