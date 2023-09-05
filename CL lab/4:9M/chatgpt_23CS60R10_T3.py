from ortools.linear_solver import pywraplp

def solve_n_queens(n):
    solver = pywraplp.Solver.CreateSolver('SCIP')

    queens = []
    for i in range(n):
        row = []
        for j in range(n):
            row.append(solver.IntVar(0, 1, f'x_{i}_{j}'))
        queens.append(row)
    
    for i in range(n):
        solver.Add(solver.Sum(queens[i]) == 1)

    for j in range(n):
        column_sum = solver.Sum(queens[i][j] for i in range(n))
        solver.Add(column_sum == 1)

    for i in range(n):
        for j in range(n):
            left_diagonal = []
            x, y = i, j
            while x >= 0 and y >= 0:
                left_diagonal.append(queens[x][y])
                x -= 1
                y -= 1
            if left_diagonal:
                solver.Add(solver.Sum(left_diagonal) <= 1)

    
    for i in range(n):
        for j in range(n):
            right_diagonal = []
            x, y = i, j
            while x >= 0 and y < n:
                right_diagonal.append(queens[x][y])
                x -= 1
                y += 1
            if right_diagonal:
                solver.Add(solver.Sum(right_diagonal) <= 1)



    objective = solver.Objective()
    for i in range(n):
        for j in range(n):
            objective.SetCoefficient(queens[i][j], 1)

    objective.SetMaximization()
    solver.Solve()

    solution = []
    for i in range(n):
        row = []
        for j in range(n):
            if queens[i][j].solution_value() == 1:
                row.append('Q')
            else:
                row.append('_')
        solution.append(row)

    return solution




n = int(input("Enter the board size (n): "))
solution = solve_n_queens(n)
for row in solution:
    print(' '.join(row))

