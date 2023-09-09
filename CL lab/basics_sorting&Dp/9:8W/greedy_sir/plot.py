
import matplotlib.pyplot as plt

x = [1, 2, 3, 4, 5, 6, 7, 8]
y = [11, 20, 20, 17, 17, 17, 17, 17]  # Added one more element to y

plt.plot(x, y, marker='o')
plt.title('Graph Title')
plt.xlabel('Divisions: K')
plt.ylabel('Maximum total poularity score')
plt.grid(True)
plt.show()