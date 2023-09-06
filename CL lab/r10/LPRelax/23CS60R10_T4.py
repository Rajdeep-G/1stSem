from ortools.linear_solver import pywraplp


def knapsack_solver(num_items,values, weights, total_capacity):

    solver = pywraplp.Solver.CreateSolver('SCIP')


    var=[]
    for i in range(num_items):
        var.append(solver.IntVar(0, 1, 'x_%i' % i))
    # print(var)

    ct = solver.Constraint(0, total_capacity,"ct")
    for i in range(num_items):
        ct.SetCoefficient(var[i], weights[i]) #setting the constraint that total sum of wt <= W

    objective = solver.Objective()

    for i in range(num_items):
        objective.SetCoefficient(var[i], values[i]) 
    
    
    objective.SetMaximization()

    solver.Solve()

    final_value = objective.Value()
    result = []
    not_taken=[]

    for i in range(num_items):
        if var[i].solution_value() > 0:
            result.append(i + 1)
        else:
            not_taken.append(i+1)
    # print(result)
    # print(not_taken)
    return final_value, result,not_taken

def print_soln(total_value,items_to_take,not_taken,n):
    print(f"Total Value: {total_value}")
    print("Items to take:")
    for item in range(n):
        if(item+1 in items_to_take):
            print(f"Item {item+1} - Weight: {weights[item]}, Value: {values[item]}")
        else:
            print(f"Item {item+1} - Weight: 0.0, Value: 0.0")


with open('input_Q1.txt', 'r') as file:
    no_of_items = int(file.readline())
    max_wt = int(file.readline())
    weights = list(map(float, file.readline().split()))
    values =list(map(float, file.readline().split()))

total_value, items_to_take,not_taken = knapsack_solver(no_of_items,values, weights, max_wt)

print_soln(total_value,items_to_take,not_taken,no_of_items)
