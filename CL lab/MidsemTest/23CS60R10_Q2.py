from ortools.linear_solver import pywraplp
import sys
def solver(n,m,k,cap,dist):
    solver = pywraplp.Solver.CreateSolver('SCIP')

    var = []
    for i in range(n):
        temp=[]
        for j in range(m):
            temp.append(solver.IntVar(0, 1, f'x_{i}_{j}')) #declaring variables
        var.append(temp)


    for i in range(n):
        ct=solver.Constraint(1,1,"ct")
        for j in range(m):
            ct.SetCoefficient(var[i][j],1) #constraint that for each student 1 center has to be allocated 

    for i in range(m):
        ct=solver.Constraint(0,cap[i],"ct")
        for j in range(n):
            ct.SetCoefficient(var[j][i],1) #constraint for capacity of each center
    
    for i in range(n):
        ct=solver.Constraint(0,k,"ct")
        for j in range(m):
            ct.SetCoefficient(var[i][j],dist[i][j]) # constraint for distance travelled by each <=k

    obj = solver.Objective()
    for i in range(n):
        for j in range(m):
            obj.SetCoefficient(var[i][j], dist[i][j]) #objective function to be minimised

    obj.SetMinimization()
    # obj.SetMaximization()
    solver.Solve()
    ans=obj.Value()
    # print(ans)

    for i in range(n):
        for j in range(m):
            if (var[i][j].solution_value()==1):
                print(f'Student {i+1} is assigned to Attendence Center {j+1}')
    
    for i in range(m):
        c=0
        for j in range(n):
            c+=var[j][i].solution_value()
            c=(int)(c)
        print(f'Attendece center {i+1} has {c} students.')

    print(f'Total Distance :{ans}')

filename=sys.argv[1]
with open(filename, 'r') as file:
    
    n,m,k=map(int,file.readline().split())
    cap = list(map(int, file.readline().split()))
    dist=[]
    for i in range(n):
        ind_dist =list(map(int, file.readline().split()))
        dist.append(ind_dist)

solver(n,m,k,cap,dist)

