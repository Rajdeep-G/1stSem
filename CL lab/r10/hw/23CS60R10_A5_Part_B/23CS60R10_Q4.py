
from ortools.linear_solver import pywraplp

def maximize_sweet_box(no_sweets, m, n, x, y, cost):
    solver = pywraplp.Solver.CreateSolver('SCIP')

    var = []
    for i in range(no_sweets):
        var.append(solver.NumVar(0.0, 1.0, 'x_%i' % i))

    objective = solver.Objective()
    cost_per_unit_area = []
    for i in range(no_sweets):
        cost_per_unit_area.append(cost[i] / (x[i] * y[i]))
    for i in range(no_sweets):
        objective.SetCoefficient(var[i], cost_per_unit_area[i]*x[i]*y[i])

    objective.SetMaximization()


    # box_constraint_m = solver.Constraint(0, m)
    # box_constraint_n = solver.Constraint(0, n)
    box_constraint_area = solver.Constraint(0, m * n)

    for i in range(no_sweets):
        # box_constraint_m.SetCoefficient(var[i], x[i])
        # box_constraint_n.SetCoefficient(var[i], y[i])
        box_constraint_area.SetCoefficient(var[i], x[i] * y[i])


    solver.Solve()

    max_sweet_cost = objective.Value()

    sweets_used = []
    for i in range(no_sweets):
        if var[i].solution_value() > 0:
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

# print(sweets_used)


def print_used_sweets(sweets_used, x_cor, y_cor):
    for i in range(len(sweets_used)):
        print(f"Size of sweet {i + 1}: {sweets_used[i]*x_cor[i]*y_cor[i]}")
    print(f"Total cost: {max_sweet_cost}")

    with open('Summary_Q4.txt', 'w') as file:

        for i in range(len(sweets_used)):
            file.write(f"Size of sweet {i + 1}: {sweets_used[i]*x_cor[i]*y_cor[i]}\n")
        file.write(f"Total cost: {max_sweet_cost}")
print_used_sweets(sweets_used, x_cor, y_cor)