from ortools.linear_solver import pywraplp

solver = pywraplp.Solver.CreateSolver("GLOP")
x= solver.NumVar(0, solver.infinity(), 'x')
y= solver.NumVar(0, solver.infinity(), 'y')


print("Enter value of a and b")
k,l=map(int,input().split())

i=1
while(True):
    print(f'for {i} th constraint, enter a,b,condition,c for ax+by condition c')
    a,b,cond,c=map(str,input().split())
    a,b,c=int(a),int(b),int(c)
    if cond == "le":
        solver.Add(a * x +b * y <=c)
    elif cond == "ge":
        solver.Add(a * x + b * y >=c)
    elif cond == "l":
        solver.Add(a * x +b * y <c)
    elif cond == "g":
        solver.Add(a * x +b * y >c)
    elif cond=="eq":
        solver.Add(a * x +b * y ==c)
    print("press 0 to evaluate the result and press 1 to add more constraints")
    choice=int(input())
    if choice==0:
        break
    i+=1


solver.Maximize(k * x + l * y)  
status = solver.Solve()
print(solver.Objective().Value())
print("x = ", x.solution_value())
print("y = ", y.solution_value())