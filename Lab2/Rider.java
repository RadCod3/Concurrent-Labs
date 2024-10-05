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
            e.printStackTrace();
        }
    }
}