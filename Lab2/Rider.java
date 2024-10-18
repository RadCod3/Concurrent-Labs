public class Rider implements Runnable {
    private final BusStop busStop;

    public Rider(BusStop busStop) {
        this.busStop = busStop;
    }

    @Override
    public void run() {
        try {
            busStop.riderArrives();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
//            System.out.println("Rider thread interrupted.");
        }
    }
}