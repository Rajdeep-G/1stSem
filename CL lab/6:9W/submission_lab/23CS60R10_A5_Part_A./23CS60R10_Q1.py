from ortools.linear_solver import pywraplp


def optimize_production(no_of_pdt,no_of_res, profits, resource_availability, resource_usage):

    solver = pywraplp.Solver.CreateSolver('GLOP')

    production_plan = []
    for i in range(no_of_pdt):
        production_plan.append(solver.NumVar(0, solver.infinity(), f'var_{i}')) #make it IntVar

    print(production_plan)

    for j in range(no_of_res):
        total_resource_usage = solver.Sum(production_plan[i] * resource_usage[i][j] for i in range(no_of_pdt))
        solver.Add(total_resource_usage <= resource_availability[j])
    
    # for j in range(no_of_res):
    #     ct = solver.Constraint(0, resource_availability[j],"ct")
    #     for i in range(no_of_pdt):
    #         ct.SetCoefficient(production_plan[i],resource_usage[i][j])

    
    objective = solver.Objective()
    
    for i in range(no_of_pdt):
        objective.SetCoefficient(production_plan[i], profits[i])
    objective.SetMaximization()

    solver.Solve()


    # for i in range(no_of_pdt):
    #     print(f"Product {i}: Qty = {production_plan[i].solution_value()}")
    # print(f"Max optimal Profit: {objective.Value()}")
    with open('output_Q1.txt', 'w') as file:
        for i in range(no_of_pdt):
            file.write(f"Product {i}: Qty = {production_plan[i].solution_value()}\n")
        file.write(f"Max optimal Profit: {objective.Value()}\n")





with open('input_Q1.txt', 'r') as file:
    no_of_pdt = int(file.readline())
    no_of_res = int(file.readline())
    profits = list(map(int, file.readline().split()))
    res_avail = list(map(int, file.readline().split()))

    all_res = []
    for i in range(no_of_pdt):
        each_res = list(map(int, file.readline().split()))
        all_res.append(each_res)

optimize_production(no_of_pdt, no_of_res, profits, res_avail, all_res)
