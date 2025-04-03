import math
import matplotlib.pyplot as plt

class Ship:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def update_position(self, dx, dy):
        self.x += dx
        self.y += dy

def simulate_pirate_chase(n=3/4, dt=0.0001, epsilon=0.00001):
    merchant = Ship(1, 0)  # Merchant ship
    pirate = Ship(0, 0)  # Pirate ship
    
    t = 0  # Start time
    merchant_path = [(merchant.x, merchant.y)]
    pirate_path = [(pirate.x, pirate.y)]
    
    while True:
        merchant.y = n * t
        merchant_path.append((merchant.x, merchant.y))
       
        dx = merchant.x - pirate.x
        dy = merchant.y - pirate.y
        theta = math.atan2(dy, dx)
        
        pirate.update_position(dt * math.cos(theta), dt * math.sin(theta))
        pirate_path.append((pirate.x, pirate.y))
        
        if abs(merchant.x - pirate.x) < epsilon and abs(merchant.y - pirate.y) < epsilon:
            break
        
        t += dt
    
    return merchant_path, pirate_path, t, merchant.y

merchant_path, pirate_path, time_caught, position_caught = simulate_pirate_chase()

print(f"The pirate ship caught the merchant ship after t = {time_caught:.5f} time at \ny = {position_caught:.50f}.")


# Setting up the figure
fig, ax = plt.subplots(figsize=(8, 6))
ax.set_xlim(0, 1.2)
ax.set_ylim(0, max(y for _, y in merchant_path) + 0.2)
ax.set_xlabel("X Position")
ax.set_ylabel("Y Position")
ax.set_title("Pirate Ship Chasing Merchant Ship")

# Extract positions
merchant_x, merchant_y = zip(*merchant_path)
pirate_x, pirate_y = zip(*pirate_path)

ax.plot(merchant_x, merchant_y, label="Merchant Ship Path", linestyle="--", color="blue", alpha=0.3)
ax.plot(pirate_x, pirate_y, label="Pirate Ship Path", linestyle="-", color="red", alpha=0.3)

#skull unicode text at capture point
plt.text(merchant_x[-1], merchant_y[-1], '\u2620', color='black', fontsize=20, ha='center', va='center')

#cross marker at capture point
#ax.scatter([merchant_x[-1]], [merchant_y[-1]], color='red', label="Capture Point", marker="X", s=100)

merchant_dot, = ax.plot([], [], 'b^', label="Merchant Ship", markersize=8) #b=blue ^=triangle
pirate_dot, = ax.plot([], [], 'ro', label="Pirate Ship", markersize=8) #r=red o=circle

ax.legend()
ax.grid()

ds = 1000  # Downsampling-Faktor to speed up the animation
merchant_x_ds = merchant_x[::ds]
merchant_y_ds = merchant_y[::ds]
pirate_x_ds = pirate_x[::ds]
pirate_y_ds = pirate_y[::ds]

# Loop for the animation
for frame in range(len(merchant_x_ds)):
    # Set the new positions of the ships
    merchant_dot.set_data([merchant_x_ds[frame]], [merchant_y_ds[frame]])
    pirate_dot.set_data([pirate_x_ds[frame]], [pirate_y_ds[frame]])
    
    
    plt.pause(0.1)  # Pause between animation, smaller number = faster

#final image after the animation
plt.show()