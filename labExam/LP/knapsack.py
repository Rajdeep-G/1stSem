from ortools.linear_solver import pywraplp


def knapsack_solver(num_items,values, weights, total_capacity):

    solver = pywraplp.Solver.CreateSolver('SCIP')


    var=[]
    for i in range(num_items):
        var.append(solver.NumVar(0.0, 1.0, 'x_%i' % i))
    # print(var)

    ct = solver.Constraint(0.0, total_capacity,"ct")
    for i in range(num_items):
        ct.SetCoefficient(var[i], weights[i]) #setting the constraint that total sum of wt <= W

    objective = solver.Objective()

    for i in range(num_items):
        objective.SetCoefficient(var[i], values[i]) 
    
    
    objective.SetMaximization()
    solver.Solve()
    final_value = objective.Value()

    values_each=[]
    weights_each=[]
    
    for i in range(num_items):
        if var[i].solution_value() > 0:
            values_each.append( var[i].solution_value() * values[i])
            weights_each.append( var[i].solution_value() * weights[i])
    
    return final_value,values_each,weights_each

def print_soln(total_value,n,values_each,weights_each):
    print(f"Total Value: {total_value}")
    print("Items to take:")
    # print(items_to_take)
    for i in range(n):
        print(f'Item {i+1} - Weight: {weights_each[i]}, Value: {values_each[i]}')


with open('input_Q1.txt', 'r') as file:
    no_of_items = int(file.readline())
    max_wt = int(file.readline())
    weights = list(map(float, file.readline().split()))
    values =list(map(float, file.readline().split()))

total_value, values_each,weights_each = knapsack_solver(no_of_items,values, weights, max_wt)


print_soln(total_value,no_of_items,values_each,weights_each)
