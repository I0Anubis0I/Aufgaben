#include <stdio.h>
#include <math.h>

// Define a struct for a ship
typedef struct {
    double x;
    double y;
} Ship;

void simulate_pirate_chase(double n, double dt, double epsilon) {
    // Initialization
    Ship merchant = {1.0, 0.0}; // Merchant ship's position
    Ship pirate = {0.0, 0.0};   // Pirate ship's position
    double t = 0.0; // Start time
    int iteration = 0;
    

    while (1) {
        // Update merchant ship position
        merchant.y = n * t;
        
        // Calculate heading angle
        double dx = merchant.x - pirate.x;
        double dy = merchant.y - pirate.y;
        double theta = atan2(dy, dx);
        
        // Move the pirate ship
        pirate.x += dt * cos(theta);
        pirate.y += dt * sin(theta);
        
        // Check if the pirate ship has caught the merchant ship
        if (hypot(merchant.x - pirate.x, merchant.y - pirate.y) < epsilon) {
            printf("The pirate ship caught the merchant ship after t = %.15f time units\n"
                   "at y = %.20f.\n", t, merchant.y);
            //printf("%d", iteration);
            return;
        }
        
        // Increase time step
        t += dt;
        iteration++;
    }

}

int main() {
    // Run simulation with given parameters
    simulate_pirate_chase(3.0 / 4.0, 0.00000001, 0.000000001);
    return 0;
}
