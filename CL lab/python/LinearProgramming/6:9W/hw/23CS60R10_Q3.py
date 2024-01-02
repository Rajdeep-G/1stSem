
from ortools.linear_solver import pywraplp

def maximize_sweet_box(no_sweets, m, n, x, y, cost):
    solver = pywraplp.Solver.CreateSolver('SCIP')

    var = []
    for i in range(no_sweets):
        var.append(solver.IntVar(0, 1, 'x_%i' % i))


    ct = solver.Constraint(0, m * n,"ct")
    for i in range(no_sweets):
        ct.SetCoefficient(var[i], x[i] * y[i])
    
    # for i in range(no_sweets):
    #     solver.Add(var[i]*x[i]<=m)
    #     solver.Add(var[i]*y[i]<=n)

    ct= solver.Constraint(0, m,"ct")
    for i in range(no_sweets):
        ct.SetCoefficient(var[i], x[i])
        
    ct= solver.Constraint(0, n,"ct")
    for i in range(no_sweets):
        ct.SetCoefficient(var[i], y[i])

    obj = solver.Objective()
    for i in range(no_sweets):
        obj.SetCoefficient(var[i], cost[i])

    obj.SetMaximization()
    solver.Solve()

    max_sweet_cost = obj.Value()

    sweets_used = []
    for i in range(no_sweets):
        if var[i].solution_value() > 0:
            print('Sweet', i + 1, 'used', var[i].solution_value(), 'times')
            sweets_used.append(var[i].solution_value())
        else:
            sweets_used.append(0)

    return max_sweet_cost, sweets_used

with open('input.txt', 'r') as file:
    no_sweets = int(file.readline())
    m,n=map(int,file.readline().split())
    x_cor = list(map(float, file.readline().split()))
    y_cor =list(map(float, file.readline().split()))
    cost=list(map(float, file.readline().split()))


max_sweet_cost, sweets_used = maximize_sweet_box(no_sweets, m, n, x_cor, y_cor, cost)

def print_used_sweets(sweets_used, x_cor, y_cor):
    for i in range(len(sweets_used)):
        print(f"Size of sweet {i + 1}: {sweets_used[i]*x_cor[i]*y_cor[i]}")
    print(f"Total cost: {max_sweet_cost}")

    with open ('Summary_Q3.txt', 'w') as file:
        for i in range(len(sweets_used)):
            file.write(f"Size of sweet {i + 1}: {sweets_used[i]*x_cor[i]*y_cor[i]}\n")
        file.write(f"Total cost: {max_sweet_cost}")

print_used_sweets(sweets_used, x_cor, y_cor)