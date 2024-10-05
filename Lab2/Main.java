import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {
    private static final double MEAN_BUS_ARRIVAL_TIME = 10000; // 2 seconds
    private static final double MEAN_RIDER_ARRIVAL_TIME = 500; // 0.5 seconds
    private static final int SIMULATION_TIME = 300 * 1000;       // 30 seconds

    public static void main(String[] args) {
        BusStop busStop = new BusStop();
        ExecutorService executor = Executors.newCachedThreadPool();

        // Start simulation
        long simulationEndTime = System.currentTimeMillis() + SIMULATION_TIME;

        // Thread for generating riders
        new Thread(() -> {
            while (System.currentTimeMillis() < simulationEndTime) {
                executor.execute(new Rider(busStop));

                // Sleep for next rider arrival
                try {
                    Thread.sleep((long) getExponentialRandom(MEAN_RIDER_ARRIVAL_TIME));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }).start();

        // Thread for generating buses
        new Thread(() -> {
            while (System.currentTimeMillis() < simulationEndTime) {
                // Sleep for next bus arrival
                try {
                    Thread.sleep((long) getExponentialRandom(MEAN_BUS_ARRIVAL_TIME));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                executor.execute(new Bus(busStop));
            }
        }).start();
    }

    // Function to generate exponential random variables
    private static double getExponentialRandom(double mean) {
        return -mean * Math.log(1 - Math.random());
    }
}