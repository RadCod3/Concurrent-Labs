import java.util.concurrent.Semaphore;

public class BusStop {
    private int waitingRiders = 0;
    private final Semaphore mutex = new Semaphore(1);
    private final Semaphore bus = new Semaphore(0);
    private final Semaphore boarded = new Semaphore(0);

    // Rider arrives at the bus stop
    public void riderArrives() throws InterruptedException {
        // Acquire mutex to update waiting riders count
        mutex.acquire();
        waitingRiders++;
        System.out.println("Rider arrived. Waiting riders: " + waitingRiders);
        mutex.release();

        // Wait until bus signals to board
        bus.acquire();
        boardBus();
        // Signal to bus that rider has boarded
        boarded.release();
    }

    // Bus arrives at the bus stop
    public void busArrives() throws InterruptedException {
        // Acquire mutex to get number of waiting riders
        mutex.acquire();
        int ridersToBoard = Math.min(waitingRiders, 50);
        System.out.println("Bus arrived. Riders to board: " + ridersToBoard);
        for (int i = 0; i < ridersToBoard; i++) {
            // Signal riders to board
            bus.release();
        }
        waitingRiders -= ridersToBoard;
        mutex.release();

        // Wait for all riders to board
        for (int i = 0; i < ridersToBoard; i++) {
            boarded.acquire();
        }
        depart();
    }

    // Simulate rider boarding the bus
    private void boardBus() {
        System.out.println("Rider boarded the bus.");
    }

    // Simulate bus departing
    private void depart() {
        System.out.println("Bus is departing.");
    }
}