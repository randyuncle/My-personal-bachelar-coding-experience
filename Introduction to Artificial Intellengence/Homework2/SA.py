import numpy as np
import random
import math
from typing import List

"""
Calculating the distance between the regions in adjacency list `mat`
"""
def total_distance(mat, path):

    dist = 0
    for i in range(len(path) - 1):
        dist += mat[path[i]][path[i + 1]]

    return dist

"""
Random changing two elements in the path list to generate new path
"""
def random_move(curr_path):

    new_path = curr_path.copy()
    i = random.randint(1, len(curr_path) - 2)
    j = random.randint(1, len(curr_path) - 2)
    new_path[i], new_path[j] = new_path[j], new_path[i]

    return new_path

"""`The simulated annealing algorithm for homework problem
Args:
    mat (List[List[int]]): The adjacency list of the distances between the regions.
    init_path (List[int]): The initial path that we chose to throw in this algo.
    init_temp (float): The initial temperature of the simulated annealing algorithm.
    cooling_rate (float): The rate to cool down the temperature, enabling to 
        make the algo. stop after a certain amount of loops.
Return:
    best_solution (List[int]): The current best solution after simulated annealing.
    best_energy (int): The current best distance after simulated annealing.
"""
def SA(
    mat: List[List[int]],
    init_path: List[int],
    init_temp: float,
    cooling_rate: float,
):
    
    curr_solution = init_path
    curr_energy = total_distance(mat, curr_solution)
    best_solution = curr_solution
    best_energy = curr_energy
    temperature = init_temp

    while temperature > 1:
        new_solution = random_move(curr_solution)
        new_energy = total_distance(mat, new_solution)
        energy_delta = curr_energy - new_energy

        if energy_delta > 0 or math.exp(energy_delta / temperature) > random.random():
            curr_solution = new_solution
            curr_energy = new_energy

            if curr_energy > best_energy:
                best_solution = curr_solution
                best_energy = curr_energy

        temperature *= cooling_rate

    return best_solution, best_energy

"""
The main function
"""
if __name__ == "__main__":

    # initial variables
    mat =  [# A, B, C, D, E
            [ 0, 1, 9, 8, 40], # A
            [ 1, 0, 2, 35, 50], # B
            [ 9, 2, 0, 30, 10], # C
            [ 8, 35, 30, 0, 5], # D
            [40, 50, 10, 5, 0], # E
           ] # be seen as an adjacency list
    init_path = [0, 1, 2, 3, 4, 0]
    init_temp = 1000.0
    cooling_rate = 0.99

    best_path, best_dist = SA(mat, init_path, init_temp, cooling_rate)
    print("最短路徑：", best_path) # 最短路徑：[0, 1, 2, 4, 3, 0]
    print("最短距離：", best_dist) # 最短距離：26