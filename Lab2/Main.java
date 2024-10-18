import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.ThreadLocalRandom;

public class Main {
    private static final double MEAN_BUS_ARRIVAL_TIME = 4 * 1000; // 5 seconds
    private static final double MEAN_RIDER_ARRIVAL_TIME = 0.1 * 1000; // 0.5 seconds
    private static final int SIMULATION_TIME = 60 * 1000;      // 30 seconds

    public static void main(String[] args) {
        BusStop busStop = new BusStop();
        ExecutorService executor = Executors.newCachedThreadPool();
        AtomicBoolean running = new AtomicBoolean(true);

        // Thread for generating riders
        Thread riderGenerator = new Thread(() -> {
            while (running.get()) {
                executor.execute(new Rider(busStop));
                try {
                    Thread.sleep((long) getExponentialRandom(MEAN_RIDER_ARRIVAL_TIME));
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    System.out.println("Rider generator thread interrupted.");
                }
            }
        });
        riderGenerator.start();

        // Thread for generating buses
        Thread busGenerator = new Thread(() -> {
            while (running.get()) {
                try {
                    Thread.sleep((long) getExponentialRandom(MEAN_BUS_ARRIVAL_TIME));
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    System.out.println("Bus generator thread interrupted.");
                }
                executor.execute(new Bus(busStop));
            }
        });
        busGenerator.start();

        // Wait for simulation to end
        try {
            Thread.sleep(SIMULATION_TIME);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            System.out.println("Main simulation thread interrupted.");
        }

        // Signal threads to stop
        running.set(false);
        riderGenerator.interrupt();
        busGenerator.interrupt();

        // Shutdown executor service
        executor.shutdown();
        try {
            if (!executor.awaitTermination(5, TimeUnit.SECONDS)) {
                executor.shutdownNow();
            }
        } catch (InterruptedException e) {
            executor.shutdownNow();
            Thread.currentThread().interrupt();
        }
    }

    private static double getExponentialRandom(double mean) {
        return -mean * Math.log(1 - ThreadLocalRandom.current().nextDouble());
    }
}