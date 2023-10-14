from typing import List
# import itertools

"""
Calculating the distance between the regions in adjacency list `mat`
"""
def total_distance(mat, path):

    dist = 0
    for i in range(len(path) - 1):
        dist += mat[path[i]][path[i + 1]]

    return dist

"""
Doing the job that serves quite same as `itertools.permutations()`
"""
def generate_permutations(arr):
    if len(arr) == 0:
        return [[]]
    
    permutations = []
    # generate permutations
    for i in range(len(arr)):
        first_elem = arr[i]
        rest = arr[:i] + arr[i+1:]
        for p in generate_permutations(rest):
            permutations.append([first_elem] + p)
    
    return permutations

"""`The Brute Force for homework problem
Args:
    mat (List[List[int]]): The adjacency list of the distances between the regions.
Return:
    shortest_path (List[int]): The current shortest path after Brute Force.
    shortest_distance (int): The current shortest distance after Brute Force.
"""
def BF(mat):

    # list the nodes without start and end points
    unvisited_nodes = list(range(1, len(mat)))

    # Generate every possible permutations between regions
    # 參考：permutations = list(itertools.permutations(unvisited_nodes))
    permutations = generate_permutations(unvisited_nodes)

    # init. properties in shortest path
    shortest_path = [0] + unvisited_nodes + [0]
    shortest_distance = total_distance(mat, shortest_path)

    # comparing which path is the shortest
    for perm in permutations:
        # generate path from our permutations
        path = [0] + perm + [0]
        # getting total distance
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