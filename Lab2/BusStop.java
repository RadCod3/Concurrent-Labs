import java.util.concurrent.Semaphore;

public class BusStop {
    private final Semaphore mutex = new Semaphore(1);                  // Mutex semaphore
    private final Semaphore busSemaphore = new Semaphore(0, true);     // For riders to wait for the bus
    private final Semaphore boardedSemaphore = new Semaphore(0, true); // For bus to wait for riders to board
    private int waitingRiders = 0;

    public void riderArrives() throws InterruptedException {
        // Acquire mutex to update waiting riders count
        mutex.acquire();
        waitingRiders++;
        System.out.print("Rider arrived. Waiting riders: " + waitingRiders + "\r");
        mutex.release();

        // Wait until bus signals to board
        busSemaphore.acquire();
        boardBus();

        // Signal to bus that rider has boarded
        boardedSemaphore.release();
    }

    public void busArrives() throws InterruptedException {
        // Acquire mutex to get number of waiting riders and prevent new riders from arriving
        mutex.acquire();
        int ridersToBoard = Math.min(waitingRiders, 50);
        System.out.println("\nBus arrived. Riders to board: " + ridersToBoard);

        // Release boarding permits to riders
        busSemaphore.release(ridersToBoard);

        // Update waiting riders count
        waitingRiders -= ridersToBoard;

        // Release mutex to allow new riders to arrive
        mutex.release();

        // Wait for all riders to board
        boardedSemaphore.acquire(ridersToBoard);
        depart(ridersToBoard);
    }

    private void boardBus() {
//        System.out.println("Rider boarded the bus.");
    }

    private void depart(int riders) {
        System.out.println("Bus is departing with " + riders + " riders. Leftover riders: " + waitingRiders + '\n');
    }
}